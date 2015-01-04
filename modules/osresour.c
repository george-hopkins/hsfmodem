/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#include "oscompat.h"
#include "osresour_ex.h"

#include <linux/pci.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/interrupt.h>

#ifndef TARGET_HCF_USB_LINUX

__shimcall__
BOOL OsGetPCIDeviceResources(HANDLE DevNode, 
	POS_PCI_DEV_RES pPhysMem, POS_PCI_DEV_RES pPhysIO1, POS_PCI_DEV_RES pPhysIO2,
       	BOOL bEnableBusMastering)
{			   
    struct pci_dev *pcidev = (struct pci_dev *)DevNode;
    int i, needed = 0;

    if(pPhysMem) {
	memset(pPhysMem, 0, sizeof(*pPhysMem));
	needed++;
    }
    if(pPhysIO1) {
	memset(pPhysIO1, 0, sizeof(*pPhysIO1));
	needed++;
    }
    if(pPhysIO2) {
	memset(pPhysIO2, 0, sizeof(*pPhysIO2));
	needed++;
    }

    for (i = 0; needed && (i < DEVICE_COUNT_RESOURCE); i++) {
	if(pPhysMem && ((pci_resource_flags(pcidev, i) & PCI_BASE_ADDRESS_SPACE) == PCI_BASE_ADDRESS_SPACE_MEMORY)) {
	    pPhysMem->addr = pci_resource_start(pcidev, i) & PCI_BASE_ADDRESS_MEM_MASK;
	    pPhysMem->len  = pci_resource_len(pcidev, i);
	    if(pPhysMem->addr && pPhysMem->len) {
		needed--;
		pPhysMem = NULL;
		continue;
	    }
	}
	if(pPhysIO1 && ((pci_resource_flags(pcidev, i) & PCI_BASE_ADDRESS_SPACE) == PCI_BASE_ADDRESS_SPACE_IO)) {
	    pPhysIO1->addr = pci_resource_start(pcidev, i) & PCI_BASE_ADDRESS_IO_MASK;
	    pPhysIO1->len  = pci_resource_len(pcidev, i);
	    if(pPhysIO1->addr && pPhysIO1->len) {
		needed--;
		pPhysIO1 = NULL;
		continue;
	    }
	}
	if(pPhysIO2 && ((pci_resource_flags(pcidev, i) & PCI_BASE_ADDRESS_SPACE) == PCI_BASE_ADDRESS_SPACE_IO)) {
	    pPhysIO2->addr = pci_resource_start(pcidev, i) & PCI_BASE_ADDRESS_IO_MASK;
	    pPhysIO2->len  = pci_resource_len(pcidev, i);
	    if(pPhysIO2->addr && pPhysIO2->len) {
		needed--;
		pPhysIO2 = NULL;
		continue;
	    }
	}
    }

    if(needed) {
	printk(KERN_ERR "%s: %d resource(s) missing for device %s\n", __FUNCTION__, needed, PCI_SLOT_NAME(pcidev));
	return FALSE;
    }

    if(bEnableBusMastering)
	pci_set_master(pcidev);

    return TRUE;
}

__shimcall__
PVOID OsMapPhysMem(UINT32 dwPhysMemBase, UINT32 dwMemSize)
{
    PVOID mem;

    mem = ioremap(dwPhysMemBase, dwMemSize);
    if(!mem) {
	printk(KERN_ERR "%s: ioremap(0x%x, 0x%x) failed\n", __FUNCTION__, dwPhysMemBase, dwMemSize);
    }
    //	printk("\n<HCF> Map %p (%u) to %p\n\n", (void *)dwPhysMemBase, dwMemSize, (void *)mem);
    return mem;
}

