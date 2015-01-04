/*
 * Octopus / Cadmus2 USB Adaptation Layer for Linux
 * Written by Marc Boucher <marc@linuxant.com>
 *
 * Copyright (c) 2002-2004 Linuxant inc.
 * All rights reserved.
 * 
 * TODO:
 * 	- Proper locking
 * 	- DCP support
 * 	- Cleanup
 */


#include "oscompat.h"

#if defined(CONFIG_USB) || defined(CONFIG_USB_MODULE)
#include "osservices.h"

#include <linux/slab.h>
#include <asm/io.h>
#include <asm/pgtable.h>

/*#undef DEBUG
#undef _DEBUG*/

#include "osusb.h"
#include "c2firmware.h"

#ifdef _DEBUG
#define CNXT_BREAKPOINT(msg)     { err(msg); KDB_ENTER();   }
#undef ASSERT
#define ASSERT(x) { if(!(x)) { err("Assertion (%s) failed in %s() (%s:%d)", #x, __FUNCTION__, __FILE__, __LINE__); CNXT_BREAKPOINT("Assert"); } }
#else
#define CNXT_BREAKPOINT(msg)
#undef ASSERT
#define ASSERT(x)
#endif

//#undef dbg
//#define dbg(format, arg...) { UINT32 dwTime = OsGetSystemTime(); printk(KERN_DEBUG "%07lu.%03lu: "__FILE__ ": " format "\n" , dwTime/1000, dwTime%1000, ## arg); }
//#define dbg(format, arg...) do {} while (0)

#define Working TRUE
#define GET_DEVICE_PNP_STATE(_USBOSHAL_) (((_USBOSHAL_) != NULL) && (_USBOSHAL_)->bActive)

#define IsKMallocMem(vaddr) (((void *)vaddr) < high_memory)

static const char *uleStateNames[] = {
	"FREE",
	"ALLOCATED",
	"PENDING",
	"SUBMITTED",
	"COMPLETED",
	"ABORTED",
	"FAILED",
	"LAST"
};

#if 0
// 20 times pipe can be stalled and reset
#define ALLOWED_PIPE_RESET_ATTEMPTS 20
#endif

static void	SubmitNextUrbListEntry(PURB_LIST_ENTRY ule);

static void
ChangeUrbListEntryState(PURB_LIST_ENTRY ule, URB_LIST_ENTRY_STATE newstate)
{
	URB_LIST_ENTRY_STATE oldstate;

	dbg("%s: ule=%p %s -> %s", __FUNCTION__, ule,
			uleStateNames[ule->state],
		   	uleStateNames[newstate]);
#if 0
	{
		int i;

		for(i = 0; i < ULE_LAST; i++) {
			dbg("%s: ul=%p %s=%d", __FUNCTION__, ule->ul,
					uleStateNames[i], ule->ul->count[i]);
		}
	}
#endif

	oldstate = ule->state;
	if(oldstate == newstate)
		return;

	ule->ul->count[ule->state]--;
	ule->state = newstate;
	ule->ul->count[ule->state]++;


	if(oldstate == ULE_QUEUED) {
		list_del(&ule->queued_entry);
	}
	if(newstate == ULE_QUEUED) {
		list_add_tail(&ule->queued_entry, &ule->ul->queued_list);
	}

	if(newstate == ULE_SUBMITTED) {
		OsModuleUseCountInc();
	} else if(oldstate == ULE_SUBMITTED) {
		OsModuleUseCountDec();
		if((newstate == ULE_COMPLETED) || (newstate == ULE_FAILED))
			SubmitNextUrbListEntry(ule);
	}

	if(newstate >= ULE_COMPLETED) {
		ChangeUrbListEntryState(ule, ULE_FREE);
	}
}

static BOOL
SubmitUrbListEntry(PURB_LIST_ENTRY ule)
{
	int res;

	if(0 == ule->ul->count[ULE_SUBMITTED]) {
		ule->startTime = OsGetSystemTime();
		ChangeUrbListEntryState(ule, ULE_SUBMITTED);
		if ((res=usb_submit_urb(ule->urb
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
			, OsContextAllowsSleeping() ? GFP_KERNEL : GFP_ATOMIC
#endif
		))) {
			warn("%s: submit failed: %d", __FUNCTION__, res);
			ChangeUrbListEntryState(ule, ULE_FREE);
			return FALSE;
		}
	} else {
		ChangeUrbListEntryState(ule, ULE_QUEUED);
		dbg("%s: submitted count=%d, added %p to pending queue (count=%d)",
				__FUNCTION__,
				ule->ul->count[ULE_SUBMITTED],
				ule,
				ule->ul->count[ULE_QUEUED]);
	}

	return TRUE;
}


static void	SubmitNextUrbListEntry(PURB_LIST_ENTRY ule)
{
	if((0 != ule->ul->count[ULE_QUEUED]) &&
			(0 == ule->ul->count[ULE_SUBMITTED])) {
		PURB_LIST_ENTRY next_ule;

		ASSERT(!list_empty(&ule->ul->queued_list));

		next_ule = list_entry(ule->ul->queued_list.next, URB_LIST_ENTRY, queued_entry);
		dbg("%s: %p", __FUNCTION__, next_ule);

		SubmitUrbListEntry(next_ule); // XXX: TODO: check return code, complete req upon failure
	}
}

static PURB_LIST_ENTRY
GetFreeUrbFromList(PURB_LIST ul)
{
	struct list_head *e;
	PURB_LIST_ENTRY ule;

	list_for_each (e, &ul->urb_list) {
		ule = list_entry(e, URB_LIST_ENTRY, entry);
		if(ule->state == ULE_FREE) {
			ChangeUrbListEntryState(ule, ULE_ALLOCATED);

			ule->userBufferPtr = NULL;
			ule->userBufferSize = 0;
			ule->startTime = 0;
			dbg("%s: ul=%p ule=%p", __FUNCTION__, ul, ule);
			return ule;
		}

	}
	//warn("%s: no more (ul=%p)", __FUNCTION__, ul);
	return NULL;
}

static PURB_LIST_ENTRY AllocateUrbListEntry(PURB_LIST ul, PUSBOSHAL pUsbOsHal, size_t bufferSize)
{
	PURB_LIST_ENTRY ule;

	ule = (PURB_LIST_ENTRY) kmalloc(sizeof(URB_LIST_ENTRY) + bufferSize, OsContextAllowsSleeping() ? GFP_KERNEL : GFP_ATOMIC);
	if(!ule) {
		err("%s: kmalloc failed", __FUNCTION__);
		return NULL;
	}

	memset(ule, 0, sizeof(*ule));
	ule->state = ULE_FREE;
	ule->ul = ul;
	ule->pUsbOsHal = pUsbOsHal;
	ule->urb = usb_alloc_urb(0
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
			, OsContextAllowsSleeping() ? GFP_KERNEL : GFP_ATOMIC
#endif
		);
	if(!ule->urb) {
		err("%s: usb_alloc_urb failed", __FUNCTION__);
		kfree(ule);
		return NULL;
	}

	if(bufferSize) {
		ule->bufferPtr = (PUINT8)(ule + 1);
		ule->bufferSize = bufferSize;
	}

	dbg("%s: ul=%p ule=%p bufsize=%ld", __FUNCTION__, ul, ule, bufferSize);
	return ule;
}

static void FreeUrbListEntry(PURB_LIST_ENTRY ule)
{
	dbg("%s: ule=%p", __FUNCTION__, ule);
	usb_free_urb(ule->urb);
	kfree(ule);
}

static void AbortListUrbs(PURB_LIST ul)
{
	struct list_head *e;
	PURB_LIST_ENTRY ule;

	dbg("%s: ul=%p", __FUNCTION__, ul);
	list_for_each (e, &ul->urb_list) {
		ule = list_entry(e, URB_LIST_ENTRY, entry);
		dbg("%s: ule=%p", __FUNCTION__, ule);
		if(ule->state != ULE_FREE) {
			dbg("%s: ule %p (urb %p) was still in state %s\n", __FUNCTION__,
					ule, ule->urb, uleStateNames[ule->state]);
			if(ule->state == ULE_SUBMITTED) {
				dbg("%s: unlinking ule %p (urb %p)\n", __FUNCTION__, ule, ule->urb);
#ifdef URB_ASYNC_UNLINK
				ule->urb->transfer_flags &= ~URB_ASYNC_UNLINK;
				usb_unlink_urb(ule->urb);
#else
				usb_kill_urb(ule->urb);
#endif
			} else
				ChangeUrbListEntryState(ule, ULE_ABORTED);
		}
	}
}

