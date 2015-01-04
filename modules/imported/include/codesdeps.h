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
*                     PVCS Version Control Information                                  *
*                                                                                       *
* $Header:   P:/V68_Projects/Universal_Access_PDB/archives/HSF Project/WinAC/Winac/codesdeps.h_v   1.62   Sep 12 2006 10:23:00   luj  $
*                                                                                       *
*****************************************************************************************/
/*----------------------------------------------------------------------------
 *
 * Copyright (C) 1996-2001 Conexant Systems Inc. 
 * All rights reserved.
 *
 *----------------------------------------------------------------------------
 *
 * File: codesdeps.h
 *
 *----------------------------------------------------------------------------
 */
 
#ifndef __CODESDEPS_H__
#define __CODESDEPS_H__
 
#if (USE_PRAGMA_PACK == 1)
#pragma pack(push,1)   /* Packing compatible with WinAC, HSF */
#endif
 
#ifndef MAX_PATH
#define MAX_PATH	260
#endif

typedef enum
{
	FAMILY_TYPE_HSF,
	FAMILY_TYPE_HCF,
	FAMILY_TYPE_ACF,
	FAMILY_TYPE_OCT,
} MODEM_FAMILY, *PMODEM_FAMILY;

typedef enum
{
    HDPLX,
    FDPLX
} CONNECTION_TYPE;

typedef enum
{
	V34_FALLBACK_NO_FALLBACK = 0,
	V34_FALLBACK_PCM_SPEED_TOO_LOW,
	V34_FALLBACK_HS_OVER_TWICE,
	V34_FALLBACK_HS_OVER_TWICE_BAD_EQM,
	V34_FALLBACK_HS_TWICE_WITH_1ST_FAILED,
	V34_FALLBACK_ALAW_ULAW_COMBINATION,		/* 5 */
	V34_FALLBACK_RETRAIN_WHILE_DOING_XID,
	V34_FALLBACK_FREQUENT_RETRAIN,
	V34_FALLBACK_BAD_SNR,
	V34_FALLBACK_INTEROP,
	V34_FALLBACK_USER_FORCED,				/* 10 */
	V34_FALLBACK_DUAL_PCM,	
    V34_FALLBACK_LOADED_LOOP,
} V34_FALLBACK_REASON, *PV34_FALLBACK_REASON;
 
typedef enum
{
	RTRN_REASON_NO_RTRN = 0,			/* 0 */
	RTRN_REASON_HIGH_EQM,
	RTRN_REASON_TWO_CONSEC_RR,
	RTRN_REASON_USER_INITIATED,
	RTRN_REASON_REMOTE,
	RTRN_REASON_POWER,					/* 5 */
	RTRN_REASON_TIMEOUT,
	RTRN_REASON_INTEROP,
	RTRN_REASON_RATE_DECREASE,
	RTRN_REASON_BAD_MP,
	RTRN_REASON_FAIL_RATES,
	RTRN_REASON_CRASH,					/* 10 */
	RTRN_REASON_DIL_BAD,
	RTRN_REASON_MOVE_POLE,
	RTRN_REASON_FALLBACK				
} RTRN_REASON, *PRTRN_REASON;

typedef enum
{
	RNG_REASON_NO_RR = 0,
	RNG_REASON_HIGH_EQM,
	RNG_REASON_LOW_EQM,
	RNG_REASON_RETRANSMISSION,
	RNG_REASON_REMOTE,
	RNG_REASON_USER_INITIATED,
	RNG_REASON_SPIKES,	
} RNG_REASON, *PRNG_REASON;

#define MOH_HOLD_TIMEOUT_NOLIMIT		0xFFFFFFFF

typedef enum
{
	MOH_LOCAL_REQ_HOLD,
	MOH_LOCAL_ACCEPT,
	MOH_LOCAL_DISCONNECT,
	MOH_LOCAL_IGNORE,
	MOH_LOCAL_RECONNECT,
	MOH_LOCAL_NONE, /* No Decision yet */
} MOH_LOCAL_DECISION_T;