__shimcall__
BOOL OsUnmapPhysMem(PVOID dwLinearAddr, UINT32 dwMemSize)
{
    if(dwLinearAddr != NULL)
	iounmap((void *)dwLinearAddr);
    return TRUE;
}

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
#ifdef FOUND_IRQ_HANDLER_T
static irqreturn_t cnxt_interrupt(int irq, void *devid)
{
    POS_DEVNODE pDevNode = (POS_DEVNODE)devid;
    return pDevNode->irq_info.isr(irq, pDevNode->irq_info.devid, NULL);
}
#else
static irqreturn_t cnxt_interrupt(int irq, void *devid, struct pt_regs *regs)
{
    POS_DEVNODE pDevNode = (POS_DEVNODE)devid;
    return pDevNode->irq_info.isr(irq, pDevNode->irq_info.devid, regs);
}
#endif
#endif

__shimcall__
BOOL OsHookInterrupt(HANDLE DevNode, __kernelcall__ OS_IRQRETURN_T (*isr)(int irq, void *devidp, void *unused), void* devid)
{
    struct pci_dev *pcidev = (struct pci_dev *)DevNode;
    const char *devname;
    POS_DEVNODE pDevNode = (POS_DEVNODE)pci_get_drvdata(pcidev);
    int err;

    if (pDevNode->hwModule && ((struct module *)pDevNode->hwModule)->name)
	devname = ((struct module *)pDevNode->hwModule)->name;
    else
	devname = (THIS_MODULE && THIS_MODULE->name) ? THIS_MODULE->name : "cnxt";

    /* for efficiency we expect the isr function passed to have the same calling
       conventions and arguments as standard Linux interrupt handlers. */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    err = request_irq(pcidev->irq, (void (*)(int irq, void *devidp, struct pt_regs *))isr, SA_SHIRQ, devname, devid);
#else
    pDevNode->irq_info.isr = isr;
    pDevNode->irq_info.devid = devid;

    err = request_irq(pcidev->irq, cnxt_interrupt, CNXT_IRQ_SHARED, devname, pDevNode);
#endif
    if (err) {
	printk(KERN_ERR "%s: request_irq(dev=%s, irq=%d) failed err %d\n", __FUNCTION__, PCI_SLOT_NAME(pcidev), pcidev->irq, err);
	return FALSE;
    }

    return TRUE;
}

__shimcall__
void OsUnhookInterrupt(HANDLE DevNode, void *devid)
{
    struct pci_dev *pcidev = (struct pci_dev *)DevNode;

    if(pcidev && pcidev->irq) { /* IRQ 0? */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	free_irq(pcidev->irq, devid);
#else
    POS_DEVNODE pDevNode = (POS_DEVNODE)pci_get_drvdata(pcidev);
	free_irq(pcidev->irq, pDevNode);
#endif
    }
}

#endif /* TARGET_HCF_USB_LINUX */


#if !TARGET_HCF_FAMILY

#ifndef __i386__
__shimcall__
unsigned char OsIoMemReadb(void *addr)
{
	unsigned char val = readb(addr);
/*  	printk(KERN_DEBUG "OsIoMemReadb read 0x%x from 0x%x\n",val,addr);  */
	return val;
}

__shimcall__
unsigned short OsIoMemReadw(void *addr)
{
	unsigned short val = readw(addr);
/*  	printk(KERN_DEBUG "OsIoMemReadw read 0x%x from 0x%x\n",val,addr);  */
	return val;
}

__shimcall__
unsigned int OsIoMemReadl(void *addr)
{
	unsigned int val = readl(addr);
/*  	printk("OsIoMemRead read 0x%x from 0x%x\n",val,addr); */
	return (val);
}

__shimcall__
void	OsIoMemWriteb(void *addr, unsigned char bVal)
{
/*  	printk(KERN_DEBUG "OsIoMemWriteb writes 0x%x to 0x%x\n",bVal,addr); */
	writeb(bVal, addr);
}
 
__shimcall__
void	OsIoMemWritew(void *addr, unsigned short wVal)
{
/*  	printk(KERN_DEBUG "OsIoMemWritew writes 0x%x to 0x%x\n",wVal,addr); */
	writew(wVal, addr);
}
 
