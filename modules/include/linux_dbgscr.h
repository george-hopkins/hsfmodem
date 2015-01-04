/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

/*
 * linux_dbgscr.h - header file for linux_dbgscr.c
 */

#ifndef _LINUX_DBGSCR_H_
#define _LINUX_DBGSCR_H_


#ifdef __cplusplus
extern "C" {
#endif

__shimcall__
	BOOL LinuxScrSetEntryPointCallback( PVOID epcbk );

#ifdef __cplusplus
}
#endif


#endif