#define MOH_REMOTE_RESPONSE_RANGE	500

typedef enum
{
	SDK_MOH_REMOTE_REQ_HOLD,			/* MHReq */
	SDK_MOH_REMOTE_REQ_RECONNECT,
	SDK_MOH_REMOTE_REQ_DISCONNECT,
	SDK_MOH_REMOTE_REQ_ACCEPT,
	SDK_MOH_REMOTE_REQ_NONE,
} DIAG_MOH_REMOTE_REQ_T;

typedef enum 
{
	SDK_MOH_REMOTE_RESPONSE_ACK = MOH_REMOTE_RESPONSE_RANGE,
	SDK_MOH_REMOTE_RESPONSE_NACK,
	SDK_MOH_REMOTE_RESPONSE_TIMEDOUT,
	SDK_MOH_REMOTE_RESPONSE_SILENCE,
	SDK_MOH_REMOTE_RESPONSE_DISCONNECT, /* remote sent disconnect acknowledge */
	SDK_MOH_REMOTE_RESPONSE_NONE,
} DIAG_MOH_REMOTE_RESPONSE_T;

typedef enum
{
	CW_FAKE_NONE,		/* Not faked */
	CW_FAKE_FLASH,		/* Fake, flash */
	CW_FAKE_DONT_FLASH, /* Fake, don't flash */
} CW_FAKE_MOH_T;

typedef enum
{
	SDK_DIAG_PORT_OPEN,
	SDK_DIAG_PORT_CLOSE,
} DIAG_NOTIFY_PORT_STATE_T;

typedef enum
{
	MOH_METHOD_MANUAL,
	MOH_METHOD_AUTOMATIC,
	MOH_METHOD_NO_HANDSET_EXCLUSION,
	MOH_METHOD_NONE,
}MOH_METHOD;
 
/* subcodes of CSDIAG_MODEM_STATE */
typedef enum 
{
	DIAG_INITIALIZING				,/* 0 */
    DIAG_IDLE						,
	DIAG_OFF_HOOK_IDLE				, 
	DIAG_DIALTONE_VALIDATE			,
	DIAG_DIALING					, 
	DIAG_RING_ON					,/* 5 */
	DIAG_CALL_PROGRESS				, 
	DIAG_V8							,
	DIAG_V8BIS						,
	DIAG_TRAINING					, 
	DIAG_PHYS_LAYER_CONNECT			,/* 10 */
	DIAG_PROT_LAYER_CONNECT			,
	DIAG_REMOTE_INIT_RATE_RENEG		,
	DIAG_LOCAL_INIT_RATE_RENEG		,
	DIAG_REMOTE_INIT_RTRN			,
	DIAG_LOCAL_INIT_RTRN			,/* 15 */
	DIAG_ESCAPE						,
	DIAG_CLEARDOWN					,
	DIAG_D3							,
	DIAG_CALL_WAITING_TONE_DETECTED	,
	DIAG_CALLER_ID_IN_PROGRESS		,/* 20 */
	DIAG_MOH						,
	DIAG_MOH_RESUMING				,
	DIAG_MOH_CLEARING_DOWN			,
	DIAG_PORT_TO_CLOSE              ,
	DIAG_MODEM_OVER_CURRENT,
	DIAG_MODEM_MOH_STATE_NOT_SUPPORTED,
	DIAG_MODEM_MOH_STATE_WAIT_FOR_USER_DECISION,   /* Accept ignore or disconnect */
	DIAG_MODEM_MOH_STATE_WAIT_FOR_CALL_RESUME,
	DIAG_MODEM_STATE_LAST
}  CNXT_DIAG_MODEM_STATE;
 
