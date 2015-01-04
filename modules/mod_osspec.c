/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#define OS_NOSTRINGREDEFS

#include "oscompat.h"
#include "framewrk.h"
#include "osservices.h"

#include "osmemory.h"

#include "osresour_ex.h"
#include "osnvm.h"
#include "oslinux.h"

#if !TARGET_HCF_FAMILY
#include "osmemory_ex.h"
#endif

#if defined(TARGET_HCF_USB_LINUX) || defined(TARGET_HSF_LINUX)
#include "osusb.h"
#endif

#if !defined NO_DIAGMGR_SUPPORT
#include "intfctrl_ex.h"
#include "osdiag.h"
#include "osdiag_dev.h"
#endif

#if !defined NO_BLAM_SUPPORT
#include "rpt_io.h"
#else
#if defined(SCR) && defined(SCR_TO_OSDEBUGPRINT)
#undef SCR
#endif
#ifdef SCR
#include "cdbgscr_ex.h"
#include "scrcommon.h"
#include "msgqueue.h"
#include "dbgsrv.h"
#endif
#endif

#if defined(DMP) || defined(DMPRETAIL)
#include "dmpdefs.h"
#endif

#ifdef USE_DCP
#include "dcp.h"
#endif

EXPORT_SYMBOL_NOVERS(OsAllocate);
EXPORT_SYMBOL_NOVERS(OsFree);

EXPORT_SYMBOL_NOVERS(OsAtomicAdd);
EXPORT_SYMBOL_NOVERS(OsAtomicCompareAndSwapEx);
EXPORT_SYMBOL_NOVERS(OsAtomicDecrement);
EXPORT_SYMBOL_NOVERS(OsAtomicIncrement);

EXPORT_SYMBOL_NOVERS(OsCreatePeriodicTimer);
EXPORT_SYMBOL_NOVERS(OsDestroyPeriodicTimer);
EXPORT_SYMBOL_NOVERS(OsSetPeriodicTimer);
EXPORT_SYMBOL_NOVERS(OsImmediateTimeOut);

EXPORT_SYMBOL_NOVERS(OsGetCurrentThread);
EXPORT_SYMBOL_NOVERS(OsGetSystemTime);

EXPORT_SYMBOL_NOVERS(OsCriticalSectionCreate);
EXPORT_SYMBOL_NOVERS(OsCriticalSectionDestroy);
EXPORT_SYMBOL_NOVERS(OsCriticalSectionAcquire);
EXPORT_SYMBOL_NOVERS(OsCriticalSectionRelease);

EXPORT_SYMBOL_NOVERS(OsLockCreate);
EXPORT_SYMBOL_NOVERS(OsLockDestroy);
EXPORT_SYMBOL_NOVERS(OsLockLock);
EXPORT_SYMBOL_NOVERS(OsLockUnlock);
EXPORT_SYMBOL_NOVERS(OsLockTry);
EXPORT_SYMBOL_NOVERS(OsLockTryUnlock);

EXPORT_SYMBOL_NOVERS(OsSleep);

EXPORT_SYMBOL_NOVERS(OsStrCpy);
EXPORT_SYMBOL_NOVERS(OsStrnCpy);
EXPORT_SYMBOL_NOVERS(OsStrCat);
EXPORT_SYMBOL_NOVERS(OsStrnCat);
EXPORT_SYMBOL_NOVERS(OsStrCmp);
EXPORT_SYMBOL_NOVERS(OsStrnCmp);
EXPORT_SYMBOL_NOVERS(OsToupper);
EXPORT_SYMBOL_NOVERS(OsTolower);
EXPORT_SYMBOL_NOVERS(OsIsDigit);
EXPORT_SYMBOL_NOVERS(OsSprintf);
EXPORT_SYMBOL_NOVERS(OsVSprintf);
EXPORT_SYMBOL_NOVERS(OsStrLen);
EXPORT_SYMBOL_NOVERS(OsAtoi);
EXPORT_SYMBOL_NOVERS(OsMemSet);
EXPORT_SYMBOL_NOVERS(OsMemCpy);
EXPORT_SYMBOL_NOVERS(OsMemCmp);
EXPORT_SYMBOL_NOVERS(OsMemMove);

