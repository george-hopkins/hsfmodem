/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */
#ifndef __OSRESOUR_EX_H
#define __OSRESOUR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
*		                     Version Control Information
*
*	$Header:   P:/d942/octopus/archives/linuxos/dist/modules/osspec/osresour_ex.h_v   1.2   28 Apr 2003 11:03:48   beliloi  $
*
*****************************************************************************************/


#include "framewrk.h"			// Compiler Directives for code modules

#include "typedefs.h"
#include "comtypes.h"

#include "configtypes.h"
#include "configcodes.h"

typedef int OS_IRQRETURN_T;

#define OS_IRQ_NONE        (0)
#define OS_IRQ_HANDLED     (1)

#ifdef FRAME_WORK_IMPORTED
typedef enum {
        DEVMGR_D0,
        DEVMGR_D1,
        DEVMGR_D2,
        DEVMGR_D3,
	DEVMGR_QUICK_D0 /* quick wake-up for CID, only one download, no QC profile restore */
} ePmeState;
#else
#include "devmgr_ex.h"
#endif

#if defined(BIG_ENDIAN_MACHINE) && (BIG_ENDIAN_MACHINE == 1)
#define CNXTHWCFG(x) ((unsigned long)(CNXTTARGET x "-bigendian"))
#else
#define CNXTHWCFG(x) ((unsigned long)(CNXTTARGET x))
#endif

typedef struct tagOS_DEVNODE {
    HANDLE hwDev;
    void *hwDevLink;
    void *hwModule;
    int   hwInstNum;
    char  hwInstName[50];
    char  hwProfile[50];
    char  hwRevision[75];
#if !TARGET_HCF_FAMILY && !defined(TARGET_DGC_LINUX)
    HW_TYPE hwType;
    void *hwIf;
#endif
    BOOL hwInUse;
    BOOL hwSuspendInProgress;
	unsigned long osPageOffset;
    HANDLE pmDev;
#if !defined(TARGET_DGC_LINUX)
    COM_STATUS (*pmControl)(struct tagOS_DEVNODE *DevNode,
		__shimcall__ COM_STATUS (*pmControlFunc)(HANDLE hDevMgr, ePmeState PmeState),
		HANDLE hDevMgr, ePmeState PmeState, ePmeState OldPmeState) __shimcall__;
	HANDLE hcomctrl;
#endif
	UINT32 *pci_state;

    struct cnxt_irq_t {
	__kernelcall__ OS_IRQRETURN_T (*isr)(int irq, void *devidp, void *unused);
	void *devid;
    } irq_info;
} OS_DEVNODE, *POS_DEVNODE;

typedef struct {
    UINT32 addr;
    UINT32 len;
} OS_PCI_DEV_RES, *POS_PCI_DEV_RES;

__shimcall__
BOOL OsGetPCIDeviceResources(HANDLE DevNode,
	POS_PCI_DEV_RES pPhysMem, POS_PCI_DEV_RES pPhysIO1, POS_PCI_DEV_RES pPhysIO2,
	BOOL bEnableBusMastering);

__shimcall__
PVOID OsMapPhysMem(UINT32 dwPhysMemBase, UINT32 dwMemSize);

__shimcall__
BOOL OsUnmapPhysMem(PVOID dwLinearAddr, UINT32 dwMemSize);

__shimcall__
BOOL OsHookInterrupt(HANDLE DevNode, __kernelcall__ OS_IRQRETURN_T (*isr)(int irq, void *devidp, void *unused), void* devid);

__shimcall__
void OsUnhookInterrupt(HANDLE DevNode, void *devid);

#ifdef __i386__
#undef OsIoMemReadb
#define OsIoMemReadb(addr) (*(volatile unsigned char *) (addr))
#undef OsIoMemReadw
#define OsIoMemReadw(addr) (*(volatile unsigned short *) (addr))
#undef OsIoMemReadl
#define OsIoMemReadl(addr) (*(volatile unsigned int *) (addr))
#undef OsIoMemWriteb
#define OsIoMemWriteb(addr,b) (*(volatile unsigned char *) (addr) = (b))
#undef OsIoMemWritew
#define OsIoMemWritew(addr,b) (*(volatile unsigned short *) (addr) = (b))
#undef OsIoMemWritel
#define OsIoMemWritel(addr,b) (*(volatile unsigned int *) (addr) = (b))
#else
__shimcall__
unsigned char OsIoMemReadb(void *addr);

__shimcall__
unsigned short OsIoMemReadw(void *addr);

__shimcall__
unsigned int OsIoMemReadl(void *addr);

__shimcall__
void	OsIoMemWriteb(void *addr, unsigned char bVal);

__shimcall__
void	OsIoMemWritew(void *addr, unsigned short wVal);

__shimcall__
void	OsIoMemWritel(void *addr, unsigned int dwVal);
#endif

__shimcall__
unsigned char OsIoPortReadb(unsigned int port);

__shimcall__
unsigned short OsIoPortReadw(unsigned int port);

__shimcall__
unsigned int OsIoPortReadl(unsigned int port);

__shimcall__
void	OsIoPortWriteb(unsigned int port, unsigned char bVal);

__shimcall__
void	OsIoPortWritew(unsigned int port, unsigned short wVal);

__shimcall__
void	OsIoPortWritel(unsigned int port, unsigned int dwVal);

__shimcall__
void OsPciReadConfigdw(void *pPciDev, unsigned char Offset, unsigned int *pVal);

__shimcall__
void OsPciReadConfigw(void *pPciDev, unsigned char Offset, unsigned short *pVal);

__shimcall__
void OsPciReadConfigb(void *pPciDev, unsigned char Offset, unsigned char *pVal);

__shimcall__
void OsPciWriteConfigdw(void *pPciDev, unsigned char Offset, unsigned int Val);

__shimcall__
void OsPciWriteConfigw(void *pPciDev, unsigned char Offset, unsigned short Val);

__shimcall__
void OsPciWriteConfigb(void *pPciDev, unsigned char Offset, unsigned char Val);

#ifdef __cplusplus
}
#endif

#endif /* __OSRESOUR_EX_H */

