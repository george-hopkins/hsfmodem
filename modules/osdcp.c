/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#include "oscompat.h"
#include "osresour_ex.h"

#include <linux/slab.h>
#include <linux/poll.h>
#ifdef FOUND_DEVFS
#include <linux/devfs_fs_kernel.h>
#endif
#include <linux/kmod.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
#include <linux/device.h>
#endif

#ifdef USE_DCP

#include "dcp.h"

#define DCP_SAMPLE_RATE 16000
#define DCP_SAMPLE_SIZE sizeof(INT16)

static struct list_head dcp_instance_list = LIST_HEAD_INIT(dcp_instance_list);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)
static DEFINE_SPINLOCK(dcp_lock);
#else
static spinlock_t dcp_lock = SPIN_LOCK_UNLOCKED;
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
#ifdef FOUND_CLASS_SIMPLE
static struct class_simple *dcp_class;
#else
static struct class *dcp_class;
#endif
#endif

typedef struct {
    struct list_head entry;
    spinlock_t lock;

    POS_DEVNODE	pDevNode;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    devfs_handle_t devfs_handle;
#endif

    struct list_head units;

    int restarted;

    int volume;

#ifdef OSDCP_SKIP
    int skip;
#endif

    unsigned char ringBuf[DCP_SAMPLE_SIZE * DCP_SAMPLE_RATE * 2]; /* buffer upto two seconds */
    int putIdx;

    wait_queue_head_t read_wait;
    struct fasync_struct *fasync;
} dcp_instance_t;

typedef struct {
    struct list_head entry;
    int minor;
    dcp_instance_t *pDcp;
    int takeIdx;
} dcp_unit_t;

static int dcpdebug = 0;
#ifdef FOUND_MODULE_PARAM
module_param(dcpdebug, int, 0);
#else
MODULE_PARM(dcpdebug, "i");
#endif
MODULE_PARM_DESC(dcpdebug, "Debug level for Digital Call Progress audio");

static int
dcp_fasync(int fd, struct file *file, int on)
{
    dcp_unit_t *su = file->private_data;
    dcp_instance_t *pDcp = su->pDcp;
    int r;

    if(!pDcp)
	return -ENODEV;

    r = fasync_helper(fd, file, on, &pDcp->fasync);
    return (r < 0) ? r : 0;
}

static unsigned int
dcp_poll(struct file *file, poll_table * wait)
{
    dcp_unit_t *su = file->private_data;
    dcp_instance_t *pDcp = su->pDcp;
    unsigned int mask = 0;
    unsigned long flags;

    if(!pDcp)
	return -ENODEV;

    poll_wait(file, &pDcp->read_wait, wait);
    spin_lock_irqsave(&pDcp->lock, flags);
    if (pDcp->putIdx != su->takeIdx)
	mask |= POLLIN | POLLRDNORM;
    spin_unlock_irqrestore(&pDcp->lock, flags);
    return mask;
}

static ssize_t
dcp_read(struct file * file, char * buf, size_t nbytes, loff_t *ppos)
{
    dcp_unit_t *su = file->private_data;
    dcp_instance_t *pDcp = su->pDcp;
    ssize_t ret = 0;
    unsigned long flags;
    char *dp;
    size_t n;

    if(nbytes <= 0)
	return 0;

    if(!pDcp)
	return -ENODEV;

    spin_lock_irqsave(&pDcp->lock, flags);

    if((pDcp->putIdx == su->takeIdx) && (su->takeIdx >= 0)) {
	DECLARE_WAITQUEUE(wait, current);

	add_wait_queue(&pDcp->read_wait, &wait);

	while(pDcp->putIdx == su->takeIdx) {

	    set_current_state(TASK_INTERRUPTIBLE);
	    if (file->f_flags & O_NONBLOCK) {
		ret = -EAGAIN;
		break;
	    }
	    if (signal_pending(current)) {
		ret = -ERESTARTSYS;
		break;
	    }

	    spin_unlock_irqrestore(&pDcp->lock, flags);
	    schedule();
	    spin_lock_irqsave(&pDcp->lock, flags);
	}

	set_current_state(TASK_RUNNING);
	remove_wait_queue(&pDcp->read_wait, &wait);

	if(ret) {
	    spin_unlock_irqrestore(&pDcp->lock, flags);
	    return ret;
	}
    }

    if((pDcp->putIdx < 0) || (su->takeIdx < 0)) {
	spin_unlock_irqrestore(&pDcp->lock, flags);
	return -ENODEV;
    }

    dp = buf;
    do {

	if(pDcp->putIdx > su->takeIdx)
		n = pDcp->putIdx - su->takeIdx;
	else
		n = sizeof(pDcp->ringBuf) - su->takeIdx;

	if (n > nbytes) {
	    n = nbytes;
	}

	spin_unlock_irqrestore(&pDcp->lock, flags);
	if(copy_to_user(dp, &pDcp->ringBuf[su->takeIdx], n)) {
	    ret = -EFAULT;
	    spin_lock_irqsave(&pDcp->lock, flags);
	    break;
	}
	spin_lock_irqsave(&pDcp->lock, flags);

	su->takeIdx += n;
	if(su->takeIdx == sizeof(pDcp->ringBuf))
	    su->takeIdx = 0;

	dp += n;
	nbytes -= n;

    } while (nbytes && (pDcp->putIdx != su->takeIdx));

    spin_unlock_irqrestore(&pDcp->lock, flags);

    if(!ret) {
	ret = dp - buf;

	if(ret) {
	    TOUCH_ATIME(file);
	}
    }

    return ret;
}

