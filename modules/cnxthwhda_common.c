/*
 * Copyright (c) 2005-2006 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

//MODULE_DEVICE_TABLE(hda, cnxthwhda_tbl);

#ifndef CNXTHW_DECL_MODULE_DEVICE_TABLE_ONLY
MODULE_AUTHOR("Copyright (C) 2005-2006 Linuxant inc.");
MODULE_DESCRIPTION("Conexant low-level hardware driver");
MODULE_LICENSE("GPL\0for files in the \"GPL\" directory; for others, only LICENSE file applies");
MODULE_INFO(supported, "yes");
#endif

#include "oshda.h"
#include "comctrl_ex.h"

#include "cnxthw_common.c"

void *GetHwFuncs(void);

#if 0
static int __devinit cnxthwhda_probe(struct hda_interface *intf, const struct hda_device_id *id);
static void __devexit cnxthwhda_disconnect(struct hda_interface *intf);

static struct hda_driver cnxthwhda_driver = {
    .owner          = THIS_MODULE,
    .name           = CNXTHWHDA_NAME,
//    .id_table       = cnxthwhda_tbl,
    .probe          = cnxthwhda_probe,
    .disconnect     = __devexit_p(cnxthwhda_disconnect),
#ifdef FOUND_LINUX_SUSPEND
    .suspend        = cnxthwhda_suspend,
    .resume         = cnxthwhda_resume,
#endif
};
#endif

/*static*/ int cnxthwhda_suspend(void *devnode, pm_message_t state)
{
	COM_STATUS err;
	POS_DEVNODE pDevNode = devnode;

	//printk(KERN_DEBUG"%s: pm event 0x%x received\n", __FUNCTION__, GET_PM_MESSAGE_EVENT(state));

	if(!pDevNode)
		return 0;
	
	if (pDevNode->hwInUse)
	{
		printk(KERN_ERR"%s: suspend failed, hardware is in use.\n", __FUNCTION__);
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


/*static*/ int cnxthwhda_resume(void *devnode)
{
	POS_DEVNODE pDevNode = devnode;
	COM_STATUS err;

	//printk(KERN_DEBUG "%s: called\n", __FUNCTION__);

	if (pDevNode && pDevNode->hcomctrl)
	{
		err = ComCtrl_Control(pDevNode->hcomctrl, COMCTRL_CONTROL_WAKEUP, NULL);
		if (err != COM_STATUS_SUCCESS) printk(KERN_ERR "%s: ComCtrl_Control returned %d.\n", __FUNCTION__, err);
	}
	else
		printk(KERN_WARNING "%s: low level power management was not called.\n", __FUNCTION__);

	return 0;
}

static int cnxthwhda_setsyms (void **ppInterfaceFuncs);

/*static*/ int /*__devinit*/ cnxthwhda_probe(void *codec, struct device *ctrlDev, void **ppDevNode, void **ppInterfaceFuncs)
{
	POS_DEVNODE pDevNode = NULL;
	PHDAOSHAL pHdaOsHal;

//	printk(KERN_DEBUG"%s: codec=%p\n", __FUNCTION__, codec);

	cnxthwhda_setsyms(ppInterfaceFuncs);

	pHdaOsHal = kmalloc(sizeof(HDAOSHAL), GFP_KERNEL);
	if (!pHdaOsHal) {
		printk(KERN_ERR"%s: kmalloc HDAOSHAL failed\n", __FUNCTION__);
		goto exit;
	}
	memset(pHdaOsHal, 0, sizeof(HDAOSHAL));

	pHdaOsHal->hda_codec = codec;

	pDevNode = kmalloc(sizeof(*pDevNode), GFP_KERNEL);
	if(!pDevNode) {
		printk(KERN_ERR"%s: kmalloc OS_DEVNODE failed\n", __FUNCTION__);
		goto exit;
	}

	memset(pDevNode, 0, sizeof(*pDevNode));
	pDevNode->hwDev = pHdaOsHal;
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	pDevNode->hwDevLink = ctrlDev;
#endif
	pDevNode->hwModule = THIS_MODULE;
	strncpy(pDevNode->hwProfile, (char*)CNXTHWCFG("hda"), sizeof(pDevNode->hwProfile));
	pDevNode->hwProfile[sizeof(pDevNode->hwProfile)-1] = '\0';
	snprintf(pDevNode->hwInstName, sizeof(pDevNode->hwInstName), "HDA-%08x:%08x-%u",
			OsHdaCodecGetVendorId(pHdaOsHal), OsHdaCodecGetSubsystemId(pHdaOsHal), OsHdaCodecGetAddr(pHdaOsHal));
	pDevNode->hwType = CNXTHWHDA_TYPE;
	pDevNode->hwIf = GetHwFuncs();
	pDevNode->pmControl = cnxthw_DevMgrPMControl;
	pDevNode->osPageOffset = PAGE_OFFSET;

	if (cnxt_serial_add(pDevNode, 0, codec, OsHdaCodecGetAddr(pHdaOsHal), THIS_MODULE) < 0) {
		goto exit;
	}

	*ppDevNode = pDevNode;
	return 0;

exit:

	if(pDevNode)
		kfree(pDevNode);

	if(pHdaOsHal)
		kfree(pHdaOsHal);

	*ppDevNode = NULL;
	return -ENODEV;
}


/*static*/ void /*__devexit*/ cnxthwhda_remove(void *ptr)
{
	POS_DEVNODE pDevNode = ptr;
	PHDAOSHAL pHdaOsHal;

//	printk(KERN_DEBUG"%s: %p\n", __FUNCTION__, pDevNode);

	pHdaOsHal = pDevNode->hwDev;

	cnxt_serial_remove(pDevNode);

	kfree(pDevNode);
	kfree(pHdaOsHal);
}

EXPORT_SYMBOL(cnxthwhda_probe);
EXPORT_SYMBOL(cnxthwhda_remove);
EXPORT_SYMBOL(cnxthwhda_suspend);
EXPORT_SYMBOL(cnxthwhda_resume);


__shimcall__ unsigned int (*OsHdaCodecGetAddr)(PHDAOSHAL pHdaOsHal) __shimcall__;
__shimcall__ unsigned int (*OsHdaCodecGetVendorId)(PHDAOSHAL pHdaOsHal) __shimcall__;
__shimcall__ unsigned int (*OsHdaCodecGetSubsystemId)(PHDAOSHAL pHdaOsHal) __shimcall__;
__shimcall__ unsigned int (*OsHdaCodecGetRevisionId)(PHDAOSHAL pHdaOsHal) __shimcall__;
__shimcall__ unsigned int (*OsHdaCodecRead)(PHDAOSHAL pHdaOsHal, unsigned short nid, int direct, unsigned int verb, unsigned int para) __shimcall__;
__shimcall__ unsigned int (*OsHdaCodecWallclock)(PHDAOSHAL pHdaOsHal) __shimcall__;
__shimcall__ void (*OsHdaCodecSetEventCallback)(PHDAOSHAL pHdaOsHal, void (*cbHdaEvent)(void *Context, unsigned int res), void *cbHdaEventContext, unsigned char *cbHdaTag) __shimcall__;
__shimcall__ void (*OsHdaCodecClearEventCallback)(PHDAOSHAL pHdaOsHal, unsigned char cbHdaTag) __shimcall__;
__shimcall__ int (*OsHdaCodecOpenDMA)(PHDAOSHAL pHdaOsHal, int hw_param_buffer_bytes, void **ppPlaybackStream, void **ppCaptureStream) __shimcall__;
__shimcall__ void (*OsHdaCodecCloseDMA)(PHDAOSHAL pHdaOsHal, void *pPlaybackStream, void *pCaptureStream) __shimcall__;
__shimcall__ void (*OsHdaCodecDMAInfo)(PHDAOSHAL pHdaOsHal, void *Stream, unsigned char *StreamID, unsigned long *FifoSize, short unsigned int **ppBufAddr) __shimcall__;
__shimcall__ int (*OsHdaCodecSetDMAState)(PHDAOSHAL pHdaOsHal, OSHDA_STREAM_STATE streamState, void *pPlaybackStream, void *pCaptureStream) __shimcall__;
__shimcall__ unsigned long (*OsHdaCodecGetDMAPos)(PHDAOSHAL pHdaOsHal, void *Stream) __shimcall__;

static int cnxthwhda_setsyms (void **ppInterfaceFuncs)
{
	OsHdaCodecGetAddr = *ppInterfaceFuncs++;
	OsHdaCodecGetVendorId = *ppInterfaceFuncs++;
	OsHdaCodecGetSubsystemId = *ppInterfaceFuncs++;
	OsHdaCodecGetRevisionId = *ppInterfaceFuncs++;
	OsHdaCodecRead = *ppInterfaceFuncs++;
	OsHdaCodecWallclock = *ppInterfaceFuncs++;
	OsHdaCodecSetEventCallback = *ppInterfaceFuncs++;
	OsHdaCodecClearEventCallback = *ppInterfaceFuncs++;
	OsHdaCodecOpenDMA = *ppInterfaceFuncs++;
	OsHdaCodecCloseDMA = *ppInterfaceFuncs++;
	OsHdaCodecDMAInfo = *ppInterfaceFuncs++;
	OsHdaCodecSetDMAState = *ppInterfaceFuncs++;
	OsHdaCodecGetDMAPos = *ppInterfaceFuncs++;

	return 0;
}

static int __init cnxthwhda_init (void)
{
#if 1
	return 0;
#else
	int ret;

//	printk(KERN_DEBUG"%s\n", __FUNCTION__);
	ret = hda_register(&cnxthwhda_driver);
	if(ret) {
		printk(KERN_ERR"%s: hda_register (%d) failed\n", __FUNCTION__, ret);
	}
	return ret;
#endif
}


static void __exit cnxthwhda_cleanup (void)
{
#if 0
//	printk(KERN_DEBUG"%s\n", __FUNCTION__);

	hda_deregister(&cnxthwhda_driver);
#endif
}

module_init(cnxthwhda_init);
module_exit(cnxthwhda_cleanup);