static void FreeUrbList(PURB_LIST ul)
{
	struct list_head *e, *n;
	PURB_LIST_ENTRY ule;

	dbg("%s: ul=%p", __FUNCTION__, ul);
	AbortListUrbs(ul);
	list_for_each_safe (e, n, &ul->urb_list) {
		ule = list_entry(e, URB_LIST_ENTRY, entry);
		dbg("%s: ule=%p", __FUNCTION__, ule);
		if(ule->state != ULE_FREE) {
			warn("%s: unlinking ule %p (urb %p); was in state %s\n", __FUNCTION__,
					ule, ule->urb, uleStateNames[ule->state]);
#ifdef URB_ASYNC_UNLINK
			ule->urb->transfer_flags &= ~URB_ASYNC_UNLINK;
			usb_unlink_urb(ule->urb);
#else
			usb_kill_urb(ule->urb);
#endif
		}
		ul->count[ule->state]--;
		list_del(&ule->entry);
		FreeUrbListEntry(ule);
	}
}

static BOOL InitUrbList(PURB_LIST ul, PUSBOSHAL pUsbOsHal, size_t bufferSize, int nurbs)
{
	INIT_LIST_HEAD(&ul->urb_list);
	INIT_LIST_HEAD(&ul->queued_list);

	dbg("%s: ul=%p bufsize=%ld nurbs=%d", __FUNCTION__, ul, bufferSize, nurbs);
	while(nurbs--) {
		PURB_LIST_ENTRY ule = AllocateUrbListEntry(ul, pUsbOsHal, bufferSize);
		dbg("%s: ule=%p", __FUNCTION__, ule);
		if(!ule) {
			FreeUrbList(ul);
			return FALSE;
		}
		list_add(&ule->entry, &ul->urb_list);
		ul->count[ULE_FREE]++;
	}

	return TRUE;
}


/******************************************************************************
 two functions below indicate to lower HAL that the modem instance is being allocated or
 deallocated. They DO NOT cause alocation/deallocation of the lower HAL which
 typically happens when device is plugged in or out
******************************************************************************/


/******************************************************************************

Name:           OsUsbCreateInstance

Description:    Indicate to the bottom HAL that modem instance is being created

Arguments:      pDeviceID	- OS specific unique ID of a device opened, such as PnP ID
				hUpper		- opaque handle to the top HAL for callbacks

Return Value:   New bottom HAL handle, NULL if failure

Notes:

******************************************************************************/
__shimcall__
HANDLE OsUsbCreateInstance (PVOID pDeviceID, HANDLE hUpper,
		__shimcall__ BOOL (*cbUsbHalNotificationReceived) (PVOID hUpper, PUINT8 pDataReceived, unsigned RecvLength),
		__shimcall__ void (*cbUsbHalDataReceived)			(PVOID hUpper, PUINT8 pDataReceived, unsigned RecvLength),
		__shimcall__ void (*cbUsbHalDataTransmitDone)		(PVOID hUpper, unsigned TxLength),
		__shimcall__ void (*cbUsbHalControlTransferDone)	(PVOID hUpper, PVOID userContext, UINT8 Request, UINT16 value, UINT16 index),
#if TARGET_HCF_FAMILY
		__shimcall__ void (*cbUsbHalDownloadTransmitDone) (PVOID hUpper),
#endif
		__shimcall__ void (*cbUsbEEPROM_Restore)	(HANDLE hOsUsb))
{
	PUSBOSHAL			pUsbOsHal = (PUSBOSHAL)pDeviceID;

	dbg("%s: pDeviceID=%p hUpper=%p", __FUNCTION__, pDeviceID, hUpper);

	if(!pUsbOsHal)
		return NULL;

	pUsbOsHal->hUpper = hUpper;

	pUsbOsHal->cbUsbHalNotificationReceived = cbUsbHalNotificationReceived;
	pUsbOsHal->cbUsbHalDataReceived = cbUsbHalDataReceived;
	pUsbOsHal->cbUsbHalDataTransmitDone = cbUsbHalDataTransmitDone;
	pUsbOsHal->cbUsbHalControlTransferDone = cbUsbHalControlTransferDone;
#if TARGET_HCF_FAMILY
	pUsbOsHal->cbUsbHalDownloadTransmitDone = cbUsbHalDownloadTransmitDone;
#endif

	if(pUsbOsHal->UpdateEEPROM && cbUsbEEPROM_Restore) {
		info("Restoring USBEEPROM");
			cbUsbEEPROM_Restore (pUsbOsHal);
	}

#if TARGET_HCF_FAMILY
    usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DataInPipe);
    usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DataOutPipe);
    usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DcpInPipe);
    usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DownloadPipe);
#endif
    usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->NotifyPipe);
   
	OsModuleUseCountInc();

	return (HANDLE)pUsbOsHal;
}

/******************************************************************************

Name:           OsUsbDestroyInstance

Description:    Indicate to the bottom HAL that modem instance is being destroyed

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   None

Notes:          Top HAL handle is invalid after this call. Bottom HAL must not call
				any callback routines

******************************************************************************/
__shimcall__
void OsUsbDestroyInstance (HANDLE hUsbOsHal)
{
	PUSBOSHAL		pUsbOsHal = (PUSBOSHAL)hUsbOsHal;

	dbg("%s: %p", __FUNCTION__, pUsbOsHal);

	pUsbOsHal->hUpper = 0;

	if (Working == GET_DEVICE_PNP_STATE(pUsbOsHal))
	{
		usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DataInPipe);
		usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DataOutPipe);
		usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DcpInPipe);
#if TARGET_HCF_FAMILY
		usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DownloadPipe);
#endif
		usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->NotifyPipe);
		OsEventSet (pUsbOsHal->ControlRequestEvent);
	}

	AbortListUrbs(&pUsbOsHal->NotifyUrbList);
	AbortListUrbs(&pUsbOsHal->RecvUrbList);
	AbortListUrbs(&pUsbOsHal->SendUrbList);
	AbortListUrbs(&pUsbOsHal->ControlUrbList);
#if TARGET_HCF_FAMILY
	AbortListUrbs(&pUsbOsHal->DownloadUrbList);
#endif
#if defined(USE_DCP) && TARGET_HCF_FAMILY
	AbortListUrbs(&pUsbOsHal->DcpUrbList);
#endif

#if !TARGET_HCF_FAMILY
{
	int err;
#if (defined(ref_to_usb_interface_cache) || ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,7) )) && ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10))
	err = __usb_reset_device(pUsbOsHal->pUsbDevice);
#else
	err = usb_reset_device(pUsbOsHal->pUsbDevice);
#endif
	if (err)
		err("%s: Failed to reset device (%d)", __FUNCTION__, err);
}
#endif

	// IB: Need to stop everything here
	OsSleep(10);

	OsModuleUseCountDec();
}