__shimcall__
void	OsIoMemWritel(void *addr, unsigned int dwVal)
{
/*  	printk(KERN_DEBUG "OsIoMemWritel(%x, %x)\n",addr, dwVal); */
	writel(dwVal, addr);
}
#endif /* __i386__ */
 
 
__shimcall__
unsigned char OsIoPortReadb(unsigned int port)
{
	unsigned char val = inb(port);
/*  	printk(KERN_DEBUG "OsIoPortReadb read 0x%x from 0x%x\n",val,port);  */
	return val;
}
 
 
__shimcall__
unsigned short OsIoPortReadw(unsigned int port)
{
	unsigned short val = inw(port);
/*  	printk(KERN_DEBUG "OsIoPortReadw read 0x%x from 0x%x\n",val,port);  */
	return val;
}
 
__shimcall__
unsigned int OsIoPortReadl(unsigned int port)
{
	unsigned int val = inl(port);
/*  	printk(KERN_DEBUG "OsIoPortReadw read 0x%x from 0x%x\n",val,port);  */
	return val;
}
 
 
__shimcall__
void	OsIoPortWriteb(unsigned int port, unsigned char bVal)
{
/*  	printk(KERN_DEBUG "OsIoPortWritew writes 0x%x to 0x%x\n",bVal,addr); */
	outb(bVal, port);
}
 
__shimcall__
void	OsIoPortWritew(unsigned int port, unsigned short wVal)
{
/*  	printk(KERN_DEBUG "OsIoPortWritew writes 0x%x to 0x%x\n",wVal,addr); */
	outw(wVal, port);
}
 
 
__shimcall__
void	OsIoPortWritel(unsigned int port, unsigned int dwVal)
{
/*  	printk(KERN_DEBUG "OsIoPortWritel(%x, %x)\n",addr, dwVal); */
	outl(dwVal, port);
}
 
__shimcall__
void OsPciReadConfigdw(void *pPciDev, unsigned char Offset, unsigned int *pVal)
{
	//printk("%s: %p Offset=%u pVal=%p\n", __FUNCTION__, pPciDev, (unsigned)Offset, pVal);
	pci_read_config_dword(pPciDev, Offset, (u32*)pVal);
}
 
__shimcall__
void OsPciReadConfigw(void *pPciDev, unsigned char Offset, unsigned short *pVal)
{
	//printk("%s: %p Offset=%u pVal=%p\n", __FUNCTION__, pPciDev, (unsigned)Offset, pVal);
	pci_read_config_word(pPciDev, Offset, pVal);
}
 
__shimcall__
void OsPciReadConfigb(void *pPciDev, unsigned char Offset, unsigned char *pVal)
{
	//printk("%s: %p Offset=%u pVal=%p\n", __FUNCTION__, pPciDev, (unsigned)Offset, pVal);
	pci_read_config_byte(pPciDev, Offset, pVal);
}
 
__shimcall__
void OsPciWriteConfigdw(void *pPciDev, unsigned char Offset, unsigned int Val)
{
	//printk("%s: %p Offset=%u val=0x%lx\n", __FUNCTION__, pPciDev, (unsigned)Offset, Val);
	pci_write_config_dword(pPciDev, Offset, Val);
}
 
__shimcall__
void OsPciWriteConfigw(void *pPciDev, unsigned char Offset, unsigned short Val)
{
	//printk("%s: %p Offset=%u val=0x%lx\n", __FUNCTION__, pPciDev, (unsigned)Offset, (unsigned long)Val);
	pci_write_config_word(pPciDev, Offset, Val);
}
 
__shimcall__
void OsPciWriteConfigb(void *pPciDev, unsigned char Offset, unsigned char Val)
{
	//printk("%s: %p Offset=%u val=0x%lx\n", __FUNCTION__, pPciDev, (unsigned)Offset, (unsigned long)Val);
	pci_write_config_byte(pPciDev, Offset, Val);
}
 
#endif /* !TARGET_HCF_FAMILY */