EXPORT_SYMBOL_NOVERS(OsRawVPrintf);
EXPORT_SYMBOL_NOVERS(OsRawPrintf);
EXPORT_SYMBOL_NOVERS(OsRawNLPrintf);
EXPORT_SYMBOL_NOVERS(OsErrorVPrintf);
EXPORT_SYMBOL_NOVERS(OsErrorPrintf);
EXPORT_SYMBOL_NOVERS(OsDebugVPrintf);
EXPORT_SYMBOL_NOVERS(OsDebugPrintf);
EXPORT_SYMBOL_NOVERS(OsDebugBreakpoint);

EXPORT_SYMBOL_NOVERS(NVM_Open);
EXPORT_SYMBOL_NOVERS(NVM_Close);
EXPORT_SYMBOL_NOVERS(NVM_Read);
EXPORT_SYMBOL_NOVERS(NVM_Write);
EXPORT_SYMBOL_NOVERS(NVM_WriteFlushList);

EXPORT_SYMBOL_NOVERS(OsEventCreate);
EXPORT_SYMBOL_NOVERS(OsEventDestroy);
EXPORT_SYMBOL_NOVERS(OsEventInit);
EXPORT_SYMBOL_NOVERS(OsEventSet);
EXPORT_SYMBOL_NOVERS(OsEventClear);
EXPORT_SYMBOL_NOVERS(OsEventState);
EXPORT_SYMBOL_NOVERS(OsEventWait);
EXPORT_SYMBOL_NOVERS(OsEventWaitTime);

EXPORT_SYMBOL_NOVERS(OsKernelUsesRegParm);

#ifdef TARGET_HCF_PCI_LINUX
EXPORT_SYMBOL_NOVERS(OsDpcCreate);
EXPORT_SYMBOL_NOVERS(OsDpcDestroy);
EXPORT_SYMBOL_NOVERS(OsDpcSchedule);
EXPORT_SYMBOL_NOVERS(OsDpcEnable);
EXPORT_SYMBOL_NOVERS(OsDpcDisable);
#endif

#if defined(CONFIG_USB) || defined(CONFIG_USB_MODULE)
#if defined(TARGET_HCF_USB_LINUX) || defined(TARGET_HSF_LINUX)
EXPORT_SYMBOL_NOVERS(OsUsbAllocateUrbs);
EXPORT_SYMBOL_NOVERS(OsUsbFWDownload);
EXPORT_SYMBOL_NOVERS(OsUsbFreeUrbs);
EXPORT_SYMBOL_NOVERS(OsUsbCreateInstance);
EXPORT_SYMBOL_NOVERS(OsUsbDestroyInstance);
#if !TARGET_HCF_FAMILY
EXPORT_SYMBOL_NOVERS(OsUsbReturnUnusedRequest);
#endif
EXPORT_SYMBOL_NOVERS(OsUsbGetControlRequest);
EXPORT_SYMBOL_NOVERS(OsUsbGetTxRequest);
EXPORT_SYMBOL_NOVERS(OsUsbIsStarted);
EXPORT_SYMBOL_NOVERS(OsUsbMakeControlRequest);
EXPORT_SYMBOL_NOVERS(OsUsbMakeControlRequestSync);
EXPORT_SYMBOL_NOVERS(OsUsbMakeDataReceiveRequest);
EXPORT_SYMBOL_NOVERS(OsUsbMakeDataTransmitRequest);
#ifdef TARGET_HCF_USB_LINUX
EXPORT_SYMBOL_NOVERS(OsUsbMakeDownloadTransmitRequest);
#endif
EXPORT_SYMBOL_NOVERS(OsUsbMakeNotifyRequest);
EXPORT_SYMBOL_NOVERS(OsUsbResetPipes);
EXPORT_SYMBOL_NOVERS(OsUsbResetPort);
EXPORT_SYMBOL_NOVERS(OsUsbWaitControlRequest);
EXPORT_SYMBOL_NOVERS(OsUsbWaitForControlRequestCompletion);
#endif
#endif // defined(CONFIG_USB) || defined(CONFIG_USB_MODULE)
#if defined(TARGET_HCF_PCI_LINUX) || defined(TARGET_HSF_LINUX)
EXPORT_SYMBOL_NOVERS(OsGetPCIDeviceResources);
EXPORT_SYMBOL_NOVERS(OsHookInterrupt);
EXPORT_SYMBOL_NOVERS(OsUnhookInterrupt);
EXPORT_SYMBOL_NOVERS(OsMapPhysMem);
EXPORT_SYMBOL_NOVERS(OsUnmapPhysMem);
#endif

