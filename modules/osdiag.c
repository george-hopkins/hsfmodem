/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#include "oscompat.h"
#include "osservices.h"
#include "osresour_ex.h"
#include "osmemory.h"

#ifndef NO_DIAGMGR_SUPPORT

#include "intfctrl_ex.h"
#include "osdiag.h"
#include "osdiag_dev.h"

#include <linux/poll.h>
#ifdef FOUND_DEVFS
#include <linux/devfs_fs_kernel.h>
#endif
#include <linux/kmod.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
#include <linux/device.h>
#endif

#define DIAG_NOTIFY_RINGBUF_ELEMENTS 100

static struct list_head diag_instance_list = LIST_HEAD_INIT(diag_instance_list);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)
static DEFINE_SPINLOCK(diag_lock);
#else
static spinlock_t diag_lock = SPIN_LOCK_UNLOCKED;
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
#ifdef FOUND_CLASS_SIMPLE
static struct class_simple *diag_class;
#else
static struct class *diag_class;
#endif
#endif

#define common_instance_header \
    spinlock_t lock; \
    struct list_head units; \
    wait_queue_head_t read_wait; \
    struct fasync_struct *fasync;

typedef struct {
    common_instance_header;

    struct list_head entry;

    int   hwInstNum;

    PSYSTEM_INSTANCES_T pSys;
    PI_DIAG_MGR_T pDiagMgrInterface;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    devfs_handle_t devfs_handle;
#endif

    int putIdx;
    unsigned char ringBuf[DIAG_NOTIFY_RINGBUF_ELEMENTS * sizeof(SDK2K_NOTIFICATION)];
} diag_instance_t;

static diag_instance_t *GetDiagInstance(int hwInstNum,
	void *hwDevLink,
	PSYSTEM_INSTANCES_T pSys, PI_DIAG_MGR_T pDiagMgrInterface);

static void PutDiagInstance(diag_instance_t *pDiag);

#if defined(DMP) || defined(DMPRETAIL)
#include "dmpdefs.h"

typedef struct {
    common_instance_header;

    int putIdx, takeIdx;
    DMP_ACTION_INFO ringBuf[DMP_MAX_SEGMENTS];
} dmp_instance_t;

static dmp_instance_t *dmp_instance;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
static devfs_handle_t dmp_devfs_handle;
#endif
#endif

typedef struct {
    struct list_head entry;
    int minor;
    union {
	void *p;
	diag_instance_t *pDiag;
#if defined(DMP) || defined(DMPRETAIL)
	dmp_instance_t *pDmp;
#endif
    } inst;
    int takeIdx;
} diag_unit_t;

static int diagdebug = 0;
#ifdef FOUND_MODULE_PARAM
module_param(diagdebug, int, 0);
#else
MODULE_PARM(diagdebug, "i");
#endif
MODULE_PARM_DESC(diagdebug, "Debug level for diagnostics");

#if defined(DMP) || defined(DMPRETAIL)

typedef struct {
	int minor;
} dmp_unit_t;