/* Subcodes for MOH State */
typedef enum
{
	MOH_STATE_NONE,
	MOH_STATE_AS_DETECTED,
	MOH_STATE_CALLER_ID_PROCESSING,
	MOH_STATE_WAIT_FOR_USER_DECISION,   /* Accept ignore or disconnect */
	MOH_STATE_WAIT_FOR_CALL_RESUME,
	MOH_STATE_REMOTE_REQ_ONHOLD,
	MOH_STATE_WAIT_FOR_CARRIER_SIGNAL,
	MOH_STATE_WAIT_FOR_MODULATION_DISCONNECT,
	MOH_STATE_NOT_SUPPORTED,
	MOH_STATE_WAIT_FOR_REMOTE_RECONNECT,
    MOH_STATE_WAIT_FOR_LOCAL_RECONNECT,
    MOH_STATE_WAIT_FOR_REMOTE_RESUME,
    MOH_STATE_OUTGOING_CALL_REQ,
	MOH_STATE_WAIT_FOR_HANDSET_OFFHOOK, /* After user pressed accept in automatic mode (skipped in manual mode) */
	MOH_STATE_WAIT_FOR_HANDSET_ONHOOK,  /* After user pressed resume in automatic mode (skipped in manual mode) */
	MOH_STATE_WAIT_FOR_REMOTE_RESPONSE,
	MOH_STATE_LAST
}MOH_STATE;
 
/* The overloaded structure for pOutBuffer parameter. */
/* It allows a timestamp could be sent back. */
typedef struct 
{
	PVOID		pOutBuf;
	DWORD		dwTimeStamp;
} OVERLOADED_OUTBUF, *POVERLOADED_OUTBUF;

/* Modem info structure passed in EnumCnxtModem. */
typedef struct MODEMINFO_T
{
	char			szComPort[MAX_PATH];
	char			szFriendlyName[MAX_PATH];
	MODEM_FAMILY	eModemType;
} MODEMINFO, *PMODEMINFO;

/* Subcodes of CSDIAG_CALL_SETUP_RES. */
/* Also Unimodem Diagnostic defines. */
 
#define UD_CallSetupInfo_None						0
#define UD_CallSetupInfo_NoDialTone					1
#define UD_CallSetupInfo_FastBusy					2
#define UD_CallSetupInfo_Busy						3
#define UD_CallSetupInfo_UnknownSignal				4
#define UD_CallSetupInfo_Voice						5
#define UD_CallSetupInfo_DataAns					7
#define UD_CallSetupInfo_DataCall					8
#define UD_CallSetupInfo_FaxAns						9
#define UD_CallSetupInfo_FaxCall					0xA
#define UD_CallSetupInfo_V8bis						0xB
 