//It downloads FW including script engine.
//The FW starts after been loaded.
//Do not need to wait after download.
__shimcall__
int OsUsbFWDownload (PUSBOSHAL pUsbOsHal)
{
	int i;
	int ret = 0;
#if TARGET_HCF_FAMILY
	unsigned char info[4];
#else
	unsigned char info[5];
#endif
	int fwSize;
	PUINT8 pC2Firmware;

	dbg("%s: %p", __FUNCTION__, pUsbOsHal);

	for (i=0; i<3; i++)			// make 3 attempts
	{
		ret = usb_control_msg(pUsbOsHal->pUsbDevice,
				usb_rcvctrlpipe(pUsbOsHal->pUsbDevice, 0),
				CD2_GET_INFROMATION,
				USB_TYPE_VENDOR | USB_DIR_IN | USB_RECIP_DEVICE,
				0,
				i,
				info,
				sizeof(info),
				USB_MS_TIMEOUT(1000));

		if (ret == sizeof(info))
		{
			dbg("GET INFO BEFORE DOWNLOAD: %d",ret);
			ret = 0;
			break;
		}
	}
	if (i == 3)
	{
		err("CD2_GET_INFROMATION returned error %d", ret);
		return ret;
	}

	dbg("FW Information: %x %x %x %x",info[0],info[1],info[2],info[3]);

    if(info[2] == 0x1)
    {
		switch (info[3])
		{
		case 3:		// X4
#ifndef FW_BLOCK_SIZE
#define FW_BLOCK_SIZE 64
#endif
#if TARGET_HCF_FAMILY
			pC2Firmware = C2FirmwareX4;
			fwSize = sizeof(C2FirmwareX4);
#else
			pC2Firmware = ROM_IMAGE;
			fwSize = sizeof(ROM_IMAGE);
#endif
			dbg("Uploading ROM_IMAGE size=%d", fwSize);
			break;
		default:
			err("Unrecognized minor version %x", info[3]);
			return -EINVAL;
		}

#if !TARGET_HCF_FAMILY
#define SET_TIMEOUT 5
		/*ret =*/ usb_control_msg(pUsbOsHal->pUsbDevice, usb_sndctrlpipe(pUsbOsHal->pUsbDevice, 0),
				USB_REQ_SET_CONFIGURATION, 0, 0/*configuration*/, 0, NULL, 0, USB_MS_TIMEOUT(5000));

		//usb_set_address(pUsbOsHal->pUsbDevice);

		{ // discard any bogus CDC ACM (0xA1) notifications on notification pipe
			PVOID cbUsbHalNotificationReceived = pUsbOsHal->cbUsbHalNotificationReceived;
			pUsbOsHal->cbUsbHalNotificationReceived = NULL;
			if(OsUsbMakeNotifyRequest(pUsbOsHal)) {
				OsSleep(50);
				AbortListUrbs(&pUsbOsHal->NotifyUrbList);

			}
			pUsbOsHal->cbUsbHalNotificationReceived = cbUsbHalNotificationReceived;

		}
		//dev->toggle[0] = 0;
		//dev->toggle[1] = 0;
#endif


		/* without this, downloads occasionally fail */
		OsSleep (50);

		i = 0;
		while (i<fwSize) {
			UINT32 nBytes = fwSize - i;
			
			if(nBytes > FW_BLOCK_SIZE)
				nBytes = FW_BLOCK_SIZE;

			if (!OsUsbMakeControlRequestSync((HANDLE)pUsbOsHal, 0,
					CD2_UPLOAD_FIRMWARE, fwSize,
#if 1 // XXX TARGET_HCF_FAMILY
					i,
#else
					0,
#endif
					pC2Firmware+i, &nBytes)) {
				err("CD2_UPLOAD_FIRMWARE error nBytes=%d i=%d", nBytes, i);
				return -EIO;
			}

			i += nBytes;
        }

        for (i=0; i<10; i++)
        {
			OsSleep(100);

			ret = usb_control_msg(pUsbOsHal->pUsbDevice,
					usb_rcvctrlpipe(pUsbOsHal->pUsbDevice,0),
					CD2_GET_INFROMATION,
					USB_TYPE_VENDOR | USB_DIR_IN | USB_RECIP_DEVICE,
					0,
					i,
					info,
					sizeof(info),
					USB_MS_TIMEOUT(1000));

			if (ret == sizeof(info))
			{
				dbg("GET INFO AFTER DOWNLOAD: %x", ret);
				dbg("FW Information: %x %x %x %x",info[0],info[1],info[2],info[3]);
				ret = 0;
				break;
			}

			dbg("GET INFO AFTER DOWNLOAD err=%d", ret);
		}
           
		if(i != 10) {
#if TARGET_HCF_FAMILY
           if(info[2] != 0x2)    // 0x2 - HCF, 0x3 - HSF
#else
           if(info[2] != 0x3)    // 0x2 - HCF, 0x3 - HSF
#endif
           {
              ret = -EINVAL;
          	  err("ERROR!!! Wrong Firmware is loaded: %x.",info[2]);
           }
		}

#if !TARGET_HCF_FAMILY
		//usb_get_configuration(pUsbOsHal->pUsbDevice);
#if 1
		/*ret =*/ usb_control_msg(pUsbOsHal->pUsbDevice, usb_sndctrlpipe(pUsbOsHal->pUsbDevice, 0),
				USB_REQ_SET_CONFIGURATION, 0, 1/*configuration*/, 0, NULL, 0, USB_MS_TIMEOUT(5000));
#else
		usb_set_configuration(pUsbOsHal->pUsbDevice, 1);
#endif
#endif
    }
    else
    {
		dbg("Firmware is ALREADY loaded.");
		ret = 0;
    }
    
	dbg("-USBHal_FWDownload");
    return ret;
}

/******************************************************************************

Name:           OsUsbIsStarted

Description:    Query if device is configured and is ready to accept USB transactions

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   TRUE if device is ready, FALSE if not

Notes:

******************************************************************************/
__shimcall__
BOOL OsUsbIsStarted (HANDLE hUsbOsHal)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;

//	dbg("%s: %p returning %s", __FUNCTION__, pUsbOsHal, (Working == GET_DEVICE_PNP_STATE(pUsbOsHal)) ? "TRUE" : "FALSE");

	return (Working == GET_DEVICE_PNP_STATE(pUsbOsHal));
;
}


/******************************************************************************

Name:           OsUsbResetPort

Description:    Reset USB port.

Arguments:      hUsbOsHal	- bottom HAL handle.

Return Value:   None

******************************************************************************/
__shimcall__
void OsUsbResetPort(HANDLE hUsbOsHal)
{
#if 1
//	int ret;

//	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;

	dbg("%s: %p", __FUNCTION__, hUsbOsHal);
#if 0
	ret = usb_reset_device(pUsbOsHal->pUsbDevice);
	if(ret) {
		err("OsUsbResetPort: usb_reset_device() failed, ret = %d", ret);
		return;
	}

	ret =  OsUsbFWDownload(pUsbOsHal);
    if (ret)
    {
		err("OsUsbResetPort: OsUsbFWDownload failed, ret = %d", ret);

    }
#endif
#else
	PUSBOSHAL			pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
    NTSTATUS			ntStatus;
	PDEVICE_EXTENSION	pDeviceExtension = pUsbOsHal->pDeviceExtension;
    ULONG				PortStatus = 0;

	ntStatus = CallUSBDI_Sync(pUsbOsHal, IOCTL_INTERNAL_USB_GET_PORT_STATUS, &PortStatus);
	RTRACE2(T__HALOS, 0,"OsUsbResetPort: IOCTL_INTERNAL_USB_GET_PORT_STATUS  NTSTATUS = 0x%x, Port Status = %X", ntStatus, PortStatus);
    if (NT_ERROR(ntStatus))
    {
		RTRACE1(T__HALOS|T__ERROR, 0,"!! OsUsbResetPort:IOCTL_INTERNAL_USB_GET_PORT_STATUS returns error  0x%x", ntStatus);
		return;
    }
	
	if ( !(PortStatus & USBD_PORT_CONNECTED) )
	{
		RTRACE0(T__HALOS|T__ERROR, 0,"!! OsUsbResetPort: Port is NOT connected");
		return;
	}

	if ( PortStatus & USBD_PORT_ENABLED )	// If Port is already enabled, 
	{										// all outstanding requests for a pipe must be canceled before issuing IOCTL_INTERNAL_USB_RESET_PORT.
		RTRACE0(T__HALOS, 0,"OsUsbResetPort: Port is already enabled. ");

		USBMODEM_KillIO (pUsbOsHal);
	}
	ntStatus = CallUSBDI_Sync(pUsbOsHal, IOCTL_INTERNAL_USB_RESET_PORT, NULL);

	RTRACE1(T__HALOS, 0,"OsUsbResetPort: IOCTL_INTERNAL_USB_RESET_PORT  NTSTATUS = 0x%x", ntStatus);

	if (NT_ERROR(ntStatus))
	{
		RTRACE1(T__HALOS|T__ERROR, 0,"!! OsUsbResetPort: IOCTL_INTERNAL_USB_RESET_PORT returns error  0x%x", ntStatus);
		return;
	}

	ntStatus =  OsUsbFWDownload(pUsbOsHal);
    if (NT_ERROR(ntStatus))
    {
		RTRACE1(T__HALOS|T__ERROR, 0,"!! OsUsbResetPort: OsUsbFWDownload returns error  0x%x", ntStatus);
    }
#endif
}