__shimcall__
BOOL
DMPSRV_Dispatcher(PCHAR ClientName, DMP_SERVER_CODE Code,
		PCHAR FileName, PBYTE pSegment, DWORD nData )
{
    unsigned long flags;

	//printk(KERN_INFO "%s: ClientName=%s Code=%d FileName=%s pSegment=%p nData=%ld\n", __FUNCTION__, ClientName ? ClientName : "<NULL>", Code, FileName? FileName : "<NULL>", pSegment, nData);

    if (!dmp_instance) {
	//printk(KERN_INFO "%s: device not initialized\n", __FUNCTION__);
	return FALSE;
    }

    spin_lock_irqsave(&dmp_instance->lock, flags);
    if(list_empty(&dmp_instance->units)) {
	spin_unlock_irqrestore(&dmp_instance->lock, flags);
	//printk(KERN_INFO "%s: device not opened (no daemon running?)\n", __FUNCTION__);
	return FALSE;
    }

    strncpy( dmp_instance->ringBuf[dmp_instance->putIdx].ClientName, ClientName, DMP_MAX_NAME_LEN );
    strncpy( dmp_instance->ringBuf[dmp_instance->putIdx].FileName, FileName, DMP_MAX_NAME_LEN );
    dmp_instance->ringBuf[dmp_instance->putIdx].Code = Code;

    if ( pSegment != NULL )
    {
	if(nData > DMP_SEG_SIZE)
	    nData = DMP_SEG_SIZE;
	memcpy( dmp_instance->ringBuf[dmp_instance->putIdx].pData, pSegment, nData );
	memset( dmp_instance->ringBuf[dmp_instance->putIdx].pData + nData, '\0', DMP_SEG_SIZE - nData );
    }

    dmp_instance->ringBuf[dmp_instance->putIdx].nData = nData;

    dmp_instance->putIdx++;
    if (dmp_instance->putIdx == DMP_MAX_SEGMENTS) 
	dmp_instance->putIdx = 0;

    if (dmp_instance->putIdx == dmp_instance->takeIdx)
	printk(KERN_WARNING "%s: ring buffer overflow\n", __FUNCTION__);

    spin_unlock_irqrestore(&dmp_instance->lock, flags);

    kill_fasync(&dmp_instance->fasync, SIGIO, POLL_IN);
    wake_up_interruptible_all(&dmp_instance->read_wait);

    return TRUE;
}

static ssize_t
dmp_read(struct file * file, char * buf, size_t nbytes, loff_t *ppos)
{
    PDMP_ACTION_INFO dpSegs;
    size_t nsegs = nbytes / sizeof(DMP_ACTION_INFO), n;
    ssize_t ret = 0;
    unsigned long flags;

    if(nsegs <= 0)
	return 0;

    spin_lock_irqsave(&dmp_instance->lock, flags);

    if((dmp_instance->putIdx == dmp_instance->takeIdx) && (dmp_instance->takeIdx >= 0)) {
	DECLARE_WAITQUEUE(wait, current);

	add_wait_queue(&dmp_instance->read_wait, &wait);

	while(dmp_instance->putIdx == dmp_instance->takeIdx) {

	    set_current_state(TASK_INTERRUPTIBLE);
	    if (file->f_flags & O_NONBLOCK) {
		ret = -EAGAIN;
		break;
	    }
	    if (signal_pending(current)) {
		ret = -ERESTARTSYS;
		break;
	    }

	    spin_unlock_irqrestore(&dmp_instance->lock, flags);
	    schedule();
	    spin_lock_irqsave(&dmp_instance->lock, flags);
	}

	set_current_state(TASK_RUNNING);
	remove_wait_queue(&dmp_instance->read_wait, &wait);

	if(ret) {
	    spin_unlock_irqrestore(&dmp_instance->lock, flags);
	    return ret;
	}
    }

    if((dmp_instance->putIdx < 0) || (dmp_instance->takeIdx < 0)) {
	spin_unlock_irqrestore(&dmp_instance->lock, flags);
	return -ENODEV;
    }

    dpSegs = (PDMP_ACTION_INFO)buf;
    do {

	if(dmp_instance->putIdx > dmp_instance->takeIdx) {
	    n = dmp_instance->putIdx - dmp_instance->takeIdx;
	} else {
	    n = DMP_MAX_SEGMENTS - dmp_instance->takeIdx;
	}

	if (n > nsegs) {
	    n = nsegs;
	}

	nsegs -= n;

	while(n--) {
	    spin_unlock_irqrestore(&dmp_instance->lock, flags);
	    if(copy_to_user( dpSegs, &dmp_instance->ringBuf[dmp_instance->takeIdx], sizeof(DMP_ACTION_INFO) )) {
		ret = -EFAULT;
		spin_lock_irqsave(&dmp_instance->lock, flags);
		break;
	    }
	    spin_lock_irqsave(&dmp_instance->lock, flags);
	    dpSegs++;
	    dmp_instance->takeIdx++;
	    if (dmp_instance->takeIdx == DMP_MAX_SEGMENTS)
		dmp_instance->takeIdx = 0;
	}

    } while(nsegs && (dmp_instance->putIdx != dmp_instance->takeIdx));

    spin_unlock_irqrestore(&dmp_instance->lock, flags);

    if(!ret) {
	ret = ((char *)dpSegs) - buf;

	if(ret) {
	    TOUCH_ATIME(file);
	}
    }

    return ret;
}
#endif /* DMP || DMP_RETAIL */

