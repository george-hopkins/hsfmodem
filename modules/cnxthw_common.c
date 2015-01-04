/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#include "serial_cnxt.h"

#ifdef CONFIG_PM
#include <linux/pm.h>
#endif

__shimcall__
static COM_STATUS cnxthw_DevMgrPMControl(POS_DEVNODE pDevNode,
	__shimcall__ COM_STATUS (*pmControlFunc)(HANDLE hDevMgr, ePmeState PmeState),
	HANDLE hDevMgr, ePmeState PmeState, ePmeState OldPmeState)
{
    COM_STATUS ret;
    BOOL enabled = ((OldPmeState == DEVMGR_D0) || (OldPmeState == DEVMGR_QUICK_D0));
    BOOL enabling = ((PmeState == DEVMGR_D0) || (PmeState == DEVMGR_QUICK_D0));

    //printk(KERN_DEBUG"%s: %d -> %d\n", __FUNCTION__, OldPmeState, PmeState);

    if(!enabled && enabling) {
	pDevNode->hwInUse++;
	OsModuleUseCountInc();
	ret = pmControlFunc(hDevMgr, PmeState);
	if(ret != COM_STATUS_SUCCESS) {
	    OsModuleUseCountDec();
	    pDevNode->hwInUse--;
	}
    } else if(pDevNode->hwInUse && enabled && !enabling) {
	ret = pmControlFunc(hDevMgr, PmeState);
	if(ret == COM_STATUS_SUCCESS) {
	    OsModuleUseCountDec();
	    pDevNode->hwInUse--;
	}
    } else
	return pmControlFunc(hDevMgr, PmeState);

    return ret;
}

