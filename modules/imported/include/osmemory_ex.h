/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2004 Conexant Systems, Inc.
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
 * 6.   Redistribution. Permission to redistribute this software without
 * modification is granted, without prejudice to Linuxant's ability to obtain
 * reparation for any unauthorized distribution of previous versions of this
 * software released under prior LICENSE terms. Modification or redistribution
 * of this software under different terms requires explicit written approval
 * signed by an authorized Linuxant officer.
 * 
 * 7.   Performance. V.92 modems are designed to be capable of receiving data at
 * up to 56Kbps with compatible phone line and server equipment, and transmitting
 * data at up to 31.2Kbps. V.90 modems are designed to be capable of receiving
 * data at up to 56 Kbps from a compatible service provider and transmitting data
 * at up to about 28.8 Kbps. Public networks currently limit download speeds to
 * about 53Kbps. Actual speeds vary and are often less than the maximum possible.
 * 
 * 
 */

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "osstring_ex.h"

#if ( USE_PRAGMA_PACK == 1 )
#pragma pack(push,1)   /* Packing compatible with WinAC */
#endif
 
#if ( OS_TYPE != OS_LINUX )
/* Define Vtoolsd constants */
#define OS_HEAPZEROINIT		0x00000001    
#define OS_HEAPZEROREINIT	0x00000002  

#define OS_HEAPSWAP			0x00000200 

#define OS_NON_PAGED		(UINT32)0
#define OS_FIXED			0x00000008

#define OS_ZEROINIT			0x00000001	
#define OS_FIXEDZERO		0x00000003		

#define OS_PAGED			OS_HEAPSWAP

#else
#define OS_PAGED			0
#define OS_NON_PAGED		1

#define OS_FIXED			OS_NON_PAGED
#define OS_HEAPSWAP			OS_PAGED

#define OS_ZEROINIT			0x00000001	
#define OS_FIXEDZERO		0x00000003		
#endif

#ifdef __cplusplus
extern "C"
{
#endif
/********************************************************************************
	
	Function Name: OsPageCommit.

	Function Description:The function commits a memory space.

	Arguments: iPage - the base address of the memory space.
			   nPages - number of pages to be commited.
	
	Return Value: BOOL.

	********************************************************************************/
#if ((OS_TYPE == OS_LINUX) || (OS_TYPE == OS_MACOSX))
#define OsPageCommit(iPage, nPages, PageData, flags) TRUE
#else
	StdCall(BOOL, OsPageCommit(UINT32 iPage, UINT32 nPages, UINT32 PageData, UINT32 flags));
#endif

	/********************************************************************************
	
	Function Name: OsPageDecommit.

	Function Description:The function decommits a given memory space.

	Arguments: iPage - the base address of the memory space.
			   nPages - number of pages to be commited.
	
	Return Value: BOOL.

	********************************************************************************/
#if ((OS_TYPE == OS_LINUX) || (OS_TYPE == OS_MACOSX))
#define OsPageDecommit(iPage, nPages) TRUE
#else
	StdCall(BOOL,OsPageDecommit(UINT32 iPage, UINT32 nPages)) ;
#endif

	/********************************************************************************
	
	Function Name: OsPageReserve.

	Function Description:The function reserves a given memory space.

	Arguments: Size - the size of memory space to br reserved.
			   Flags - the flags that should be send to the reserve funcyion.
	
	Return Value: BOOL.

	********************************************************************************/
__shimcall__
	StdCall(PVOID, OsPageReserve(UINT32 Size, UINT32 Flags));

	/********************************************************************************
	
	Function Name: OsPageFree.

	Function Description:The function frees a given memory space.

	Arguments: iPages - the base address of the memory space.
			   nPages - number of pages to free.
	

	Return Value: BOOL.
	********************************************************************************/
__shimcall__
	StdCall(BOOL, OsPageFree(PVOID	pMem, UINT32 nPages));

	/********************************************************************************
	
	Function Name: OsHeapAllocate.

	Function Description:The function allocates a given memory space and initializes it to zero.

	Arguments: Size - the size of memory space (heap) to br allocated.
	
	Return Value: PVOID - a pointer to the allocated memory.

	********************************************************************************/
#if OS_TYPE == OS_LINUX
#include "osmemory.h"

#define OsHeapAllocate(size, flags) OsAllocate(size)
#define OsHeapFree(ptr) OsFree(ptr)
#else
	StdCall(PVOID, OsHeapAllocate(UINT32 Size, int flags));

	/********************************************************************************
	
	Function Name: OsHeapFree.

	Function Description:The function frees a memory space.

	Arguments: pMem - a pointer to the memory to be frees.
	
	Return Value: None.

	********************************************************************************/
	StdCall(void, OsHeapFree(PVOID pMem));
#endif

	/********************************************************************************
	
	Function Name: OsGetPageSize.

	Function Description:The function returns the size of a page in the specific OS.

	Arguments: None.
	
	Return Value: The size of a page in the specific OS.

	********************************************************************************/
__shimcall__
	StdCall(UINT32, OsGetPageSize(void));

	/********************************************************************************
	
	Function Name: OsMemDMAAllocate.

	Function Description:The function allocate a dma.

	Arguments: nPages - number of page to allocate..
			   ppPhysAddr - a pointer to be filled with the physical address of the dma.
			   pMemHandle - a pointer to be filled with the memory handle.
			   ppBuffer - a pointer to be filled with the ring-0 linear memory address.
	
	Return Value: boolean.

	********************************************************************************/
__shimcall__
#if (PROJECT == PRJ_SDSL)
	StdCall(BOOL, OsMemDMAAllocate(UINT32 nPages,UINT16 **ppPhysAddr, HANDLE *pMemHandle, UINT16 **ppBuffer, PVOID pResources));
#else	
	StdCall(BOOL,OsMemDMAAllocate(UINT32 nPages,PUINT16* ppPhysAddr, HANDLE *pMemHandle, PUINT16* ppBuffer));
#endif

	/********************************************************************************
	
	Function Name: OsMemDMAFree.

	Function Description:The function allocate a dma.

	Arguments: MemHandle - a memory handle.
			   Flags - flags.
	
	Return Value: boolean.

	********************************************************************************/
__shimcall__
	StdCall(BOOL, OsMemDMAFree(HANDLE MemHandle, UINT32 nPages));
	/********************************************************************************
	
	Function Name: OsAllignmentAlloc.
 
	Function Description:The function allocate a memory alligned on a 2**AllognLog2.
 
	Arguments: ppMem    -  Actual allocation pointer ( not alligned  ) to be called to 
                            OsFree function
			   ppAllignedMem    - Alligned allocation.
               AllocaSize       - number of bytes to alloc
               AllognLog2       - log2 of number of bytes allignment
	
	Return Value: None  .
 
	********************************************************************************/
__shimcall__
	StdCall(VOID, OsAllignmentAlloc(  OUT VOID    **ppMem, 
                                OUT VOID    **ppAllignedMem, 
                                IN  int     AllocaSize,
                                IN  int     AllognLog2));
	/********************************************************************************/
__shimcall__
	void    OsDebugBreakpoint   ( const char * szMsg );
__shimcall__
	void    OsDebugPrintf       ( const char * szFormat, ...);
	/********************************************************************************/
 
	
#ifdef __cplusplus
}
#endif

#if ( USE_PRAGMA_PACK == 1 )
#pragma pack(pop)   /* Packing compatible with our VXDs */
#endif

#endif /*__MEMORY_H__*/