/* Subcodes of CSDIAG_TERMINATION_CAUSE. */
/* Also Unimodem Diagnostic defines. */
#define UD_CallTermination_CauseUndefined			0x0
#define UD_CallTermination_NoPreviousCall			0x1
#define UD_CallTermination_CallInProgress			0x2
#define UD_CallTermination_CallWaiting				0x3
#define UD_CallTermination_Delayed					0x4
#define UD_CallTermination_InacTimerExpired			0x19
#define UD_CallTermination_DTRDrop					0x20
#define UD_CallTermination_BlackListed				0x29
#define UD_CallTermination_ExtOffhook				0x2B
#define UD_CallTermination_S7Expired				0x2C
#define UD_CallTermination_LoopCurrentInterrupted	0x2E
#define UD_CallTermination_NoDialTone				0x2F
#define UD_CallTermination_Voice					0x30
#define UD_CallTermination_ReorderTone				0x31
#define UD_CallTermination_CarrierLost				0x3C
#define UD_CallTermination_TrainingFailed			0x3D
#define UD_CallTermination_NoModinCommon			0x3E
#define UD_CallTermination_RetrainedFail			0x3F
#define UD_CallTermination_GSTNCleardown			0x41
#define UD_CallTermination_FaxDetected				0x42
#define UD_CallTermination_AnykeyAbort				0x50
#define UD_CallTermination_ATH						0x51
#define UD_CallTermination_ATZ						0x52
#define UD_CallTermination_FrameReject				0x5A
#define UD_CallTermination_NoECEstablished			0x5B
#define UD_CallTermination_ProtViolation			0x5C
#define UD_CallTermination_n400Exceeded				0x5D
#define UD_CallTermination_NegotiatFailed			0x5E
#define UD_CallTermination_DiscontFrame				0x5F
#define UD_CallTermination_SabmeFrame				0x60
#define UD_CallTermination_LostSync					0x64
#define UD_CallTermination_DLG						0x65
#define UD_CallTermination_LineInUse                0x66
#define UD_CallTermination_MOHTimeout	            0x67
#define UD_CallTermination_PulseAbort				0x68
#define UD_CallTermination_LineMissing              0x69
#define UD_CallTermination_MOHDisconnect   		    0x6A
#define UD_CallTermination_SmsOK   					0x70
#define UD_CallTermination_SmsFailed   				0x71
#define UD_CallTermination_SmsBadCRC	   			0x72
#define UD_CallTermination_SmsBadCommand			0x73
#define UD_CallTermination_SmsCasTimeoutExpired		0x74
#define UD_CallTermination_SmsDataError				0x75
#define UD_CallTermination_SmsNoUserSelection		0x76


 
/* Common subcodes of CSDIAG_TX_NEG_RES and CSDIAG_RX_NEG_RES. */
/* Also Unimodem Diagnostic defines. */
#define UD_MOD_V17									0x0
#define UD_MOD_V21									0x1
#define UD_MOD_V22									0x2
#define UD_MOD_V22bis								0x3
#define UD_MOD_V23CC								0x4
#define UD_MOD_V23SC								0x5
#define UD_MOD_V27ter								0x8
#define UD_MOD_V29HD								0x9
#define UD_MOD_V32									0xA
#define UD_MOD_V32bis								0xB
#define UD_MOD_V34									0xC
#define UD_MOD_V90									0xE
#define UD_MOD_V92									0xf /* Corrsponds to V90 issue 2, symetric in at#ud standard */
#define UD_MOD_K56									0x81
#define UD_MOD_B212A								0x84
#define UD_MOD_B103									0x85
#define UD_MOD_Unknown								0xFF
 
/* Subcodes of CSDIAG_PROTOCOL_NEG_RES. */
/* Also Unimodem Diagnostic defines.    */
#define UD_Protocol_NONE							0
#define UD_Protocol_LAPM							1
#define UD_Protocol_ALT								2
 
/* Subcodes of CSDIAG_CMPRS_NEG_RES. */
/* Also Unimodem Diagnostic defines. */
#define UD_Compression_NONE							0
#define UD_Compression_V42BIS						1
#define UD_Compression_ALT							0x80
#define UD_Compression_V44							2
 
/* Subcodes of CSDIAG_MULTI_MEDIA_MODE. */
/* Also Unimodem Diagnostic defines.    */
#define UD_MultimediaMode_DataOnly					0x0
#define UD_MultimediaMode_FaxOnly					0x1
#define UD_MultimediaMode_VoiceOnly					0x2
#define UD_MultimediaMode_VoiceView					0x3
#define UD_MultimediaMode_DSVD						0x8
#define UD_MultimediaMode_H324						0x9
#define UD_MultimediaMode_OtherV80					0xA
 
#define UD_DteDceModes_AsyncData					0x0
#define UD_DteDceModes_V80TransSync					0x1
#define UD_DteDceModes_V80FramedSync				0x2

typedef enum
{
	LAST_DIAL_TYPE_TONE = 0,
	LAST_DIAL_TYPE_PULSE = 1,
}LAST_DIAL_TYPE;
 
/* DataPump type codes */
typedef enum 
{
	RKID_V32BIS = 0,
	RKID_V34,
	RKID_V22BIS,
	RKID_V23,
	RKID_V21,

	RKID_V17,
	RKID_V29,
	RKID_V27,

	RKID_V8,

	RKID_TONE_DET,
	RKID_TONE_GEN,
	RKID_DTMF_DET,
	RKID_DTMF_GEN,
	RKID_CR_TONE_DET,
	RKID_CR_TONE_GEN,

	RKID_RKSAMPLE,
	RKID_ANS_DET,
	RKID_ANS_GEN,
	RKID_WINAC,
	RKID_ROKV42,

	RKID_K56FLEX,
	RKID_BELL103,
	RKID_BELL212A,
	RKID_SPKP,
	RKID_VOICE,

	RKID_V90,

	RKID_AMOS,

	RKID_SDK,

	RKID_TEST,

	RKID_V92,

	RKID_LAST,
} RK_DP_IDS; 

