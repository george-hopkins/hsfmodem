/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

MODULE_DEVICE_TABLE(usb, cnxthwusb_tbl);

#ifndef CNXTHW_DECL_MODULE_DEVICE_TABLE_ONLY
MODULE_AUTHOR("Copyright (C) 2003-2004 Linuxant inc.");
MODULE_DESCRIPTION("Conexant low-level hardware driver");
MODULE_LICENSE("GPL\0for files in the \"GPL\" directory; for others, only LICENSE file applies");
MODULE_INFO(supported, "yes");
#endif

#ifdef CONFIG_PPC_PMAC
static int pmacftrmodemenable = 1;
#ifdef FOUND_MODULE_PARAM
module_param(pmacftrmodemenable, int, 0);
#else
MODULE_PARM(pmacftrmodemenable, "i");
#endif
MODULE_PARM_DESC(pmacftrmodemenable, "Enable internal modem if non-zero");
#endif

#include "osusb.h"
#include "comctrl_ex.h"

#include <linux/slab.h>

#ifdef CONFIG_PPC_PMAC
#include <asm/machdep.h>
#include <asm/pmac_feature.h>
#endif

#include "cnxthw_common.c"

//#undef dbg
//#define dbg(format, arg...) { UINT32 dwTime = OsGetSystemTime(); printk(KERN_DEBUG "%07lu.%03lu: "__FILE__ ": " format "\n" , dwTime/1000, dwTime%1000, ## arg); }
#ifndef dbg
#define dbg(format, arg...) printk(KERN_DEBUG KBUILD_MODNAME ": " format "\n" , ## arg)
#endif

#ifndef info
#define info(format, arg...) printk(KERN_INFO KBUILD_MODNAME ": " format "\n" , ## arg)
#endif

#ifndef warn
#define warn(format, arg...) printk(KERN_WARNING KBUILD_MODNAME ": " format "\n" , ## arg)
#endif

#ifndef err
#define err(format, arg...) printk(KERN_ERR KBUILD_MODNAME ": " format "\n" , ## arg)
#endif

#if !TARGET_HCF_FAMILY
void *GetHwFuncs(void);
#endif

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0) )
#define __devinit
#define __devexit
#endif

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
static void * __devinit cnxthwusb_probe(struct usb_device *pUsbDevice, unsigned int ifnum, const struct usb_device_id *id);
static void __devexit cnxthwusb_disconnect(struct usb_device *pUsbDevice, void *ptr);
#else
static int __devinit cnxthwusb_probe(struct usb_interface *intf, const struct usb_device_id *id);
static void __devexit cnxthwusb_disconnect(struct usb_interface *intf);
#endif

#ifdef FOUND_LINUX_SUSPEND
static int cnxthwusb_suspend(struct usb_interface *intf, pm_message_t state);
static int cnxthwusb_resume(struct usb_interface *intf);
#endif

static struct usb_driver cnxthwusb_driver = {
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20) ) && defined(FOUND_USB_DRIVER_OWNER)
    .owner          = THIS_MODULE,
#endif
    .name           = CNXTHWUSB_NAME,
    .id_table       = cnxthwusb_tbl,
    .probe          = cnxthwusb_probe,
    .disconnect     = __devexit_p(cnxthwusb_disconnect),
#ifdef FOUND_LINUX_SUSPEND
    .suspend        = cnxthwusb_suspend,
    .resume         = cnxthwusb_resume,
#endif
};

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
#define CFGDESC(x) x
#else
#define CFGDESC(x) desc.x
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
#define USB_BYTEORDER16(x) (__cpu_to_le16(x))
#else
#define USB_BYTEORDER16(x) (x)
#endif

#ifdef FOUND_LINUX_SUSPEND
static int cnxthwusb_suspend(struct usb_interface *intf, pm_message_t state)
{
	COM_STATUS err;
	POS_DEVNODE pDevNode = usb_get_intfdata(intf);

	printk(KERN_DEBUG "%s: pm event 0x%x received\n", __FUNCTION__, GET_PM_MESSAGE_EVENT(state));

	if(!pDevNode)
		return 0;
	
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

	return 0;
}


