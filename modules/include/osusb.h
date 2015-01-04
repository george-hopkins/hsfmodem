
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


#ifndef __OSUSB_H
#define __OSUSB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "usbhalos.h"
#include <linux/usb.h>


#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,4,18) )
#define usb_fill_int_urb	FILL_INT_URB
#define usb_fill_bulk_urb	FILL_BULK_URB
#define usb_fill_control_urb	FILL_CONTROL_URB
#endif

#define	MAX_NOTIFY_DATA_PACKET	64
#define	MAX_BULK_DATA_PACKET	64
#define	MAX_CONTROL_DATA_PACKET	64
#if TARGET_HCF_FAMILY
#define DATA_PIPE_NUM			4
#else
#define DATA_PIPE_NUM			2
#endif
#define INT_PIPE_NUM			1

#define MAX_RECEIVE_URB_BUFSIZE	(MAX_BULK_DATA_PACKET*4)
#define MAX_SEND_URB_BUFSIZE	(MAX_BULK_DATA_PACKET*4)
#define MAX_NOTIFY_URB_BUFSIZE	MAX_NOTIFY_PACKET
#define MAX_CONTROL_URB_BUFSIZE	(MAX_BULK_DATA_PACKET*4)
#define MAX_DOWNLOAD_URB_BUFSIZE (8*1024)
#define MAX_DCP_URB_BUFSIZE		MAX_BULK_DATA_PACKET

#if TARGET_HCF_FAMILY
#define NUM_RECEIVE_URBS		2
#define NUM_SEND_URBS			2
#else
#define NUM_RECEIVE_URBS		32
#define NUM_SEND_URBS			32
#endif
#define NUM_NOTIFY_URBS			1
#define NUM_CONTROL_URBS		2
#define NUM_DOWNLOAD_URBS		2

#if !defined(URB_ASYNC_UNLINK) && defined(USB_ASYNC_UNLINK)
#define URB_ASYNC_UNLINK USB_ASYNC_UNLINK
#endif

typedef struct urb *PURB;

typedef enum {
	ULE_FREE,
	ULE_ALLOCATED,
	ULE_QUEUED,
	ULE_SUBMITTED,
	ULE_COMPLETED,
	ULE_ABORTED,
	ULE_FAILED,
	ULE_LAST
} URB_LIST_ENTRY_STATE;


typedef struct {
	__u8 bRequestType;
	__u8 bRequest;
	__u16 wValue;
	__u16 wIndex;
	__u16 wLength;
} __attribute__ ((packed)) USB_CTRLREQUEST;

struct tagUSBOSHAL;
typedef struct {
	struct list_head entry;
	struct list_head queued_entry;
	struct tagURB_LIST *ul;
	struct tagUSBOSHAL *pUsbOsHal;
	URB_LIST_ENTRY_STATE state;
	PURB volatile urb;
	USB_CTRLREQUEST	devRequest;
	PUINT8 bufferPtr;
	size_t bufferSize;
	PUINT8 userBufferPtr;
	size_t userBufferSize;
	PVOID userContext;
	UINT32	startTime;
} URB_LIST_ENTRY, *PURB_LIST_ENTRY;

typedef struct tagURB_LIST {
	struct list_head urb_list;
	struct list_head queued_list;
	volatile int count[ULE_LAST];
} URB_LIST, *PURB_LIST;

//
// A structure representing the instance information associated with
// this particular device.
//
typedef struct tagUSBOSHAL {
	BOOL			bActive;

	BOOL (*cbUsbHalNotificationReceived)	(PVOID hUpper, PUINT8 pDataReceived, unsigned RecvLength) __shimcall__;
	void (*cbUsbHalDataReceived)			(PVOID hUpper, PUINT8 pDataReceived, unsigned RecvLength) __shimcall__;
	void (*cbUsbHalDataTransmitDone)		(PVOID hUpper, unsigned TxLength) __shimcall__;
	void (*cbUsbHalControlTransferDone)		(PVOID hUpper, PVOID userContext, UINT8 Request, UINT16 value, UINT16 index) __shimcall__;
#if TARGET_HCF_FAMILY
	void (*cbUsbHalDownloadTransmitDone)	(PVOID hUpper) __shimcall__;
#endif


	HOSEVENT		ControlRequestEvent;

	struct usb_device	*pUsbDevice;
	struct usb_interface	*pUsbDataInterface;
	struct usb_interface	*pUsbCommInterface;

	// URBs for 
	// data transmit pipe
	URB_LIST SendUrbList;
	// data receive pipe
	URB_LIST RecvUrbList;
	// notify pipe
	URB_LIST NotifyUrbList;
	// control pipe
	URB_LIST ControlUrbList;
#if TARGET_HCF_FAMILY
	// datapump download
	URB_LIST DownloadUrbList;
#endif
#ifdef USE_DCP
	// datapump download
	URB_LIST DcpUrbList;
#endif

	int				DataInPipe,
					DataOutPipe,
					DcpInPipe,
#if TARGET_HCF_FAMILY
					DownloadPipe,
#endif
					NotifyPipe;

	__u8			NotifyInterval;

	HANDLE hUpper; // handle of OS independent portion of the USB HAL

	BOOL			UpdateEEPROM;

//	BOOL				bPipeResetInProgress;
//    int		        ResetAttemptCount;

	PVOID				pLastControlData;
	size_t				nLastControlSize;

	size_t				MaxControlSize;
	size_t				MaxNotifySize;

} USBOSHAL, *PUSBOSHAL;

//It downloads FW including script engine.
//The FW starts after been loaded.
//Do not need to wait after download.
__shimcall__
int OsUsbFWDownload (PUSBOSHAL pUsbOsHal);

__shimcall__
BOOL OsUsbAllocateUrbs (PUSBOSHAL pUsbOsHal);
__shimcall__
void OsUsbFreeUrbs (PUSBOSHAL pUsbOsHal);

#ifdef __cplusplus
}
#endif

#endif /* __OSUSB_H */
