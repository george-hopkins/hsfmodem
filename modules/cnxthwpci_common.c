/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

MODULE_DEVICE_TABLE(pci, cnxthwpci_tbl);

#ifndef CNXTHW_DECL_MODULE_DEVICE_TABLE_ONLY
MODULE_AUTHOR("Copyright (C) 2003-2004 Linuxant inc.");
MODULE_DESCRIPTION("Conexant low-level hardware driver");
MODULE_LICENSE("GPL\0for files in the \"GPL\" directory; for others, only LICENSE file applies");
MODULE_INFO(supported, "yes");
#endif

#include "comctrl_ex.h"
#include "osmemory.h"

#include "cnxthw_common.c"

#if !TARGET_HCF_FAMILY
__shimcall__
void *GetHwFuncs(void);
#endif

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0) )
#define __devinit
#define __devexit
#endif

static int __devinit cnxthwpci_probe (struct pci_dev *pdev,
                                         const struct pci_device_id *ent)
{
    int r, i;
    unsigned int iobase = 0;
    void *membase = NULL;
    POS_DEVNODE pDevNode;

    if (pci_request_regions(pdev, (char*)(THIS_MODULE->name))) {
	printk(KERN_ERR "%s: pci_request_regions(%s) failed\n", CNXTHWPCI_NAME, PCI_SLOT_NAME(pdev)); 
	return -ENODEV;
    }

    /* Users expect setserial to show hw parameters, so we get plausible
     * ones to pass to the serial driver. These parameters are only for
     * display purposes and otherwise not really used;
     * The real ones are obtained through OsGetPCIDeviceResources()
     */
    for (i = 0; (i < DEVICE_COUNT_RESOURCE) && (!iobase || !membase); i++) {
	if(!membase && ((pci_resource_flags(pdev, i) & PCI_BASE_ADDRESS_SPACE) == PCI_BASE_ADDRESS_SPACE_MEMORY)) {
	    membase = (void*)(pci_resource_start(pdev, i) & PCI_BASE_ADDRESS_MEM_MASK);
	}
	if(!iobase && ((pci_resource_flags(pdev, i) & PCI_BASE_ADDRESS_SPACE) == PCI_BASE_ADDRESS_SPACE_IO)) {
	    iobase = pci_resource_start(pdev, i) & PCI_BASE_ADDRESS_IO_MASK;
	}
    }

    if ( (r = pci_enable_device(pdev)) ) {
	printk(KERN_ERR "%s: pci_enable_device(%s) failed (err=%d)\n", CNXTHWPCI_NAME, PCI_SLOT_NAME(pdev), r);
	pci_release_regions(pdev);
	return r;
    }

    pDevNode = kmalloc(sizeof(*pDevNode), GFP_KERNEL);
    if(!pDevNode) {
	pci_disable_device(pdev);
	pci_release_regions(pdev);
	return -ENOMEM;
    }

    memset(pDevNode, 0, sizeof(*pDevNode));
    pDevNode->hwDev = pdev;
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
    pDevNode->hwDevLink = &pdev->dev;
#endif
    pDevNode->hwModule = THIS_MODULE;
    strncpy(pDevNode->hwProfile, (char*)ent->driver_data, sizeof(pDevNode->hwProfile));
    pDevNode->hwProfile[sizeof(pDevNode->hwProfile)-1] = '\0';
    snprintf(pDevNode->hwInstName, sizeof(pDevNode->hwInstName), "PCI-%04x:%04x-%04x:%04x",
	    	pdev->vendor, pdev->device, pdev->subsystem_vendor, pdev->subsystem_device);
#ifdef CNXTHWPCI_TYPE
    pDevNode->hwType = CNXTHWPCI_TYPE;
    pDevNode->hwIf = GetHwFuncs();
#endif
    pDevNode->pmControl = cnxthw_DevMgrPMControl;
    pDevNode->osPageOffset = PAGE_OFFSET;

    pci_set_drvdata(pdev, pDevNode);

#if !TARGET_HCF_FAMILY
#ifdef CNXTHWPCI_ENABLE_ALI_WORKAROUNDS
    /* ugly workaround for trident driver's ali_ac97_get() not always calling
     * releasecodecaccess()
     */
    {
#define ALI_AC97_WRITE 0x40
#define ALI_AC97_AUDIO_BUSY 0x4000

	struct pci_dev *ac97_pdev = PCI_GET_DEVICE(0x10B9, 0x5451, NULL);

	if(ac97_pdev) {
	    for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
		if ((pci_resource_flags(ac97_pdev, i) & PCI_BASE_ADDRESS_SPACE) == PCI_BASE_ADDRESS_SPACE_MEMORY) {
		    unsigned long ac97_membase = pci_resource_start(ac97_pdev, i) & PCI_BASE_ADDRESS_MEM_MASK;

		    if(ac97_membase && (pci_resource_len(ac97_pdev, i) >= (ALI_AC97_WRITE + sizeof(unsigned long)))) {
			void *ac97_mem;
			ac97_mem = ioremap(ac97_membase, pci_resource_len(ac97_pdev, i));
			if(ac97_mem) {
			    if((*((unsigned long *)(ac97_mem + ALI_AC97_WRITE)) != 0xffffffff) && (*((unsigned long *)(ac97_mem + ALI_AC97_WRITE)) & ALI_AC97_AUDIO_BUSY)) {
				OsSleep(200);
			    	if((*((unsigned long *)(ac97_mem + ALI_AC97_WRITE)) != 0xffffffff) && (*((unsigned long *)(ac97_mem + ALI_AC97_WRITE)) & ALI_AC97_AUDIO_BUSY)) {
				    *((unsigned long *)(ac97_mem + ALI_AC97_WRITE)) &= ~ALI_AC97_AUDIO_BUSY;
				    printk(KERN_WARNING"%s: cleared ALI_AC97_AUDIO_BUSY bit, set by buggy trident driver?\n", CNXTHWPCI_NAME);
				}
			    }
			    iounmap(ac97_mem);
			}
			break;
		    }
		}
	    }
	}
    }