static int cnxthwusb_resume(struct usb_interface *intf)
{
	POS_DEVNODE pDevNode = usb_get_intfdata(intf);
	COM_STATUS err;

	printk(KERN_DEBUG "%s: called\n", __FUNCTION__);

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
#endif


#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
static void * __devinit cnxthwusb_probe(
	struct usb_device *pUsbDevice, unsigned int ifnum,
	const struct usb_device_id *id)
#else
static int __devinit cnxthwusb_probe(struct usb_interface *intf,
	const struct usb_device_id *id)
#endif
{
    int i;
    struct usb_interface *pUsbDataInterface, *pUsbCommInterface;
    POS_DEVNODE pDevNode = NULL;
    PUSBOSHAL pUsbOsHal = NULL;
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
    struct usb_device *pUsbDevice = interface_to_usbdev(intf);
    __u8 ifnum = intf->altsetting->desc.bInterfaceNumber;
#endif

#if TARGET_HCF_FAMILY
    dbg("%s: pUsbDevice=%p ifnum=%d id=%p bConfigurationValue=%d", __FUNCTION__, pUsbDevice, ifnum, id, pUsbDevice->config[0].CFGDESC(bConfigurationValue));
    if (((USB_BYTEORDER16(pUsbDevice->descriptor.idVendor) != USB_AAPL_VENDOR_ID) &&
		(USB_BYTEORDER16(pUsbDevice->descriptor.idVendor) != USB_CNXT_VENDOR_ID)) ||
	    ((USB_BYTEORDER16(pUsbDevice->descriptor.idProduct) != USB_AAPL_PRODUCT_ID) &&
	     (USB_BYTEORDER16(pUsbDevice->descriptor.idProduct) != USB_CNXT_PRODUCT_ID))) {
	err("Not the one we are interested about");
	goto exit;
    }
#else
    dbg("%s: pUsbDevice=%p ifnum=%d id=%p", __FUNCTION__, pUsbDevice, ifnum, id);
#endif

    if(pUsbDevice->descriptor.bNumConfigurations != 1) {
	err("Wrong number of device configurations (%d)", pUsbDevice->descriptor.bNumConfigurations);
	goto exit;
    }

    if(pUsbDevice->actconfig->CFGDESC(bNumInterfaces) != 2) {
	err("Wrong number of device interfaces (%d)", pUsbDevice->actconfig->CFGDESC(bNumInterfaces));
	goto exit;
    }

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
    pUsbDataInterface = pUsbDevice->actconfig->interface + 0;
#else
    pUsbDataInterface = pUsbDevice->actconfig->interface[0];
#endif
    if(pUsbDataInterface->num_altsetting != 1) {
	warn("DataInterface has more than one alternate setting (%d)", pUsbDataInterface->num_altsetting);
    }
    if(pUsbDataInterface->altsetting->CFGDESC(bNumEndpoints) != DATA_PIPE_NUM) {
	err("Wrong number of endpoints (%d) for DataInterface", pUsbDataInterface->altsetting->CFGDESC(bNumEndpoints));
	goto exit;
    }

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
    pUsbCommInterface = pUsbDevice->actconfig->interface + 1;
#else
    pUsbCommInterface = pUsbDevice->actconfig->interface[1];
#endif
    if(pUsbCommInterface->num_altsetting != 1) {
	warn("CommInterface has more than one alternate setting (%d)", pUsbCommInterface->num_altsetting);
    }
    if(pUsbCommInterface->altsetting->CFGDESC(bNumEndpoints) != INT_PIPE_NUM) {
	err("Wrong number of endpoints (%d) for CommInterface", pUsbCommInterface->altsetting->CFGDESC(bNumEndpoints));
	goto exit;
    }

	if(usb_interface_claimed(pUsbCommInterface)) {
		err("CommInterface already claimed");
		goto exit;
	}

    pUsbOsHal = kmalloc(sizeof(USBOSHAL), GFP_KERNEL);
    if (!pUsbOsHal) {
	err ("Out of memory");
	goto exit;
    }
    memset(pUsbOsHal, 0, sizeof(USBOSHAL));

    for(i=0; i < pUsbDataInterface->altsetting->CFGDESC(bNumEndpoints); i++) {
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
	dbg("%s: pUsbDataInterface endpoint=%d len=%d type=0x%02x addr=0x%02x attr=0x%02x maxsize=%d interval=%d",
		__FUNCTION__,
		i,
		pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bLength),
		pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bDescriptorType),
		pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress),
		pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bmAttributes),
		USB_BYTEORDER16(pUsbDataInterface->altsetting->endpoint[i].CFGDESC(wMaxPacketSize))&0x7ff,
		pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bInterval)
	   );
#endif

	if (pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) == 0x81) {
	    pUsbOsHal->DataInPipe = usb_rcvbulkpipe (pUsbDevice, pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) & USB_ENDPOINT_NUMBER_MASK);
	} else if (pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) == 0x01) {
	    pUsbOsHal->DataOutPipe = usb_sndbulkpipe (pUsbDevice, pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) & USB_ENDPOINT_NUMBER_MASK);
	} else if (pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) == 0x83) {
	    pUsbOsHal->DcpInPipe = usb_rcvbulkpipe (pUsbDevice, pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) & USB_ENDPOINT_NUMBER_MASK);
