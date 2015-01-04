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

File Name:			MemMgrE.h	

File Description:	Memory Manager enumerated types, data structures, and prototypes.

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


#ifndef __MEM_MGR_E_H__
#define __MEM_MGR_E_H__
#include "comtypes.h"

/*#include "comtypes.h" */
/*#include "statcode.h" */
/*#include "intfctrl_ex.h" */

#define	MEM_MGR_MAJOR_VERSION	0
#define	MEM_MGR_MINOR_VERSION	2

#define MEM_MGR_VERSION (((MEM_MGR_MAJOR_VERSION) << 16) | (MEM_MGR_MINOR_VERSION))


typedef struct H_MEMMGR_TAG
{
	int dummy;

} *HMEMMGR;



/*   Direct API */

#ifdef __cplusplus
extern "C"
{
#endif


UINT32		MemMgrGetInterfaceVersion   (void);
HMEMMGR     MemMgrCreate                (void);
COM_STATUS  MemMgrDestroy               (IN HMEMMGR hMemMgr);
#ifndef USE_INLINE_OS_MEMMGR
PVOID	    MemMgrAlloc                 (IN HMEMMGR hMemMgr, IN UINT32 dwSize);
COM_STATUS	MemMgrFree                  (IN HMEMMGR hMemMgr, PVOID	pMem);
#else

//#include "ostypedefs.h"
//#include "comtypes.h"
//#include "intfctrl_ex.h"
#include "osmemory_ex.h"

#if ( USE_MEM_MGR == 1 )
static inline PVOID       MemMgrAlloc    (HMEMMGR hMemMgr, UINT32 dwSize)
{
    return OsHeapAllocate(dwSize, OS_NON_PAGED);
}

static inline COM_STATUS  MemMgrFree     (HMEMMGR hMemMgr, PVOID  pMem)
{
    OsHeapFree(pMem);
    return COM_STATUS_SUCCESS;
}
#endif /* USE_MEM_MGR */
#endif /* USE_INLINE_OS_MEMMGR */

#if defined(_DEBUG) && ( USE_MEM_MGR == 1 )
void		MemMgrPrintAll (HMEMMGR hMemMgr);
#else
#define 	MemMgrPrintAll(x)
#endif

#ifdef __cplusplus
}
#endif

/*  Indirect API */

GLOBAL	PVOID	MemMgrGetInterface(void);


typedef struct I_MEM_MGR_TAG
{
	UINT32		(*GetInterfaceVersion)(void);

	HMEMMGR		(*Create)    (void);
	COM_STATUS	(*Destroy)   (IN HMEMMGR	hMemMgr);
	PVOID	    (*Alloc)     (IN	HMEMMGR	hMemMgr, IN UINT32 dwSizeSize);
	COM_STATUS	(*Free)      (IN	HMEMMGR	hMemMgr, IN PVOID pMem);

    #ifdef _DEBUG
    void		(*PrintAll)  (IN HMEMMGR hMemMgr);
    #endif

} I_MEM_MGR_T, *PI_MEM_MGR_T;


#if defined(USE_TOTAL_SIGNATURE_CHECK) && defined(_DEBUG)
    void SIG_CHECK(PVOID pMem);
#else
    #define SIG_CHECK(pMem)
#endif

/* "Macro" API */

#ifdef USE_DIRECT_API

#if ( USE_MEM_MGR == 0 )
    #define MemMgrAlloc(               pSys, dwSize)   OsHeapAllocate              (dwSize, OS_NON_PAGED)
    #define MemMgrFree(                pSys, pMem)     OsHeapFree                  (pMem)
#endif

    #define MEMMGR_GetInterfaceVersion( pSys)           MemMgrGetInterfaceVersion   ()
    #define MEMMGR_Create(              pSys)           MemMgrCreate                ()
    #define MEMMGR_Destroy(             pSys)           MemMgrDestroy               (pSys->hMemMgr)
    #define MEMMGR_Alloc(               pSys, dwSize)   MemMgrAlloc                 (pSys->hMemMgr, dwSize)
    #define MEMMGR_Free(                pSys, pMem)     MemMgrFree                  (pSys->hMemMgr, pMem)
    #define MEMMGR_PrintAll(            pSys)           MemMgrPrintAll              (pSys->hMemMgr)

/** For DevMgr uses only . **/
    #define MEMMGR_Gen_Alloc(			pSys,hClient,Name, dwSize)	MemMgrAlloc   (pSys->hMemMgr, dwSize)
    #define MEMMGR_Gen_Free(			pSys,hClient, pMem)			MemMgrFree    (pSys->hMemMgr, pMem)

#else

    #define MEMMGR_GetInterfaceVersion( pSys)           pSys->pftMemMgr->GetInterfaceVersion()
    #define MEMMGR_Create(              pSys)           pSys->pftMemMgr->Create  ()
    #define MEMMGR_Destroy(             pSys)           pSys->pftMemMgr->Destroy (pSys->hMemMgr)
    #define MEMMGR_Alloc(               pSys, dwSize)   pSys->pftMemMgr->Alloc   (pSys->hMemMgr, dwSize)
    #define MEMMGR_Free(                pSys, pMem)     pSys->pftMemMgr->Free    (pSys->hMemMgr, pMem)
    #define MEMMGR_PrintAll(            pSys)           pSys->pftMemMgr->PrintAll(pSys->hMemMgr)

/** For DevMgr uses only . **/
    #define MEMMGR_Gen_Alloc(			pSys,hClient,Name, dwSize)	pSys->pftMemMgr->Alloc(pSys->hMemMgr, dwSize)
    #define MEMMGR_Gen_Free(			pSys,hClient, pMem)			pSys->pftMemMgr->Free(pSys->hMemMgr, pMem)
#endif


#endif /* __MEM_MGR_E_H__ */
