/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2004 Conexant Systems, Inc.
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
 * 6.   Redistribution. Permission to redistribute this software without
 * modification is granted, without prejudice to Linuxant's ability to obtain
 * reparation for any unauthorized distribution of previous versions of this
 * software released under prior LICENSE terms. Modification or redistribution
 * of this software under different terms requires explicit written approval
 * signed by an authorized Linuxant officer.
 * 
 * 7.   Performance. V.92 modems are designed to be capable of receiving data at
 * up to 56Kbps with compatible phone line and server equipment, and transmitting
 * data at up to 31.2Kbps. V.90 modems are designed to be capable of receiving
 * data at up to 56 Kbps from a compatible service provider and transmitting data
 * at up to about 28.8 Kbps. Public networks currently limit download speeds to
 * about 53Kbps. Actual speeds vary and are often less than the maximum possible.
 * 
 * 
 */

/****************************************************************************************
 *                     Version Control Information                                      *
 *                                                                                      *
 * $Header:   R:/pvcs68/vm/common/Cyprus Project/archives/HSF4Octopus/SC_include/ComCtrl_Ex.h_v   1.7   03 Mar 2004 19:34:06   shay  $
 * 
*****************************************************************************************/

/****************************************************************************************

File Name:          ComCtrl_Ex.h    

File Description:   Com Controller object data structures

*****************************************************************************************/

#ifndef __COMCTRL_EX_H__
#define __COMCTRL_EX_H__


/* Current ComController version */
#define COMCTRL_MAJOR_VERSION   0
#define COMCTRL_MINOR_VERSION   1

#define COMCTRL_VERSION (((COMCTRL_MAJOR_VERSION) << 16) | (COMCTRL_MINOR_VERSION))


/* Supported Configure codes        */
typedef enum 
{
    COMCTRL_CONFIG_DEVICE_ID,           /* [in] PVOID               - required before Init()*/
    COMCTRL_CONFIG_EVENT_HANDLER,       /* [in] PPORT_EVENT_HANDLER - optional              */
	COMCTRL_CONFIG_PORT_NAME,           /* [in] PVOID               - requred before Init()???*/

    COMCTRL_CONFIG_LAST /* Dummy */

}COMCTRL_CONFIG_CODE;

#if !defined(NO_POUNDUG_SUPPORT) && (OS_TYPE == OS_LINUX)
#define COMCTRL_MONITOR_POUND_UG_SUPPORT
#endif

/* 
Supported Monitor codes          
*/
typedef enum 
{
    COMCTRL_MONITOR_STATUS,             /* [out] UINT32 - combination of status bits STS_XXX */
    COMCTRL_MONITOR_DEVICEID,           /* Device Node */

    COMCTRL_MONITOR_TXSIZE,
    COMCTRL_MONITOR_TXCOUNT,
    COMCTRL_MONITOR_TXFREE,

    COMCTRL_MONITOR_RXSIZE,
    COMCTRL_MONITOR_RXCOUNT,
    COMCTRL_MONITOR_RXFREE,

	COMCTRL_MONITOR_PORTNAME,
	COMCTRL_MONITOR_MOH,				/* [out] UINT32 - connection supports MOH */
/*#$YS$ */
	COMCTRL_MONITOR_SYSINST,
#ifdef COMCTRL_MONITOR_POUND_UG_SUPPORT
	COMCTRL_MONITOR_POUND_UG,
	COMCTRL_MONITOR_POUND_UD,
#endif
	COMCTRL_MONITOR_LAST /* Dummy */
}COMCTRL_MONITOR_CODE;

