/*
 * Copyright (c) 2003-2010 Linuxant inc.
 * Copyright (c) 2001-2010 Conexant Systems, Inc.
 * 
 * 1.   Permitted use. Redistribution and use in source and binary forms,
 * without modification, are only permitted under the terms set forth herein.
 * 
 * 2.   Disclaimer of Warranties. LINUXANT, ITS SUPPLIERS, AND OTHER CONTRIBUTORS
 * MAKE NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOFTWARE FOR ANY PURPOSE.
 * IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTIES OF ANY KIND.
 * LINUXANT AND OTHER CONTRIBUTORS DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, GOOD TITLE AND AGAINST INFRINGEMENT.
 * 
 * This software has not been formally tested, and there is no guarantee that
 * it is free of errors including, but not limited to, bugs, defects,
 * interrupted operation, or unexpected results. Any use of this software is
 * at user's own risk.
 * 
 * 3.   No Liability.
 * 
 * (a) Linuxant, its suppliers, or contributors shall not be responsible for
 * any loss or damage to users, customers, or any third parties for any reason
 * whatsoever, and LINUXANT, ITS SUPPLIERS OR CONTRIBUTORS SHALL NOT BE LIABLE
 * FOR ANY ACTUAL, DIRECT, INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL, OR
 * CONSEQUENTIAL (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED, WHETHER IN CONTRACT, STRICT OR OTHER LEGAL THEORY OF
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 * (b) User agrees to hold Linuxant, its suppliers, and contributors harmless
 * from any liability, loss, cost, damage or expense, including attorney's fees,
 * as a result of any claims which may be made by any person, including
 * but not limited to User, its agents and employees, its customers, or
 * any third parties that arise out of or result from the manufacture,
 * delivery, actual or alleged ownership, performance, use, operation
 * or possession of the software furnished hereunder, whether such claims
 * are based on negligence, breach of contract, absolute liability or any
 * other legal theory.
 * 
 * 4.   Notices. User hereby agrees not to remove, alter or destroy any
 * copyright, trademark, credits, other proprietary notices or confidential
 * legends placed upon, contained within or associated with the Software,
 * and shall include all such unaltered copyright, trademark, credits,
 * other proprietary notices or confidential legends on or in every copy of
 * the Software.
 * 
 * 5.   Reverse-engineering. User hereby agrees not to reverse engineer,
 * decompile, or disassemble the portions of this software provided solely
 * in object form, nor attempt in any manner to obtain their source-code.
 * 
 * 6.   Redistribution. Redistribution of this software is only permitted
 * for exact copies (without modification) of versions explicitly marked
 * and officially released by Linuxant with the word "free" in their name.
 * Redistribution or disclosure of other versions, derivatives or license key
 * information is expressly prohibited without explicit written approval signed
 * by an authorized Linuxant officer.
 * 
 * 7.   Performance. V.92 modems are designed to be capable of receiving data at
 * up to 56Kbps with compatible phone line and server equipment, and transmitting
 * data at up to 31.2Kbps. V.90 modems are designed to be capable of receiving
 * data at up to 56 Kbps from a compatible service provider and transmitting data
 * at up to about 28.8 Kbps. Public networks currently limit download speeds to
 * about 53Kbps. The free version of the drivers is limited to 14.4Kbps.
 * Actual speeds vary and are often less than the maximum possible.
 * 
 * 
 */

/****************************************************************************************

File Name:          IntfCtrl_ex.h

File Description:   Interface Controller object data structures.

*****************************************************************************************/


/****************************************************************************************
*****************************************************************************************
***                                                                                   ***
***                                 Copyright (c) 2000                                ***
***                                                                                   ***
***                                Conexant Systems, Inc.                             ***
***                             Personal Computing Division                           ***
***                                                                                   ***
***                                 All Rights Reserved                               ***
***                                                                                   ***
*****************************************************************************************
*****************************************************************************************/


#ifndef _INTFCTRL_H_
#define _INTFCTRL_H_
#include "comtypes.h"

/* This structure doesn't really belong to Interface Controller, but we don't have better */
/* place do define it ( unless we want to put it in separate file) */