/*****************************************************************************
/
/
/           USB completion routines
/
/
*****************************************************************************/

static void UsbControlCompletionRoutine (PURB urb
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) ) && defined(FOUND_USB_COMPLETE_PT_REGS)
	, struct pt_regs *regs
#endif
	)
{
	PURB_LIST_ENTRY ule = urb->context;
	PUSBOSHAL pUsbOsHal;
	int UsbStatus = urb->status;
	USB_CTRLREQUEST dr;
    
	dbg("%s: ule=%p urb=%p UsbStatus=%d", __FUNCTION__, ule, urb, UsbStatus);

	if((UsbStatus == -ECONNRESET) || (UsbStatus == -ENOENT)) {
		dbg("%s: -ECONNRESET or -ENOENT", __FUNCTION__);
		ChangeUrbListEntryState(ule, ULE_ABORTED);
		return;
	}

	if(!ule)
		return;

	pUsbOsHal = ule->pUsbOsHal;

	memcpy(&dr, urb->setup_packet, sizeof(dr));

	if (OsGetSystemTime() - ule->startTime > 20)
	{
		warn("%s: control request=%d (index=0x%x, value=0x%x) took too long (%dms)",
				__FUNCTION__,
				dr.bRequest,
				le16_to_cpup(&dr.wIndex),
				le16_to_cpup(&dr.wValue),
				OsGetSystemTime() - ule->startTime);
	}

#if 0
	if (bError && (0x80000004 == urb->UrbHeader.Status) && 
		CD2_CONTROL_SCRIPT == urb->UrbControlVendorClassRequest.Request && 
		3 != urb->UrbControlVendorClassRequest.Index)	// do not restart the transaction if STALL was caused
														// by an attempt to delete script (index = 3)
#endif
#if 0
	// restart if STALLED except if caused by an attempt to
	// delete script (index = 3)
	if ((UsbStatus == -EPIPE) && (CD2_CONTROL_SCRIPT == dr.request) && (3 != le16_to_cpup(&dr.index))) {	
    	urb->next = NULL;
		if(!OsUsbMakeControlRequest (pUsbOsHal, urb, dr.request, le16_to_cpup(&dr.value), le16_to_cpup(&dr.index), pUsbOsHal->pLastControlData, pUsbOsHal->nLastControlSize)) {
			err("%s: Control pipe restart failed", __FUNCTION__);
			CNXT_BREAKPOINT ("Control pipe restart failed");
		} else {
			// previous call incremented value below
			// we need to put it back
			OsUsbDecrementIoCount(pUsbOsHal);
		}
		return;
	}
#endif

	if (UsbStatus)
	{
//		pUsbOsHal->ResetAttemptCount++;
		err("%s: error: URB status %d, req=%d, index=0x%x, value=0x%x",
				__FUNCTION__,
				UsbStatus,
				dr.bRequest,
				le16_to_cpup(&dr.wIndex),
				le16_to_cpup(&dr.wValue));
	}

	OsEventSet (pUsbOsHal->ControlRequestEvent);
	if (pUsbOsHal->hUpper)
		pUsbOsHal->cbUsbHalControlTransferDone (pUsbOsHal->hUpper, ule->userContext,
				dr.bRequest,
				le16_to_cpup(&dr.wValue),
				le16_to_cpup(&dr.wIndex));

	ChangeUrbListEntryState(ule, UsbStatus ? ULE_FAILED: ULE_COMPLETED);
}

/****************************************************************
 *                                                              *
 *      NAME:                                                   *
 *      PARAMETERS:
 *                                                              *
 *      USAGE:                                                  *
 *                                                              *
 *      DESCRIPTION:                                            *
 *                                                              *
 *      NOTES:          None.                                   *
 ****************************************************************/
static void UsbNotifyCompletionRoutine (PURB urb
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) ) && defined(FOUND_USB_COMPLETE_PT_REGS)
	, struct pt_regs *regs
#endif
	)
{
	PURB_LIST_ENTRY ule = urb->context;
	PUSBOSHAL pUsbOsHal;
	unsigned char pDataReceived[MAX_NOTIFY_DATA_PACKET];
	int UsbStatus = urb->status, UsbRecvLength = urb->actual_length;
	USB_CTRLREQUEST *dr;
    
	dbg("%s: ule=%p urb=%p UsbStatus=%d UsbRecvLength=%d", __FUNCTION__, ule, urb, UsbStatus, UsbRecvLength);

	if((UsbStatus == -ECONNRESET) || (UsbStatus == -ENOENT)) {
		dbg("%s: -ECONNRESET or -ENOENT", __FUNCTION__);
		ChangeUrbListEntryState(ule, ULE_ABORTED);
		return;
	}

	if(!ule)
		return;

	pUsbOsHal = ule->pUsbOsHal;

	if (UsbStatus)
	{
		if(UsbStatus != -ESHUTDOWN)
			err("%s: error: URB status %d, unlinking", __FUNCTION__, UsbStatus);
		// game over
		usb_unlink_urb(urb);

		ChangeUrbListEntryState(ule, ULE_FAILED);
		return;
	}
	// save a copy of notification data and received length
	ASSERT(UsbRecvLength <= MAX_NOTIFY_DATA_PACKET);
	memcpy (pDataReceived, urb->transfer_buffer, UsbRecvLength); 

	dr = (USB_CTRLREQUEST *)pDataReceived;
	dr->wValue = le16_to_cpup(&dr->wValue);
	dr->wIndex = le16_to_cpup(&dr->wIndex);
	dr->wLength = le16_to_cpup(&dr->wLength);

#if 0
	{int i;
		for (i=0; i< UsbRecvLength; i++) {
			dbg("%s: pDataReceived[%d]=0x%02x", __FUNCTION__, i, pDataReceived[i]);
		}
	}
#endif

	if (pUsbOsHal->hUpper && pUsbOsHal->cbUsbHalNotificationReceived)
	{
		pUsbOsHal->cbUsbHalNotificationReceived (pUsbOsHal->hUpper, pDataReceived, UsbRecvLength);
	}

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	UsbStatus = usb_submit_urb (urb, GFP_ATOMIC);
	if (UsbStatus)
		err ("%s - usb_submit_urb failed with result %d", __FUNCTION__, UsbStatus);
#endif
}

/****************************************************************
 *                                                              *
 *      NAME:                                                   *
 *      PARAMETERS: Standard for completion routine,			*
 *          Context points to the URB							*
 *                                                              *
 *      USAGE:                                                  *
 *                                                              *
 *      DESCRIPTION:                                            *
 *                                                              *
 *      NOTES:          None.                                   *
 ****************************************************************/
static void UsbTransmitDataCompletionRoutine(PURB urb
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) ) && defined(FOUND_USB_COMPLETE_PT_REGS)
	, struct pt_regs *regs
#endif
	)
{
	PURB_LIST_ENTRY ule = urb->context;
	PUSBOSHAL pUsbOsHal;
	int UsbStatus = urb->status;
	int length = urb->actual_length;

	dbg("%s: ule=%p urb=%p UsbStatus=%d", __FUNCTION__, ule, urb, UsbStatus);

	if((UsbStatus == -ECONNRESET) || (UsbStatus == -ENOENT)) {
		dbg("%s: -ECONNRESET or -ENOENT", __FUNCTION__);
		ChangeUrbListEntryState(ule, ULE_ABORTED);
		return;
	}

	if(!ule)
		return;

	pUsbOsHal = ule->pUsbOsHal;

	if (UsbStatus)
	{
		err("%s: error: URB status %d", __FUNCTION__, UsbStatus);
	}

	if (pUsbOsHal->hUpper && !UsbStatus)
		pUsbOsHal->cbUsbHalDataTransmitDone (pUsbOsHal->hUpper, length);

	ChangeUrbListEntryState(ule, UsbStatus ? ULE_FAILED: ULE_COMPLETED);
}

