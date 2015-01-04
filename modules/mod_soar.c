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
#include "comtypes.h"

PVOID GetSOARLibInterface(void);

EXPORT_SYMBOL_NOVERS(GetSOARLibInterface);

MODULE_AUTHOR("Copyright (C) 1996-2001 Conexant Systems Inc.");
MODULE_DESCRIPTION("HSF module for SmartDAA(tm) devices");
MODULE_LICENSE("GPL\0for files in the \"GPL\" directory; for others, only LICENSE file applies");
MODULE_INFO(supported, "yes");


static int __init
cnxtsoar_init(void)
{
	return 0;
}

static void __exit
cnxtsoar_exit(void)
{
}

module_init(cnxtsoar_init);
module_exit(cnxtsoar_exit);