EXPORT_SYMBOL_NOVERS(OsThreadCreate);
EXPORT_SYMBOL_NOVERS(OsThreadDestroy);
EXPORT_SYMBOL_NOVERS(OsThreadScheduleInit);
EXPORT_SYMBOL_NOVERS(OsThreadSchedule);
EXPORT_SYMBOL_NOVERS(OsThreadScheduleDone);
EXPORT_SYMBOL_NOVERS(OsMdmThread);

#if !TARGET_HCF_FAMILY /* HSF */

EXPORT_SYMBOL_NOVERS(OsGetProcessorFreq);
#ifdef __x86_64__
EXPORT_SYMBOL_NOVERS(OsReadCpuCnt);
#endif

EXPORT_SYMBOL_NOVERS(OsCreateTimer);
EXPORT_SYMBOL_NOVERS(OsSetTimer);
EXPORT_SYMBOL_NOVERS(OsCancelTimer);
EXPORT_SYMBOL_NOVERS(OsChangeTimerTimeOut);
EXPORT_SYMBOL_NOVERS(OsDestroyTimer);

EXPORT_SYMBOL_NOVERS(OsFloatSuffix);
EXPORT_SYMBOL_NOVERS(OsFloatPrefix);

EXPORT_SYMBOL_NOVERS(OsMemDMAAllocate);
EXPORT_SYMBOL_NOVERS(OsMemDMAFree);

#ifndef __i386__
EXPORT_SYMBOL_NOVERS(OsIoMemReadb);
EXPORT_SYMBOL_NOVERS(OsIoMemReadw);
EXPORT_SYMBOL_NOVERS(OsIoMemReadl);
EXPORT_SYMBOL_NOVERS(OsIoMemWriteb);
EXPORT_SYMBOL_NOVERS(OsIoMemWritew);
EXPORT_SYMBOL_NOVERS(OsIoMemWritel);
#endif
EXPORT_SYMBOL_NOVERS(OsIoPortReadb);
EXPORT_SYMBOL_NOVERS(OsIoPortReadw);
EXPORT_SYMBOL_NOVERS(OsIoPortReadl);
EXPORT_SYMBOL_NOVERS(OsIoPortWriteb);
EXPORT_SYMBOL_NOVERS(OsIoPortWritew);
EXPORT_SYMBOL_NOVERS(OsIoPortWritel);
EXPORT_SYMBOL_NOVERS(OsPciReadConfigdw);
EXPORT_SYMBOL_NOVERS(OsPciReadConfigw);
EXPORT_SYMBOL_NOVERS(OsPciReadConfigb);
EXPORT_SYMBOL_NOVERS(OsPciWriteConfigdw);
EXPORT_SYMBOL_NOVERS(OsPciWriteConfigw);
EXPORT_SYMBOL_NOVERS(OsPciWriteConfigb);

#if defined(SCR) && defined(NO_BLAM_SUPPORT)
EXPORT_SYMBOL_NOVERS(C_ScrFormat);
EXPORT_SYMBOL_NOVERS(LinuxScrSetEntryPointCallback);
EXPORT_SYMBOL_NOVERS(DBGSRV_Print);
#endif

#if defined(DMP) || defined(DMPRETAIL)
EXPORT_SYMBOL_NOVERS(DMPSRV_Dispatcher);
#endif
#endif /* TARGET_HCF_FAMILY */

#ifdef USE_DCP
EXPORT_SYMBOL_NOVERS(DcpCreate);
EXPORT_SYMBOL_NOVERS(DcpDestroy);
EXPORT_SYMBOL_NOVERS(DcpSetVolume);
EXPORT_SYMBOL_NOVERS(DcpCallback);
EXPORT_SYMBOL_NOVERS(OsDcpEnsureDaemonIsRunning);
#endif

