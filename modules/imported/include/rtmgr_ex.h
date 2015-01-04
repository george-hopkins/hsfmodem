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

File Name:			RTMgrE.h	

File Description:	Runtime Manager enumerated types and prototypes

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


#ifndef __RTMGR_H__
#define __RTMGR_H__

#include "comtypes.h"
#include "memmgr_ex.h"
#include "intfctrl_ex.h"

/*********************** Run time Manager Exported types :********************/

#define	RT_MGR_MAJOR_VERSION	((UINT32)0)
#define	RT_MGR_MINOR_VERSION	((UINT32)2)

#define RT_MGR_VERSION ((UINT32)(((RT_MGR_MAJOR_VERSION) << 16) | (RT_MGR_MINOR_VERSION)))


typedef enum ASYNC_CALL_TYPE_ENUM
{
	ASYNC_CALL_ONE_SHOT, 
	ASYNC_CALL_PERIODIC
} ASYNC_CALL_TYPE;

typedef struct ASYNC_CALL_CONFIG_STRUCT
{
	ASYNC_CALL_TYPE		Type;
	unsigned			TimeOut;
	PUINT32				pStack;				
	UINT32				StackSize;		
	PCBFUNC     		pFuncAsyncCall;
	PVOID				pParam;
}ASYNC_CALL_CONFIG, *PASYNC_CALL_CONFIG;

#define ASYNC_CALL_CONFIG_T ASYNC_CALL_CONFIG

/* Handle definitions for strict type control */
typedef struct RTMGR_handle_tag
{
	int dummy;
} *HRTMGR;

typedef struct RTMGR_client_handle_tag
{
	int dummy;
} *HRTCLIENT;

typedef struct RTMGR_async_call_tag
{
	int dummy;
} *HASYNCCALL;