static int dcp_open(struct inode* inode, struct file*  file)
{
    int minor = minor(inode->i_rdev);
    dcp_unit_t *su;
    struct list_head *e;
    unsigned long flags, flags2;

    su = kmalloc(sizeof(*su), GFP_KERNEL);
    if(!su) {
	return -ENOMEM;
    }
    memset(su, 0, sizeof(*su));

    su->minor = minor;

    spin_lock_irqsave(&dcp_lock, flags);
    list_for_each (e, &dcp_instance_list) {
	if((list_entry(e, dcp_instance_t, entry))->pDevNode->hwInstNum == minor) {
	    su->pDcp = list_entry(e, dcp_instance_t, entry);
	    break;
	}
    }

    if(su->pDcp) {
	spin_lock_irqsave(&su->pDcp->lock, flags2);
    	list_add(&su->entry, &su->pDcp->units);
	su->takeIdx = su->pDcp->putIdx;
	spin_unlock_irqrestore(&su->pDcp->lock, flags2);
    }

    spin_unlock_irqrestore(&dcp_lock, flags);

    if(!su->pDcp) {
	kfree(su);
	return -ENODEV;
    }

    file->private_data = su;

    return 0;
}

static int dcp_release(struct inode* inode, struct file*  file)
{
    dcp_unit_t *su = file->private_data;
    dcp_instance_t *pDcp = su->pDcp;
    unsigned long flags;

    if(pDcp) {
	dcp_fasync(-1, file, 0);

    	spin_lock_irqsave(&pDcp->lock, flags);
    	list_del(&su->entry);
    	su->takeIdx = -1;
    	spin_unlock_irqrestore(&pDcp->lock, flags);
#if 0
    	wake_up_interruptible_all(&pDcp->read_wait);
    	OsSleep(100);
#endif
    }

    kfree(su);

    return 0;
}

static void
call_dcp_daemon(int instNum, char *action)
{
	char *argv[3], *envp[6];
	int i=0;
	char devbuf[30];
	char actbuf[30];
	char spdbuf[30];

	envp [i++] = "HOME=/";
	envp [i++] = "PATH=/sbin:/bin:/usr/sbin:/usr/bin";
	sprintf(actbuf, "CNXTDCPACTION=%s", action);
	envp [i++] = actbuf;
	sprintf(spdbuf, "CNXTDCPRATE=%d", DCP_SAMPLE_RATE);
	envp [i++] = spdbuf;
	sprintf(devbuf, "CNXTDCPDEVICE=%d", instNum);
	envp [i++] = devbuf;
	envp [i++] = 0;

	argv[0] = CNXTSBINDIR"/"CNXTTARGET"dcpd";
	argv[1] = devbuf + sizeof("CNXTDCPDEVICE");
	argv[2] = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	i = call_usermodehelper(argv[0], argv, envp);
#else
	i = call_usermodehelper(argv[0], argv, envp, 1);
#endif
	if(i && (i != SIGTERM)) {
	    printk(KERN_ERR"%s: %s returned %d\n", __FUNCTION__, argv[0], i);
	}
}

static int dcpmajor = CNXTDCPMAJOR;
#ifdef FOUND_MODULE_PARAM
module_param(dcpmajor, int, 0);
#else
MODULE_PARM(dcpmajor, "i");
#endif
MODULE_PARM_DESC(dcpmajor, "Major device number for dcp device");

