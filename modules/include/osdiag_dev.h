/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */
#ifndef __OSDIAG_IOCTL_H__
#define __OSDIAG_IOCTL_H__

#include <linux/ioctl.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    DWORD		TimeStamp;
    CNXT_DIAG_CODES	Code;
    UINT32		Data;
} SDK2K_NOTIFICATION, *PSDK2K_NOTIFICATION;

typedef struct {
    CNXT_DIAG_CTRL_CODES ControlCode;
    PVOID Buffer;
    UINT32 BufferSize;
} SDK2K_IOCTLARGS, *PSDK2K_IOCTLARGS;

#define SDK2K_IOC_CONFIGURE	_IOWR('2', 0, SDK2K_IOCTLARGS)
#define SDK2K_IOC_CONTROL	_IOWR('2', 1, SDK2K_IOCTLARGS)
#define SDK2K_IOC_MONITOR	_IOWR('2', 2, SDK2K_IOCTLARGS)

#ifdef __KERNEL__
__shimcall__
int OsDiagInit(void);
__shimcall__
void OsDiagExit(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __OSDIAG_IOCTL_H__ */
