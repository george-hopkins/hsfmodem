/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */
#ifndef __OSMODULE_H__
#define __OSMODULE_H__

#ifdef __cplusplus
extern "C"
{
#endif

__shimcall__
const void *OsModuleSymGet(const PCHAR mod, const PCHAR sym);

__shimcall__
void OsModuleSymPut(const PCHAR mod, const PCHAR sym);

//void OsModuleUseCountInc(void);
//void OsModuleUseCountDec(void);

#ifdef __cplusplus
}
#endif

#endif /* __OSMODULE_H__ */