#ifdef __cplusplus
extern "C"
{
#endif


/* Direct API */

GLOBAL	UINT32 RTMgrGetInterfaceVersion(void);


/********************************************************************************
Function Name: RTMgrGetInterface.

Function Description:Return pointer to RunTime Manager interface.

rguments: None.

Return Value: Pointer to RunTime interface function table.

********************************************************************************/
GLOBAL	PVOID	RTMgrGetInterface(void);

/********************************************************************************
Function Name: RTMGR_Create.

Function Description: The function creates an instance of the Runtime Manager

Arguments: System handle - structure used to access any block handle associated
			with a given instance of hardware

Return Value: An handle to the created instance. (this handle will passed as an 
			  argument to all other functions to identify the instance).
			  (in case of error it returns NULL).

Interrupt safe: no
********************************************************************************/
HRTMGR	RTMgrCreate(PSYSTEM_INSTANCES_T  pSys);			


/********************************************************************************

Function Name: RTMGR_Destroy.

Function Description: The function destroy the Runtime Manager instance 
					  The handle becomes invalid upon function retunr

Arguments: hRtMgr - an instance handle to destroy.

Return Value: COM_STATUS (COM_STATUS_SUCCESS if the instance was successfully 
			  destroyed). Note, that even if the function returned some fail code
			  the instance is destroyed and the handle is no longer valid
Interrupt safe: no
********************************************************************************/
COM_STATUS	RTMgrDestroy (IN HRTMGR hRtMgr);


/********************************************************************************

Function Name: RTMGR_Open.

Function Description: The function initialize the instance. This call must precede 
					  all other calls to the specified instance, except 
					  RTMGR_Destroy.

Arguments: a handle to open

Return Value: COM_STATUS (COM_STATUS_SUCCESS if the instance was successfully
			  initialized). 
Interrupt safe: no
********************************************************************************/
COM_STATUS	RTMgrOpen (IN	HRTMGR hRtMgr);

	
/********************************************************************************
Function Name: RTMGR_Close.

Function Description: The function closes the instance identified by the hRtMgr 
					  handle. After the instance was closed, its handle can be 
					  destroyed by the destroy function.

Arguments: a handle to the instance to close.

Return Value: COM_STATUS (COM_STATUS_SUCCESS if the block instance was 
				successfully closed).
Interrupt safe: no
********************************************************************************/
COM_STATUS	RTMgrClose	(IN	HRTMGR hRtMgr);		



/************ Specific Interface functions. ************/
	
/********************************************************************************
Function Name:RegisterClient
  
Function Description: The function registers a client to the Runtime Manager identified
					  by hRunTime. 

Arguments: hRunTime - a handle for the Runtime Manager instance.
		   TimerResolution - OS timer resolution required for a new client.

Return Value: a handle (identifier) for the new client. This handle should be used as
			  a parameter for all client-related functions).
Interrupt safe: no
********************************************************************************/
HRTCLIENT RTMgrRegisterClient	(	IN	HRTMGR		hRunTime,
									IN	unsigned	TimerResolution);


/********************************************************************************
Function Name:SetClientTimerResolution
  
Function Description: Updates the timer resolution required by the 
					  Client. 

Arguments:	hClient	- handle to the client
		    TimerResolution - New client reqired timer resolution

Return Value: COM_STAUTS_SUCCESS when client resolution is successfuly 
				updated, else failure code.
Interrupt safe: no
********************************************************************************/
COM_STATUS	RTMgrSetClientTimerResolution	(	IN	HRTCLIENT	hClient,
												IN	unsigned	TimerResolution);


/********************************************************************************
Function Name: UnregisterClient
  
Function Description: The function unregisters a client from the Runtime manager.

Arguments: hClient - a handle for the specific Runtime client (which returns from
		   the RegisterClient function).

Return Value: COM_STATUS (COM_STATUS_SUCCESS if the client was successfully unregistered)
Interrupt safe: no
********************************************************************************/
COM_STATUS	RTMgrUnregisterClient	(IN	HRTCLIENT	hClient);


/********************************************************************************
Function Name: CreateAsyncCall
  
Function Description: Creates a non scheduled async call.
Arguments: hClient - a handle for the specific RunTIme client (which returns from 
		   the RegisterClient function).

Return Value: a handle to the AsyncCall. (in case of error it returns NULL).
Interrupt safe: no
********************************************************************************/
HASYNCCALL	RTMgrCreateAsyncCall	(IN	HRTMGR	hRtMgr);


/********************************************************************************
Function Name: SetAsyncCall
  
Function Description: This function is a request to the runtime manager to schedule 
					  an async call as described in the structure pConfig poins to.
					  Function specified in pConfig-pointed structure will be 
					  called after time elapses. The timer can be be one-shot 
					  or periodic.

Arguments: hAsyncCall - a handle to the AsyncCall that was returned from the CreateAsyncCall
		   pRefData - Reference data to be passed to the async callback function
		   pConfig - a pointer to a structure containing the data about the request
		   for the async call .

Return Value: COM_STATUS_SUCCES on success, else error code.
Interrupt safe: yes
********************************************************************************/
COM_STATUS	RTMgrSetAsyncCall	(	IN	HASYNCCALL			hAsyncCall,
									IN	PASYNC_CALL_CONFIG	pConfig);


/********************************************************************************
Function Name: RestartAsyncCall
  
Function Description:	Sets an async call with configuration passed in
						a previous SetAsyncCall of the specified hAsyncCall.
						If the async call was not set previously the 
						behaviour is not defined.

Arguments: hAsyncCall - a handle to the AsyncCall that was returned from the CreateAsyncCall
		   pRefData - Reference data to be passed to the async callback function

Return Value: COM_STATUS_SUCCES on success, else error code.
Interrupt safe: no
********************************************************************************/
COM_STATUS	RTMgrRestartAsyncCall	(IN	HASYNCCALL	hAsyncCall);


/********************************************************************************
Function Name: CancelAsyncCall
  
Function Description: The function cancels the AsyncCall identified by the hAsyncCall
					  handle from scheduler. The async call can be rescheduled through 
					  SetAsyncCall function, or destroyed with DestroyAsyncCall function.

Arguments: hAsyncCall - a handle to the AsyncCall that was returned from the CreateAsyncCall
		   function.

Return Value: COM_STATUS (COM_STATUS_SUCCESS if the AsyncCall was successfully canceled)
Interrupt safe: no
********************************************************************************/
COM_STATUS	RTMgrCancelAsyncCall	(IN	HASYNCCALL	hAsyncCall);


/********************************************************************************
Function Name: DestroyAsyncCall
  
Function Description: The function destroys the AsyncCall identified by the hAsyncCall
					  handle.The async call handle becomes unusable.

Arguments: hAsyncCall - a handle to the AsyncCall that was returned from the CreateAsyncCall
		   function.

Return Value: COM_STATUS (COM_STATUS_SUCCESS if the AsyncCall was successfully canceled)
Interrupt safe: no
********************************************************************************/
COM_STATUS	RTMgrDestroyAsyncCall	(IN	HASYNCCALL	hAsyncCall);

#ifdef MACTOPUS
/********************************************************************************
Function Name: CalledFromTimer
  
Function Description: The function check if current execution context is in RtMgr timer thread

Arguments: hRtMgr - RunTime manager handle 

Return Value: TRUE if execution context is in timer thread, otherwise - FALSE

********************************************************************************/
BOOL RTMgrCalledFromTimer	(IN	HRTMGR	hRtMgr);
#endif


#ifdef __cplusplus
}
#endif