#define RKID_V90_FAST   	RKID_V92

typedef enum
{
	CW_SET_WAIT_FOR_EXTERNAL_DECISION,
	CW_SET_HANG_UP,
	CW_SET_IGNORE,
} CW_SET_T;

/* SINGLETONEDETECT - contains parameters for single tone detector.            */
/* WARNING: when adding tones to this list, make sure you add equivalent       */
/* entries in configcodes.h to the CFGMGR_CODE enum (SINGLETONEDETECT section),*/
/* as the TONE_DET_TYPE_TO_TONE_DET_CONFIG_CODE() assumes 1-1 mapping          */

typedef enum {
	TONE_PRIMARY,				/* 0x0 */
	TONE_ALTERNATE,
	TONE_ATBEL,
	TONE_ATV25,
	TONE_1100,
	TONE_2130,
	TONE_2312,
	TONE_2750,
	TONE_2912,
	VOICETONEACALLPROG,
	VOICETONEB_1100,
	VOICETONEC_1300,
	VOICETONEC_2100,
	VOICETONEB_2225,
	V8BTONE_2100,				/* 0xE */
	V8BTONE_2225,				/* 0xF */
    V8BTONEC_2100, 
	V8BTONEB_2225,
	CI_CENTER_TONEB,
	CI_SIDE_TONEC,
	VOICEVIEW_TONEC_2912,
	VOICEVIEW_TONEB_2312,
	VOICEVIEW_TONEB_2750,
	VOICEVIEW_TONEA_2130,
	VOICEVIEW_TONEA_NOT_ST,
	TONEC_1650,
	CReSEG1_2002,
    TONE_1270,
	TONEB_980,    
	TONE_CAS,
	TONE_SAS,
	TONE_DTMF,
} eTONEDETECT_TYPE;
 
/* SINGLETONEDETECT - contains parameters for single tone generator            */
/* WARNING: when adding tones to this list, make sure you add equivalent       */
/* entries in configcodes.h to the CFGMGR_CODE enum (SINGLETONEDETECT section),*/
/* as the TONE_GEN_TYPE2TONE_GEN_CONFIG_CODE() assumes 1-1 mapping			   */
typedef enum {
	TONE_GEN_1650,
	TONE_GEN_V25_CALL,
	TONE_GEN_CNG,
	TONE_GEN_CED,
} eTONEGEN_TYPE;

typedef struct 
{
	eTONEDETECT_TYPE ToneType;
	DWORD			 TimeOut; /* Milisecs */
}TONE_DETECTOR_TEST_T;
 
 
typedef struct
{
	WORD	freq1; /* Hz */
	WORD	freq2;
	WORD	amplitude1;
	WORD	amplitude2;
	DWORD	OnTime;
	DWORD	OffTime;
    BOOL	bRepeatedTone;
} TONEGEN, *PTONEGEN;
 
typedef struct
{
	TONEGEN  ToneGenParams;
	BOOL     StartGenerating; /* TRUE to START, FALSE to STOP */
}TONE_GENERATOR_TEST_T;
 
 
typedef enum
{ 
	DPAL_DIGIT_0, 
	DPAL_DIGIT_1, 
	DPAL_DIGIT_2, 
	DPAL_DIGIT_3, 
	DPAL_DIGIT_4,
	DPAL_DIGIT_5, 
	DPAL_DIGIT_6, 
	DPAL_DIGIT_7, 
	DPAL_DIGIT_8, 
	DPAL_DIGIT_9,
	DPAL_DIGIT_STAR, 
	DPAL_DIGIT_POUND, 
	DPAL_DIGIT_A, 
	DPAL_DIGIT_B,
	DPAL_DIGIT_C, 
	DPAL_DIGIT_D
} DPDIALDIGITS;
 
