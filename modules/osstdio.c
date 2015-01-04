/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

/*************************************************************************

  File Description: Functions to emulate stdio within the kernel.

  ************************************************************************/

#include "oscompat.h"
#include "osservices.h"
#include "osstdio.h"
#include "testdebug.h"

#include <linux/fs.h>
#include <asm/uaccess.h>

#ifdef FOUND_CURRENT_CRED
static int set_current_fsuid(uid_t fsuid)
{
	struct cred *new;

	new = prepare_creds();
	if (!new)
		return -ENOMEM;

	new->fsuid = fsuid;

	return commit_creds(new);
}
#endif

__shimcall__
FILE *
OsFOpen (const char *path, const char *mode, int *errp)
{
	uid_t origfsuid;
	FILE *filp;
	int error = 0;
	int flags, creatmode = 0, do_trunc = 0;
	struct inode * inode;

	ASSERT(OsContextAllowsSleeping());

	if(!path || !mode) {
		error = -EIO;
		filp = NULL;
		goto out;
	}

	//printk(KERN_INFO "%s(\"%s\", \"%s\")\n", __FUNCTION__, path, mode); //KDB_ENTER();

	if(!strcmp(mode, "r") || !strcmp(mode, "rb")) {
		flags = O_RDONLY;
	} else if(!strcmp(mode, "r+") || !strcmp(mode, "r+b") || !strcmp(mode, "rb+")) {
		flags = O_RDWR;
	} else if(!strcmp(mode, "w") || !strcmp(mode, "wb")) {
		flags = O_WRONLY | O_CREAT;
		do_trunc = 1;
		creatmode = 0600;
	} else if(!strcmp(mode, "w+") || !strcmp(mode, "w+b") || !strcmp(mode, "wb+")) {
		flags = O_RDWR | O_CREAT;
		do_trunc = 1;
		creatmode = 0600;
	} else {
		error = -ENOSYS; // not implemented
		filp = NULL;
		goto out;
	}

#if 0
	flags |= O_NOFOLLOW;
#endif

again:
#ifdef FOUND_CURRENT_CRED
	origfsuid = current_fsuid();
	error = set_current_fsuid(0);
	if (error) {
		filp = NULL;
		goto out;
	}
#else
	origfsuid = current->fsuid;
	current->fsuid = 0;
#endif

	filp = filp_open(path, flags, creatmode);

#ifdef FOUND_CURRENT_CRED
	error = set_current_fsuid(origfsuid);
	if (error) {
		if (IS_ERR(filp))
			filp = NULL;
		goto out;
	}
#else
	current->fsuid = origfsuid;
#endif

	if (IS_ERR(filp)) {
		error = PTR_ERR(filp);
		filp = NULL;
		goto out;
	}

	inode = filp->f_dentry->d_inode;
#if 0
	if (inode->i_nlink > 1) {
		error = -EACCES;
		printk(KERN_ERR "%s(\"%s\", \"%s\"): file has multiple links\n", __FUNCTION__, path, mode);
		filp_close(filp, NULL);
		filp = NULL;
		goto out;
	}
#endif

	if (!(S_ISREG(inode->i_mode))) {
		error = -EACCES;
		printk(KERN_ERR "%s(\"%s\", \"%s\"): not a regular file\n", __FUNCTION__, path, mode);
		filp_close(filp, NULL);
		filp = NULL;
		goto out;
	}

	if (!filp->f_op || !filp->f_op->read || !filp->f_op->write) {
		error = -EIO;
		filp_close(filp, NULL);
		filp = NULL;
		goto out;
	}

	if(do_trunc) {
		do_trunc = 0;
		filp_close(filp, NULL);
		filp = NULL;
		flags |= O_TRUNC;
		goto again;
	}

out:
	if(errp)
		*errp = error;
	else if(error && (error != -ENOENT))
		printk(KERN_ERR "%s(\"%s\", \"%s\"): error %d\n", __FUNCTION__, path, mode, error);

	if (filp) {
		filp->f_pos = 0;            /* start offset */
	}

	return filp;
}