/* 
Supported Control codes
*/
typedef enum 
{
    COMCTRL_CONTROL_SETRTS,           /* Set RTS high                   - no extra data                     */
    COMCTRL_CONTROL_CLRRTS,           /* Set RTS low                    - no extra data                     */
    COMCTRL_CONTROL_SETDTR,           /* Set DTR high                   - no extra data                     */
    COMCTRL_CONTROL_CLRDTR,           /* Set DTR low                    - no extra data                     */
    
    COMCTRL_CONTROL_SET_XON,
    COMCTRL_CONTROL_SET_XOFF,
    COMCTRL_CONTROL_SEND_XON,
    COMCTRL_CONTROL_SEND_XOFF,
    
    COMCTRL_CONTROL_RESETDEV,         /* Reset device if possible       - no extra data                     */
    COMCTRL_CONTROL_CHAR,             /* Send immediate character.      - [IN] (char)pData - char to send   */

    COMCTRL_CONTROL_START_MONITOR_CHAR, /* Start monitoring special character in Rx queue */
    COMCTRL_CONTROL_STOP_MONITOR_CHAR,  /* Stop monitoring special character in Rx queue */

    COMCTRL_CONTROL_INIT_STATE,

    COMCTRL_CONTROL_PORTCONFIG,       /* Set Port Configuration         - [IN] (PPORT_CONFIG)pData          */
    
    COMCTRL_CONTROL_SLEEP,
    COMCTRL_CONTROL_WAKEUP,
	COMCTRL_CONTROL_FLUSH,			  /* flushing FIFO */
/*#$YS$ Added for Break handling */
	COMCTRL_CONTROL_SET_BREAK_ON,	  /* Set break ON */
	COMCTRL_CONTROL_SET_BREAK_OFF,	  /* Set break OFF */

    COMCTRL_CONTROL_LAST              /* Dummy */

}COMCTRL_CONTROL_CODE;

/*#$YS$ Added for Voice support */
typedef enum tagQueueType
{
	
	TX_QUEUE,
	RX_QUEUE

} QUEUE_TYPE;


/*
Port Configuration bits - specify which fields of PORT_CONFIG structure are valid
*/
#define PC_DTE_SPEED        0x00000001
#define PC_PARITY           0x00000010
#define PC_DATA_BITS        0x00000020

#define PC_XINPUT           0x00000100
#define PC_XOUTPUT          0x00000200
#define PC_ERROR            0x00000400
#define PC_SKIPNULL         0x00000800
#define PC_CTS              0x00001000
#define PC_RTS              0x00002000

#define PC_XON_CHAR         0x00010000
#define PC_XOFF_CHAR        0x00020000
#define PC_ERROR_CHAR       0x00040000
#define PC_PEALT_CHAR       0x00080000

#define PC_EVERYTHING       0xFFFFFFFF

/*
Port Configuration structure (used in COMCTRL_CONTROL_PORTCONFIG control)
TODO(?): structure may be compacted (replace BOOLs by bitfields)
*/
typedef struct tagPORT_CONFIG
{
    UINT32          dwValidFileds;  /* Combination of PortConfiguration bits*/

    UINT32          dwDteSpeed;

    enum
    {
        PC_PARITY_NONE,
        PC_PARITY_ODD,
        PC_PARITY_EVEN,
        PC_PARITY_MARK,
        PC_PARITY_SPACE
    }               eParity;        /* Parity check*/

    enum
    {
        PC_DATABITS_7,
        PC_DATABITS_8
    }               eDataBits;      /* Data bits - only 7 and 8 are enabled*/

    BOOL            fXOutput;       /* Xoff/Xon enabled for output*/
    BOOL            fXInput;        /* Xoff/Xon enabled for input*/
    BOOL            fError;         /* Replace chars with parity error by ErrorChar*/
    BOOL            fNull;          /* Skip NULL characters*/
    BOOL            fCTS;           /* Use CTS for Tx flow control*/
    BOOL            fRTS;           /* Use RTS for Rx flow control*/

    char            cXonChar;
    char            cXoffChar;
    char            cErrorChar;
    char            cPEAltChar;

}   PORT_CONFIG, *PPORT_CONFIG;


/*
Port Event Handler - used in Configure(COMCTRL_CONFIG_EVENT_HANDLER)
*/
typedef struct tagPORT_EVENT_HANDLER
{
    void    (*pfnCallback) (PVOID pRef, UINT32 dwEventMask) __shimcall__;
    PVOID   pRef;

} PORT_EVENT_HANDLER, *PPORT_EVENT_HANDLER;

/*
Port Monitor Data - used in Monitor(COMCTRL_MONITOR_POUND_UD etc..)
*/
typedef struct tagPORT_MONITOR_DATA
{
	UINT32	dwSize;
    PVOID   pBuf;

} PORT_MONITOR_DATA, *PPORT_MONITOR_DATA;