static int diag_ioctl(struct inode* inode, struct file*  file, unsigned int cmd, unsigned long arg)
{
    diag_unit_t *su = file->private_data;
    diag_instance_t *pDiag = su->inst.pDiag;
    int ret = 0;
    SDK2K_IOCTLARGS ioctlArgs;
    unsigned char ioctlBuf[512], *pIoctlBuf;

#if defined(DMP) || defined(DMPRETAIL)
    if(su->minor == CNXTDIAGDMPMINOR)
	return -EINVAL;
#endif

    if(!pDiag || !pDiag->pSys)
	return -ENODEV;

    switch(cmd) {
	case SDK2K_IOC_CONFIGURE:
	case SDK2K_IOC_CONTROL:
	case SDK2K_IOC_MONITOR:
	    break;
	default:
	    return -EINVAL;
    }

    if (copy_from_user(&ioctlArgs, (u8 *)arg, sizeof(ioctlArgs))) {
	return -EFAULT;
    }

    if(ioctlArgs.BufferSize <= sizeof(ioctlBuf)) {
	pIoctlBuf = ioctlBuf;
    } else {
	pIoctlBuf = OsAllocate(ioctlArgs.BufferSize);
	if(!pIoctlBuf) {
	    printk(KERN_ERR"%s: could not allocate %u bytes\n", __FUNCTION__, ioctlArgs.BufferSize);
	    return -ENOMEM;
	}
    }

    switch(cmd) {
	case SDK2K_IOC_CONFIGURE:
	    if (copy_from_user(pIoctlBuf, (u8 *)ioctlArgs.Buffer, ioctlArgs.BufferSize)) {
		ret = -EFAULT;
		break;
	    }
	    if (pDiag->pDiagMgrInterface->Configure(pDiag->pSys->hDiagMgr, ioctlArgs.ControlCode, pIoctlBuf, ioctlArgs.BufferSize) != COM_STATUS_SUCCESS)
		ret = -EIO;
	    break;
	case SDK2K_IOC_CONTROL:
	    if (copy_from_user(pIoctlBuf, (u8 *)ioctlArgs.Buffer, ioctlArgs.BufferSize)) {
		ret = -EFAULT;
		break;
	    }
	    if (pDiag->pDiagMgrInterface->Control(pDiag->pSys->hDiagMgr, ioctlArgs.ControlCode, pIoctlBuf, ioctlArgs.BufferSize) != COM_STATUS_SUCCESS)
		ret = -EIO;
	    break;
	case SDK2K_IOC_MONITOR:
	    if (pDiag->pDiagMgrInterface->Monitor(pDiag->pSys->hDiagMgr, ioctlArgs.ControlCode, pIoctlBuf, &ioctlArgs.BufferSize) != COM_STATUS_SUCCESS) {
		ret = -EIO;
	    }
	    if (copy_to_user((u8 *)arg, &ioctlArgs, sizeof(ioctlArgs))) {
		ret = -EFAULT;
	    }
	    if (copy_to_user(ioctlArgs.Buffer, pIoctlBuf, ioctlArgs.BufferSize)) {
		ret = -EFAULT;
	    }
	    break;
    }

    if(pIoctlBuf && (pIoctlBuf != ioctlBuf))
	OsFree(pIoctlBuf);

    return ret;
}

