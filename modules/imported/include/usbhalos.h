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

#ifndef __USBHALOS_H
#define __USBHALOS_H

#ifdef __cplusplus
extern "C"
{
#endif

// function prototypes which make interface between USB HAL and USB OS HAL

__shimcall__
HANDLE	OsUsbGetInstance		(UINT32 Unit);

/********************************************************************************
 two functions below indicate to lower HAL that the modem instance is being allocated or
 deallocated. They DO NOT cause alocation/deallocation of the lower HAL which
 typically happens when device is plugged in or out
********************************************************************************/

/********************************************************************************

Name:           OsUsbCreateInstance

Description:    Indicate to the bottom HAL that modem instance is being created

Arguments:      pDeviceID	- OS specific unique ID of a device opened, such as PnP ID
				hUpper		- opaque handle to the top HAL for callbacks

Return Value:   New bottom HAL handle, NULL if failure

Notes:          

********************************************************************************/
#if OS_TYPE == OS_LINUX
__shimcall__
HANDLE	OsUsbCreateInstance		(PVOID pDeviceID, HANDLE hUpper,
		__shimcall__ BOOL (*cbUsbHalNotificationReceived) (PVOID hUpper, PUINT8 pDataReceived, unsigned RecvLength),
		__shimcall__ void (*cbUsbHalDataReceived)			(PVOID hUpper, PUINT8 pDataReceived, unsigned RecvLength),
		__shimcall__ void (*cbUsbHalDataTransmitDone)		(PVOID hUpper, unsigned TxLength),
		__shimcall__ void (*cbUsbHalControlTransferDone)	(PVOID hUpper, PVOID userContext, UINT8 Request, UINT16 value, UINT16 index),
#if TARGET_HCF_FAMILY
		__shimcall__ void (*cbUsbHalDownloadTransmitDone) (PVOID hUpper),
#endif
		__shimcall__ void (*cbUsbEEPROM_Restore) (HANDLE hOsUsb));
#else
HANDLE	OsUsbCreateInstance		(PVOID pDeviceID, HANDLE hUpper);
#endif

/********************************************************************************

Name:           OsUsbDestroyInstance

Description:    Indicate to the bottom HAL that modem instance is being destroyed

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   None

Notes:          Top HAL handle is invalid after this call. Bottom HAL must not call
				any callback routines

********************************************************************************/
__shimcall__
void	OsUsbDestroyInstance	(HANDLE hUsbOsHal);

/********************************************************************************

Name:           OsUsbIsStarted

Description:    Query if device is configured and is ready to accept USB transactions

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   TRUE if device is ready, FALSE if not

Notes:

********************************************************************************/
__shimcall__
BOOL OsUsbIsStarted						(HANDLE hUsbOsHal);

/********************************************************************************

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

********************************************************************************/
__shimcall__
BOOL OsUsbMakeControlRequest			(HANDLE hUsbOsHal, HANDLE hCntrlReq, PVOID userContext,
										UINT8 Request, UINT16 Value, UINT16 Index, 
										IN PVOID pBuf, IN UINT32 nBytes);

/********************************************************************************

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

********************************************************************************/
__shimcall__
BOOL OsUsbMakeControlRequestSync		(HANDLE hUsbOsHal, IN UINT8 Direction, UINT8 Request, UINT16 Value, UINT16 Index, 
										IN PVOID pBuf, IO PUINT32 pnBytes);


__shimcall__
BOOL OsUsbMakeGetStringDescriptorRequestSync	(HANDLE hUsbOsHal, IN UINT8 StrIndex, IN PVOID pBuf, IO PUINT32 pnBytes);



/********************************************************************************

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

********************************************************************************/
__shimcall__
HANDLE OsUsbWaitControlRequest			(HANDLE hUsbOsHal);


//#$YS$
/********************************************************************************

Name:           OsUsbWaitForControlRequestCompletion

Description:    Waits until control request is completed

Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   None.

Notes:

********************************************************************************/
__shimcall__
void OsUsbWaitForControlRequestCompletion(HANDLE hUsbOsHal);


/********************************************************************************

Name:           OsUsbGetControlRequest

Description:    Reserves a data structure needed to send a control request.
				
Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   handle to an OS specific data structure needed to 
				initiate a control request or NULL if none available

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

********************************************************************************/
__shimcall__
HANDLE OsUsbGetControlRequest			(HANDLE hUsbOsHal);

/********************************************************************************

Name:           OsUsbGetTxRequest

Description:    Reserves a data structure needed to send a host-to-device bulk request
				on pipe 1.
				
Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   handle to an OS specific data structure needed to 
				initiate a bulk request or NULL if none available

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

********************************************************************************/
__shimcall__
HANDLE OsUsbGetTxRequest				(HANDLE hUsbOsHal);

__shimcall__
VOID OsUsbReturnUnusedRequest (HANDLE hRequest);

/********************************************************************************

Name:           OsUsbMakeNotifyRequest

Description:    Schedules device-to-host interrupt request on pipe 2 (notification).
				
Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   result of the call

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

********************************************************************************/
__shimcall__
BOOL OsUsbMakeNotifyRequest				(HANDLE hUsbOsHal);

/********************************************************************************

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

********************************************************************************/
__shimcall__
BOOL OsUsbMakeDataTransmitRequest		(HANDLE hUsbOsHal, HANDLE hRequest, PVOID pBuf, UINT32 nBytes);

/********************************************************************************

Name:           OsUsbMakeDataReceiveRequest

Description:    Schedules device-to-host bulk request on pipe 1 (data).
				
Arguments:      hUsbOsHal	- bottom HAL handle
				pBuf		- points to a buffer to store received data
				nBytes		- size of data to receive

Return Value:   result of the call

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

********************************************************************************/
__shimcall__
BOOL OsUsbMakeDataReceiveRequest		(HANDLE hUsbOsHal, PVOID pBuf, UINT32 nBytes);

#if TARGET_HCF_FAMILY
/********************************************************************************

Name:           OsUsbMakeDownloadTransmitRequest

Description:    Schedules host-to-device bulk request on pipe 3 (DSP download).
				
Arguments:      hUsbOsHal	- bottom HAL handle
				pBuf		- points to data to be transmitted
				nBytes		- size of data to be transmitted

Return Value:   result of the call

Notes:			This function can be called on high CPU priority level and must return
				immediately and must not wait on any synchronization object.

********************************************************************************/
__shimcall__
BOOL OsUsbMakeDownloadTransmitRequest	(HANDLE pUsbOsHal, PVOID pBuf, UINT32 nBytes);
#endif

//#$YS$
/********************************************************************************

Name:           OsUsbResetPort

Description:    Reset USB port.
				
Arguments:      hUsbOsHal	- bottom HAL handle.

Return Value:   None

********************************************************************************/
__shimcall__
void OsUsbResetPort(HANDLE hUsbOsHal);


#ifdef USE_DCP
/********************************************************************************

Name:           OsUsbMakeDcpRequest

Description:    Schedules device-to-host bulk request on pipe 3 (DCP).
				
Arguments:      hUsbOsHal	- bottom HAL handle
				pBuf		- buffer to read samples to
				nBytes		- size of such buffer

Return Value:   none

Notes:			After USB completion routine is called, bottom HAL reschedules USB
				bulk request on pipe 3 without top HAL intervention until 
				OsUsbCancelDcpRequests() is called.

********************************************************************************/
__shimcall__
BOOL OsUsbMakeDcpRequest				(HANDLE hUsbOsHal, PVOID pBuf, UINT32 nBytes);
__shimcall__
void OsUsbStartDcpRequests				(HANDLE hUsb, HANDLE hDcp);

/********************************************************************************

Name:           OsUsbCancelDcpRequests

Description:    Stops scheduling bulk read requests on DCP pipe.
				
Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   none

Notes:			

********************************************************************************/
__shimcall__
void OsUsbCancelDcpRequests				(HANDLE hUsbOsHal);
#endif

/********************************************************************************

Name:           OsUsbResetPipes

Description:    Aborts any transactions-in-progress across USB
				
Arguments:      hUsbOsHal	- bottom HAL handle

Return Value:   none

Notes:			

********************************************************************************/
__shimcall__
void OsUsbResetPipes					(HANDLE hUsbOsHal);

/********************************************************************************
 The callback functions below are called from USB completion routines. 
 We need to be careful not to do any extensive processing in these functions
 *******************************************************************************/
/********************************************************************************

Name:           UsbHalNotificationReceived

Description:    Informs top HAL that host has completed read transaction on 
				notification pipe
				
Arguments:      hUpper			- top HAL handle
				pDataReceived	- pointer to received data
				RecvLength		- size of received data

Return Value:   needed for top HAL internal processing, should be ignored

Notes:			

********************************************************************************/
__shimcall__
BOOL UsbHalNotificationReceived (PVOID hUpper, PUINT8 pDataReceived, unsigned RecvLength);

/********************************************************************************

Name:           UsbHalDataReceived

Description:    Informs top HAL that host has completed read transaction on 
				data pipe
				
Arguments:      hUpper			- top HAL handle
				pDataReceived	- pointer to received data
				RecvLength		- size of received data

Return Value:   none

Notes:			

********************************************************************************/
__shimcall__
void UsbHalDataReceived			(PVOID hUpper, PUINT8 pDataReceived, unsigned RecvLength);

/********************************************************************************

Name:           UsbHalDataTransmitDone

Description:    Informs top HAL that host has completed write transaction on 
				data pipe
				
Arguments:      hUpper			- top HAL handle

Return Value:   none

Notes:			

********************************************************************************/
__shimcall__
void UsbHalDataTransmitDone		(PVOID hUpper, unsigned TxLength);

/********************************************************************************

Name:           UsbHalDataTransmitDone

Description:    Informs top HAL that host has completed write transaction on 
				control pipe
				
Arguments:      hUpper		- top HAL handle
				Request		- bRequest field of completed transaction
				value		- wValue field of completed transaction
				index		- wIndex field of completed transaction

Return Value:   none

Notes:			

********************************************************************************/
__shimcall__
void UsbHalControlTransferDone	(PVOID hUpper, PVOID userContext, UINT8 Request, UINT16 value, UINT16 index);

#if TARGET_HCF_FAMILY
/********************************************************************************

Name:           UsbHalDataTransmitDone

Description:    Informs top HAL that host has completed write transaction on 
				DSP download pipe
				
Arguments:      hUpper			- top HAL handle

Return Value:   none

Notes:			

********************************************************************************/
__shimcall__
void UsbHalDownloadTransmitDone (PVOID hUpper);
#endif


// HSF specific control request types (bRequest)
typedef enum		
{
	CD2_GET_INFROMATION,
	CD2_UPLOAD_FIRMWARE,
	CD2_CONTROL_SCRIPT,
	CD2_READ_EEPROM,
	CD2_WRITE_EEPROM,
	CD2_RESET,
	CD2_WAKEONRING
} CD2REQUESTTYPE;

#pragma pack (1)
typedef struct
{
	UINT8	bmRequestType;
	UINT8	bRequest;
	UINT16	wValue;
	UINT16	wIndex;
	UINT16	wLength;
	PVOID   pData;
} USB_REQUEST_HEADER, *PUSB_REQUEST_HEADER;
#pragma pack ()

#define MAX_NOTIFY_PACKET	64

#ifdef __cplusplus
}
#endif

#endif /* __USBHALOS_H */