#endif
#endif /* !TARGET_HCF_FAMILY */

    if ((r=cnxt_serial_add(pDevNode, iobase, membase, pdev->irq, THIS_MODULE)) < 0) {
        pci_set_drvdata(pdev, NULL);
	kfree(pDevNode);
	pci_disable_device(pdev);
	pci_release_regions(pdev);
	return r;
    }

    return 0;
}


static void __devexit cnxthwpci_remove (struct pci_dev *pdev)
{
    POS_DEVNODE pDevNode = pci_get_drvdata(pdev);

    cnxt_serial_remove(pDevNode);

    pci_set_drvdata(pdev, NULL);

    kfree(pDevNode);

    pci_disable_device(pdev);
    pci_release_regions(pdev);
}


static int cnxthwpci_suspend(struct pci_dev *pdev, pm_message_t state)
{
	COM_STATUS err;
	POS_DEVNODE pDevNode = pci_get_drvdata(pdev);

	printk(KERN_DEBUG "%s: pm event 0x%x received\n", __FUNCTION__, GET_PM_MESSAGE_EVENT(state));

	if(!pDevNode)
		return 0;
	
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
	pDevNode->pci_state = (UINT32 *) OsAllocate(64/sizeof(UINT32));
	if (!pDevNode->pci_state) return -ENOMEM;
#endif
 
	if (pDevNode->hwInUse)
	{
		printk(KERN_ERR "%s: suspend failed, hardware is in use.\n", __FUNCTION__);
		return -EBUSY;
	}

	if (pDevNode->hcomctrl)
	{
		err = ComCtrl_Control(pDevNode->hcomctrl, COMCTRL_CONTROL_SLEEP, NULL);
		if (err != COM_STATUS_SUCCESS)
		    printk(KERN_ERR "%s: ComCtrl_Control returned %d.\n", __FUNCTION__, err);
	}
	else
		printk(KERN_WARNING "%s: low level power management was not called.\n", __FUNCTION__);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10)
	pci_save_state(pdev);
#else
	pci_save_state(pdev, pDevNode->pci_state);
#endif
	pci_set_power_state(pdev, pci_choose_state(pdev, state));

	return 0;
}


static int cnxthwpci_resume(struct pci_dev *pdev)
{
	POS_DEVNODE pDevNode = pci_get_drvdata(pdev);
	COM_STATUS err;

	printk(KERN_DEBUG "%s: called\n", __FUNCTION__);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10)
	pci_restore_state(pdev);
#else
 	pci_restore_state(pdev, pDevNode->pci_state);
	OsFree(pDevNode->pci_state);
#endif
 	
	if (pDevNode && pDevNode->hcomctrl)
	{
		err = ComCtrl_Control(pDevNode->hcomctrl, COMCTRL_CONTROL_WAKEUP, NULL);
		if (err != COM_STATUS_SUCCESS)
		    printk(KERN_ERR "%s: ComCtrl_Control returned %d.\n", __FUNCTION__, err);
	}
	else
		printk(KERN_WARNING "%s: low level power management was not called.\n", __FUNCTION__);

	return 0;
}


static struct pci_driver cnxthwpci_driver = {
    .name           = CNXTHWPCI_NAME,
    .id_table       = cnxthwpci_tbl,
    .probe          = cnxthwpci_probe,
    .remove         = __devexit_p(cnxthwpci_remove),
    .suspend        = cnxthwpci_suspend,
    .resume         = cnxthwpci_resume,
};


static int __init cnxthwpci_init (void)
{
    struct pci_dev *pdev;
    int i;

    /* workaround for serial driver falsely claiming to handle our devices. */
    for(i=0; cnxthwpci_tbl[i].vendor; i++) {
	pdev = NULL;
	while( (pdev = PCI_GET_DEVICE(cnxthwpci_tbl[i].vendor, cnxthwpci_tbl[i].device, pdev)) ) {
	    if ((((pdev->class >> 8) != PCI_CLASS_COMMUNICATION_SERIAL) &&
			((pdev->class >> 8) != PCI_CLASS_COMMUNICATION_MODEM)) ||
		    (pdev->class & 0xff) > 6)
		continue;

	    if(pdev->driver) {
		if(pdev->driver->name && !strcmp(pdev->driver->name, "serial")) {
		    printk(KERN_WARNING"%s: %s driver grabbed our device (%s), reclaiming it..\n", CNXTHWPCI_NAME, pdev->driver->name, PCI_SLOT_NAME(pdev));
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
		    {
		    	struct device *dev = get_device(&pdev->dev);

		    	if(dev) {
				device_release_driver(dev);
				put_device(dev);
		    	}
		    }
#else
		    if(pdev->driver) {
			if(pdev->driver->remove) {
			    pdev->driver->remove(pdev);
			}
			pdev->driver = NULL;
		    }
#endif

		}
	    }
	}
    }

    return PCI_REGISTER_DRIVER(&cnxthwpci_driver);
}


static void __exit cnxthwpci_cleanup (void)
{
    pci_unregister_driver (&cnxthwpci_driver);
}

module_init(cnxthwpci_init);
module_exit(cnxthwpci_cleanup);