static int
diag_fasync(int fd, struct file *file, int on)
{
    diag_unit_t *su = file->private_data;
    int r;

#if defined(DMP) || defined(DMPRETAIL)
    if(su->minor == CNXTDIAGDMPMINOR) {
	r = fasync_helper(fd, file, on, &dmp_instance->fasync);
    } else
#endif
    {
	diag_instance_t *pDiag = su->inst.pDiag;

	if(!pDiag)
	    return -ENODEV;

	r = fasync_helper(fd, file, on, &pDiag->fasync);
    }
    return (r < 0) ? r : 0;
}

static unsigned int
diag_poll(struct file *file, poll_table * wait)
{
    diag_unit_t *su = file->private_data;
    unsigned int mask = 0;
    unsigned long flags;

#if defined(DMP) || defined(DMPRETAIL)
    if(su->minor == CNXTDIAGDMPMINOR) {
	poll_wait(file, &dmp_instance->read_wait, wait);
	spin_lock_irqsave(&dmp_instance->lock, flags);
	if (dmp_instance->putIdx != dmp_instance->takeIdx)
		mask |= POLLIN | POLLRDNORM;
	spin_unlock_irqrestore(&dmp_instance->lock, flags);
    } else
#endif
    {
	diag_instance_t *pDiag = su->inst.pDiag;

	if(!pDiag)
	    return -ENODEV;

	poll_wait(file, &pDiag->read_wait, wait);
	spin_lock_irqsave(&pDiag->lock, flags);
	if (pDiag->putIdx != su->takeIdx)
	    mask |= POLLIN | POLLRDNORM;
	spin_unlock_irqrestore(&pDiag->lock, flags);
    }

    return mask;
}

static ssize_t
diag_read(struct file * file, char * buf, size_t nbytes, loff_t *ppos)
{
    diag_unit_t *su = file->private_data;
    diag_instance_t *pDiag = su->inst.pDiag;
    ssize_t ret = 0;
    unsigned long flags;
    char *dp;
    size_t n;

#if defined(DMP) || defined(DMPRETAIL)
    if(su->minor == CNXTDIAGDMPMINOR)
	return dmp_read(file, buf, nbytes, ppos);
#endif

    if(!pDiag)
	return -ENODEV;

    if(nbytes % sizeof(SDK2K_NOTIFICATION)) {
	nbytes -= nbytes % sizeof(SDK2K_NOTIFICATION);
    }

    if(nbytes <= 0)
	return 0;

    spin_lock_irqsave(&pDiag->lock, flags);

    if((pDiag->putIdx == su->takeIdx) && (su->takeIdx >= 0)) {
	DECLARE_WAITQUEUE(wait, current);

	add_wait_queue(&pDiag->read_wait, &wait);

	while(pDiag->putIdx == su->takeIdx) {

	    set_current_state(TASK_INTERRUPTIBLE);
	    if (file->f_flags & O_NONBLOCK) {
		ret = -EAGAIN;
		break;
	    }
	    if (signal_pending(current)) {
		ret = -ERESTARTSYS;
		break;
	    }

	    spin_unlock_irqrestore(&pDiag->lock, flags);
	    schedule();
	    spin_lock_irqsave(&pDiag->lock, flags);
	}

	set_current_state(TASK_RUNNING);
	remove_wait_queue(&pDiag->read_wait, &wait);

	if(ret) {
	    spin_unlock_irqrestore(&pDiag->lock, flags);
	    return ret;
	}
    }

    if((pDiag->putIdx < 0) || (su->takeIdx < 0)) {
	spin_unlock_irqrestore(&pDiag->lock, flags);
	return -ENODEV;
    }

    dp = buf;
    do {

	if(pDiag->putIdx > su->takeIdx)
		n = pDiag->putIdx - su->takeIdx;
	else
		n = sizeof(pDiag->ringBuf) - su->takeIdx;

	if (n > nbytes) {
	    n = nbytes;
	}

	spin_unlock_irqrestore(&pDiag->lock, flags);
	if(copy_to_user(dp, &pDiag->ringBuf[su->takeIdx], n)) {
	    ret = -EFAULT;
	    spin_lock_irqsave(&pDiag->lock, flags);
	    break;
	}
	spin_lock_irqsave(&pDiag->lock, flags);

	su->takeIdx += n;
	if(su->takeIdx == sizeof(pDiag->ringBuf))
	    su->takeIdx = 0;

	dp += n;
	nbytes -= n;

    } while (nbytes && (pDiag->putIdx != su->takeIdx));

    spin_unlock_irqrestore(&pDiag->lock, flags);

    if(!ret) {
	ret = dp - buf;

	if(ret) {
	    TOUCH_ATIME(file);
	}
    }

    return ret;
}