/****************************************************************
 *                                                              *
 *      NAME:                                                   *
 *      PARAMETERS: Standard for completion routine,			*
 *          Context points to the URB							*
 *                                                              *
 *      USAGE:                                                  *
 *                                                              *
 *      DESCRIPTION:                                            *
 *                                                              *
 *      NOTES:          None.                                   *
 ****************************************************************/
static void
UsbReceiveDataCompletionRoutine(PURB urb
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) ) && defined(FOUND_USB_COMPLETE_PT_REGS)
	, struct pt_regs *regs
#endif
	)
{
	PURB_LIST_ENTRY ule = urb->context;
	PUSBOSHAL pUsbOsHal;
	int UsbStatus = urb->status;
	// save a copy of data and received length
    int UsbRecvLength = urb->actual_length;
	PUINT8 pDataReceived = urb->transfer_buffer;

	dbg("%s: ule=%p urb=%p UsbStatus=%d UsbRecvLength=%d", __FUNCTION__, ule, urb, UsbStatus, UsbRecvLength);

	if((UsbStatus == -ECONNRESET) || (UsbStatus == -ENOENT)) {
		dbg("%s: -ECONNRESET or -ENOENT", __FUNCTION__);
		ChangeUrbListEntryState(ule, ULE_ABORTED);
		return;
	}

	if(!ule)
		return;

	pUsbOsHal = ule->pUsbOsHal;

	if (UsbStatus)
	{
		err("%s: error: URB status %d", __FUNCTION__, UsbStatus);
	}

	if (pUsbOsHal->hUpper && !UsbStatus) {
		if(ule->userBufferPtr) {
			memcpy(ule->userBufferPtr, pDataReceived, UsbRecvLength);
			pDataReceived = ule->userBufferPtr;
		}
		pUsbOsHal->cbUsbHalDataReceived (pUsbOsHal->hUpper, pDataReceived, UsbRecvLength);
	}

	ChangeUrbListEntryState(ule, UsbStatus ? ULE_FAILED: ULE_COMPLETED);
}

#if TARGET_HCF_FAMILY
/****************************************************************
 *                                                              *
 *      NAME:                                                   *
 *      PARAMETERS: Standard for completion routine,			*
 *          Context points to the URB							*
 *                                                              *
 *      USAGE:                                                  *
 *                                                              *
 *      DESCRIPTION:                                            *
 *                                                              *
 *      NOTES:          None.                                   *
 ****************************************************************/
static void UsbDownloadCompletionRoutine (PURB urb
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) ) && defined(FOUND_USB_COMPLETE_PT_REGS)
	, struct pt_regs *regs
#endif
	)
{
	PURB_LIST_ENTRY ule = urb->context;
	PUSBOSHAL pUsbOsHal;
	int UsbStatus = urb->status;

	dbg("%s: ule=%p urb=%p UsbStatus=%d", __FUNCTION__, ule, urb, UsbStatus);

	if((UsbStatus == -ECONNRESET) || (UsbStatus == -ENOENT)) {
		dbg("%s: -ECONNRESET or -ENOENT", __FUNCTION__);
		ChangeUrbListEntryState(ule, ULE_ABORTED);
		return;
	}

	if(!ule)
		return;

	pUsbOsHal = ule->pUsbOsHal;

	if (UsbStatus)
	{
		err("%s: error: URB status %d", __FUNCTION__, UsbStatus);
	}

	if (pUsbOsHal->hUpper && !UsbStatus)
		pUsbOsHal->cbUsbHalDownloadTransmitDone (pUsbOsHal->hUpper);

	ChangeUrbListEntryState(ule, UsbStatus ? ULE_FAILED: ULE_COMPLETED);
}
#endif

/******************************************************************************

Name:           OsUsbMakeDataReceiveRequest

Description:    Schedules device-to-host bulk request on pipe 1 (data).

Arguments:      hUsbOsHal	- bottom HAL handle
				pBuf		- points to a buffer to store received data
				nBytes		- size of data to receive

Return Value:   result of the call

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

******************************************************************************/
__shimcall__
BOOL OsUsbMakeDataReceiveRequest	(HANDLE hUsbOsHal, PVOID pBuf, UINT32 nBytes)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	PURB_LIST_ENTRY ule;

	dbg("%s: %p pBuf=%p nBytes=%ld", __FUNCTION__, hUsbOsHal, pBuf, nBytes);

	if (Working != GET_DEVICE_PNP_STATE(pUsbOsHal))
		return FALSE;

	// extract an URB from the list
	ule = GetFreeUrbFromList (&pUsbOsHal->RecvUrbList);
	if(!ule) {
		// normal?
		dbg("%s: no more free urbs", __FUNCTION__);
		return FALSE;
	}

	if(!IsKMallocMem(pBuf)) {
		ASSERT(nBytes <= ule->bufferSize);

		ule->userBufferPtr = pBuf;
		pBuf = ule->bufferPtr;
	} else
		ule->userBufferPtr = NULL;

	usb_fill_bulk_urb(ule->urb, pUsbOsHal->pUsbDevice, pUsbOsHal->DataInPipe,
			pBuf,
			nBytes,
			UsbReceiveDataCompletionRoutine,
			ule);


	return SubmitUrbListEntry(ule);
}

/******************************************************************************

Name:           OsUsbMakeDataTransmitRequest

Description:    Schedules host-to-device bulk request on pipe 1 (data).
				
Arguments:      hUsbOsHal	- bottom HAL handle
				hRequest	- handle to an OS specific data structure needed to 
							  initiate a bulk request. This handle is returned by
							  OsUsbGetTxRequest() call
				pBuf		- points to data to be transmitted
				nBytes		- size of data to be transmitted

Return Value:   result of the call

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

******************************************************************************/
__shimcall__
BOOL OsUsbMakeDataTransmitRequest	(HANDLE hUsbOsHal, HANDLE hRequest, PVOID pBuf, UINT32 nBytes)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	PURB_LIST_ENTRY ule = (PURB_LIST_ENTRY)hRequest;

	dbg("%s: ule=%p pBuf=%p nBytes=%ld", __FUNCTION__, hRequest, pBuf, nBytes);

	if (Working != GET_DEVICE_PNP_STATE(pUsbOsHal))
		return FALSE;

	if(!IsKMallocMem(pBuf)) {
		ASSERT(nBytes <= ule->bufferSize);
		memcpy(ule->bufferPtr, pBuf, nBytes);
		pBuf = ule->bufferPtr;
	}

	usb_fill_bulk_urb(ule->urb, pUsbOsHal->pUsbDevice, pUsbOsHal->DataOutPipe,
			pBuf,
			nBytes,
			UsbTransmitDataCompletionRoutine,
			ule);

	return SubmitUrbListEntry(ule);
}

#if TARGET_HCF_FAMILY
/******************************************************************************

Name:           OsUsbMakeDownloadTransmitRequest

Description:    Schedules host-to-device bulk request on pipe 3 (DSP download).
				
Arguments:      hUsbOsHal	- bottom HAL handle
				pBuf		- points to data to be transmitted
				nBytes		- size of data to be transmitted

Return Value:   result of the call

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

******************************************************************************/
__shimcall__
BOOL OsUsbMakeDownloadTransmitRequest	(HANDLE hUsbOsHal, PVOID pBuf, UINT32 nBytes)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	PURB_LIST_ENTRY ule;

	dbg("%s: %p pBuf=%p nBytes=%ld", __FUNCTION__, hUsbOsHal, pBuf, nBytes);

	if (Working != GET_DEVICE_PNP_STATE(pUsbOsHal))
		return FALSE;

	// extract an URB from the list
	ule = GetFreeUrbFromList (&pUsbOsHal->DownloadUrbList);
	if(!ule) {
		err("%s: no more free urbs", __FUNCTION__);
		return FALSE;
	}

	if(!IsKMallocMem(pBuf)) {
		ASSERT(nBytes <= ule->bufferSize);
		memcpy(ule->bufferPtr, pBuf, nBytes);
		pBuf = ule->bufferPtr;
	}

	usb_fill_bulk_urb(ule->urb, pUsbOsHal->pUsbDevice, pUsbOsHal->DownloadPipe,
			pBuf,
			nBytes,
			UsbDownloadCompletionRoutine,
			ule);

