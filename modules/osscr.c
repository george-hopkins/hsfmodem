/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

/* This module has been obsoleted by osdiag/blam */

#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/poll.h>
#include <linux/slab.h>

#include "oscompat.h"
#include "osservices.h"
#include "scrcommon.h"
#include "msgqueue.h"
#include "dbgsrv.h"

typedef struct {
	int minor;
} scr_unit_t;


static DECLARE_WAIT_QUEUE_HEAD(scr_read_wait);

static unsigned int
scr_poll(struct file *file, poll_table * wait)
{
    scr_unit_t *su = file->private_data;
    unsigned int mask = 0;

    poll_wait(file, &scr_read_wait, wait);
    if(DbgSrvLinuxAvailNumOfRecords() > 0)
	mask |= POLLIN | POLLRDNORM;
    return mask;
}

static void
scr_wake_app(void)
{
    wake_up_interruptible(&scr_read_wait);
}

static ssize_t
scr_read(struct file * file, char * buf, size_t nbytes, loff_t *ppos)
{
    DECLARE_WAITQUEUE(wait, current);
    QUEUE_RECORD qRec;
    QUEUE_RECORD	*pSegments = (QUEUE_RECORD *)buf;
    size_t nSegs = nbytes / sizeof(QUEUE_RECORD);
    ssize_t retval = 0;

    if(nbytes % sizeof(QUEUE_RECORD)) {
	printk(KERN_ERR "%s: invalid size %d; must be a multiple of %d bytes\n", __FUNCTION__, nbytes, sizeof(QUEUE_RECORD));
    }

    if(nSegs <= 0)
	return 0;

    add_wait_queue(&scr_read_wait, &wait);

    while(nSegs > 0) {
	set_current_state(TASK_INTERRUPTIBLE);

	if(!DbgSrvLinuxGetQueueRecord(&qRec)) {
	    if (file->f_flags & O_NONBLOCK) {
		if(!retval)
		    retval = -EAGAIN;
		break;
	    }
	    if (signal_pending(current)) {
		retval = -ERESTARTSYS;
		break;
	    }
	    schedule();
	    continue;
	}

	if(copy_to_user( pSegments, &qRec, sizeof(QUEUE_RECORD) )) {
	    retval = -EFAULT;
	    break;
	}
	nSegs--;
	pSegments++;

	TOUCH_ATIME(file);

	retval += sizeof(QUEUE_RECORD);
    }

    set_current_state(TASK_RUNNING);
    remove_wait_queue(&scr_read_wait, &wait);
    return retval;
}

static int
scr_open(struct inode* inode, struct file*  file)
{
    int minor = minor(inode->i_rdev);
    scr_unit_t *su;

    su = kmalloc(sizeof(*su), GFP_KERNEL);
    if(!su)
	return -ENOMEM;

    DbgSrvLinuxOnOpenHandle();

    su->minor = minor;

    file->private_data = su;

    OsModuleUseCountInc();

    return 0;
}

static int
scr_release(struct inode* inode, struct file*  file)
{
    int minor = minor(inode->i_rdev);
    scr_unit_t *su = file->private_data;

    DbgSrvLinuxOnCloseHandle();

    kfree(su);
    OsModuleUseCountDec();

    return 0;
}

static struct file_operations scr_fops =
{
	owner:            THIS_MODULE,
	read:             scr_read,
	poll:             scr_poll,
	open:             scr_open,
	release:          scr_release,
};

static int scrmajor = CNXTSCRMAJOR;
#ifdef FOUND_MODULE_PARAM
module_param(scrmajor, int, 0);
#else
MODULE_PARM(scrmajor, "i");
#endif
MODULE_PARM_DESC(scrmajor, "Major device number for scr device");

__shimcall__
void OsScrExit(void)
{
    if(scrmajor > 0)
	unregister_chrdev(scrmajor, CNXTTARGET"scr");

    DbgSrvLinuxCleanup();
}

__shimcall__
int OsScrInit(void)
{
    int ret;

    if(!DbgSrvLinuxInit(scr_wake_app)) {
	printk(KERN_ERR CNXTTARGET"scr: cannot initialize DbgSrv\n");
	return -EIO;
    }

    if(scrmajor == 0)
	scrmajor = register_chrdev(0, CNXTTARGET"scr", &scr_fops);
    else if((ret=register_chrdev(scrmajor, CNXTTARGET"scr", &scr_fops))) {
	scrmajor = ret;
    }

    if(scrmajor < 0) {
	printk(KERN_ERR CNXTTARGET"scr: cannot register chrdev\n");
	OsScrExit();
	return -EIO;
    }

    return 0;
}