#if TARGET_HCF_FAMILY
	} else if (pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) == 0x03) {
	    pUsbOsHal->DownloadPipe = usb_sndbulkpipe (pUsbDevice, pUsbDataInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) & USB_ENDPOINT_NUMBER_MASK);
	    if((USB_BYTEORDER16(pUsbDataInterface->altsetting->endpoint[i].CFGDESC(wMaxPacketSize))&0x7ff) == 64)
		pUsbOsHal->UpdateEEPROM = TRUE;
#endif
	}
    }

    for(i=0; i < pUsbCommInterface->altsetting->CFGDESC(bNumEndpoints); i++) {
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
	dbg("%s: pUsbCommInterface endpoint=%d len=%d type=0x%02x addr=0x%02x attr=0x%02x maxsize=%d interval=%d",
		__FUNCTION__,
		i,
		pUsbCommInterface->altsetting->endpoint[i].CFGDESC(bLength),
		pUsbCommInterface->altsetting->endpoint[i].CFGDESC(bDescriptorType),
		pUsbCommInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress),
		pUsbCommInterface->altsetting->endpoint[i].CFGDESC(bmAttributes),
		USB_BYTEORDER16(pUsbCommInterface->altsetting->endpoint[i].CFGDESC(wMaxPacketSize))&0x7ff,
		pUsbCommInterface->altsetting->endpoint[i].CFGDESC(bInterval)
	   );
#endif

	if (pUsbCommInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) == 0x82) {
	    pUsbOsHal->NotifyPipe = usb_rcvintpipe (pUsbDevice, pUsbCommInterface->altsetting->endpoint[i].CFGDESC(bEndpointAddress) & USB_ENDPOINT_NUMBER_MASK);
	    pUsbOsHal->NotifyInterval = pUsbCommInterface->altsetting->endpoint[i].CFGDESC(bInterval);
	    pUsbOsHal->MaxNotifySize = USB_BYTEORDER16(pUsbCommInterface->altsetting->endpoint[i].CFGDESC(wMaxPacketSize))&0x7ff;
	}
    }

    if(!pUsbOsHal->DataInPipe ||
	    !pUsbOsHal->DataOutPipe ||
#if TARGET_HCF_FAMILY
	    !pUsbOsHal->DcpInPipe ||
	    !pUsbOsHal->DownloadPipe ||
#endif
	    !pUsbOsHal->NotifyPipe) {
	err("Missing endpoint(s)");
	goto exit;
    }

    if(!pUsbOsHal->NotifyInterval)
	pUsbOsHal->NotifyInterval = 1; // ms

    pUsbOsHal->pUsbDevice = pUsbDevice;
    pUsbOsHal->pUsbCommInterface = pUsbCommInterface;
    pUsbOsHal->pUsbDataInterface = pUsbDataInterface;

    pUsbOsHal->ControlRequestEvent = OsEventCreate("USB ControlRequestEvent");
    if(!pUsbOsHal->ControlRequestEvent) {
	goto exit;
    }

    pDevNode = kmalloc(sizeof(*pDevNode), GFP_KERNEL);
    if(!pDevNode) {
	err ("Out of memory");
	goto exit;
    }

    memset(pDevNode, 0, sizeof(*pDevNode));
    pDevNode->hwDev = pUsbOsHal;
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	pDevNode->hwDevLink = &pUsbDevice->dev;
#endif
    pDevNode->hwModule = THIS_MODULE;
    strncpy(pDevNode->hwProfile, (char*)CNXTHWCFG("cadmus2"), sizeof(pDevNode->hwProfile));
    pDevNode->hwProfile[sizeof(pDevNode->hwProfile)-1] = '\0';
    snprintf(pDevNode->hwInstName, sizeof(pDevNode->hwInstName), "USB-%04x:%04x",
	    USB_BYTEORDER16(pUsbDevice->descriptor.idVendor), USB_BYTEORDER16(pUsbDevice->descriptor.idProduct));
#ifdef CNXTHWUSB_TYPE
    pDevNode->hwType = CNXTHWUSB_TYPE;
    pDevNode->hwIf = GetHwFuncs();