//	ule->urb->transfer_flags |= USB_QUEUE_BULK; // XXX

	return SubmitUrbListEntry(ule);
}
#endif

/******************************************************************************

Name:           OsUsbWaitControlRequest

Description:    Waits until bottom HAL is ready to accept new control request.
				Returns a handle to a data structure needed to send such request.
				Wait must be accomplished by blocking a calling thread on some sort
				of syncronization object, like semaphore, so system is not locked and
				USB completions can be called by an OS

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   hCntrlReq	- handle to an OS specific data structure needed to
							  initiate a control request.

Notes:

******************************************************************************/
__shimcall__
HANDLE OsUsbWaitControlRequest (HANDLE hUsbOsHal)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	PURB_LIST_ENTRY ule;

	dbg("%s: %p", __FUNCTION__, hUsbOsHal);

	while(!(ule = GetFreeUrbFromList (&pUsbOsHal->ControlUrbList)) &&
			(Working == GET_DEVICE_PNP_STATE(pUsbOsHal))) {
		if( OSEVENT_WAIT_TIMEOUT == OsEventWaitTime(pUsbOsHal->ControlRequestEvent, 5000)) { // give it 5s
			warn("OsWaitForControlRequestAvailable: timeout");
			CNXT_BREAKPOINT ("OsWaitForControlRequestAvailable: timeout");
		}
	}

	dbg("%s: returning %p", __FUNCTION__, ule);
	return (HANDLE)ule;
}

/******************************************************************************

Name:           OsUsbWaitForControlRequestCompletion

Description:    Waits until control request is completed

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   None.

Notes:

******************************************************************************/
__shimcall__
void OsUsbWaitForControlRequestCompletion(HANDLE hUsbOsHal)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;

	dbg("%s: %p", __FUNCTION__, hUsbOsHal);

	if( OSEVENT_WAIT_TIMEOUT == OsEventWaitTime(pUsbOsHal->ControlRequestEvent, 5000)) { // give it 5s
		err("OsUsbWaitForControlRequestCompletion: !! Control Request Timeout");
	}
}


/******************************************************************************

Name:           OsUsbGetControlRequest

Description:    Reserves a data structure needed to send a control request.

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   handle to an OS specific data structure needed to
				initiate a control request or NULL if none available

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

******************************************************************************/
__shimcall__
HANDLE OsUsbGetControlRequest (HANDLE hUsbOsHal)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	HANDLE h;

	h = (HANDLE)GetFreeUrbFromList (&pUsbOsHal->ControlUrbList);
	dbg("%s: %p returning %p", __FUNCTION__, hUsbOsHal, h);
	return h;
}

/******************************************************************************

Name:           OsUsbGetTxRequest

Description:    Reserves a data structure needed to send a host-to-device bulk request
				on pipe 1.

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   handle to an OS specific data structure needed to
				initiate a bulk request or NULL if none available

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

******************************************************************************/
__shimcall__
HANDLE OsUsbGetTxRequest (HANDLE hUsbOsHal)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	HANDLE h;

	h = (HANDLE)GetFreeUrbFromList (&pUsbOsHal->SendUrbList);
	dbg("%s: %p returning %p", __FUNCTION__, hUsbOsHal, h);
	return h;
}

__shimcall__
VOID OsUsbReturnUnusedRequest (HANDLE hRequest)
{
	PURB_LIST_ENTRY ule = (PURB_LIST_ENTRY)hRequest;

	dbg("%s: %p", __FUNCTION__, ule);

	ChangeUrbListEntryState(ule, ULE_FREE);
}

/******************************************************************************

Name:           OsUsbMakeControlRequest

Description:    Formats a host-to-device control request and sends it via the default control 
				pipe (pipe 0). Does not wait for control request to be completed by USB

Arguments:      hUsbOsHal	- bottom HAL handle
				hCntrlReq	- handle to an OS specific data structure needed to
							  initiate a control request. Must be returned via previous
							  call to OsUsbWaitControlRequest() or OsUsbGetControlRequest()
				Request		- bRequest field of a control request header
				Value		- wValue field of a control request header
				Index		- wIndex field of a control request header
				pBuf		- pointer to data to be transmitted
				nBytes		- size of data pointed by pBuf

Return Value:   TRUE if request is being accepted by USB stack, FALSE if not

Notes:			Top HAL can call this function from a context where memory allocations
				are not permitted, such as DPC level. Since it is expected that bottom HAL
				has finite number of control structures (URBs/PBs) needed to initiate a USB
				transaction, top HAL first reserves such structure via OsUsbWaitControlRequest()
				or OsUsbGetControlRequest().

******************************************************************************/
__shimcall__
BOOL OsUsbMakeControlRequest (HANDLE hUsbOsHal, HANDLE hCntrlReq, IN PVOID userContext, UINT8 Request, UINT16 Value, UINT16 Index,
							  IN PVOID pBuf, IN UINT32 nBytes)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	PURB_LIST_ENTRY ule;
	USB_CTRLREQUEST *dr;
	UINT16 snBytes = (UINT16)nBytes;

	dbg("%s: ule=%p req=0x%x val=%d idx=%d pbuf=%p len=%ld", __FUNCTION__, hCntrlReq, Request, Value, Index, pBuf, nBytes);

	if (Working != GET_DEVICE_PNP_STATE(pUsbOsHal))
		return FALSE;

#if 0
	if(nBytes > pUsbOsHal->pUsbDevice->descriptor.bMaxPacketSize0) {
//		warn("%s: nBytes (%ld) larger than MaxPacketSize (%d)", __FUNCTION__, nBytes, pUsbOsHal->pUsbDevice->descriptor.bMaxPacketSize0);
#if 0
		ASSERT(nBytes <= pUsbOsHal->pUsbDevice->descriptor.bMaxPacketSize0);
		nBytes = pUsbOsHal->pUsbDevice->descriptor.bMaxPacketSize0;
#endif
	}
#endif

	ule = (PURB_LIST_ENTRY)hCntrlReq;
	if (!ule) {
		err("%s: not enough control URBs?", __FUNCTION__);
		ASSERT(ule != NULL);
		return FALSE;
	}

	if(0 == ule->ul->count[ULE_FREE])
		OsEventClear (pUsbOsHal->ControlRequestEvent);

	ASSERT(!(1 == Index && nBytes == 0));

	if(!IsKMallocMem(pBuf)) {
		ASSERT(nBytes <= ule->bufferSize);
		memcpy(ule->bufferPtr, pBuf, nBytes);
		pBuf = ule->bufferPtr;
	}

	ule->userContext = userContext;

	dr = &ule->devRequest;
	dr->bRequestType = USB_TYPE_VENDOR | USB_RECIP_INTERFACE | USB_DIR_OUT;
	dr->bRequest = Request;

	dr->wValue = cpu_to_le16p(&Value);
	dr->wIndex = cpu_to_le16p(&Index);
	dr->wLength = cpu_to_le16p(&snBytes);

	pUsbOsHal->pLastControlData = pBuf;
	pUsbOsHal->nLastControlSize = nBytes;

	usb_fill_control_urb(ule->urb, pUsbOsHal->pUsbDevice,
			usb_sndctrlpipe(pUsbOsHal->pUsbDevice, 0),
			(unsigned char*)dr,
			pBuf,
			nBytes,
			UsbControlCompletionRoutine,
			ule);

	return SubmitUrbListEntry(ule);
}