__shimcall__
void DcpDestroy  (HANDLE hDcp)
{
    dcp_instance_t *pDcp = hDcp;
    int i;
    unsigned long flags;
    struct list_head *e;

    if(dcpdebug)
    	printk("%s: hDcp=%p\n", __FUNCTION__, hDcp);

    spin_lock_irqsave(&dcp_lock, flags);
    list_del(&pDcp->entry);
    spin_unlock_irqrestore(&dcp_lock, flags);

    call_dcp_daemon(pDcp->pDevNode->hwInstNum, "stop");

    spin_lock_irqsave(&pDcp->lock, flags);
    pDcp->putIdx = -2;
    spin_unlock_irqrestore(&pDcp->lock, flags);

    spin_lock_irqsave(&pDcp->lock, flags);
    for(i = 0; !list_empty(&pDcp->units); i++) {
	if(i == 1) {
	    list_for_each (e, &pDcp->units) {
		(list_entry(e, dcp_unit_t, entry))->takeIdx = -1;
		//(list_entry(e, dcp_unit_t, entry))->pDcp = NULL;
	    }
	}
	spin_unlock_irqrestore(&pDcp->lock, flags);
	if(i >= 1) {
	    if((i % 20) == 10)
	    	printk(KERN_ERR"%s: units still active, waiting..\n", __FUNCTION__);
	    kill_fasync(&pDcp->fasync, SIGIO, POLL_IN);
	    wake_up_interruptible_all(&pDcp->read_wait);
	}
	schedule_timeout(HZ/10);
	spin_lock_irqsave(&pDcp->lock, flags);
    }
    spin_unlock_irqrestore(&pDcp->lock, flags);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    devfs_unregister(pDcp->devfs_handle);
#else
#ifdef FOUND_DEVFS
    {
	char buf[32];
	snprintf(buf, sizeof(buf), CNXTTARGET"dcp%d", pDcp->pDevNode->hwInstNum);
	devfs_remove(buf);
    }
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    if (!IS_ERR(dcp_class))
        CLASS_DEVICE_DESTROY(dcp_class, MKDEV(dcpmajor, pDcp->pDevNode->hwInstNum));
#endif
#endif

    kfree(pDcp);
}


static struct file_operations dcp_fops = {
    .owner	= THIS_MODULE,
    .fasync	= dcp_fasync,
    .poll	= dcp_poll,
    .read	= dcp_read,
    .open	= dcp_open,
    .release	= dcp_release
};

__shimcall__
HANDLE DcpCreate(HANDLE hDevNode)
{
    unsigned long flags;
    dcp_instance_t *pDcp;
    char buf[32];

    if(dcpdebug)
	printk("%s: hDevNode=%p\n", __FUNCTION__, hDevNode);

    pDcp = kmalloc(sizeof(*pDcp), GFP_KERNEL);
    if(!pDcp)
	return NULL;
    memset(pDcp, 0, sizeof(*pDcp));

    spin_lock_init(&pDcp->lock);
    init_waitqueue_head(&pDcp->read_wait);
    INIT_LIST_HEAD(&pDcp->units);
    pDcp->pDevNode = hDevNode;

    spin_lock_irqsave(&dcp_lock, flags);
    list_add(&pDcp->entry, &dcp_instance_list);
    spin_unlock_irqrestore(&dcp_lock, flags);

    snprintf(buf, sizeof(buf), CNXTTARGET"dcp%d", pDcp->pDevNode->hwInstNum);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    pDcp->devfs_handle = devfs_register(NULL, buf, DEVFS_FL_DEFAULT,
	    dcpmajor, pDcp->pDevNode->hwInstNum,
	    S_IFCHR | S_IRUSR | S_IWUSR,
	    &dcp_fops,
	    NULL);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    if (!IS_ERR(dcp_class)) {
	CLASS_DEVICE_CREATE(dcp_class, MKDEV(dcpmajor, pDcp->pDevNode->hwInstNum), pDcp->pDevNode->hwDevLink, CNXTTARGET"dcp%d", pDcp->pDevNode->hwInstNum);
	}
#endif
#ifdef FOUND_DEVFS
    devfs_mk_cdev(MKDEV(dcpmajor, pDcp->pDevNode->hwInstNum),
		  S_IFCHR | S_IRUSR | S_IWUSR, buf);
#endif
#endif

    call_dcp_daemon(pDcp->pDevNode->hwInstNum, "start");
    return (HANDLE)pDcp;
}
 