#endif
    pDevNode->pmControl = cnxthw_DevMgrPMControl;
	pDevNode->osPageOffset = PAGE_OFFSET;

    usb_driver_claim_interface(&cnxthwusb_driver, pUsbCommInterface, pDevNode);

    if(!OsUsbAllocateUrbs(pUsbOsHal)) {
	err("Cannot allocate URBs");
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	usb_set_intfdata (pUsbCommInterface, NULL);
#endif
	usb_driver_release_interface(&cnxthwusb_driver, pUsbCommInterface);
	goto exit;
    }

    pUsbOsHal->bActive = TRUE;

    if(OsUsbFWDownload (pUsbOsHal)) {
	err("Firmware download failed");
	pUsbOsHal->bActive = FALSE;
	OsUsbFreeUrbs(pUsbOsHal);
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	usb_set_intfdata (pUsbCommInterface, NULL);
#endif
	usb_driver_release_interface(&cnxthwusb_driver, pUsbCommInterface);
	goto exit;
    }

    if (cnxt_serial_add(pDevNode, 0, pUsbDevice, ifnum, THIS_MODULE) < 0) {
	pUsbOsHal->bActive = FALSE;
	OsUsbFreeUrbs(pUsbOsHal);
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	usb_set_intfdata (pUsbCommInterface, NULL);
#endif
	usb_driver_release_interface(&cnxthwusb_driver, pUsbCommInterface);
	goto exit;
    }

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
    return pDevNode;
#else
    usb_set_intfdata (intf, pDevNode);
    return 0;
#endif

exit:

    if(pDevNode) {
	kfree(pDevNode);
    }
    if(pUsbOsHal) {
	if(pUsbOsHal->ControlRequestEvent)
    		OsEventDestroy(pUsbOsHal->ControlRequestEvent);
	kfree(pUsbOsHal);
    }
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
    return NULL;
#else
    return -ENODEV;
#endif
}


#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
static void __devexit cnxthwusb_disconnect(struct usb_device *pUsbDevice, void *ptr)
#else
static void __devexit cnxthwusb_disconnect(struct usb_interface *intf)
#endif
{
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
    POS_DEVNODE pDevNode = ptr;
#else
    POS_DEVNODE pDevNode = usb_get_intfdata(intf);
#endif
    PUSBOSHAL pUsbOsHal;

    dbg("%s: %p", __FUNCTION__, pDevNode);

    if(!pDevNode)
	return;

    pUsbOsHal = pDevNode->hwDev;
    pDevNode->hwDev = NULL;

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
    usb_set_intfdata (intf, NULL);
#endif

    if(!pUsbOsHal)
	return;

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	usb_set_intfdata ((intf == pUsbOsHal->pUsbCommInterface) ? pUsbOsHal->pUsbDataInterface : pUsbOsHal->pUsbCommInterface, NULL);
#endif

    pUsbOsHal->bActive = FALSE;

    cnxt_serial_remove(pDevNode);

    OsUsbFreeUrbs(pUsbOsHal);

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
    usb_driver_release_interface(&cnxthwusb_driver, pUsbOsHal->pUsbCommInterface);
#else
	// XXX needed on older 2.6 kernels ? usb_driver_release_interface(&cnxthwusb_driver, (intf == pUsbOsHal->pUsbCommInterface) ? pUsbOsHal->pUsbDataInterface : pUsbOsHal->pUsbCommInterface);
#endif 


    OsEventDestroy(pUsbOsHal->ControlRequestEvent);
    kfree(pUsbOsHal);
    pUsbOsHal = NULL;

    kfree(pDevNode);
}




static int __init cnxthwusb_init (void)
{
    int ret;

    dbg("%s", __FUNCTION__);

#ifdef CONFIG_PPC_PMAC
    if(pmacftrmodemenable) {
	ret = pmac_call_feature(PMAC_FTR_MODEM_ENABLE, NULL, 0, 1);
	if(ret) {
	    err("%s: cannot enable internal modem (%d)", __FUNCTION__, ret);
	    return ret;
	}
    }
#endif

    ret = usb_register(&cnxthwusb_driver);
    if(ret) {
	err("%s: usb_register (%d) failed", __FUNCTION__, ret);
    }
    return ret;
}


static void __exit cnxthwusb_cleanup (void)
{
    dbg("%s", __FUNCTION__);

    usb_deregister(&cnxthwusb_driver);

#ifdef CONFIG_PPC_PMAC
    if(pmacftrmodemenable) {
	int ret = pmac_call_feature(PMAC_FTR_MODEM_ENABLE, NULL, 0, 0);
	if(ret) {
	    err("%s: cannot disable internal modem (%d)", __FUNCTION__, ret);
	    return;
	}
    }
#endif
}

module_init(cnxthwusb_init);
module_exit(cnxthwusb_cleanup);