/******************************************************************************

Name:           OsUsbMakeControlRequestSync

Description:    Formats a host-to-device control request and sends it via the default control 
				pipe (pipe 0). Wait for control request to be completed by USB

Arguments:      hUsbOsHal	- bottom HAL handle
				Direction	- control request direction: 0 - out, otherwise - in
				Request		- bRequest field of a control request header
				Value		- wValue field of a control request header
				Index		- wIndex field of a control request header
				pBuf		- pointer to data to be transmitted
				pnBytes		- pointer to size of data pointed by pBuf

Return Value:   TRUE if request is being accepted by USB stack, FALSE if not

Notes:			Top HAL can call this function from a context where memory allocations
				are not permitted, such as DPC level. Since it is expected that bottom HAL
				has finite number of control structures (URBs/PBs) needed to initiate a USB
				transaction, top HAL first reserves such structure via OsUsbWaitControlRequest()
				or OsUsbGetControlRequest().

******************************************************************************/
__shimcall__
BOOL OsUsbMakeControlRequestSync		(HANDLE hUsbOsHal, IN UINT8 Direction, UINT8 Request, UINT16 Value, UINT16 Index,
										IN PVOID pBuf, IO PUINT32 pnBytes)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	int ret;
	UINT8 ControlRequestBuffer[MAX_CONTROL_DATA_PACKET];

	dbg("%s: req=0x%x dir=%d val=%d idx=%d pbuf=%p len=%ld", __FUNCTION__, Request, Direction, Value, Index, pBuf, *pnBytes);

	if(*pnBytes > pUsbOsHal->pUsbDevice->descriptor.bMaxPacketSize0) {
		warn("%s: nBytes (%d) larger than MaxPacketSize (%d)", __FUNCTION__, *pnBytes, pUsbOsHal->pUsbDevice->descriptor.bMaxPacketSize0);
		ASSERT(*pnBytes <= pUsbOsHal->pUsbDevice->descriptor.bMaxPacketSize0);
		*pnBytes = pUsbOsHal->pUsbDevice->descriptor.bMaxPacketSize0;
	}

	if(!IsKMallocMem(pBuf)) {
		ASSERT(*pnBytes <= sizeof(ControlRequestBuffer));
		memcpy(ControlRequestBuffer, pBuf, *pnBytes);
		pBuf = ControlRequestBuffer;
	}

	ret = usb_control_msg(pUsbOsHal->pUsbDevice,
			Direction ? usb_rcvctrlpipe(pUsbOsHal->pUsbDevice, 0) : usb_sndctrlpipe(pUsbOsHal->pUsbDevice, 0),
			Request,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | (Direction ? USB_DIR_IN : USB_DIR_OUT),
			Value,
			Index,
			pBuf,
			*pnBytes,
			USB_MS_TIMEOUT(10000));

	if(ret >= 0) {
		*pnBytes = ret;
		return TRUE;
	} else {
		err("%s: ERROR!!! req=0x%x err=%d", __FUNCTION__, Request, ret);
		return FALSE;
	}
}
//

#if 0
__shimcall__
BOOL OsUsbMakeGetStringDescriptorRequestSync(
	HANDLE hUsbOsHal,
	IN UINT8 uStrIndex,	// String Descriptor Index
	IN PVOID pBuf,
	IN OUT PUINT32 pnBytes)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	int ret;

	ret = usb_string(pUsbOsHal->pUsbDevice, uStrIndex, pBuf, *pnBytes);
	if(ret < 0) {
		err"ERROR!!! OsUsbMakeGetStringDescriptorRequestSync: %d", ret);
		return FALSE;
	}

	*pnBytes = ret;
	return TRUE;
}
#endif
//
			
/******************************************************************************

Name:           OsUsbMakeNotifyRequest

Description:    Schedules device-to-host interrupt request on pipe 2 (notification).

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   result of the call

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

******************************************************************************/
__shimcall__
BOOL OsUsbMakeNotifyRequest	(HANDLE hUsbOsHal)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
	PURB_LIST_ENTRY ule;

	dbg("%s: %p", __FUNCTION__, hUsbOsHal);

	if (Working != GET_DEVICE_PNP_STATE(pUsbOsHal))
		return FALSE;

	// extract an URB from the list
	ule = GetFreeUrbFromList (&pUsbOsHal->NotifyUrbList);
	if(!ule) {
		// normal?
		dbg("%s: no more free urbs; request already active?", __FUNCTION__);
		return FALSE;
	}

	usb_fill_int_urb(ule->urb, pUsbOsHal->pUsbDevice, pUsbOsHal->NotifyPipe,
			ule->bufferPtr,
			ule->bufferSize,
			UsbNotifyCompletionRoutine,
			ule,
			pUsbOsHal->NotifyInterval);

#ifdef URB_ASYNC_UNLINK
	ule->urb->transfer_flags |= URB_ASYNC_UNLINK;
#endif

	return SubmitUrbListEntry(ule);
}

/******************************************************************************

Name:           OsUsbResetPipes

Description:    Aborts any transactions-in-progress across USB

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   none

Notes:

******************************************************************************/
__shimcall__
void OsUsbResetPipes (HANDLE hUsbOsHal)
{
#if TARGET_HCF_FAMILY
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hUsbOsHal;
#endif

	dbg("%s: %p", __FUNCTION__, hUsbOsHal);
//	pUsbOsHal->bPipeResetInProgress = TRUE;

	// XXX if (NULL == pUsbOsHal->pDownloadUrb || NULL == pUsbOsHal->pDownloadUrb->next)
#if TARGET_HCF_FAMILY
	if(0 == pUsbOsHal->DownloadUrbList.count[ULE_FREE]) {
		err("Download pipe stuck, resetting");
        usb_clear_halt(pUsbOsHal->pUsbDevice, pUsbOsHal->DownloadPipe);
	}
#endif

//	pUsbOsHal->bPipeResetInProgress = FALSE;
	// To Do; if we have some stuck pipes here, reset them
}

__shimcall__
void OsUsbFreeUrbs (PUSBOSHAL pUsbOsHal)
{
	dbg("%s: %p", __FUNCTION__, pUsbOsHal);

	dbg("%s: NotifyUrbList", __FUNCTION__);
	FreeUrbList(&pUsbOsHal->NotifyUrbList);
	dbg("%s: RecvUrbList", __FUNCTION__);
	FreeUrbList(&pUsbOsHal->RecvUrbList);
	dbg("%s: SendUrbList", __FUNCTION__);
	FreeUrbList(&pUsbOsHal->SendUrbList);
	dbg("%s: ControlUrbList", __FUNCTION__);
	FreeUrbList(&pUsbOsHal->ControlUrbList);
#if TARGET_HCF_FAMILY
	dbg("%s: DownloadUrbList", __FUNCTION__);
	FreeUrbList(&pUsbOsHal->DownloadUrbList);
#endif
#if defined(USE_DCP) && TARGET_HCF_FAMILY
	dbg("%s: DcpUrbList", __FUNCTION__);
	FreeUrbList(&pUsbOsHal->DcpUrbList);
#endif
}

__shimcall__
BOOL OsUsbAllocateUrbs (PUSBOSHAL pUsbOsHal)
{
	dbg("%s: %p", __FUNCTION__, pUsbOsHal);

	// for now allocate just one IRP/URB per pipe

	dbg("%s: RecvUrbList", __FUNCTION__);
	if(!InitUrbList(&pUsbOsHal->RecvUrbList, pUsbOsHal,
				MAX_RECEIVE_URB_BUFSIZE, NUM_RECEIVE_URBS)) {
		goto error;
	}

	dbg("%s: SendUrbList", __FUNCTION__);
	if(!InitUrbList(&pUsbOsHal->SendUrbList, pUsbOsHal,
				MAX_SEND_URB_BUFSIZE, NUM_SEND_URBS)) {
		goto error;
	}

	dbg("%s: NotifyUrbList", __FUNCTION__);
	if(!InitUrbList(&pUsbOsHal->NotifyUrbList, pUsbOsHal,
				MAX_NOTIFY_URB_BUFSIZE, NUM_NOTIFY_URBS)) {
		goto error;
	}

	dbg("%s: ControlUrbList", __FUNCTION__);
	if(!InitUrbList(&pUsbOsHal->ControlUrbList, pUsbOsHal,
				MAX_CONTROL_URB_BUFSIZE, NUM_CONTROL_URBS)) {
		goto error;
	}

#if TARGET_HCF_FAMILY
	dbg("%s: DownloadUrbList", __FUNCTION__);
	if(!InitUrbList(&pUsbOsHal->DownloadUrbList, pUsbOsHal,
				MAX_DOWNLOAD_URB_BUFSIZE, NUM_DOWNLOAD_URBS)) {
		goto error;
	}
#endif

#if defined(USE_DCP) && TARGET_HCF_FAMILY
	dbg("%s: DcpUrbList", __FUNCTION__);
	if(!InitUrbList(&pUsbOsHal->DcpUrbList, pUsbOsHal,
				MAX_DCP_URB_BUFSIZE, NUM_DCP_URBS)) {
		goto error;
	}
#endif

	return TRUE;

error:
	OsUsbFreeUrbs(pUsbOsHal);
	return FALSE;
}