typedef struct 
{
	BOOL		 result;
	DPDIALDIGITS dtmf;
}DIAG_DTMF_DETECTOR_RESULT;
 
typedef struct 
{
	BOOL		     result;
	eTONEDETECT_TYPE tone;
}DIAG_SINGLE_TONE_DETECTOR_RESULT;
 
 
typedef enum
{
	B2_1,
	B2_15,
	CHARLEMANGE_1,
	CHARLEMANGE_2,
	CHURCHILL_STD,
	CHURCHILL_SMART,
    WELIINGTON = 10,
    RIESLING=20,
    LAGUNA=30,
    MALIBU,
    SPARTACUS=40,
    WAIKIKI,
    VENICE=50,
    ZION,
    TETON=60,
	LASSEN,
	/* ... */
}DIAG_ASIC_TYPE;

typedef enum
{
    SRI_OLD_DISCRETE = 0,
    SRI_HOMER,
    SRI_HAL,
    SRI_NEW_DISCRETE,
    SRI_DAA_LATEST
}DIAG_SOFT_RING_INDEX;
 
typedef enum
{
	POWER_TYPE_S0,
	POWER_TYPE_S3,
/*	... */
}DIAG_POWER_TYPE;
 
typedef struct 
{
	DWORD DeviceID;
	DWORD VendorID;
	DWORD SubDeviceID;
	DWORD SubVendorID;
	DWORD Revision;
	DWORD Version;
}DIAG_EEPROM_PNP_T;
 
typedef struct
{
	DWORD StartAddress;
	DWORD Length; /* in bytes (not supported!) */
	BOOL  AddressType; /* 0 - memory, 1-IO */
}DIAG_ADDRESS_SPACE_INFO_T;
 
typedef struct
{
	DIAG_ADDRESS_SPACE_INFO_T AddressSpace [6];
	DWORD Irq;
}DIAG_BUS_IF_T;
 
typedef struct
{
	DWORD PinsMask;
	DWORD ValuesMask;
}DIAG_GET_SET_GPIO_T;
 
typedef enum {
	MODEM_NEAR_END_EC,		/* Echo canceller  */
	MODEM_FAR_END_EC,		/* Echo canceller  */
	MODEM_EQUALIZER,		/* Equalizer       */
	MODEM_DFE,				/* Decision Feedback Equalizer */
	MODEM_NOISE_PREDICTOR	/* Noise Predictor */
} MODEM_FILTER_T;

#define CONFIGURATIONS_MASK   DWORD
#define MAX_TEST_STREAM_SIZE  255

typedef enum {
	CW_NOP,		/* No operation (Param is ignored)			*/
	CW_FLASH,	/* Param is the time between on and offhook */
	CW_SLEEP,	/* Param is the delay in ms				    */
	CW_DTMF,	/* Param is the DTMF						*/
} CALL_WAITING_ACTIONS;

typedef struct CntryCallWaitingAction_Tag {
	CALL_WAITING_ACTIONS	Action;
	DWORD					Param;
} CntryCallWaitingAction;

typedef enum
{
	SERVER_TYPE_NONE,
	SERVER_TYPE_V92,
	SERVER_TYPE_V90,
	SERVER_TYPE_V34,
	SERVER_TYPE_OTHER,
}SERVER_TYPE_INFO;
 

#define CID_NONE_TYPE            0
#define CID_USA_TYPE             1
#define CID_JAPAN_TYPE           2
#define CID_JAPAN_UNIMDM_TYPE    3
#define CID_FRENCH_TYPE          4  /*[LKS10aug98]: Add French type */
#define CID_UK_TYPE              5  /*[LKS10aug98]: Add UK type */
#define CID_DTMF_TYPE			 6	/* DTMF type only */

#define CID_DTMF_SUPPORT         0x80  /*[LKS10aug98]: Add DTMF type */

#define MOH_MAX_SWITCH_COMMANDS		4

#if (USE_PRAGMA_PACK == 1)
#pragma pack(pop)   /* Packing compatible with HSF VXDs */
#endif

#endif /* __CODESDEPS_H__ */