static int diag_open(struct inode* inode, struct file*  file)
{
    int minor = minor(inode->i_rdev);
    diag_unit_t *su;
    unsigned long flags;

    su = OsAllocate(sizeof(*su));
    if(!su) {
	return -ENOMEM;
    }

    su->minor = minor;

    if(minor == CNXTDIAGDMPMINOR) {
#if defined(DMP) || defined(DMPRETAIL)
	if(!dmp_instance) {
	    /* allocation of big dmp_instance buffer is delayed until first open of CNXTDIAGDMPMINOR */
	    dmp_instance = OsAllocate(DMP_MAX_SEGMENTS * sizeof(DMP_ACTION_INFO));
	    if (!dmp_instance) {
		printk(KERN_ERR "%s: failed to allocate dmp_instance (%d bytes)", __FUNCTION__, DMP_MAX_SEGMENTS * sizeof(DMP_ACTION_INFO));
		OsFree(su);
		return -ENOMEM;
	    }
	    spin_lock_init(&dmp_instance->lock);
	    INIT_LIST_HEAD(&dmp_instance->units);
	    init_waitqueue_head(&dmp_instance->read_wait);
	}

	su->inst.pDmp = dmp_instance;

	spin_lock_irqsave(&su->inst.pDmp->lock, flags);
	list_add(&su->entry, &su->inst.pDmp->units);
	dmp_instance->takeIdx = su->inst.pDmp->putIdx;
	spin_unlock_irqrestore(&su->inst.pDmp->lock, flags);
#else
	return -ENODEV;
#endif

    } else {
	su->inst.pDiag = GetDiagInstance(minor, NULL, NULL, NULL);
	if(!su->inst.pDiag) {
	    OsFree(su);
	    return -ENODEV;
	}

	spin_lock_irqsave(&su->inst.pDiag->lock, flags);
	list_add(&su->entry, &su->inst.pDiag->units);
	su->takeIdx = su->inst.pDiag->putIdx;
	spin_unlock_irqrestore(&su->inst.pDiag->lock, flags);

    }

    file->private_data = su;

    return 0;
}

static int diag_release(struct inode* inode, struct file*  file)
{
    diag_unit_t *su = file->private_data;
    diag_instance_t *pDiag = su->inst.pDiag;
    unsigned long flags;

    diag_fasync(-1, file, 0);

    if(pDiag) { /* also for pDmp; we rely on lock being at same location */
    	spin_lock_irqsave(&pDiag->lock, flags);
    	list_del(&su->entry);
    	su->takeIdx = -1;
    	spin_unlock_irqrestore(&pDiag->lock, flags);
    }

    OsFree(su);

    return 0;
}

static int diagmajor = CNXTDIAGMAJOR;
#ifdef FOUND_MODULE_PARAM
module_param(diagmajor, int, 0);
#else
MODULE_PARM(diagmajor, "i");
#endif
MODULE_PARM_DESC(diagmajor, "Major device number for diag device");

__shimcall__
COM_STATUS OsDiagMgrClose(HANDLE hOsDiagMgr)
{
    diag_instance_t *pDiag = hOsDiagMgr;
    unsigned long flags;

    if(diagdebug)
	printk("%s: hOsDiagMgr=%p\n", __FUNCTION__, hOsDiagMgr);

    if(pDiag) {
	pDiag->pSys = NULL;

	spin_lock_irqsave(&diag_lock, flags);
	list_del(&pDiag->entry);
	spin_unlock_irqrestore(&diag_lock, flags);

	PutDiagInstance(pDiag);
    }

    return COM_STATUS_SUCCESS;
}

