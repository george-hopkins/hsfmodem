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
*                     Version Control Information
*
*	$Header:   R:/pvcs68/vm/common/Cyprus Project/archives/managers/include/diagmgr_ex.h-arc   1.1   Jan 27 2003 19:59:08   Shai  $
*
*****************************************************************************************/


/****************************************************************************************

File Name:			DiagMgr_ex.h	

File Description:	Diagnostics Manager Data Strucutres	

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


#ifndef __DIAG_MGR_EX_H__
#define __DIAG_MGR_EX_H__


#include "cnxt_diag_codes_ex.h"



#define	DIAG_MGR_MAJOR_VERSION	0
#define	DIAG_MGR_MINOR_VERSION	1

#define DIAG_MGR_VERSION (((DIAG_MGR_MAJOR_VERSION) << 16) | (DIAG_MGR_MINOR_VERSION))


/* Handle definitions for strict type control */
typedef struct DIAGMGR_handle_tag
{
	int dummy;

} *HDIAGMGR;


__shimcall__
PVOID		DiagMgrGetInterface				(void);

__shimcall__
UINT32		DiagMgrGetInterfaceVersion		(void);

__shimcall__
HDIAGMGR	DiagMgrCreate					(IN		PSYSTEM_INSTANCES_T pSys);			

__shimcall__
COM_STATUS	DiagMgrDestroy					(IN		HDIAGMGR	hInstance);

__shimcall__
COM_STATUS	DiagMgrOpen						(IN		HDIAGMGR	hInstance);

__shimcall__
COM_STATUS	DiagMgrClose					(IN		HDIAGMGR	hInstance);

__shimcall__
COM_STATUS	DiagMgrMonitor					(IN		HDIAGMGR			hInstance,
											 IN		CNXT_DIAG_CODES		MonitorCode,
											 OUT	PVOID				pOutBuffer,
											 IO		PUINT32				pdwOutBufferSize);

__shimcall__
COM_STATUS	DiagMgrConfigure				(IN		HDIAGMGR			hInstance,
											 IN		CNXT_DIAG_CFG_CODES	ConfigureCode,
											 IN		PVOID				pInBuffer,
											 IN		UINT32				dwBufferSize);

__shimcall__
COM_STATUS	DiagMgrControl					(IN		HDIAGMGR			hInstance,
											 IN		CNXT_DIAG_CTRL_CODES ControlCode,
											 IN		PVOID				pInBuffer,
											 IN		UINT32				dwBufferSize);


typedef struct I_DIAG_MGR_TAG{

	/************ Common Interface functions. ************/
	
	/********************************************************************************
	
	Function Name: GetInterfaceVersion.

	Function Description: The function returns the interface compilation version.
						 The function should be called before any other function 
						 in the function table in order to verify that the calling 
						 code and the block code are compatible.
	
	Arguments: None.
	
	Return Value: The interface compilation version.

	********************************************************************************/
	UINT32		(*GetInterfaceVersion)(void);


	/********************************************************************************

	Function Name: Create.

	Function Description: The function creates a block instance.
	
	Arguments: pSys - Pointer to the global Octopus system instances structure

	Return Value: An handle to the created diagnostic instance. (this handle will passed as 
			      an argument to all other functions to identify the block instance).

	********************************************************************************/
	HDIAGMGR		(*Create)(IN	PSYSTEM_INSTANCES_T	pSys);			

	/********************************************************************************

	Function Name: Destroy.

	Function Description: The function destroy the block instance identify by the hBlock
						  handle. After the destroying this handle becomes invalid. 
	
	Arguments: hblock - a handle to the block to destroy. (this handle was returned by 
						the create function when we created this block instance).
	
	Return Value: COM_STATUS (COM_STATUS_SUCCESS if the block instance was successfully 
				  destroyed).

	********************************************************************************/
	COM_STATUS	(*Destroy)(IN HDIAGMGR	hBlock);

	/********************************************************************************

	Function Name: Open.

	Function Description: The function initialize the block identified by the hBlock 
						  parameter.
	
	Arguments: a handle to the block to initialize.
	
	Return Value: COM_STATUS (COM_STATUS_SUCCESS if the block instance was successfully
				  initialized).

	********************************************************************************/
	COM_STATUS	(*Open)(IN	HDIAGMGR	hBlock);

	/********************************************************************************

	Function Name: Close.

	Function Description: The function closes the block identified by the hBlock handle.
						  After the instance was closed, its handle can be destroyed by 
						  the destroy function.
	
	Arguments: a handle to the block to close.
	
	Return Value: COM_STATUS (COM_STATUS_SUCCESS if the block instance was successfully
				  closed).

	********************************************************************************/
	COM_STATUS	(*Close)(IN HDIAGMGR	hBlock);		

	/********************************************************************************

	Function Name: Monitor.

	Function Description: The function is used to query/monitor the codes in Octopus.

	Arguments: hBlock - block handle (identify of the block).
			   MonitorCode - enumerated code identifying the monitor type.
			   pInBuffer - pointer to the input buffer.
			   nInBufferSize - the size of data in the input buffer.
			   pOutBuffer - pointer to the output buffer.
			   pdwOutBufferSize - [IN] - size of the buffer, [OUT] - size of the data 

	Return Value: COM_STATUS (COM_STATUS_SUCCESS if the block instance was successfully
				  monitored).

	********************************************************************************/
	COM_STATUS	(*Monitor)(	IN	HDIAGMGR			hBlock,
							IN	CNXT_DIAG_CODES		MonitorCode,
							OUT	PVOID				pOutBuffer,
							IO	PUINT32				pdwOutBufferSize);

	/********************************************************************************

	Function Name: Configure.

	Function Description: The function is used to configure Octopus by a configuration code.

	Arguments: hBlock - block handle (identify of the block).
			   ConfigCode - enumerated code for configuration.
			   pInBuffer - pointer to the input buffer.
			   nInBufferSize - the size of data in the input buffer.

	Return Value: COM_STATUS (COM_STATUS_SUCCESS if the block instance was successfully
				  configured).

	********************************************************************************/
	COM_STATUS	(*Configure)(	IN	HDIAGMGR			hBlock,
								IN	CNXT_DIAG_CFG_CODES	ConfigCode,
								IN	PVOID				pInBuffer,
								IN	UINT32				nInBufferSize);

	/********************************************************************************

	Function Name: Control.

	Function Description: The function is used to control Octopus by a configuration code.

	Arguments: hBlock - block handle (identify of the block).
			   ControlCode - enumerated code for control.
			   pInBuffer - pointer to the input buffer.
			   nInBufferSize - the size of data in the input buffer.

	Return Value: COM_STATUS (COM_STATUS_SUCCESS if the block instance was successfully
				  controlled).

	********************************************************************************/
	COM_STATUS	(*Control)(	IN	HDIAGMGR				hBlock,
							IN	CNXT_DIAG_CTRL_CODES	ControlCode,
							IN	PVOID					pInBuffer,
							IN	UINT32					nInBufferSize);

} I_DIAG_MGR_T, *PI_DIAG_MGR_T;

