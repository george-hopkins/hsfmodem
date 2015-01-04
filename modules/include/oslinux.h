/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */
#ifndef __OSLINUX_H
#define __OSLINUX_H

#ifdef __cplusplus
extern "C"
{
#endif

__shimcall__
int OsInit(void);

__shimcall__
int OsDcpInit(void);
__shimcall__
void OsDcpExit(void);
__shimcall__
void OsDcpEnsureDaemonIsRunning(int hwInstNum);

__shimcall__
int OsScrInit(void);
__shimcall__
void OsScrExit(void);

#ifdef __cplusplus
}
#endif

#endif /* __OSLINUX_H */
