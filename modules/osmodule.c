/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

/*
 * File Description: Functions to find/release kernel symbols in other modules.
 */

#include "oscompat.h"
#include <linux/kmod.h>

#include "osservices.h"

__shimcall__
const void *OsModuleSymGet(const PCHAR mod, const PCHAR sym)
{
	const void *addr;

	//printk(KERN_DEBUG "%s(mod=%s, sym=%s)\n", __FUNCTION__, mod, sym);
	ASSERT(OsContextAllowsSleeping());

#ifdef HAVE_INTER_MODULE
	addr = inter_module_get_request(sym, mod);
#else
	request_module(mod);
	       
	addr = (const void *)get_module_symbol(mod, sym);
#endif

	//printk(KERN_DEBUG "%s(%s, %s): addr=0x%p\n", __FUNCTION__, mod, sym, addr);

	return addr;
}

__shimcall__
void OsModuleSymPut(const PCHAR mod, const PCHAR sym)
{
	//printk(KERN_DEBUG "%s(mod=%s, sym=%s)\n", __FUNCTION__, mod, sym);
#ifdef HAVE_INTER_MODULE
	inter_module_put(sym);
#endif
}
