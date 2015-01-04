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
#include "comctrl_ex.h"

MODULE_AUTHOR("Copyright (C) 1996-2003 Conexant Systems Inc.");
MODULE_DESCRIPTION("Conexant modem engine");
MODULE_LICENSE("see LICENSE file distributed with driver");
MODULE_INFO(supported, "yes");

EXPORT_SYMBOL_NOVERS(ComCtrl_Create);
EXPORT_SYMBOL_NOVERS(ComCtrl_Destroy);
EXPORT_SYMBOL_NOVERS(ComCtrl_Open);
EXPORT_SYMBOL_NOVERS(ComCtrl_Close);
EXPORT_SYMBOL_NOVERS(ComCtrl_Configure);
EXPORT_SYMBOL_NOVERS(ComCtrl_Monitor);
EXPORT_SYMBOL_NOVERS(ComCtrl_Control);
EXPORT_SYMBOL_NOVERS(ComCtrl_Read);
EXPORT_SYMBOL_NOVERS(ComCtrl_Write);

static int __init
cnxtengine_init(void)
{
#ifdef TARGET_HSF_LINUX
	__shimcall__ int HsfEngineInit(void);

	return HsfEngineInit();
#else
	return 0;
#endif
}

static void __exit
cnxtengine_exit(void)
{
#ifdef TARGET_HSF_LINUX
	__shimcall__ void HsfEngineExit(void);

	HsfEngineExit();
#endif
}

module_init(cnxtengine_init);
module_exit(cnxtengine_exit);