static struct file_operations diag_fops = {
    .owner	= THIS_MODULE,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
    .ioctl	= diag_ioctl,
#endif
    .fasync	= diag_fasync,
    .poll	= diag_poll,
    .read	= diag_read,
    .open	= diag_open,
    .release	= diag_release
};

static diag_instance_t *GetDiagInstance(int hwInstNum,
	void *hwDevLink,
	PSYSTEM_INSTANCES_T pSys, PI_DIAG_MGR_T pDiagMgrInterface)
{
    diag_instance_t *pDiag, *pOldDiag = NULL;
    struct list_head *e;
    unsigned long flags;

    pDiag = OsAllocate(sizeof(*pDiag));
    if(!pDiag)
	return NULL;

    spin_lock_init(&pDiag->lock);
    init_waitqueue_head(&pDiag->read_wait);
    INIT_LIST_HEAD(&pDiag->units);

    pDiag->pSys = pSys;
    pDiag->pDiagMgrInterface = pDiagMgrInterface;
    pDiag->hwInstNum = hwInstNum;

    spin_lock_irqsave(&diag_lock, flags);
    list_for_each (e, &diag_instance_list) {
	if((list_entry(e, diag_instance_t, entry))->hwInstNum == hwInstNum) {
	    pOldDiag = list_entry(e, diag_instance_t, entry);
	    break;
	}
    }

    if(pOldDiag) {
	unsigned long flags2;

	spin_lock_irqsave(&pOldDiag->lock, flags2);
	if(pOldDiag->pSys && pSys) {
	    spin_unlock_irqrestore(&pOldDiag->lock, flags2);
	    spin_unlock_irqrestore(&diag_lock, flags);
	    printk(KERN_ERR "%s: entry (%d) already active!", __FUNCTION__, hwInstNum);
	    OsFree(pDiag);
	    return NULL;
	}
	if(pSys) {
	    pOldDiag->pSys = pSys;
	    pOldDiag->pDiagMgrInterface = pDiagMgrInterface;
	}
	spin_unlock_irqrestore(&pOldDiag->lock, flags2);
	spin_unlock_irqrestore(&diag_lock, flags);
	OsFree(pDiag);
	pDiag = pOldDiag;

    } else {
	list_add(&pDiag->entry, &diag_instance_list);
	spin_unlock_irqrestore(&diag_lock, flags);
    }

    if(!pOldDiag) {
	char buf[32];

	snprintf(buf, sizeof(buf), CNXTTARGET"diag%d", pDiag->hwInstNum);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	pDiag->devfs_handle = devfs_register(NULL, buf, DEVFS_FL_DEFAULT,
		diagmajor, pDiag->hwInstNum,
		S_IFCHR | S_IRUSR | S_IWUSR,
		&diag_fops,
		NULL);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
	if (!IS_ERR(diag_class)) {
	    CLASS_DEVICE_CREATE(diag_class, MKDEV(diagmajor, pDiag->hwInstNum), hwDevLink, CNXTTARGET"diag%d", pDiag->hwInstNum);
	}
#endif
#ifdef FOUND_DEVFS
	devfs_mk_cdev(MKDEV(diagmajor, pDiag->hwInstNum), S_IFCHR | S_IRUSR | S_IWUSR, buf);
#endif
#endif
    }

    return pDiag;
}

__shimcall__
HANDLE OsDiagMgrOpen(PSYSTEM_INSTANCES_T pSys, PI_DIAG_MGR_T pDiagMgrInterface)
{
    if(diagdebug)
	printk("%s: pSys=%p hDevNode=%p\n", __FUNCTION__, pSys, pSys->hDevNode);

    if(!pSys->hDevNode) {
	printk(KERN_ERR"%s: failed; hDevNode is NULL\n", __FUNCTION__);
	return NULL;
    }

    return GetDiagInstance(
	    ((POS_DEVNODE)pSys->hDevNode)->hwInstNum,
	    ((POS_DEVNODE)pSys->hDevNode)->hwDevLink,
	    pSys, pDiagMgrInterface);
}
 