__shimcall__
void DcpSetVolume(HANDLE hDcp, int nVolume)
{
    dcp_instance_t *pDcp = hDcp;
    unsigned long flags;

    if(dcpdebug)
	printk("%s: hDcp=%p nVolume=%d\n", __FUNCTION__, hDcp, nVolume);

    if(!pDcp)
	return;

    spin_lock_irqsave(&pDcp->lock, flags);
#ifdef OSDCP_SKIP
    if(!pDcp->volume && nVolume) {
	pDcp->skip = DCP_SAMPLE_RATE * DCP_SAMPLE_SIZE; /* skip spikes */
    }
#endif
    pDcp->volume = nVolume;
    spin_unlock_irqrestore(&pDcp->lock, flags);
}
 
__shimcall__
void DcpCallback(HANDLE hDcp, PVOID pData, UINT32 dwSize)
{
    dcp_instance_t *pDcp = hDcp;
    unsigned long flags;
    int s;

    if(dcpdebug)
	printk("%s: hDcp=%p pData=%p dwSize=%u\n", __FUNCTION__, hDcp, pData, dwSize);

    if(!dwSize)
	return;

    spin_lock_irqsave(&pDcp->lock, flags);
    if (!pDcp->volume || list_empty(&pDcp->units) || (pDcp->putIdx < 0)) {
	spin_unlock_irqrestore(&pDcp->lock, flags);
	return;
    }

#ifdef OSDCP_SKIP
    if(pDcp->skip) {
	s = pDcp->skip;
	if(dwSize < s)
	    s = dwSize;

	pDcp->skip -= s;
	pData += s;
	dwSize -= s;
    }
#endif

    while(dwSize) {
	s = sizeof(pDcp->ringBuf) - pDcp->putIdx;
	if(dwSize < s)
	    s = dwSize;

	memcpy(&pDcp->ringBuf[pDcp->putIdx], pData, s);

	pDcp->putIdx += s;

	if(pDcp->putIdx == sizeof(pDcp->ringBuf))
	    pDcp->putIdx = 0;

	pData += s;
	dwSize -= s;
    }

    spin_unlock_irqrestore(&pDcp->lock, flags);

    kill_fasync(&pDcp->fasync, SIGIO, POLL_IN);
    wake_up_interruptible_all(&pDcp->read_wait);
}

__shimcall__
void OsDcpExit(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    if (!IS_ERR(dcp_class))
	    CLASS_DESTROY(dcp_class);
#endif
    if(dcpmajor > 0)
	unregister_chrdev(dcpmajor, CNXTTARGET"dcp");
}

__shimcall__
int OsDcpInit(void)
{
    int ret;

    if(dcpmajor == 0)
	dcpmajor = devfs_register_chrdev(0, CNXTTARGET"dcp", &dcp_fops);
    else if((ret=devfs_register_chrdev(dcpmajor, CNXTTARGET"dcp", &dcp_fops))) {
	dcpmajor = ret;
    }

    if(dcpmajor < 0) {
	printk(KERN_ERR "%s: cannot register chrdev (%d)\n", __FUNCTION__, dcpmajor);

	return dcpmajor;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    dcp_class = CLASS_CREATE(THIS_MODULE, CNXTTARGET"dcp");
    if (IS_ERR(dcp_class)) {
		printk(KERN_ERR "%s: cannot create simple class (%ld)\n", __FUNCTION__, PTR_ERR(dcp_class));
		if(dcpmajor > 0)
			unregister_chrdev(dcpmajor, CNXTTARGET"dcp");
	    return PTR_ERR(dcp_class);
    }
#endif
    return 0;
}

__shimcall__
void OsDcpEnsureDaemonIsRunning(int hwInstNum)
{
    struct list_head *e;
    unsigned long flags, flags2;
    dcp_instance_t *pDcp = NULL;
    int start = 0;

    spin_lock_irqsave(&dcp_lock, flags);
    list_for_each (e, &dcp_instance_list) {
	if((list_entry(e, dcp_instance_t, entry))->pDevNode->hwInstNum == hwInstNum) {

	    pDcp = list_entry(e, dcp_instance_t, entry);
	    spin_lock_irqsave(&pDcp->lock, flags2);
	    if(list_empty(&pDcp->units) && !pDcp->restarted) {
		pDcp->restarted = start = 1;
	    }
	    spin_unlock_irqrestore(&pDcp->lock, flags2);
	    break;
	}
    }
    spin_unlock_irqrestore(&dcp_lock, flags);

    if(start)
	    call_dcp_daemon(pDcp->pDevNode->hwInstNum, "start");
}

#endif /* USE_DCP */