#if !defined NO_DIAGMGR_SUPPORT
EXPORT_SYMBOL_NOVERS(OsDiagMgrOpen);
EXPORT_SYMBOL_NOVERS(OsDiagMgrClose);
EXPORT_SYMBOL_NOVERS(OsDiagMgrNotify);
#endif

#if !defined NO_BLAM_SUPPORT
EXPORT_SYMBOL_NOVERS(GetIOClientFromId);
EXPORT_SYMBOL_NOVERS(RPT_Init);
EXPORT_SYMBOL_NOVERS(RPT_Read);
EXPORT_SYMBOL_NOVERS(RPT_ScrRead);
EXPORT_SYMBOL_NOVERS(RPT_ScrWrite);
EXPORT_SYMBOL_NOVERS(RPT_SetMask);
EXPORT_SYMBOL_NOVERS(RPT_Shutdown);
EXPORT_SYMBOL_NOVERS(RPT_Write);
#endif

MODULE_AUTHOR("Copyright (C) 2003-2004 Linuxant inc.");
MODULE_DESCRIPTION("OS-specific support module");
MODULE_LICENSE("GPL\0for files in the \"GPL\" directory; for others, only LICENSE file applies");
MODULE_INFO(supported, "yes");

static int __init
cnxtosspec_init(void)
{
    int r;

    r = OsInit();
    if(r < 0)
	return r;

#if defined(SCR) && defined(NO_BLAM_SUPPORT)
    r = OsScrInit();
    if(r < 0)
	return r;
#endif

#if !defined NO_DIAGMGR_SUPPORT
    r = OsDiagInit();
    if(r < 0) {
#if defined(SCR) && defined(NO_BLAM_SUPPORT)
	OsScrExit();
#endif
	return r;
    }
#endif
#ifdef USE_DCP
    r = OsDcpInit();
    if(r < 0) {
#if !defined NO_DIAGMGR_SUPPORT
	OsDiagExit();
#endif
#if defined(SCR) && defined(NO_BLAM_SUPPORT)
	OsScrExit();
#endif
	return r;
    }
#endif /* USE_DCP */

    return 0;
}

static void __exit
cnxtosspec_exit(void)
{
	void OsNvmExit(void);
	OsNvmExit();
#ifdef USE_DCP
    OsDcpExit();
#endif
#if !defined NO_DIAGMGR_SUPPORT
    OsDiagExit();
#endif
#if defined(SCR) && defined(NO_BLAM_SUPPORT)
    OsScrExit();
#endif
}

module_init(cnxtosspec_init);
module_exit(cnxtosspec_exit);

#if defined(USE_TRACE) && !defined(USE_TRACE_ONLY)
#include "testdebug.h"

unsigned long ulTraceMask =
                            //(0xFFFFFFFF & ~T__RTMGR) |
                            T__ERROR | T__FATAL_ERROR | 
/*                            T__TIMING | */
/*                            T__PROFILE | */
/*                            T__HDLC | */
/*                            T__ATPARSER |  */
                            T__PORT | 
                            T__DRIVER |
/*                            T__HAL |  */
/*                            T__DOWNLOAD | */
/*                            T__HALOS | */
/*                            T__LMON | */
/*                            T__STATMGR | */
/*                            T__CALLPROG |  */
/*                            T__SESSIONMGR |  */
/*                            T__STREAMMGR |  */
/*                            T__UTILS | */
/*                            T__DATASESSION | */
/*                            T__TESTSESSION | */
/*                            T__V8BIS | */
/*                            T__RTMGR | */
/*                            T__V42      | */
/*                            T__V42BIS	|	 */
/*                            T__FAXSESSION   |	 */
/*                            T__FAXPIM		| */
/*							T__CALLERID | */
/*							T__MOH | */
/*                            T__CW | */
/*							T__QC | */
/*                            T__SOFT |  */
                            0;

unsigned long ulLogThread = 0xFFFFFFFF;

EXPORT_SYMBOL_NOVERS(ulTraceMask);
EXPORT_SYMBOL_NOVERS(ulLogThread);
#endif