#if 0
static void ReturnUrbToList(volatile PURB * ppUrb, PURB urb)
{
    PURB oldhead;

	dbg("%s: %p to %p", __FUNCTION__, urb, ppUrb);
    do 
	{
        oldhead = *ppUrb;
        urb->next = oldhead;
    } while(!OsAtomicCompareAndSwap(oldhead, urb, ppUrb));
}

static PURB GetUrbFromList(volatile PURB * ppUrb)
{
    PURB urb, urb1;

    do {
        urb = * ppUrb;
        if (NULL == urb)
            {
            return NULL;
            }
        urb1 = urb->next;
        } 
    while(!OsAtomicCompareAndSwap(urb, urb1, ppUrb));
    urb->next = NULL;

	dbg("%s: got %p from %p", __FUNCTION__, urb, ppUrb);
    return urb;
}
#endif

#if defined(USE_DCP) && TARGET_HCF_FAMILY

#error not yet supported under Linux

#define DCP_BLOCK_SIZE (1024*1024)

typedef struct tagDCP
{
    HANDLE	hHalOs;

    BOOL	fOn;
	int		nBufNo;

	PIO_WORKITEM	pWorkItem;

    UINT16  Buffer[2][DCP_BLOCK_SIZE];
    UINT32  dwCount;
	UINT32	hFile;

} DCP, *PDCP;


static DCP gDcp;

static void UsbDcpReceived ( int UsbRecvLength);
static NTSTATUS UsbDcpCompletionRoutine(PDEVICE_OBJECT DeviceObject, PIRP Irp, PVOID Context);


__shimcall__
HANDLE DcpCreate(HANDLE hHalOs, HANDLE hDevNode)
{
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)hHalOs;
    gDcp.fOn        = FALSE;
    gDcp.dwCount    = 0;
    gDcp.hHalOs     = hHalOs;
	gDcp.nBufNo		= 0;

	gDcp.pWorkItem = IoAllocateWorkItem (pUsbOsHal->pDeviceExtension->DeviceObject);
    return &gDcp;
}

__shimcall__
void DcpDestroy  (HANDLE hDcp)
{
    PDCP pDcp = (PDCP)hDcp;
	IoFreeWorkItem (pDcp->pWorkItem);
}

__shimcall__
void DcpWorkItemRtn (PDEVICE_OBJECT pDevObject, PVOID Context)
{
    PDCP pDcp = (PDCP)Context;
	if (pDcp->fOn && pDcp->hFile)
		R0FileWrite(pDcp->hFile, (UINT8*)pDcp->Buffer[1-pDcp->nBufNo], sizeof(pDcp->Buffer[0]));
}

__shimcall__
void DcpSetVolume(HANDLE hDcp, int nVolume)
{
    PDCP pDcp = (PDCP)hDcp;

    if( nVolume && !pDcp->fOn)
    {
		static unsigned fileNo;
        static char fileName[255];
		OsSprintf (fileName, "\\??\\C:\\temp\\dcp%d.snd", fileNo++);
        pDcp->hFile = R0FileOpen(fileName);

        pDcp->fOn = TRUE;
        pDcp->dwCount = 0;
        OsUsbMakeDcpRequest(pDcp->hHalOs, &pDcp->Buffer[pDcp->dwCount], 2048);
    }
    else if( !nVolume && pDcp->fOn )
    {
		pDcp->fOn = FALSE;
		if(pDcp->hFile)
		{
		   R0FileWrite(pDcp->hFile, (UINT8*)pDcp->Buffer[pDcp->nBufNo], 2*pDcp->dwCount);
		   R0FileClose(pDcp->hFile);
		   pDcp->hFile = 0;
		}
		// Flush data into file
		RTRACE1(T__ERROR, 0,"************* Recorded %d DCP samples", pDcp->dwCount );
    }
}

static void UsbDcpReceived ( int UsbRecvLength)
{
    PDCP pDcp = &gDcp;

    if( pDcp->fOn )
    {
        pDcp->dwCount += UsbRecvLength/2;
        
        if (pDcp->dwCount == DCP_BLOCK_SIZE)
		{	
			pDcp->nBufNo = 1-pDcp->nBufNo;
			IoQueueWorkItem (pDcp->pWorkItem, DcpWorkItemRtn, DelayedWorkQueue, pDcp);
			pDcp->dwCount = 0;
            OsUsbMakeDcpRequest(pDcp->hHalOs, pDcp->Buffer[pDcp->nBufNo], 2048);
		}
		else
            OsUsbMakeDcpRequest(pDcp->hHalOs, &pDcp->Buffer[pDcp->nBufNo][pDcp->dwCount], 2048);
			
    }
}

static NTSTATUS UsbDcpCompletionRoutine(
    IN PDEVICE_OBJECT DeviceObject,
       IN PIRP Irp,
       IN PVOID Context)
{
    PURB urb = (PURB) Context;
	PUSBOSHAL pUsbOsHal = (PUSBOSHAL)((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->HALExtension;

	// save a copy of data and received length
    int UsbRecvLength = urb->actual_length;

	if (UsbStatus)
	{
		RTRACE2(T__HALOS|T__ERROR, 0, "UsbDcpCompletionRoutine error: 0x%x, URB status 0x%x",
				Irp->IoStatus.Status, urb->UrbHeader.Status);
	}

    ReturnUrbToList (&pUsbOsHal->pDcpUrb, urb);
    ReturnIrpToList (&pUsbOsHal->pDcpIrp, Irp);

	if (!UsbStatus)
		UsbDcpReceived (UsbRecvLength);

	RTRACE0(T__HALOS, 0,"Exit UsbDcpCompletionRoutine");
}


/******************************************************************************

Name:           OsUsbMakeDcpRequest

Description:    Schedules device-to-host bulk request on pipe 3 (DCP).

Arguments:      hUsbOsHal	- bottom HAL handle
				pBuf		- buffer to read samples to
				nBytes		- size of such buffer

Return Value:   none

Notes:			After USB completion routine is called, bottom HAL reschedules USB
				bulk request on pipe 3 without top HAL intervention until
				OsUsbCancelDcpRequests() is called.

******************************************************************************/
__shimcall__
BOOL OsUsbMakeDcpRequest(HANDLE hOsHal, PVOID pBuf, UINT32 nBytes)
{
	PIRP irp;
	PURB urb;
    PUSBOSHAL pUsbOsHal = hOsHal;

	// extract an URB from the list
    urb = GetUrbFromList (& pUsbOsHal->pDcpUrb);
	if (NULL == urb)
		return FALSE;

	// extract an IRP from the list
	irp = GetIrpFromList(&pUsbOsHal->pDcpIrp);
	if (NULL == irp)
	{
		// no IRP available or IRP is cancelled and deallocated
		RTRACE0(T__ERROR | T__HALOS, 0,"NULL == DcpIrp");
		ReturnUrbToList( & pUsbOsHal->pDcpUrb, urb);
		return FALSE;
	}

	if (STATUS_PENDING != CallUsbd (
		pUsbOsHal, irp, urb, pUsbOsHal->DcpInPipe,
		pBuf, nBytes, UsbDcpCompletionRoutine, CALL_USBD_FLAGS_INPUT))
		return FALSE;

    return TRUE;
}


#endif // USE_DCP && TARGET_HCF_FAMILY

#endif // defined(CONFIG_USB) || defined(CONFIG_USB_MODULE)
