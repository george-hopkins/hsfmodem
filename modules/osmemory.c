/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */
#include "oscompat.h"
#include "framewrk.h"
#include "osservices.h"
#include "osmemory.h"

#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/highmem.h>

__shimcall__
void *OsAllocate(unsigned size)
{
	PVOID pMem = kmalloc(size, OsContextAllowsSleeping() ? GFP_KERNEL : GFP_ATOMIC);
	if(!pMem) {
	    if(OsContextAllowsSleeping()) {
		/* try vmalloc since kmalloc is generally limited to 64k */
		pMem = vmalloc(size);
	    }
	}

	if(pMem) {
#ifdef OS_DEBUG_MEM
	    printk(KERN_DEBUG "%s(%u): %p\n", __FUNCTION__, size, pMem);
#endif
	    memset(pMem, 0, size); /* caller expects memory to be initialized to zero */
	} else {
		printk(KERN_ERR "%s(%u): failed, OsContextAllowsSleeping=%s\n", __FUNCTION__, size, OsContextAllowsSleeping() ? "TRUE" : "FALSE");
	}

	return pMem;
}
 
__shimcall__
void OsFree(PVOID pMem)
{
    if(!pMem) {
	printk(KERN_ERR "%s: NULL\n", __FUNCTION__);
	return;
    }
    if((pMem >= (PVOID)VMALLOC_START) && (pMem < (PVOID)VMALLOC_END)) {
#ifdef OS_DEBUG_MEM
	printk(KERN_DEBUG "%s: vfree %p\n", __FUNCTION__, pMem);
#endif
	vfree(pMem);
    } else {
#ifdef OS_DEBUG_MEM
	printk(KERN_DEBUG "%s: %p\n", __FUNCTION__, pMem);
#endif
	kfree(pMem);
    }
}
	
#if !TARGET_HCF_FAMILY
#include "osmemory_ex.h"

#include <asm/io.h> /* for virt_to_bus() */

__shimcall__
BOOL	OsMemDMAAllocate(UINT32 nPages,PUINT16* ppPhysAddr, PHANDLE pMemHandle, PUINT16* ppBuffer)
{
    int count;
    unsigned long allocs[256];

    for ( count = 0 ; count < 256 ; count++  )
    {
	allocs[count] = __get_dma_pages(OsContextAllowsSleeping() ? GFP_KERNEL : GFP_ATOMIC, nPages);
	if ( !allocs[count] )
	{
	    break;
	}

	if ( 0x0 == (((UINT32)(allocs[count])) & 0x0000FFFF) )
	{
	    *ppBuffer = (PUINT16)allocs[count];
	    if(pMemHandle)
		*pMemHandle = (PVOID) allocs[count];
	    *ppPhysAddr = (PUINT16)virt_to_bus(*ppBuffer);
	    //printk(KERN_DEBUG"%s: success - 0x%lx(%p) after %d attempts\n", __FUNCTION__, allocs[count], *ppPhysAddr, count);
	    for ( count--; count >= 0; count-- )
	    {
		free_pages(allocs[count], nPages);
	    }
	    return (TRUE);
	}
    }

    printk(KERN_ERR "%s: failed (attempt=%d, npages=%u)\n", __FUNCTION__, count, nPages);

    for ( count--; count >= 0; count-- )
	free_pages(allocs[count], nPages);

    return (FALSE);
}

__shimcall__
BOOL	OsMemDMAFree(HANDLE MemHandle, UINT32 nPages)
{
    if (NULL == MemHandle)
	return (FALSE);

    ASSERT(nPages > 0);

    //		printk( "OsMemDMAFree: freeing 0x%lx (%d pages)\n", MemHandle, nPages );
    free_pages((unsigned long)MemHandle, nPages);
    return (TRUE);
}
#endif