__shimcall__
COM_STATUS OsDiagMgrNotify(HANDLE hOsDiagMgr, CNXT_DIAG_CODES Code, UINT32 dwData)
{
    diag_instance_t *pDiag = hOsDiagMgr;
    unsigned long flags;
    SDK2K_NOTIFICATION Notification;

    if(diagdebug)
	printk("%s: hOsDiagMgr=%p Code=%d dwData=0x%x\n", __FUNCTION__, hOsDiagMgr, Code, dwData);

    if(!hOsDiagMgr)
	return COM_STATUS_INVALID_HANDLE;

    Notification.Code = Code;
    Notification.Data = dwData;

    spin_lock_irqsave(&pDiag->lock, flags);
    if (list_empty(&pDiag->units) || (pDiag->putIdx < 0)) {
	spin_unlock_irqrestore(&pDiag->lock, flags);
	return COM_STATUS_INST_NOT_INIT;
    }

    Notification.TimeStamp = OsGetSystemTime();

    /* this assumes that sizeof(ringBuf) is a multiple of sizeof(Notification) */
    memcpy(&pDiag->ringBuf[pDiag->putIdx], &Notification, sizeof(Notification));
    pDiag->putIdx += sizeof(Notification);
    if(pDiag->putIdx == sizeof(pDiag->ringBuf))
	pDiag->putIdx = 0;

    spin_unlock_irqrestore(&pDiag->lock, flags);

    kill_fasync(&pDiag->fasync, SIGIO, POLL_IN);
    wake_up_interruptible_all(&pDiag->read_wait);

    return COM_STATUS_SUCCESS;
}

static void PutDiagInstance(diag_instance_t *pDiag)
{
    unsigned long flags;
    struct list_head *e;
    int i;

    spin_lock_irqsave(&pDiag->lock, flags);

    pDiag->putIdx = -2;

    for(i = 0; !list_empty(&pDiag->units); i++) {
	if(i == 1) {
	    list_for_each (e, &pDiag->units) {
		(list_entry(e, diag_unit_t, entry))->takeIdx = -1;
		//(list_entry(e, diag_unit_t, entry))->inst.pDiag = NULL;
	    }
	}
	spin_unlock_irqrestore(&pDiag->lock, flags);
	if(i >= 1) {
	    if((i % 20) == 10)
		printk(KERN_ERR"%s: units still active, waiting..\n", __FUNCTION__);
	    kill_fasync(&pDiag->fasync, SIGIO, POLL_IN);
	    wake_up_interruptible_all(&pDiag->read_wait);
	}
	OsSleep(100);
	spin_lock_irqsave(&pDiag->lock, flags);
    }
    spin_unlock_irqrestore(&pDiag->lock, flags);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    devfs_unregister(pDiag->devfs_handle);
#else
#ifdef FOUND_DEVFS
    {
	char buf[32];
	snprintf(buf, sizeof(buf), CNXTTARGET"diag%d", pDiag->hwInstNum);
	devfs_remove(buf);
    }
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    if (!IS_ERR(diag_class))
	CLASS_DEVICE_DESTROY(diag_class, MKDEV(diagmajor, pDiag->hwInstNum));
#endif
#endif

    OsFree(pDiag);
}