#ifdef USE_DIRECT_API

    #define DIAGMGR_GetInterfaceVersion( pSys)								DiagMgrGetInterfaceVersion   ()
    #define DIAGMGR_Create(   pSys)											DiagMgrCreate                (pSys)
    #define DIAGMGR_Destroy(  pSys)											DiagMgrDestroy               (pSys->hDiagMgr)
    #define DIAGMGR_Open(     pSys)											DiagMgrOpen                  (pSys->hDiagMgr)
    #define DIAGMGR_Close(    pSys)											DiagMgrClose                 (pSys->hDiagMgr)
    #define DIAGMGR_Configure(pSys, Code, pInBuf, InBufSize)				DiagMgrConfigure             (pSys->hDiagMgr, Code, pInBuf, InBufSize)
    #define DIAGMGR_Monitor(  pSys, Code, pOutBuf, pOutBufSize)				DiagMgrMonitor               (pSys->hDiagMgr, Code, pOutBuf, pOutBufSize)
    #define DIAGMGR_Control(  pSys, Code, pInBuf, InBufSize)				DiagMgrControl               (pSys->hDiagMgr, Code, pInBuf, InBufSize)

#else

    #define DIAGMGR_GetInterfaceVersion( pSys)								pSys->pftDiagMgr->GetInterfaceVersion   ()
    #define DIAGMGR_Create(   pSys)											pSys->pftDiagMgr->Create                (pSys)
    #define DIAGMGR_Destroy(  pSys)											pSys->pftDiagMgr->Destroy               (pSys->hDiagMgr)
    #define DIAGMGR_Open(     pSys)											pSys->pftDiagMgr->Open                  (pSys->hDiagMgr)
    #define DIAGMGR_Close(    pSys)											pSys->pftDiagMgr->Close                 (pSys->hDiagMgr)
    #define DIAGMGR_Configure(pSys, Code, pInBuf, InBufSize)				pSys->pftDiagMgr->Configure             (pSys->hDiagMgr, Code, pInBuf, InBufSize)
    #define DIAGMGR_Monitor(  pSys, Code, pOutBuf, pOutBufSize)				pSys->pftDiagMgr->Monitor               (pSys->hDiagMgr, Code, pOutBuf, pOutBufSize)
    #define DIAGMGR_Control(  pSys, Code, pInBuf, InBufSize)				pSys->pftDiagMgr->Control               (pSys->hDiagMgr, Code, pInBuf, InBufSize)

#endif


#endif /* __DIAG_MGR_EX_H__ */
