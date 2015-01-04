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
*		                     Version Control Information
*
*	$Header:   R:/pvcs68/vm/common/Cyprus Project/archives/managers/include/osdiag.h-arc   1.3   Aug 14 2003 23:34:52   shay  $
*
*****************************************************************************************/


/****************************************************************************************

File Name:			OsDiag.h

File Description:	Operating Specific Diagnostics prototypes.

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

/**********************************************************************************
History:
04/06/01:	HL. The initial version.

**********************************************************************************/

#ifndef __OSDIAG__H
#define __OSDIAG__H

#include "comtypes.h"
#include "diagmgr_ex.h"


/****************************************************************************************

Function Name:			osOpenSDK2KDriver	

Function Description:	This function will open the SDK2K driver and exchange function tables
                        with it.

Arguments:				pSys - The pointer to the system instance.

Return Value:			Returns a HANDLE of OsDiagModem object
						
Notes:					

*****************************************************************************************/
__shimcall__
HANDLE OsDiagMgrOpen(PSYSTEM_INSTANCES_T pSys,
#if OS_TYPE == OS_LINUX
	PI_DIAG_MGR_T pDiagMgrInterface
#endif
	);

/****************************************************************************************

Function Name:			osCloseSDK2KDriver	

Function Description:	This function will open the SDK2K driver and exchange function tables
                        with it.

Arguments:				pSys - The pointer to the system instance.
                        pSDK2KExportFuncTable - The SDK2K function table pointer.

Return Value:			Returns a status of closing success or failure.
						
Notes:					

*****************************************************************************************/
__shimcall__
COM_STATUS OsDiagMgrClose(HANDLE hOsDiagModem);


/****************************************************************************************

Function Name:			osNotifySDK2K

Function Description:	This function will call back the SDK2K driver.

Arguments:				pSys - The pointer to the system instance.
                        pSDK2KExportFuncTable - The SDK2K function table pointer.
						pInBuf - The pointer to a diagnostic code.
						nInBufSize - The size of the diagnostic code.
						pOutBuf - The pointer to the output buffer (result and time stamp).
						nOutBufSize - The size of the output buffer.

Return Value:			Returns a status of success or failure.
						
Notes:					

*****************************************************************************************/
__shimcall__
COM_STATUS OsDiagMgrNotify(	IN HANDLE			hOsDiagMgr,						 
							CNXT_DIAG_CODES		Code,
							UINT32				dwData		);


#if !defined NO_DUMPMGR_SUPPORT
#if OS_TYPE != OS_LINUX
#include "dumpmgr_ex.h"
HDUMPMGR        OsDumpMgrOpen(PSYSTEM_INSTANCES_T pSys);
COM_STATUS	    OsDumpMgrClose(HDUMPMGR hDumpMgr);
COM_STATUS OsDumpMgrChannelCreated(HDUMPMGR hDumpMgr, unsigned int nChannel);
COM_STATUS OsDumpMgrChannelDestroyed(HDUMPMGR hDumpMgr, unsigned int nChannel);
COM_STATUS OsDumpMgrChannelRestarted(HDUMPMGR hDumpMgr, unsigned int nChannel);
#endif

#endif /* #if !defined NO_DUMPMGR_SUPPORT*/
#endif 