__shimcall__
int
OsFClose(FILE *filp)
{
	//printk(KERN_INFO "%s(%p)\n", __FUNCTION__, filp); //KDB_ENTER();
	if(!IS_ERR(filp)) {
		filp_close(filp, NULL);
	}
	return 0;
}

__shimcall__
size_t
OsFRead(void *ptr, size_t size, size_t nmemb, FILE *filp, int *errno_p)
{
	int bytes;

	ASSERT(OsContextAllowsSleeping());
	//printk(KERN_INFO "%s(ptr=%p, size=%d, nmemb=%d, filp=%p)\n", __FUNCTION__, ptr, size, nmemb, filp); //KDB_ENTER();
	if(IS_ERR(filp)) {
		bytes = -EINVAL;
	} else {
		mm_segment_t oldfs;

		oldfs = get_fs();
		set_fs(get_ds());
		bytes = filp->f_op->read(filp, ptr, size*nmemb, &filp->f_pos);
		set_fs(oldfs);
	}

	if (errno_p && bytes < 0) *errno_p = -(bytes);

	if(bytes < 0) {
		printk(KERN_ERR "%s(filp %p): error %d\n", __FUNCTION__, filp, bytes);
		return 0;
	} else
		return bytes / size;
}

__shimcall__
size_t
OsFWrite(const void *ptr, size_t size, size_t nmemb, FILE *filp, int *errno_p)
{
	int bytes;

	ASSERT(OsContextAllowsSleeping());
	//printk(KERN_INFO "%s(ptr=%p, size=%d, nmemb=%d, filp=%p)\n", __FUNCTION__, ptr, size, nmemb, filp); //KDB_ENTER();
	if(IS_ERR(filp)) {
		bytes = -EINVAL;
	} else {
		mm_segment_t oldfs;

		oldfs = get_fs();
		set_fs(get_ds());
		bytes = filp->f_op->write(filp, ptr, size*nmemb, &filp->f_pos);
		set_fs(oldfs);
	}

	if (errno_p && bytes < 0) *errno_p = -(bytes);

	if(bytes < 0) {
		printk(KERN_ERR "%s(filp %p): error %d\n", __FUNCTION__, filp, bytes);
		return 0;
	} else
		return bytes / size;
}


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
static loff_t default_llseek(struct file *file, loff_t offset, int origin)
{
	long long retval;

	switch (origin) {
		case 2:
			offset += file->f_dentry->d_inode->i_size;
			break;
		case 1:
			offset += file->f_pos;
	}
	retval = -EINVAL;
	if (offset >= 0) {
		if (offset != file->f_pos) {
			file->f_pos = offset;
			file->f_reada = 0;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,2,14)
			file->f_version = ++event;
#else
			file->f_version = ++global_event;
#endif
		}
		retval = offset;
	}
	return retval;
}
#endif

__shimcall__
int
OsFSeek(FILE *filp, long offset, int origin)
{
	loff_t (*fn)(struct file *, loff_t, int);
	loff_t retval;
	mm_segment_t oldfs;

	ASSERT(OsContextAllowsSleeping());
	//printk(KERN_INFO "%s(filp=%p, offset=%ld, origin=%d)\n", __FUNCTION__, filp, offset, origin); //KDB_ENTER();
	if(IS_ERR(filp)) {
		return -EINVAL;
	}

	if (filp->f_op && filp->f_op->llseek)
		fn = filp->f_op->llseek;
	else
		fn = default_llseek;

	oldfs = get_fs();
	set_fs(get_ds());
	retval = fn(filp, offset, origin);
	set_fs(oldfs);
	if(retval < 0) {
		printk(KERN_ERR "%s(filp %p, offset=%ld origin=%d): error %d\n", __FUNCTION__, filp, offset, origin, (int)retval);
	}
	return retval;
}