typedef struct SYSTEM_INSTANCES_TAG
{
    struct I_INTF_CTRL_TAG*     pftInfCntrl;

#if ( USE_MEM_MGR == 1 )
    struct I_MEM_MGR_TAG*       pftMemMgr;
    struct H_MEMMGR_TAG*        hMemMgr;
#endif

    struct I_RT_MGR_TAG*        pftRtMgr;
    struct RTMGR_handle_tag*    hRtMgr;

    struct I_CONFIG_MGR_TAG*    pftCfgMgr;
    struct CFGMGR_handle_tag*   hCfgMgr;

    struct I_STAT_MGR_TAG*      pftStatMgr;
    struct STATMGR_handle_tag*  hStatMgr;

    struct I_AT_DTE_TAG*        pftAtParser;
    struct ATP_handle_tag*      hAtParser; 

    struct I_SESSION_MGR_TAG*   pftSessionMgr;
    struct SESMGR_handle_tag*   hSessionMgr;

    struct I_DEVICE_MGR_TAG*    pftDevMgr;
    HANDLE                      hDevMgr;

#if !defined NO_DIAGMGR_SUPPORT
    struct I_DIAG_MGR_TAG*		pftDiagMgr;
    struct DIAGMGR_handle_tag*	hDiagMgr;
#endif

#if !defined NO_DUMPMGR_SUPPORT
    struct DUMPMGR_handle_tag*	hDumpMgr;
#endif

    struct I_COM_CTRL_TAG*      pftComCtrl;
    HANDLE                      hComCtrl;

#if (!defined NO_BLAM_SUPPORT || defined SCR)
    struct tagClientstruct*		hDebugMgr; /*DH */
#endif

#if !TARGET_HCF_FAMILY
    HANDLE                      hDevNode;
#endif /* TARGET_HCF_FAMILY */
} SYSTEM_INSTANCES_T, *PSYSTEM_INSTANCES_T;


/*******************************************************************/
#define OCTOPUS_BASE			0
#define SOFT_MODEM_BLOCK_BASE	100
#define NILE_BLOCK_BASE         200
#define CYPRUS_BLOCK_BASE		300
#define NON_BLOCK				255

typedef enum
{
    BLOCK_COM_CTRL,
    BLOCK_MEM_MGR, 
    BLOCK_RT_MGR, 
    BLOCK_DEBUG_MGR, 
    BLOCK_STATUS_MGR, 
    BLOCK_CONFIG_MGR,
    BLOCK_DEVICE_MGR,
    BLOCK_SESSION_MGR,
    BLOCK_NVM_MGR,
    BLOCK_AT_DTE,

	BLOCK_DIAG_MGR,

    BLOCK_SCR_SRV,
	BLOCK_DMP_SRV,
    /**  HSF blocks : **/

	BLOCK_HW_NONE , /* HSF hardware manager*/
	BLOCK_HW_BASIC2,
	BLOCK_HW_ICH,
	BLOCK_HW_VIA,
	BLOCK_HW_ALI,
	BLOCK_HW_YUKON,
    BLOCK_HW_QUAKE,
    BLOCK_HW_ELMA,
	BLOCK_HW_RIO,
	BLOCK_HW_CADMUS2,
	BLOCK_HW_ATI,
	BLOCK_HW_SIS,
	BLOCK_HW_HDA,
	BLOCK_HW_BASIC3,
	BLOCK_TONE_DET,
	BLOCK_TONE_GEN,
	BLOCK_ANS_DET,
	BLOCK_ANS_GEN,
	BLOCK_DTMF_DET,
	BLOCK_DTMF_GEN,
	BLOCK_CR_DET,
	BLOCK_CR_GEN,
	BLOCK_V23,
	BLOCK_V8,
	BLOCK_TEST,
	BLOCK_V21,
	BLOCK_V34,
	BLOCK_V90,
	BLOCK_V90_FAST,
	BLOCK_V32,
	BLOCK_V32BIS,
	BLOCK_V22,
	BLOCK_V22BIS,
	BLOCK_BELL103,
	BLOCK_BELL212,
	BLOCK_V17,
	BLOCK_V27_TX,
	BLOCK_V27_RX,
	BLOCK_V29,

    /* Nile blocks */
    BLOCK_NILE_HW = NILE_BLOCK_BASE,
    BLOCK_DSL_DATAPUMP,

 
	/* Cyprus blocks */
 
	BLOCK_CYPRUS_HW = CYPRUS_BLOCK_BASE,
	BLOCK_CYPRUS_V32,
 
    BLOCK_NONE = NON_BLOCK,

    BLOCK_LAST
}BLOCK_TYPE;


/* Direct API */

#ifdef __cplusplus
extern "C"
{
#endif

PVOID       CreateInterface     (IN BLOCK_TYPE BlockType, IN UINT32 BlockID);
COM_STATUS  DestroyInterface    (IN PVOID Interface);

#ifdef __cplusplus
}
#endif


/* Indirect API */

typedef struct I_INTF_CTRL_TAG
{
    
    PVOID       (*CreateInterface)   (BLOCK_TYPE BlockType, UINT32 BlockID);
    COM_STATUS  (*DestroyInterface)  (PVOID Interface);

} I_INTF_CTRL_T, *PI_INTF_CTRL_T;

PI_INTF_CTRL_T GetInterfaceControllerFuncTable(void);


/* "Macro" API */

#ifdef USE_DIRECT_API

    #define INTFCTRL_CreateInterface(   pSys, BlockType, BlockID)   CreateInterface  (BlockType, BlockID)
    #define INTFCTRL_DestroyInterface(  pSys, Interface)            DestroyInterface (Interface)

#else

    #define INTFCTRL_CreateInterface(   pSys, BlockType, BlockID)   pSys->pftInfCntrl->CreateInterface (BlockType, BlockID)
    #define INTFCTRL_DestroyInterface(  pSys, Interface)            pSys->pftInfCntrl->DestroyInterface   (Interface)

#endif



#endif