/********************************************************************/
/* Bitmaks definition*/
/********************************************************************/ 

/**/
/* Modem status bits*/
/**/ 
#define COMCTRL_STS_CTS_HOLD    0x00000001  /* Transmit is on CTS hold                  */
#define COMCTRL_STS_DSR_HOLD    0x00000002  /* Transmit is on DSR hold                  */
#define COMCTRL_STS_RLSD_HOLD   0x00000004  /* Transmit is on RLSD hold                 */
#define COMCTRL_STS_XOFF_HOLD   0x00000008  /* Received handshake                       */
#define COMCTRL_STS_XOFF_SENT   0x00000010  /* Issued handshake                         */
#define COMCTRL_STS_EOF         0x00000020  /* EOF character found                      */
#define COMCTRL_STS_TXIMMED     0x00000040  /* Immediate character being transmitted    */


/**/
/* Event bits*/
/**/
#define COMCTRL_EVT_RXCHAR      0x00000001  /* Any Character received                  */
#define COMCTRL_EVT_RXFLAG      0x00000002  /* Received certain character              */
#define COMCTRL_EVT_TXCHAR      0x00000004  /* Any character transmitted               */
#define COMCTRL_EVT_TXEMPTY     0x00000008  /* Transmit Queue Empty                    */
#define COMCTRL_EVT_CTS         0x00000010  /* CTS changed state                       */
#define COMCTRL_EVT_CTSS        0x00000020  /* CTS state                               */
#define COMCTRL_EVT_DSR         0x00000040  /* DSR changed state                       */
#define COMCTRL_EVT_DSRS        0x00000080  /* DSR state                               */
#define COMCTRL_EVT_RLSD        0x00000100  /* RLSD changed state                      */
#define COMCTRL_EVT_RLSDS       0x00000200  /* RLSD state                              */
#define COMCTRL_EVT_BREAK       0x00000400  /* BREAK received                          */
#define COMCTRL_EVT_ERR         0x00000800  /* Line status error occurred              */
#define COMCTRL_EVT_RING        0x00001000  /* Ring signal detected                    */
/*#$YS$ */
#define COMCTRL_EVT_RINGS       0x00002000  /* Ring signal detected                    */
/*#define COMCTRL_EVT_PARITY_ERR  0x00002000  Parity Error occured                   */
/*#define COMCTRL_EVT_RXOVRN      0x00004000  DTE Rx buffer overrun detected          */
#define COMCTRL_EVT_PARITY_ERR  0x00004000  /* Parity Error occured                    */
#define COMCTRL_EVT_RXOVRN      0x00008000  /* DTE Rx buffer overrun detected          */
#define COMCTRL_EVT_EXT_PICKUP	0x00010000	/* 2nd phone offhook detection				*/

/* Modem related */
#define COMCTRL_MAX_PORT_NAME_LENGTH	256

/* Direct API */
__shimcall__ UINT32       ComCtrl_GetInterfaceVersion  (void);
__shimcall__ HANDLE       ComCtrl_Create               (void);            
__shimcall__ COM_STATUS   ComCtrl_Destroy              (HANDLE hInst);
__shimcall__ COM_STATUS   ComCtrl_Open                 (HANDLE hInst);
__shimcall__ COM_STATUS   ComCtrl_Close                (HANDLE hInst);
__shimcall__ COM_STATUS   ComCtrl_Configure            (HANDLE hInst, COMCTRL_CONFIG_CODE  eCode,  PVOID pConfig);
__shimcall__ COM_STATUS   ComCtrl_Monitor              (HANDLE hInst, COMCTRL_MONITOR_CODE eCode, PVOID pMonitor);
__shimcall__ COM_STATUS   ComCtrl_Control              (HANDLE hInst, COMCTRL_CONTROL_CODE eCode, PVOID pControl);
__shimcall__ UINT32       ComCtrl_Read                 (HANDLE hInst, PVOID pBuf, UINT32 dwSize);
__shimcall__ UINT32       ComCtrl_Write                (HANDLE hInst, PVOID pBuf, UINT32 dwSize);