__shimcall__
void OsDiagExit(void)
{
    diag_instance_t *pDiag;
    unsigned long flags;
#if defined(DMP) || defined(DMPRETAIL)
    struct list_head *e;
    int i;
#endif

    /* Free all diag_instances */
    spin_lock_irqsave(&diag_lock, flags);
    while(diag_instance_list.next != &diag_instance_list) {
	pDiag = list_entry(diag_instance_list.next, diag_instance_t, entry);

	list_del(&pDiag->entry);
	spin_unlock_irqrestore(&diag_lock, flags);

	PutDiagInstance(pDiag);

	spin_lock_irqsave(&diag_lock, flags);
    }
    spin_unlock_irqrestore(&diag_lock, flags);

#if defined(DMP) || defined(DMPRETAIL)
    /* Free dmp_instance */
    if(dmp_instance) {
	spin_lock_irqsave(&dmp_instance->lock, flags);
	dmp_instance->putIdx = -2;
	spin_unlock_irqrestore(&dmp_instance->lock, flags);

	//call_diag_daemon(dmp_instance->hwInstNum, "stop");

	spin_lock_irqsave(&dmp_instance->lock, flags);
	for(i = 0; !list_empty(&dmp_instance->units); i++) {
	    if(i == 1) {
		list_for_each (e, &dmp_instance->units) {
		    (list_entry(e, diag_unit_t, entry))->takeIdx = -1;
		    //(list_entry(e, diag_unit_t, entry))->inst.pDmp = NULL;
		}
	    }
	    spin_unlock_irqrestore(&dmp_instance->lock, flags);
	    if(i >= 1) {
		if((i % 20) == 10)
		    printk(KERN_ERR"%s: units still active, waiting..\n", __FUNCTION__);
		kill_fasync(&dmp_instance->fasync, SIGIO, POLL_IN);
		wake_up_interruptible_all(&dmp_instance->read_wait);
	    }
	    OsSleep(100);
	    spin_lock_irqsave(&dmp_instance->lock, flags);
	}
	spin_unlock_irqrestore(&dmp_instance->lock, flags);

	OsFree(dmp_instance);
	dmp_instance = NULL;
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    devfs_unregister(dmp_devfs_handle);
#else
#ifdef FOUND_DEVFS
    devfs_remove(CNXTTARGET"diagdmp");
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    if (!IS_ERR(diag_class))
	CLASS_DEVICE_DESTROY(diag_class, MKDEV(diagmajor, CNXTDIAGDMPMINOR));
#endif
#endif
#endif /* DMP || DMP_RETAIL */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    if (!IS_ERR(diag_class))
	CLASS_DESTROY(diag_class);
#endif
    if(diagmajor > 0)
	unregister_chrdev(diagmajor, CNXTTARGET"diag");
}

__shimcall__
int OsDiagInit(void)
{
    int ret;

    if(diagmajor == 0) {
	diagmajor = devfs_register_chrdev(0, CNXTTARGET"diag", &diag_fops);
    } else if((ret=devfs_register_chrdev(diagmajor, CNXTTARGET"diag", &diag_fops))) {
	diagmajor = ret;
    }

    if(diagmajor < 0) {
	printk(KERN_ERR "%s: cannot register chrdev (%d)\n", __FUNCTION__, diagmajor);

	return diagmajor;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    diag_class = CLASS_CREATE(THIS_MODULE, CNXTTARGET"diag");
    if (IS_ERR(diag_class)) {
	printk(KERN_ERR "%s: cannot create simple class (%ld)\n", __FUNCTION__, PTR_ERR(diag_class));
	if(diagmajor > 0)
	    unregister_chrdev(diagmajor, CNXTTARGET"diag");
	return PTR_ERR(diag_class);
    }
#endif

#if defined(DMP) || defined(DMPRETAIL)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    dmp_devfs_handle = devfs_register(NULL, CNXTTARGET"diagdmp", DEVFS_FL_DEFAULT,
	    diagmajor, CNXTDIAGDMPMINOR,
	    S_IFCHR | S_IRUSR | S_IWUSR,
	    &diag_fops,
	    NULL);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,2)
    if (!IS_ERR(diag_class)) {
	CLASS_DEVICE_CREATE(diag_class, MKDEV(diagmajor, CNXTDIAGDMPMINOR), NULL, CNXTTARGET"diagdmp");
    }
#endif
#ifdef FOUND_DEVFS
    devfs_mk_cdev(MKDEV(diagmajor, CNXTDIAGDMPMINOR), S_IFCHR | S_IRUSR | S_IWUSR, CNXTTARGET"diagdmp");
#endif
#endif
#endif /* DMP || DMP_RETAIL */

    return 0;
}

#endif /* NO_DIAGMGR_SUPPORT */