typedef struct I_RT_MGR_TAG
{
	
	UINT32		(*GetInterfaceVersion)  (	void);
	HRTMGR		(*Create)               (PSYSTEM_INSTANCES_T pSys);
	COM_STATUS	(*Destroy)              (IN HRTMGR hBlock);
	COM_STATUS	(*Open)                 (IN	HRTMGR hBlock);
	COM_STATUS	(*Close)                (IN	HRTMGR hBlock);		
	HRTCLIENT	(*RegisterClient)       (IN	HRTMGR hRunTime,
										 IN	unsigned	TimerResolution);
	COM_STATUS	(*SetClientTimerResolution)(IN	HRTCLIENT	hClient, IN	unsigned TimerResolution);
	COM_STATUS	(*UnregisterClient)     (IN	HRTCLIENT hClient);
	HASYNCCALL	(*CreateAsyncCall)      (IN	HRTMGR	hRtMgr);
	COM_STATUS	(*SetAsyncCall)         (IN	HASYNCCALL hAsyncCall, IN PASYNC_CALL_CONFIG pConfig);
	COM_STATUS	(*RestartAsyncCall)     (IN	HASYNCCALL hAsyncCall);
	COM_STATUS	(*CancelAsyncCall)      (IN	HASYNCCALL hAsyncCall);
	COM_STATUS	(*DestroyAsyncCall)     (IN	HASYNCCALL hAsyncCall);

} I_RT_MGR_T, *PI_RT_MGR_T;





#ifdef USE_DIRECT_API

	#define RTMGR_GetInterfaceVersion(      pSys)                           RTMgrGetInterfaceVersion        ()
	#define RTMGR_Create(                   pSys)                           RTMgrCreate                     (pSys)
	#define RTMGR_Destroy(                  pSys)                           RTMgrDestroy                    (pSys->hRtMgr)
	#define RTMGR_Open(                     pSys)                           RTMgrOpen                       (pSys->hRtMgr)
	#define RTMGR_Close(                    pSys)		                    RTMgrClose                      (pSys->hRtMgr)
	#define RTMGR_RegisterClient(           pSys, TimerResolution)          RTMgrRegisterClient             (pSys->hRtMgr, TimerResolution)
	#define RTMGR_SetClientTimerResolution( pSys, hClient, TimerResolution) RTMgrSetClientTimerResolution   (hClient, TimerResolution)
	#define RTMGR_UnregisterClient(         pSys, hClient)                  RTMgrUnregisterClient           (hClient)
	#define RTMGR_CreateAsyncCall(          pSys)                           RTMgrCreateAsyncCall            (pSys->hRtMgr)
	#define RTMGR_SetAsyncCall(             pSys, hAsyncCall, pConfig)      RTMgrSetAsyncCall               (hAsyncCall, pConfig)
	#define RTMGR_RestartAsyncCall(         pSys, hAsyncCall)               RTMgrRestartAsyncCall           (hAsyncCall)
	#define RTMGR_CancelAsyncCall(          pSys, hAsyncCall)               RTMgrCancelAsyncCall            (hAsyncCall)
	#define RTMGR_DestroyAsyncCall(         pSys, hAsyncCall)               RTMgrDestroyAsyncCall           (hAsyncCall)

#else

	#define RTMGR_GetInterfaceVersion(      pSys)                           pSys->pftRtMgr->GetInterfaceVersion        ()
	#define RTMGR_Create(                   pSys)                           pSys->pftRtMgr->Create                     (pSys)
	#define RTMGR_Destroy(                  pSys)                           pSys->pftRtMgr->Destroy                    (pSys->hRtMgr)
	#define RTMGR_Open(                     pSys)                           pSys->pftRtMgr->Open                       (pSys->hRtMgr)
	#define RTMGR_Close(                    pSys)		                    pSys->pftRtMgr->Close                      (pSys->hRtMgr)
	#define RTMGR_RegisterClient(           pSys, TimerResolution)          pSys->pftRtMgr->RegisterClient             (pSys->hRtMgr, TimerResolution)
	#define RTMGR_SetClientTimerResolution( pSys, hClient, TimerResolution) pSys->pftRtMgr->SetClientTimerResolution   (hClient, TimerResolution)
	#define RTMGR_UnregisterClient(         pSys, hClient)                  pSys->pftRtMgr->UnregisterClient           (hClient)
	#define RTMGR_CreateAsyncCall(          pSys)                           pSys->pftRtMgr->CreateAsyncCall            (pSys->hRtMgr)
	#define RTMGR_SetAsyncCall(             pSys, hAsyncCall, pConfig)      pSys->pftRtMgr->SetAsyncCall               (hAsyncCall, pConfig)
	#define RTMGR_RestartAsyncCall(         pSys, hAsyncCall)               pSys->pftRtMgr->RestartAsyncCall           (hAsyncCall)
	#define RTMGR_CancelAsyncCall(          pSys, hAsyncCall)               pSys->pftRtMgr->CancelAsyncCall            (hAsyncCall)
	#define RTMGR_DestroyAsyncCall(         pSys, hAsyncCall)               pSys->pftRtMgr->DestroyAsyncCall           (hAsyncCall)

#endif



#endif /* __RTMGR_H__ */