/* Interface through function table */

typedef struct I_COM_CTRL_TAG
{
    /************ Common Interface functions. ************/

    UINT32      (*GetInterfaceVersion)  (void);
    HANDLE      (*Create)               (void);
    COM_STATUS  (*Destroy)              (HANDLE hInst);
    COM_STATUS  (*Open)                 (HANDLE hInst);
    COM_STATUS  (*Close)                (HANDLE hInst);
    COM_STATUS  (*Configure)            (HANDLE hInst, COMCTRL_CONFIG_CODE  eCode,  PVOID pConfig);
    COM_STATUS  (*Monitor)              (HANDLE hInst, COMCTRL_MONITOR_CODE eCode, PVOID pMonitor);
    COM_STATUS  (*Control)              (HANDLE hInst, COMCTRL_CONTROL_CODE eCode, PVOID pControl);

    /************ Specific Interface functions. ************/

    UINT32      (*Read)   (HANDLE hInst, PVOID pBuf, UINT32 dwSize);
    UINT32      (*Write)  (HANDLE hInst, PVOID pBuf, UINT32 dwSize);

} I_COM_CTRL_MGR_T, *PI_COM_CTRL_T;

PVOID ComCtrlGetInterface(void);



#ifdef USE_DIRECT_API

    #define COMCTRL_GetInterfaceVersion(pSys)                  ComCtrl_GetInterfaceVersion  ()
    #define COMCTRL_Create(             pSys)                  ComCtrl_Create               ()
    #define COMCTRL_Destroy(            pSys)                  ComCtrl_Destroy              (pSys->hComCtrl)
    #define COMCTRL_Open(               pSys)                  ComCtrl_Open                 (pSys->hComCtrl)
    #define COMCTRL_Close(              pSys)                  ComCtrl_Close                (pSys->hComCtrl)
    #define COMCTRL_Configure(          pSys, eCode, pConfig)  ComCtrl_Configure            (pSys->hComCtrl, eCode, pConfig)
    #define COMCTRL_Monitor(            pSys, eCode, pMonitor) ComCtrl_Monitor              (pSys->hComCtrl, eCode, pMonitor)
    #define COMCTRL_Control(            pSys, eCode, pControl) ComCtrl_Control              (pSys->hComCtrl, eCode, pControl)
    #define COMCTRL_Read(               pSys, pBuf, dwSize)    ComCtrl_Read                 (pSys->hComCtrl, pBuf, dwSize)
    #define COMCTRL_Write(              pSys, pBuf, dwSize)    ComCtrl_Write                (pSys->hComCtrl, pBuf, dwSize)

#else

    #define COMCTRL_GetInterfaceVersion(pSys)                  pSys->pftComCtrl->GetInterfaceVersion  ()
    #define COMCTRL_Create(             pSys)                  pSys->pftComCtrl->Create               ()
    #define COMCTRL_Destroy(            pSys)                  pSys->pftComCtrl->Destroy              (pSys->hComCtrl)
    #define COMCTRL_Open(               pSys)                  pSys->pftComCtrl->Open                 (pSys->hComCtrl)
    #define COMCTRL_Close(              pSys)                  pSys->pftComCtrl->Close                (pSys->hComCtrl)
    #define COMCTRL_Configure(          pSys, eCode, pConfig)  pSys->pftComCtrl->Configure            (pSys->hComCtrl, eCode, pConfig)
    #define COMCTRL_Monitor(            pSys, eCode, pMonitor) pSys->pftComCtrl->Monitor              (pSys->hComCtrl, eCode, pMonitor)
    #define COMCTRL_Control(            pSys, eCode, pControl) pSys->pftComCtrl->Control              (pSys->hComCtrl, eCode, pControl)
    #define COMCTRL_Read(               pSys, pBuf, dwSize)    pSys->pftComCtrl->Read                 (pSys->hComCtrl, pBuf, dwSize)
    #define COMCTRL_Write(              pSys, pBuf, dwSize)    pSys->pftComCtrl->Write                (pSys->hComCtrl, pBuf, dwSize)

#endif


#endif      /* #ifndef __MODEM_H__  */
