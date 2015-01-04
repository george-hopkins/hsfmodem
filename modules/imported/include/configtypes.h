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
*	$Header:   R:/pvcs68/vm/common/Cyprus Project/archives/Octopus/include/ConfigTypes.h_v   1.26   24 Dec 2003 18:30:12   shay  $
*
*****************************************************************************************/


/****************************************************************************************

File Name:			ConfigTypes.h

File Description:	Configuration parameter data structures.

*****************************************************************************************/


#ifndef __CONFIGTYPES_H__
#define __CONFIGTYPES_H__

#include "configcodes.h"
#include "codesdeps.h"
#include "comtypes.h"

#include "targetconfigtypes.h"

typedef struct tagDTE_USER_CONFIG
{
    UINT8       bQuiet;     /* DTE result codes disabled*/
    UINT8       bVerbose;   /* Long result form*/
    UINT8       bLevel;     /* Result codes level */
    UINT8       bConnect;   /* Connect Message Form*/

} DTE_USER_CONFIG, *PDTE_USER_CONFIG;

typedef struct tagMS_USER_CONFIG
{
   MODULATION_TYPE	eModulation;
   BOOL				bAutomode;
   UINT32			dwMinTxSpeed;
   UINT32			dwMaxTxSpeed;
   UINT32			dwMinRxSpeed;
   UINT32			dwMaxRxSpeed;

} MS_USER_CONFIG, *PMS_USER_CONFIG;

typedef struct tagDS_USER_CONFIG 
{
	UINT32	dwCompressDict;
	UINT32	dwCompressStr;
	UINT32	dwCompressDir;
	UINT32	dwCompressNeg;
} DS_USER_CONFIG, *PDS_USER_CONFIG;

#if !defined NO_V44_SUPPORT
typedef struct DS44InfoStructTag
{
	UINT32 dwV44CompressDir;
	UINT32 dwV44CompressNeg;
	UINT32 dwV44Capability;
	UINT32 dwV44TxDict;
	UINT32 dwV44RxDict;
	UINT32 dwV44TxStr;
	UINT32 dwV44RxStr;
	UINT32 dwV44TxHist;
	UINT32 dwV44RxHist;
} DS44InfoStruct;
#endif

typedef struct tagEC_USER_CONFIG
{
   UINT32   dwOrgReq;
   UINT32   dwOrgFbk;
   UINT32   dwAnsFbk;
   BOOL     bV42SkipToXID;
} EC_USER_CONFIG, *PEC_USER_CONFIG;

/*#$YS$ Added for Break handling */
typedef struct tagBRK_USER_CONFIG
{
   UINT32   dwBreakSel;
   UINT32   dwTimed;
   UINT32   dwDefaultLen;
} BRK_USER_CONFIG, *PBRK_USER_CONFIG;


/*YS@001130 */
typedef struct tagITF_USER_CONFIG
{
   UINT32   dwXoffThresh;
   UINT32   dwXonThresh;
   UINT32   dwRptPeriod;
} ITF_USER_CONFIG, *PITF_USER_CONFIG;

/*#$YS$ */
/* CFGMGR_VDR_PARAMS */
typedef struct tagVDRParams 
{
	UINT32  dwRingReportEnable;
    UINT32  dwRingReportType;
} VDR_PARAMS, *PVDR_PARAMS;


typedef struct
{
	MODULATION_TYPE Modulation;
	UINT32 TxBitRate, RxBitRate;
} CARRIER_REPORT;

#define QC_PROFILE_RAMDATA_VALID		0x01
#define	QC_PROFILE_IAGAINS_VALID		0x10
#define	QC_PROFILE_WITH_PCMUPSTREAM		0x02

#define V92_PCMUPSTREAM_ENABLED			0x01
#define V92_PCMUPSTREAM_SYMBOLRATE_CHECKED	0x02
#define V92_PCMUPSTREAM_ACTIVE			0x10

typedef enum MOHControlStateTag
{
   MOH_CNTRL_IDLE,
   MOH_CNTRL_SEND_MHreq,
   MOH_CNTRL_GET_MHack,
   MOH_CNTRL_GET_MHnack,
   MOH_CNTRL_SEND_MHclrd,
   MOH_CNTRL_GET_MHcda,
   MOH_CNTRL_SEND_MHfrr,
   MOH_CNTRL_SEND_HOLD,
   MOH_CNTRL_SEND_V8clrd,
   MOH_CNTRL_SEND_V8qc
} MOH_CONTROL_STATE;

typedef enum MOHAppMsgTag
{
   MOH_APP_MSG_WAITING,
   MOH_APP_MSG_ACCEPT,
   MOH_APP_MSG_RESUME,
   MOH_APP_MSG_REJECT,
   MOH_APP_MSG_DISCONNECT,
   MOH_APP_MSG_OUT_GOING_CALL
} MOH_APP_MSG_STATE;

typedef struct ModemOnHoldInfoTag
{
   UINT32	dwAllowHoldTime;
   UINT32	dwHoldEnable;
   UINT8	cRemoteModemInfo;
/*   UINT8	cRawOnHoldTime; */
   BOOL		bServerOnHold;

   MOH_STATE eMxpertState;
} ModemOnHoldInfo;

#define	MOH_RECONFIG_V90		0x80
#define	MOH_OUTGOING_CALL		0x40
#define	MOH_LOCAL_DECISION_MASK	(0xFF & ~(MOH_RECONFIG_V90|MOH_OUTGOING_CALL))

/*JT PCM Upstream, do I need a whole structure? */

typedef struct tagPIG_USER_CONFIG
{
	int dwATCmdPIGValue;
   
} PIG_USER_CONFIG, *PPIG_USER_CONFIG;

typedef enum
{
	TONE,
#if !defined NO_PULSE_SUPPORT
	PULSE
#endif /* !defined NO_PULSE_SUPPORT */
} DIAL_MODE;

#pragma pack(1)
/*
typedef struct PACKED_ATTRIB
{
	UINT8	RingMaxFrequency,
			RingMinFrequency;
	UINT16	RingStable,
			MinTimeBeforeAnswering,
			MinDelayBtwnRings,
			MaxDelayBtwnRings,
			MinRingOnTime,
			MinImmediateRingOn,
			MaxImmediateRingOn;
} RING_PARAMS, *PRING_PARAMS;
*/

typedef struct tagRingInfo
{
	short			FilterDC;
	short			SwitchLimit;
	short			RingLimit;
} RING_PARAMS;

#define NUM_LIMITED_SREGS	10		/*Number of S-registers limited by entries in the INF file*/

typedef struct PACKED_ATTRIB
{
	UINT16	InterCallDelay1,		/*Delay when same number dialed after outgoing call*/
			InterCallDelay2,		/*Delay when different number dialed after outgoing call*/
			InterCallDelay3,		/*Delay after incoming call*/
			BillingDelay;			/*Delay between ATA and start of v8bis*/
	UINT16	uiDummy1,
			uiDummy2;
} CALL_PROGRESS_TIMING_CONFIG, *PCALL_PROGRESS_TIMING_CONFIG;

typedef struct PACKED_ATTRIB
{
	UINT16	DialToneStable,
			WDialToneStable,
			AnswerToneStable,
			CallProgressToneStable;
} TONE_DEBOUNCE_CONFIG, *PTONE_DEBOUNCE_CONFIG;

typedef struct  PACKED_ATTRIB
{
	UINT8 v8o,		/* 1 = enable +A8x indicators, 6 = disable +A8x (originate)*/
		  v8a,		/* 1 = enable +A8x indicators, 5 = disable +A8x (answer)*/
		  v8c,		/* value of V.8 CI signal call function*/
		  v8b,		/* 0 = disable V.8bis, 1 = DCE-controlled V.8bis, 2 = DTE-controlled V.8bis*/
          v8pf;     /* ???, but we need it*/
/*	PCHAR cfrange,	*/
/*		  protrange;*/
} V8BIS_OPERATION_CONTROL, *PV8BIS_OPERATION_CONTROL;

#pragma pack()

typedef enum 
{ 
	V8BIS_FAILED, 
	V8BIS_SUCCESSFUL, 
	V8BIS_MODE_NOT_SUPPORTED
} V8BIS_STATE;

typedef enum 
{
    V8BIS_OK,
    V8BIS_NO_CRe,
    V8BIS_CRe_DETECTOR_FAILED,
    V8BIS_CRd_GENERATOR_FAILED,
    V8BIS_NO_CRd,
    V8BIS_NO_CL,
    V8BIS_NO_MS,
    V8BIS_NO_ACK_NAK,
    V8BIS_NAK,
    V8BIS_NO_CTS,
    V8BIS_NOT_SUPPORTED,
    V8BIS_UNDEFINED
} V8BIS_FAILURE_REASON;

typedef struct
{
    UINT8	    NS_FlexVersion;                     /* NS - Flex Version Number*/
    UINT8	    NS_DSPVersion;                      /* NS - DSP Version Number*/
	UINT8	    bRemoteIsConexant;                  /* remote modem is Conexant*/
    UINT8	    bK56UseUlaw;                        /* use u-law in K56 negotiation*/
    UINT8	    bK56flexNegotiated;                 /* remote server supports K56*/
    UINT8	    bV90InNonStdFld;                    /* v.90 support is detected in NSF*/
    UINT8	    bV90InStdFld;                       /* v.90 support is detected in SF*/
    UINT8	    bV70_Enabled;
    V8BIS_STATE eState;
    V8BIS_FAILURE_REASON    eReason;
} V8BIS_RESULT, *PV8BIS_RESULT;

/* Structure passed with CFGMGR_ANSWER_TONE_DETECTOR/CFGMGR_BELL_TONE_DETECTOR */
typedef struct tagAnswerToneParams {
    UINT16  m_wCenterFrequency;
    UINT16  m_wBandwidth;
    short   m_nAmplitude;			/* normed to 32768*/
} ANSWER_TONE_PARAMS, *PANSWER_TONE_PARAMS;



/* Unimodem Diagnostic defines*/
typedef enum
{
	UD_DISPLAY_PREVIOUS,
	UD_DISPLAY_NEW,
	UD_DISPLAY_NONE
} UD_DISPLAY;

typedef enum
{
	CauseUndefined			= UD_CallTermination_CauseUndefined,
	NoPreviousCall			= UD_CallTermination_NoPreviousCall,
	CallInProgress			= UD_CallTermination_CallInProgress,		
	CallWaiting				= UD_CallTermination_CallWaiting,			
	Delayed					= UD_CallTermination_Delayed,
	InacTimerExpired		= UD_CallTermination_InacTimerExpired,
	DTRDrop					= UD_CallTermination_DTRDrop,
	BlackListed				= UD_CallTermination_BlackListed,
	ExtOffhook				= UD_CallTermination_ExtOffhook,
	S7Expired				= UD_CallTermination_S7Expired,
	LoopCurrentInterrupted	= UD_CallTermination_LoopCurrentInterrupted,
	NoDialTone				= UD_CallTermination_NoDialTone,
	Voice					= UD_CallTermination_Voice,
	ReorderTone				= UD_CallTermination_ReorderTone,
	CarrierLost				= UD_CallTermination_CarrierLost,
	TrainingFailed			= UD_CallTermination_TrainingFailed,
	NoModinCommon			= UD_CallTermination_NoModinCommon,
	RetrainFailed			= UD_CallTermination_RetrainedFail,
	GSTNCleardown			= UD_CallTermination_GSTNCleardown,
	FaxDetected				= UD_CallTermination_FaxDetected,
	AnykeyAbort				= UD_CallTermination_AnykeyAbort,
	ATH						= UD_CallTermination_ATH,
	ATZ						= UD_CallTermination_ATZ,
	FrameReject				= UD_CallTermination_FrameReject,
	NoECEstablished			= UD_CallTermination_NoECEstablished,
	ProtViolation			= UD_CallTermination_ProtViolation,
	n400Exceeded			= UD_CallTermination_n400Exceeded,
	NegotiatFailed			= UD_CallTermination_NegotiatFailed,
	DiscontFrame			= UD_CallTermination_DiscontFrame,
	SabmeFrame				= UD_CallTermination_SabmeFrame,
	LostSync				= UD_CallTermination_LostSync,
	DLG						= UD_CallTermination_DLG,
	MOHTimeout				= UD_CallTermination_MOHTimeout,
	PulseAbort				= UD_CallTermination_PulseAbort,
	LineMissing				= UD_CallTermination_LineMissing,
    OverVoltage
} CALL_TERMINATION_CAUSE;

/*****************************************************************
OEM & COUNTRY specific parameters, should be loaded from NVRAM
BEGIN
*****************************************************************/
/* COUNTRY PARAMETRERS*/



#define MAX_INTERVALS				4
#define MAX_OEM_STR_LEN				80
/* Number of S-registers limited by entries in the INF file*/
#define NUM_LIMITED_SREGS			10
/* !!!NOTE: _LAST_RELAY_ENTRY == DEVMGR_LAST_RELAY_ENTRY*/

typedef enum {
	DEVMGR_GPIO_RELAY_MASK,
	DEVMGR_GPIO_DEFAULT_STATE,
	DEVMGR_OFFHOOK_PHONETOLINE,
	DEVMGR_OFFHOOK_PHONEOFFLINE,
	DEVMGR_ONHOOK_PHONETOLINE_CALLID,
	DEVMGR_ONHOOK_PHONETOLINE_NOCALLID,
	DEVMGR_ONHOOK_PHONEOFFLINE_CALLID,
	DEVMGR_ONHOOK_PHONEOFFLINE_NOCALLID,
	DEVMGR_OFFHOOK_PULSE_MAKE,
	DEVMGR_OFFHOOK_PULSE_BREAK,
	DEVMGR_OFFHOOK_PULSESETUP,
	DEVMGR_OFFHOOK_PULSECLEAR,
	DEVMGR_LAST_RELAY_ENTRY
} DEVMGR_DAA_RELAY_CODE;


/* Transmit Level*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagTxlevel {
	UINT8	TxDataLevelMin;
	UINT8	TxDataLevelMax;
	UINT8	TxDataLevelDefault;
	UINT8	TxFaxLevelMin;
	UINT8	TxFaxLevelMax;
	UINT8	TxFaxLevelDefault;
	UINT8	TxVoiceLevelMin;
	UINT8	TxVoiceLevelMax;
	UINT8	TxVoiceLevelDefault;
	char	TxLvlAdj;							/* can be + or - */
	UINT8	LowDialLevel;
	UINT8	HighDialLevel;
	UINT8	MaxVTSLineSingleToneLevel;
} CntryTxlevelStructure;
#pragma pack()

/* Pulse Dial Parameters*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagPulse {
 
#if ( PRJ_FLAVOUR == PRJFLVR_CYPRUS )
/*
		a bug in gnu assembler version 2.9.5 will give a bad value for
		the field below (PulseInterd). This forces alignment of 4 and 
		workaround the bug. It should be removed if future versions of
		GAS fix this (Ahi).
*/
	UINT8	Padding;
#endif
 
	UINT8	PulseDialMode;
	UINT8	PulseMapAmperP0;					/* Maps the &P values*/
	UINT8	PulseMapAmperP1;
	UINT8	PulseMapAmperP2;
	UINT8	PulseMapAmperP3;
	char	PulseMakeOffset;					/* can be +/- */
	char	PulseBreakOffset;					/* can be +/- */
	UINT8	PulseSetupTime;
	UINT8	PulseClearTime;
	UINT16	PulseInterd;
	UINT16	LineMuteDelay;						/*VRO: Line mute delay*/
} CntryPulseStructure;
#pragma pack()

/* Ring Detection Parameters*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagRing {
	UINT8	RingMaxFrequency;
	UINT8	RingMinFrequency;
	UINT16	RingStable;
	UINT16	MinTimeBeforeAnswering;
	UINT16	MinDelayBtwnRings;
	UINT16	MaxDelayBtwnRings;
    UINT16	MinRingOnTime;
    UINT16	MinImmediateRingOn;
    UINT16	MaxImmediateRingOn;
	UINT8	P85RingWakePulseWidth;
} CntryRingStructure;
#pragma pack()

/* DTMF Dial Parameters*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagDTMF {
	UINT16	DTMFOnTime;
	UINT16	DTMFInterdigit;   
} CntryDTMFStructure;
#pragma pack()

#pragma pack(1)
typedef struct PACKED_ATTRIB FilterParamTAG {
	UINT16	FilterType;
	UINT16	Biquad1[5];
	UINT16	Biquad2[5];    
	UINT16	LpFBK;
	UINT16	LpGain;
	UINT16	ThreshU;
	UINT16	ThreshL;
	UINT16	Biquad1_PreF[5];
	UINT16	Biquad2_PreF[5];
	BOOL	SqDisState;
} FilterParam, *ptrFilterParam;
#pragma pack()

/* Filter Parameters*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagFilter {
	FilterParam		Primary;
	FilterParam		Alternate;
	FilterParam		VoiceToneACallProgress;
} CntryFilterStructure;
#pragma pack()

/* RLSD Threshold Adjustment*/
typedef struct tagRLSD {
	UINT32		XrthOffset;				/* can be + or -*/
} CntryRLSDStructure;

/* Tone Detection Parameters*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagTone {
	UINT16		DialStable;
	UINT16		WDialStable; 
	UINT16		AnswerStable;
	UINT16		ProgressStable;
	UINT8		DialtoneWaitTime;
    UINT16		PolarityReversalIgnoreTime;
} CntryToneStructure;
#pragma pack()

#pragma pack(1)
typedef struct PACKED_ATTRIB tagCadPrm {
	struct {
		UINT32	lMin;
		UINT32	lMax;
	} Interval[MAX_INTERVALS];
	UINT32		lNumIntervalsNeeded;
} CadPrm;
#pragma pack()

/* TONE CADENCE*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagCadence {
	UINT8		ToneCadence_NumberOfPhases;
	CadPrm		DialtoneParams;
//new        CadPrm		WDialtoneParams;
	CadPrm		BusyParams;
	CadPrm		RingbackParams;
	CadPrm		CongestionParams;
} CntryCadenceStructure;
#pragma pack()

/* Blacklisting FLAGS*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagBL_Flags {
	char		fPermBlst: 1;
	char		fResetOnSuccess: 1;
	char		fDistictFailures: 1;
	char		fFlag0Spare3: 1;
	char		fFlag0Spare4: 1;
	char		fFlag0Spare5: 1;
	char		fFlag0Spare6: 1;
	char		fFlag0Spare7: 1;
} BLFlagStructure;
#pragma pack()

/* Blacklisting Parameters*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagBlacklisting {
	UINT8			BlstAction_NoDialTone;
	UINT8			BlstAction_KeyAbort;
	UINT8			BlstAction_Busy;
	UINT8			BlstAction_NoConnection;
	UINT8			BlstAction_NoAnswer;
	UINT8			BlstAction_VoiceAnswer;
	UINT16			BlackListingDelay1;			/* InterCall Delay upon Failure when redialing same number */
	UINT16			BlackListingDelay2;			/* InterCall Delay upon Failure when redialing different number */
	UINT16			BlackListingDelay3;			/* Method 2 Delay */
	UINT16			BlackListingDelay4;			/* Method 3 Delay */ 
	UINT16			BlackListingCount1;			/* Method 2 # of Failures */
	UINT16			BlackListingCount2;			/* Method 3 # of Failures */
	BLFlagStructure	BlstFlags;
} CntryBlacklistingStructure;
#pragma pack()

#define VRID_INFO_SIZE	128

typedef enum {
            NO_CALLER_ID,
            STORED_CALLER_ID,
            VCID_CALLER_ID,
            VRID_NEW_CALLER_ID
            } CALLER_ID_INFO;

/* CallerID Parameter*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagCallerID {
	UINT8		Cid_TYPE;
	UINT8		Cid_Prefix;
	UINT8		Cid_Suffix;
	UINT16		Cid_WaitTime;
	UINT8		PreRing_Cid_Indicator;
} CntryCallerIDStructure;
#pragma pack()

#pragma pack(1)
typedef struct PACKED_ATTRIB
{
  /* Flag0*/
  int fAltDialToneFilter: 1;
  int fAltCallProgressFilter: 1;
  int fAltWToneFilter: 1;
  int fAltWAfterIntCodeFilter: 1;
  int fFlag0Spare4: 1;
  int fFlag0Spare5: 1;
  int fFlag0Spare6: 1;
  int fFlag0Spare7: 1;

  /* Flag1*/
  int fEnforceDialTone: 1;
  int fEnforceCallingToneorCI: 1;
  int fEnforceGuardTone: 1;
  int fEnforceBusyTone: 1;
  int fUseS7whenW: 1;
  int fHangupOnCallWaiting: 1;
  int fFlag1Spare6: 1;
  int fFlag1Spare7: 1;

  /* Flag2*/
  int fDisableATH1: 1;
  int fDisablePulseAfterTone: 1;
  int fAllowPulseDialing: 1;
  int fAllowDTMFabcd: 1;
  int fEnableBlacklisting: 1;
  int fLongToneIsBusy: 1;
  int fSelectUlaw: 1;
  int fNoBellMode: 1;

  /* Flag3*/
  int fSwissComma: 1;		/* Swiss approval do not want first comma in dial string*/
  int fDtmfCompensation: 1; /* Use the DAA compesation table for DTMF Level.*/
  int fFlag3Spare2: 1;
  int fFlag3Spare3: 1;
  int fFlag3Spare4: 1;
  int fFlag3Spare5: 1;
  int fFlag3Spare6: 1;
  int fFlag3Spare7: 1;
} CALL_PROGRESS_FLAGS, *PCALL_PROGRESS_FLAGS;
#pragma pack()

/* Agress speed Parameter*/
#pragma pack(1)
typedef struct PACKED_ATTRIB tagAgressSpeedIndex {
	UINT8	cV90AgressSpeedIndex;
	UINT8	cK56AgressSpeedIndex;
	UINT8	cV34AgressSpeedIndex;
} CntryAgressSpeedIndexStructure;
#pragma pack()

/*******************************************************************/
/*******************************************************************/
/* CallerID type 2 Parameters */
typedef enum {
	CAS_NOP,
	CAS_SEND_DTMF,
	CAS_SEND_DUAL_TONE,
	CAS_RECEIVE_DTMF,
	CAS_RECEIVE_DUAL_TONE,
	CAS_SLEEP,
} CAS_ACTION, *PCAS_ACTION;

#pragma pack(1)
typedef struct PACKED_ATTRIB CAS_Action_Tag {
	CAS_ACTION		Action;
	UINT32			Tone;
	UINT32			Duration;
} CntryCASAction;
#pragma pack()

#define MAX_CAS_ACTIONS		4

#pragma pack(1)
typedef struct PACKED_ATTRIB CallerID2_Tag {
  UINT8				Cid_TYPE;
  UINT32			CASDetectionTime; /* time given for cas detection */
  CntryCASAction	CASAction[MAX_CAS_ACTIONS];
} CntryCallerID2Structure;
#pragma pack()

/* Call waiting parameters */

#define CW_ENABLE_SIGNAL_DTE_ON_DETECTED     CW_SET_WAIT_FOR_EXTERNAL_DECISION
#define CW_ENABLE_HANGUP_ON_DETECTED         CW_SET_HANG_UP
#define CW_DISABLE_FEATURE                   CW_SET_IGNORE

#pragma pack(1)
typedef struct PACKED_ATTRIB CW_HWToneDetectorParams_tag
{
   UINT16 wToneFrequency;
   UINT8 cToneCounterTolerance;
   UINT8 cToneCheckCounterPeriod;
   UINT16 wToneSampleDuration;
} CW_HWToneDetectorParams;
#pragma pack()

#pragma pack(1)
/*
typedef struct PACKED_ATTRIB CntryCallWaitingHwToneDetector_Tag
{
   CW_HWToneDetectorParams SAS_DetectorSettings;
   CW_HWToneDetectorParams CAS_DetectorSettings;

   UINT16 wCallWaitingTimeOut;
   UINT16 wCW_AGCMAXG;
   UINT16 wCW_AGCSLR;
   UINT16 wCW_AGCREF;
   UINT16 wCW_Alpha2Coefficient;
   UINT16 wCW_Beta2Coefficient;
   UINT16 wCW_Alpha1Coefficient;
   UINT16 wCW_UpdateRate;
   UINT16 wCW_DetectThreshold;
} CntryCallWaitingHwToneDetector;
*/
#pragma pack()

#define MOH_MAX_SWITCH_COMMANDS		4

#pragma pack(1)
typedef struct PACKED_ATTRIB CntryCallWaitingParams_Tag {
	UINT8							ValidFlag;	/* if set to 0 CW isn't supported */
	UINT32							CycleTime;	/* in ms */
	CntryCallWaitingToneDetector	CW_ToneDetector;
//    SINGLETONEDETECT		        ToneAS;
	CntryCallWaitingAction			AcceptAction[MOH_MAX_SWITCH_COMMANDS];
	CntryCallWaitingAction			ResumeAction[MOH_MAX_SWITCH_COMMANDS];
	CntryCallWaitingAction			RejectAction[MOH_MAX_SWITCH_COMMANDS];
} CntryCallWaitingParams;

/*******************************************************************/
/*******************************************************************/

typedef struct PACKED_ATTRIB CallWaitingInfoTag
{
	UINT32	dwCWEnable;
	BOOL		bCWDetected;
	BOOL		bCWCIDCollected;
	CW_HWToneDetectorParams *pCWToneParams;
	BOOL		bLookingForCAS;
	BOOL		bInCWCID;
} CallWaitingInfo;

typedef struct PACKED_ATTRIB CntryV92Control_Tag {
	UINT32	UpstreamEnable : 1;
	UINT32	Fill1		   : 15;
	UINT32	MohDetectCw    : 1;
	UINT32	OgcEnable	   : 1;
	UINT32	Fill2		   : 14;
} CntryV92Control;

typedef struct CntryOGCParams_Tag
{
	CntryCallWaitingAction	OgcInitAction[MOH_MAX_SWITCH_COMMANDS];
	CntryCallWaitingAction	OgcDoneAction[MOH_MAX_SWITCH_COMMANDS];
}CntryOGCParams;
 
 
#if !defined NO_QC_SUPPORT
typedef struct PACKED_ATTRIB QuickConnectInfoTag
{
   BOOL bProfileStorageEnable;
   UINT8 cEraseProfile;
   UINT32 dwCW_MOH_QCPrintState;
/*   UINT32 dwATCmdQCValue; */
   UINT32 dwQCEnable;
   BOOL bSaveProfile;

   UINT16 wRxCurrState;
   UINT16 wRxPrevState;
   UINT16 wTxCurrState;
   UINT16 wTxPrevState;
   UINT8 cCurrConf;
   UINT8 cPrevConf;
   BOOL bStartWA;
   BOOL bDpConfigChangeAA;
   BOOL bSentQC2a;
   BOOL bGotQCA1d;
   BOOL bUseV8Normal;   
   BOOL bStartedTraining;

   /* Measurement, remove when done */
   UINT32 dwTotalTime;
   UINT32 dwStartTime;
   UINT32 dwStopTime;
   UINT32 dwV8QCSetupTimeStamp;
   UINT32 dwV8GotQC1dTimeStamp;
   UINT32 dwATCmdQCPSSValue;
   BOOL  bQCPSSEnable;
   UINT32 dwATCmdQCPSValue;
} QuickConnectInfo;
#endif

typedef struct PACKED_ATTRIB tagCtryPrmsStruct {
	UINT16							T35Code;							/* T.35 Country Identification*/
	char							cInter[MAX_OEM_STR_LEN];			/* Country Name*/
	char							cIntCode[MAX_OEM_STR_LEN];			/* Country International Access code*/
	CntryTxlevelStructure			Txlevel;							/* + Transmit Level*/
	DAA_RELAY_TYPE					Relays[DEVMGR_LAST_RELAY_ENTRY];	/* + Relay Control Parameters*/
	CntryPulseStructure				Pulse;								/* Pulse Dial Parameters*/
	CntryRingStructure				Ring;								/* + Ring Detection Parameters*/
	UINT8							SRegLimits[NUM_LIMITED_SREGS][4];	/* S Register Limiting*/
	CntryDTMFStructure				DTMF;								/* + DTMF Dial Parameters*/
	CntryFilterStructure			Filter;								/* + Filter Parameters*/
	CntryThresholdStructure			Threshold;							/* + Thresholds Parameters*/
	CntryRLSDStructure				RLSD;								/* + RLSD Threshold Adjustment*/
	CntryToneStructure				Tone;								/* Tone Detection Parameters*/
	CALL_PROGRESS_TIMING_CONFIG		Timing;								/* Timing Parameter*/
	CntryCadenceStructure			Cadence;							/* TONE CADENCE*/
	CntryBlacklistingStructure		Blacklisting;						/* Blacklisting Parameters*/
	CntryCallerIDStructure			CallerID;							/* CallerId Control*/
	CntryCallerID2Structure			CallerID2;
	CALL_PROGRESS_FLAGS				Flags;								/* + Flag Control*/
	CntryAgressSpeedIndexStructure	AgressSpeedIndex;					/* + Agress Speed Parameter*/
	CntryCallWaitingParams			CallWaitingParms;					/* Call Waiting */
	CntryV92Control					V92Control;                         /* */
	CntryOGCParams		            OgcParams;                          /* */
} CtryPrmsStruct;

/* OEM PARAMETERS*/

typedef struct PACKED_ATTRIB tagOEM_Flags {
	/* OEMFlag0*/
	int		fUseTIES: 1;
	int		fAnalogSpeaker: 1;
	int		fDataFaxRemoteTAM: 1;
	int		fDataFaxVoiceView: 1;
	int		fLCSPresent: 1;
	int		f3VoltIA: 1;
	int		fRemHangupPresent: 1;
	int		fSpkMuteHWPresent: 1;
	/* OEMFlag1*/
	int		fReadCountryFromSwitch: 1;
	int		fHeatSensitive: 1;				/* Heat sensitive board (notebook) */
	int		fPME_Enable: 1;					/* enable D3 sleep and arming PME*/
	int		fAlternateConnectMSG: 1;
	int		fCountryNoneSelect: 1;			/* Option whether to allow NONE in country select tab*/
	int		fSpkMuteHWPolarity: 1;
	int		fLocalSpkBoostTAM: 1;			/* ON = Set IA Spk Output to maximum level, OFF = pickup default value */
	int		fMicSpkControl: 1;				/* ON = allow +VGM, +VGS; OFF = not allow +VGM, +VGS & load DSVD_Default values*/
	/* OEMFlag2*/
	int		fNEC_ACPI: 1;					/* NEC special handling of APM/ACPI*/
	int		fDisForceLAPMWhenNoV8bis: 1;	/* Disable forcing LAPM when V.8bis fails in V.90*/
	int		fDosSupportDisabled: 1;			/* indicates if DOS support checkbox is toggled on/off (1=OFF)*/
	int		fDosCheckBoxSupport: 1;			/* indicates if DOS support checkbox is shaded/unshaded (0=Shaded)*/
	int		fCIDHandsetMode: 1;				/* ON = special local handset handling based on +VCID setting.*/
	int		fExtOffHookHWPresent: 1;		/* ON = hardware is able to detect extension off hook*/
	int		fLAN_Modem_Support: 1;			/* ON = customize for lan/modem*/
	int		fPortCloseConnected: 1;			/* ON = maintain modem connection on PortClose*/
	/* OEMFlag3*/
	int		fFullGPIO: 1;					/* ON=full control to GPIO with AT-GPIO command*/
	int		fD3CIDCapable: 1;				/* does DAA support d3 caller ID circuitry*/
	int		fLineInUsed: 1;					/* When 1 enable customized line in used detection before dialing*/
	int		fV42ThrottleDisable: 1;			/* When one, disable V.42 Rx throttle*/
	int		fxDSLCombo: 1;					/* xDSL modem only, 1 = combo board*/
	int		fV23AnswerDis: 1;
	int		fDialSkipS6: 1;
	int		fFxCallCadFlag: 1;
} OEMFlagStructure;

typedef struct PACKED_ATTRIB OEM_Flags2_Tag {
  /* FLAGS2 Flag0 */
  unsigned int DisableBO_V34Fax: 1;
  unsigned int DisableBO_VoiceMode: 1;
  unsigned int DisableBO_Speakerphone: 1;
  unsigned int fLTAMSpeakerVIASelect: 1;   
  unsigned int fEnableP95ContinousBestCurveFit: 1;
  unsigned int fMusicVIASelect: 1;
  unsigned int fPolarityReversalEnabled: 1;
  unsigned int fUseINPUT1ForDLG: 1;

  /* FLAGS2 Flag1 */
  unsigned int fRipTideHW: 1;
  unsigned int fP85WakeOnRINGWAKE: 1;
  unsigned int fSdaaMuteDtgSpkrInCallProg: 1;
  unsigned int fUseINPUT2ForDLG: 1;
  unsigned int fCWDpRamPresent: 1;
  unsigned int fMOHDpRamPresent: 1;
  unsigned int fQCDpRamPresent: 1;
  unsigned int fUseDigtalCallProg: 1;

  /* FLAGS2 Flag2 */
  unsigned int fV92Enable: 1;
  unsigned int fUsbDpSleepIdle: 1;
  unsigned int fPIGDpRamPresent: 1;
  unsigned int fFlag2Spare3: 1;
  unsigned int fFlag2Spare4: 1;
  unsigned int fFlag2Spare5: 1;
  unsigned int fFlag2Spare6: 1;
  unsigned int fFlag2Spare7: 1;

  /* FLAGS2 Flag3 */
  unsigned int fFlag3Spare0: 1;
  unsigned int fFlag3Spare1: 1;
  unsigned int fFlag3Spare2: 1;
  unsigned int fFlag3Spare3: 1;
  unsigned int fFlag3Spare4: 1;
  unsigned int fFlag3Spare5: 1;
  unsigned int fFlag3Spare6: 1;
  unsigned int fFlag3Spare7: 1;
  } OEMFlag2Structure;

#pragma pack(1)
typedef struct OEMCadence_Tag {
    UINT8       ToneCadence_NumberOfPhases;  
    CadPrm      FaxCallingParams;		
    UINT32      FaxCallingToneTimer;  
} OEMCadenceStructure;
#pragma pack()

#define MAX_SREG			42      /* Size of contiguous array sregs     */

#define MAX_OTHERCOMMANDS		14		/* TIA-602 cmds  + Miscellaneous */

#pragma pack(1)
typedef struct tagV25Ter_Struct
{
    /* V.25ter variables */
    UINT32 dwAutoDetect;        /*+IPR fixed DTE rate */
    UINT32 dwDTERateReport;     /*+ILRR DTE rate reporting */
    UINT32 dwModulationReport;  /*+MR modulation reporting */
    UINT32 dwSelReject;         /*+ESR selective reject */
    UINT32 dwFCSSelect;         /*+EFCS frame check sequence select */
    UINT32 dwECReport;          /*+ER error correction reporting */
    UINT32 dwCompressReport;    /*+DR data compression reporting */

    /*   BOOL  bStringParm;         // String parm for parsing */
    UINT8  cCFallowed;          /* V.25ter Annex A accepted call functions */
    UINT8  cV8pf;               /* V.25ter Annex A accepted pr */

    /*   UINT32 dwTempo; */

    /* +A8E parms    */
    UINT32 dwV8o;
    UINT32 dwV8a;
    UINT32 dwV8cf;
    UINT32 dwV8b;
    UINT8  szCfrange[20];
    UINT8  szProtrange[20];

    /* +DS parms */
    UINT32 dwCompressDir;
    UINT32 dwCompressNeg;
    UINT32 dwCompressDict;
    UINT32 dwCompressStr;

    /* +EB parms */
    UINT32 dwBreakSel;
    UINT32 dwBreakTimed;
    UINT32 dwBreakLen;

    /* +ES parms */
    UINT32 dwOrgReq;                   /* +ES parm 1 */
    UINT32 dwOrgFbk;                   /* +ES parm 2 */
    UINT32 dwAnsFbk;                   /* +ES parm 3 */


    /* +ESA parms */
    UINT32 dwV80TransIdle;             /* +ESA parm 1 -- Transparent sub-Mode Idle Control */
    UINT32 dwV80FramedIdle;            /* +ESA parm 2 -- Framed sub-Mode Idle Control (Flags or Marks) */
    UINT32 dwV80FramedUnOv;            /* +ESA parm 3 -- Framed sub-Mode Underrun/Overrun Control (Abort or Flag) */
    /*UINT32 dwV80HdAuto;                // +ESA parm 4 -- Half Duplex Carrier Control --- NOT IMPLEMENTED */
    UINT32 dwV80CRCType;               /* +ESA parm 5 -- CRC type (none, LAPM's 16-bit CRC, LAPM's 32-bit CRC) */
    UINT32 dwV80NRZIEn;                /* +ESA parm 6 -- NRZI Encoding Enable */
    UINT32 dwV80Syn1;                  /* +ESA parm 7 -- Sync char 1 (used on idle) */
    /*UINT32 dwV80Syn2;                  // +ESA parm 8 -- Sync char 2 (used on idle) --- NOT IMPLEMENTED */

    /* +ETBM parms */
    UINT32 dwPendingTD;
    UINT32 dwPendingRD;
    UINT32 dwPendingTimer;

    /* +FIT parms */
    UINT32 dwInactiveTmr;
    UINT32 dwInactiveAction;

    /* +ICF parms */
    UINT32 dwCharFormat;
    UINT32 dwCharParity;

    /* +IFC parms */
    UINT32 dwDCEByDTE;
    UINT32 dwDTEByDCE;

    /* +ITF parms */
    UINT32 dwV80XOffThreshold;         /* +ITF parm 1 -- Tx buffer threshold at which XOFF is generated */
    UINT32 dwV80XOnThreshold;          /* +ITF parm 2 -- Tx buffer threshold at which XON is generated */
    UINT32 dwV80RptInterval;           /* +ITF parm 3 -- Report # of bytes in TX buffer at defined  */

    /*+PCW, +PMH, and +PQC parms RLC */
    UINT8  cPCW;						/* Call Waiting Enable */
    UINT8  cPMH;						/* Modem on Hold Enable */
    UINT8  cPQC;						/* Quick Connect Enable */

    /* V.25ter/TIA-602 plus Misc.       // 0 1 2   3   4   5 6 7 8     9     10 11 12 13 */
    /*   UINT8    OtherCmds[14];              // E,Q,V,(P,T),&D,&C,L,M,X,(&T4,&T5),%E,&G,&P,&R */
    UINT8  OtherCmds[MAX_OTHERCOMMANDS];              /* E,Q,V,(P,T),&D,&C,L,M,X,(&T4,&T5),%E,&G,&P,&R */
									    /*JT increased for &A ,&K, &M,X, &Y,&B, &H,&I,&R, Y,Z,&F for USR */
											
} V25Ter_Struct;
#pragma pack()

typedef struct SoftMSParmsStructTAG {
	UINT32      dwMsMode;
	UINT32      dwMsAuto;
	UINT32      dwMsMinTxSp;
	UINT32      dwMsMaxTxSp;
	UINT32      dwMsMinRxSp;
	UINT32      dwMsMaxRxSp;
} SoftMSParmsStruct;

typedef struct MiscParms_Struct {
	UINT32      dwExtResponse;     /*W setting */
	UINT32      dwFaxAutoAnswer;   /*+FAE setting */
    BOOL        bV90Enabled;
    BOOL        bV34FEnabled;
} MiscParmsStruct;

typedef struct PACKED_ATTRIB tagOEM_Filter {
	FilterParam		Tone1100;
	FilterParam		V8bToneC2100;
	FilterParam		V8bToneB2225;
	FilterParam		VoiceToneB1100;
	FilterParam		VoiceToneC1300;
	FilterParam		VoiceToneC2100;
	FilterParam		VoiceToneB2225;
    FilterParam     ToneB_2002;
} OEMFilterStructure;

typedef struct PACKED_ATTRIB tagModulation {
	UINT16		Org;
	UINT16		Ans;
} Modulation;

typedef struct PACKED_ATTRIB tagOEM_Threshold {
	Modulation		V21;
	Modulation		V21fax;
	Modulation		V23;
	Modulation		V22;
	Modulation		V22b;
	Modulation		V32;
	Modulation		V32b;
	Modulation		V34;
	Modulation		K56;
	Modulation		V90;
	Modulation		V27;
	Modulation		V29;
	Modulation		V17;
	UINT16			bRx28800Thr;
} OEMThresholdStruct;

typedef struct PACKED_ATTRIB tagPROFILE_DATA
{
    UINT8       Echo;          /* ATE 	CFGMGR_DTE_ECHO*/
    UINT8       Volume;        /* ATL 	CFGMGR_SPEAKER_VOLUME*/
    UINT8       Speaker;       /* ATM 	CFGMGR_SPEAKER_CONTROL*/
    UINT8   	Pulse;         /* ATT/ATD 1-PULSE, 0-TONE 	CFGMGR_DIAL_MODE*/

    UINT8       Quiet;         /* ATQ	CFGMGR_DTE_CONFIG.bQuiet*/
    UINT8       Verbose;       /* ATV	DTE_USER_CONFIG.bVerbose*/
    UINT8       Level;         /* ATX	DTE_USER_CONFIG.bLevel*/
    UINT8       Connect;

    UINT8       AmperC;     /* CFGMGR_RLSD_BEHAVIOR*/
    UINT8       AmperD;     /* CFGMGR_DTR_BEHAVIOR*/

    UINT8       S0;         /* Sregisters CFGMGR_SREG + <number>*/
    UINT8       S1;
    UINT8       S2;
    UINT8       S3;
    UINT8       S4;
    UINT8       S5;
    UINT8       S6;
    UINT8       S7;
    UINT8       S8;
    UINT8       S10;
    UINT8       S11;
    UINT8       S12;
    UINT8       S16;
    UINT8       S18;
    UINT8       S29;

} PROFILE_DATA, *PPROFILE_DATA;

#if !defined NO_POUNDUG_SUPPORT
#define MAX_STATES  150

typedef struct tagDownloadFailure
{
    UINT8   bCode;
    UINT8   bDesiredCRC;
    UINT8   bActualCRC;
} DOWNLOAD_FAILURE, *PDOWNLOAD_FAILURE;

#define MAX_DOWNLOAD_FAILURE    10

typedef struct PACKED_ATTRIB tagPOUND_UD_DATA
{
    UD_DISPLAY          cUDDisplay;         /*Flag used to detemine if to display #UD/#UG   Done*/
	UINT8				cUDCallSetup;		/*0x1, Table 2                                  Done*/
	UINT8		        cUDMultMediaMode;   /*0x2, Table 3                                  Done*/
	UINT8			    cUDDTEDCEmode;      /*0x3, Table 4*/

	char    sUDV8CM[20];					/* 0x4 V8_CM                                    Done*/
	char    sUDV8JM[20];					/* 0x5 V8_JM                                    Done*/

	UINT8   cUDRXdb;						/*0x10, 2 Digits in Hex                         Done*/
	UINT8   cUDTXdb;						/*0x11, 2 Digits in Hex                         Done*/
	UINT8   cUDSNratio;						/*0x12, 2 Digits in Hex                         Done*/
	UINT8   cUDMSE;							/*0x13 Normalized Mean Squared error*/
	UINT8   cUDNearEchoLoss;				/*0x14 near echo loss*/
	UINT8   cUDFarEchoLoss;					/*0x15 far echo loss*/
	UINT16  wUDFarEchoDelay;				/*0x16 far echo delay*/
	UINT16  wUDRoundTripDelay;				/*0x17 round trip delay*/
    UINT32  cV34InfoBits;                   /*0x18 status */
   
	UINT8   cUDTXCarrierNegotiation;		/*0x20, Table 6                                 Done*/
	UINT8   cUDRXCarrierNegotiation;		/*0x21, Table 6                                 Done*/
	UINT16  wUDTXSymbol;					/*0x22, 4 Digits in Hex                         Done*/
	UINT16  wUDRXSymbol;					/*0x23, 4 Digits in Hex                         Done*/
	UINT16  wUDTXCarrierFreq;				/*0x24, 4 Digits in Hex                         Done*/
	UINT16  wUDRXCarrierFreq;				/*0x25, 4 Digits in Hex                         Done*/
	UINT16  wUDFirstTXrate;					/*0x26, 4 Digits in Hex                         Done*/
	UINT16  wUDFirstRXrate;					/*0x27, 4 Digits in Hex                         Done*/

	UINT8   cUDCarrierLossCount;			/*0x30, 2 Digits in Hex                         Done*/
    UINT8   cUDRateRenegReqCount;           /*0x31, 2 Digits in Hex, (half)                 Done*/
    UINT8   cUDRateRenegDetCount;           /*0x31, 2 Digits in Hex, (half)                 Done*/
    UINT8   cUDRetrainReqCount;             /*0x32, 2 digits in Hex                         Done*/

    UINT32  dwEqmAboveRtrnThreshold;
    UINT32  dwEqmAboveRenegThreshold;
    UINT32  dwEqmBelowRenegThreshold;
    UINT32  dwBlockErrors;
    UINT32  dwRtrnCounterExceeded;
    UINT32  dwRtrnAsThirdReneg;
	UINT32	dwInvalidSymbolRate;
	UINT32	dwInvalidMsIndex;
    UINT8   cUDRetrainDetCount;             /*0x33  2 digits, Retrains granted              Done*/
    UINT16  wUDLastTXrate;                  /*0x34, 4 digits in Hex                         Done*/
    UINT16  wUDLastRXrate;                  /*0x35, 4 digits in Hex                         Done*/

    UINT8   cUDProtocolNegotiation;     /*0x40, table 4                             Done*/
    UINT16  wUDMaxFrameSize;                    /*0x41, 3 digits*/
    UINT8   cUDECLinkTimeout;                   /*0x42, 1 digits*/
    UINT8   cUDECLinkNAK;                       /*0x43, 1 digits*/
    UINT8   cUDCompressionNegotiation;  /*0x44, table 5                             Done*/
    UINT16  cUDCompressionDictSize;             /*0x45, 4 digits                            Done*/

    UINT8   cUDTXFlowControl;               /*0x50, 1 digits                                Done*/
    UINT8   cUDRXFlowControl;               /*0x51, 1 digits                                Done*/
    UINT32  lUDTXCharFromDTE;               /*0x52, 8 digits                                Done*/
    UINT32  lUDRXCharToDTE;                 /*0x53, 8 digits                                Done*/
    UINT16  wUDTXCharLost;                  /*0x54, 4 digits                                ---*/
    UINT16  wUDRXCharLost;                  /*0x55, 4 digits                                ---*/
    UINT32  lUDTXFrameCount;                /*0x56, 8 digits                                Done ?*/
    UINT32  lUDRXFrameCount;                /*0x57, 8 digits                                Done ?*/
    UINT32  lUDTXFrameErrorCount;           /*0x58, 8 digits                                Done ?*/
    UINT32  lUDRXFrameErrorCount;           /*0x59, 8 digits                                Done ?*/

    CALL_TERMINATION_CAUSE cUDTermination;  /*0x60 Tables 8-9                               In process*/

    UINT8   bHighPassDpFilterOn;            /*                                              Done*/
    UINT8   bRBSpattern;                    /*                                              Done*/
    UINT8   bPadDetected;                   /*                                              Done*/
    UINT16  wDigitalLossEstimate;           /*                                              Done*/
    UINT8   bDualPCMDetected;               /*                                              Done*/
    UINT32  wTrainingCount;                 /*                                              Done*/
    UINT32  dwTrainingEqmSum;               /*                                              Done*/
    short   EqmValue;                       /*                                              Done*/
    UINT8   cUDV34FBCause;                  /*                                              Done*/

	UINT32	lCallWaitingCounter;
	UINT8	cSaveProfileResult;
	UINT16  wIMDRatio;
	UINT16  wA63Val;
	UINT8	bAltRBS; 
	UINT8   bSpeedDrop;
	UINT32	dwHandshakeTime;
	UINT32	dwHandshakeStarted,
			dwTrainingDone,
			dwNegotiatingDone,
			dwSessionEnded;

	UINT8	nLAPMNeg;						/*	holds result of LAPM neg in v.8(bis)	*/

#ifdef SAVE_TXRX_STATES
	struct TxRxStateRecord
	{
		UINT32 Time;
		UINT8 TxState, RxState, Conf;
	} TxRxStates[MAX_STATES];
    unsigned   cTxRxIndex;
	UINT8 cDpSpeed, cOurSpeed;
#endif
#ifdef SAVE_MOH_STATES
#define MAX_MOH_STATES  30
    struct MohStateRecord
    {
		UINT32  dwTime;
		UINT8   bTxState, bRxState;
        UINT8   bAbCode, bDpState;
    } MohStates[MAX_MOH_STATES];
    unsigned   cMohIndex;
#endif

#ifdef CADMUS2
	UINT8	nScriptReturnedWrongSize,
			nScriptTimeout;
	UINT16	wSpdCnt;
#endif

    UINT8   nDownloadFailureIndex;
    DOWNLOAD_FAILURE    DownloadFailure[MAX_DOWNLOAD_FAILURE];

#ifdef LAPM_NEG_STATS

#define LAPM_MAX_EQM_INDEX				50
#define	LAPM_MAX_EQM_INDEX_DECIMATOR	50

	UINT32	dwXidReceived;
	UINT32	dwValidXidReceived;
	UINT32	dwXidSent;
	UINT32	dwSabmeSent;
	UINT32  dwTimeWaitingForXID;
	UINT32  dwTimeWaitingForUA;
	UINT32  dwInvalidXidFormatId;
	UINT32  dwInvalidXidLength;
	UINT16	wEQM[LAPM_MAX_EQM_INDEX];
	UINT16	wEqmIndex;
	UINT16	wHDLC_abort;
	UINT16	wHDLC_badcrc;
	UINT16	wHDLC_noteof;
	UINT16	wHDLC_overrun;
	UINT16	wHDLC_goodeof;
#endif
#ifdef LOOPBACK_RETRAIN

#define	LOOPBACK_MAX_EQM_NUMBER	64

	UINT16	wLoopbackEqm[LOOPBACK_MAX_EQM_NUMBER];
	UINT32	dwLoopbackEqmIndex;
	UINT32	dwTotalLoopbackRetrains;
#endif

#if defined(USE_LOOPBACK_RESTART) && defined(_DEBUG)
	UINT32	dwLalSetupFailure;
#endif

#ifdef DP_CLOCK_WA_STAT
	UINT32	dwWa3333[4];
#endif
    } POUND_UD_DATA, *PPOUND_UD_DATA;


#define SET_POUNDUD_FIELD(pObj, FieldName, FieldValue) (pObj->FieldName = (FieldValue))
#define SET_POUNDUD_STRING(pObj, FieldName, FieldValue) OsStrCpy(pObj->FieldName, FieldValue)
#define ADD_POUNDUD_FIELD(pObj, FieldName, Value) (pObj->FieldName) += (Value)
#define FILL_POUNDUD_FIELD(pObj, FieldName, Value) OsMemSet((pObj->FieldName), (Value), sizeof(pObj->FieldName))
#else
#define SET_POUNDUD_FIELD(pObj, FieldName, FieldValue)
#define SET_POUNDUD_STRING(pObj, FieldName, FieldValue)
#define ADD_POUNDUD_FIELD(pObj, FieldName, Value)
#define FILL_POUNDUD_FIELD(pObj, FieldName, Value)
#endif

typedef enum {
    PERCTT_INFO,
    PERCTT_DTMF,
    PERCTT_FSK,
    PERCTT_QAM,
    PERCTT_MISC,
    PERCTT_FAX,
    PERCTT_V34,
	PERCTT_TEST,
} PERCTT_TYPE;

typedef struct PACKED_ATTRIB tagPercTT_Parms
{
    PERCTT_TYPE eType;
    UINT32      x;
    UINT32      y;
    UINT32      z;
} PERCTT_PARAMS, *PPERCTT_PARAMS;


#pragma pack()

#if !defined NO_MT_SUPPORT
typedef enum tagMT_TYPE
{
	MT_NONE,
	MT_PNPID,
	MT_DMA,
	MT_GPIO,
	MT_LC,
	MT_DT,
	MT_DIAL,
	MT_NE,
	MT_BG,
	MT_DC,
	MT_RDT,
	MT_CRYSTAL,
	MT_CONNECT,
	MT_LAST
} MT_TYPE;

typedef enum tagMT_FAILURE_REASON 
{
	MT_STATUS_SUCCESS,
	MT_STATUS_GENERAL_FAILURE,
	MT_STATUS_UNIMPLEMENTED,
	MT_STATUS_DIALTONE_DETECTED,
	MT_STATUS_DIALTONE_GONE,
	MT_STATUS_NO_PNPID,
	MT_STATUS_UNEXPECTED_PNPID,
	MT_STATUS_NO_DIALTONE,
	MT_STATUS_DIALTONE_AFTER_DIAL,
	MT_STATUS_LESS_INTERRUPTS,
	MT_STATUS_MORE_INTERRUPTS,
	MT_STATUS_NO_GPIO_PINS,
	MT_STATUS_UNEXPECTED_GPIO_VALUE,
	MT_STATUS_EMPTY_DIAL_STRING,
	MT_STATUS_HIGH_BG_NOISE,
	MT_STATUS_NO_RINGS,
	MT_STATUS_FEW_RINGS,
	MT_STATUS_NO_CONNECTION,
	MT_STATUS_NO_LOOP_CURRENT_EVENT,

} MT_STATUS;
/*
typedef enum tagMT_EVENT_TYPE
{
	MT_GENERAL_FAILURE,
	MT_DIAL_RESULT_DT_DETECTED,
	MT_DIAL_RESULT_DT_NOT_DETECTED,
	MT_DIAL_RESULT_DT_NOT_GONE,
	MT_DIAL_RESULT_DT_GONE,
	MT_BG_COMPLETE
} MT_EVENT_TYPE;
*/
#define MAX_MT_DIAL_STRING	64

typedef struct tagMtParams
{
	MT_TYPE			Type;
	UINT32			Timeout;
	DIAL_MODE		DialMode;
	MT_STATUS		Status;
	union {
		UINT32		Value;
		struct		
		{
			UINT32	NoDialtoneTimeout;
			UINT32	PostDelayTimeout;
		} DialtoneDetection;
		struct 
		{
			UINT32	NoiseLevel;
			UINT32	MaxNoiseLevel;
		} BackgroundNoise;
		struct
		{
			UINT32	MinRings;
			UINT32	Rings;
		} RingDetection;
		struct
		{
			UINT32			ConnectionTimeout;
			UINT32			ConnectionTime;
			CARRIER_REPORT	CarrierInfo;
		} DataConnection;
	};
//	UINT32			Param1;	
//	UINT32			Param2;
//	UINT32			Param3;
	char			DialString[MAX_MT_DIAL_STRING];
} MT_PARAMS, *PMT_PARAMS;

#endif /* !defined NO_MT_SUPPORT */

/*****************************************************************
OEM & COUNTRY specific parameters, should be loaded from NVRAM
END
*****************************************************************/

/*#$YS$ */
#if !defined NO_VOICE_SUPPORT || !defined NO_DTMF_CID_SUPPORT
#define POLARITY_REVERSAL_DEBOUNCE_TIME		200   /* in units of ms */
#endif

#if !defined NO_VOICE_SUPPORT
/*****************************************************************
Voice related structures and enums
*****************************************************************/

#define MAX_VTS_ENTRY   40
#define DIGIT_FLASH     0x20
#define VOICE_EXTOFFHOOK_DEBOUNCE_TIME  	500   /* in units of ms */
#define VOICE_HANDSET_DEBOUNCE_TIME			200   /* in units of ms */
#define VOICE_REMOTE_HANGUP_DEBOUNCE_TIME	700   /* in units of ms */
#define LINE_IN_USED_DEBOUNCE_TIME			100   /* in units of ms */

#define MAX_MIC_STEPS        6

typedef struct tagVoiceEvent {
    UINT32  dwEvent;
    BOOL    bPostfix;
    } VOICE_EVENT;

/* Voice Event code reporting from DCE */
#define DLEevent                     0x10   /* <DLE> */
#define SUBevent                     0x1A   /* <SUB> */
#define ETXevent                     0x03   /* <ETX> */
#define XONevent                     0x51   /* <Q> */
#define XOFFevent                    0x53   /* <S> */
#define SOHevent                     0x4D   /* <M> */
#define ACKevent                     0x57   /* <W> */
#define NAKevent                     0x46   /* <F> */
#define ENQevent                     0x55   /* <U> */
#define ETBevent                     0x47   /* <G> */
#define TIMEMARKevent                0x54   /* <T> */
#define HEADERevent                  0x58   /* <X> */
#define TERMINATORevent              0x2E   /* <.> */
#define DTMFSTARTevent               0x2F   /* </> */
#define DTMFOFFevent                 0x7E   /* <~> */
#define RINGevent                    0x52   /* <R> */
#define DTMF0event                   0x30   /* <0> */
#define DTMF1event                   0x31   /* <1> */
#define DTMF2event                   0x32   /* <2> */
#define DTMF3event                   0x33   /* <3> */
#define DTMF4event                   0x34   /* <4> */
#define DTMF5event                   0x35   /* <5> */
#define DTMF6event                   0x36   /* <6> */
#define DTMF7event                   0x37   /* <7> */
#define DTMF8event                   0x38   /* <8> */
#define DTMF9event                   0x39   /* <9> */
#define DTMFAevent                   0x41   /* <A> */
#define DTMFBevent                   0x42   /* <B> */
#define DTMFCevent                   0x43   /* <C> */
#define DTMFDevent                   0x44   /* <D> */
#define DTMFEevent                   0x2A   /* <*> */
#define DTMFFevent                   0x23   /* <#> */
#define RXOVERRUNevent               0x6F   /* <o> */
#define FAXCALLINGTONEevent          0x63   /* <c> */
#define DATACALLINGTONEevent         0x65   /* <e> */
#define PHONEONHOOKevent             0x68   /* <h> */
#define PHONEOFFHOOKevent            0x48   /* <H> */
#define SILENCEevent                 0x73   /* <s> */
#define QUIETevent                   0x71   /* <q> */
#define SITTONEevent                 0x4A   /* <J> */
#define BONGTONEevent                0x24   /* <$> */
#define REMOTEONHOOKevent            0x6C   /* <I> */
#define POLARITYREVERSALevent        0x4C   /* <L> */
#define CALLWAITINGevent             0x77   /* <w> */
#define TDDevent                     0x74   /* <t> */
#define RINGBACKevent                0x72   /* <r> */
#define BUSYTONEevent                0x62   /* <b> */
#define DIALTONEevent                0x64   /* <d> */
#define FASTBUSYTONEevent            0x4B   /* <K> */
#define V21FLAGSevent                0x4E   /* <N> */
#define TXUNDERRUNevent              0x75   /* <u> */
#define EXTENSIONONHOOKevent         0x70   /* <p> */
#define EXTENSIONOFFHOOKevent        0x50   /* <P> */
#define FAXORDATAANSWERevent         0x61   /* <a> */
#define DATAANSWERevent              0x66   /* <f> */
#define VOICEDETECTHIGHevent         0x56   /* <V> */
#define VOICEDETECTLOWevent          0x76   /* <v> */
#define CIDCWevent                   0x67   /* <g> */
#define STUTTEREDDIALTONEevent       0x69   /* <i> */
#define BADDATAFORMATevent           0x45   /* <E> */
#define LOSTDATAevent                0x59   /* <Y> */
#define FAXANSWERevent               0x6D   /* <m> */
#define CASTONEevent                 0x40   /* <@> */
#define EDTTDDevent                  0x6E   /* <n> */
#define OEM1event                    0x25   /* <%> */
#define OEM2event                    0x26   /* <&> */
#define OEM3event                    0x27   /* <'> */
#define OEM4event                    0x28   /* <(> */
#define OEM5event                    0x29   /* <)> */

/* Voice Command code from DTE */
#define NULLcommand                  0x00   /* <NUL> */
#define DLEcommand                   0x10   /* <DLE> */
#define SUBcommand                   0x1A   /* <SUB> */
#define XONcommand                   0x51   /* <Q> */
#define XOFFcommand                  0x53   /* <S> */
#define SOHcommand                   0x4D   /* <M> */
#define ETBcommand                   0x57   /* <W> */
#define ACKcommand                   0x46   /* <F> */
#define NAKcommand                   0x55   /* <U> */
#define ENQcommand                   0x47   /* <G> */
#define TIMEMARKcommand              0x54   /* <T> */
#define DTMFSTARTcommand             0x3C   /* </> */
#define DTMFOFFcommand               0x2F   /* <~> */
#define UPVOLUMEcommand              0x75   /* <u> */
#define DOWNVOLUMEcommand            0x64   /* <d> */
#define UPINGAINcommand              0x6E   /* <n> */
#define DOWNINGAINcommand            0x73   /* <s> */
#define ENDRXcommand                 0x1B   /* <ESC> */
#define PAUSEcommand                 0x70   /* <p> */
#define RESUMEcommand                0x72   /* <r> */
#define STOPTXcommand                0x03   /* <ETX> */
#define CANCELTXcommand              0x18   /* <CAN> */
#define CONCATTXcommand              0x1C   /* <FS> */
#define ENDDUPLEXcommand             0x5E   /* <^> */
#define TXBUFFSTATcommand            0x3F   /* <?> */
#define ABORTRXcommand               0x21   /* <!> */
#define CASTONEcommand               0x40   /* <@> */

#define BUFFSTATcommand              0x04   /* <EOT> */
#define PURGEBUFFcommand             0x45   /* <E> */

#define DTMFTONE            0x01
#define DATACALLINGTONE     0x02
#define FAXCALLINGTONE      0x04
#define V25ANSWERTONE       0x08
#define BELLANSWERTONE      0x10
#define CALLPROGRESSTONE    0x20
#define ALLTONES            (DTMFTONE | DATACALLINGTONE | FAXCALLINGTONE | V25ANSWERTONE | BELLANSWERTONE | CALLPROGRESSTONE)

typedef enum tagExtDetection {
    EXT_DETECT_UP,
    EXT_DETECT_DOWN,
    EXT_DETECT_OFF
    } EXT_DETECTION;

typedef enum {
    FROM_IDLE,
    FROM_FDSP,
    FROM_DATA,
    FROM_VOICE,
    } VOICE_PREVIOUS_MODE;

typedef enum tagVoiceActiveMode {
    VOICE_IDLING,
    VOICE_VTX,
    VOICE_VTR,
    VOICE_VRX,
	VOICE_FDSP
    } VOICE_ACTIVE_MODE;

typedef enum VoiceSupportedVSMCode {
    VSM_8000,
    VSM_7200_8000
    } VOICE_SUPPORTED_VSM_CODE;

typedef enum CompressionMode {
    EIGHTBIT_PCM_1      = 1,
    EIGHTBIT_PCM_2      = 128,
    FOURBIT_ADPCM       = 129,
    EIGHTBIT_PCM_3      = 130,
    EIGHTBIT_ULAW       = 131,
    EIGHTBIT_ALAW       = 132,
    SIXTEENBIT_LIN      = 133,
    } VOICE_COMPRESSION_MODE;

/* CFGMGR_VOICE_FDSP_PARAMS, CFGMGR_VOICE_FDSP_PREV_PARAMS */
typedef struct tagVoiceFDSPParams {
    UINT32  dwEnable;
    UINT32  dwDuplexMode;
    UINT32  dwMicGain;
    UINT32  dwSpeakerGain;
    UINT32  dwSpeakerStep;
    UINT32  dwMicControl;
    } VOICE_FDSP_PARAMS, *PVOICE_FDSP_PARAMS;

/* CFGMGR_VOICE_VBT_PARAMS */
typedef struct tagVoiceVBTParams {
    UINT32  dwTxXONMark;
    UINT32  dwTxXOFFMark;
    } VOICE_VBT_PARAMS, *PVOICE_VBT_PARAMS;

/* CFGMGR_VOICE_VDID_PARAMS */
typedef struct tagVoiceVDIDParams {
    UINT32  dwSetting;
    UINT32  dwDigits;
    UINT32  dwTimeOut;
    } VOICE_VDID_PARAMS, *PVOICE_VDID_PARAMS;

/* CFGMGR_VOICE_VDT_PARAMS */
typedef struct tagVoiceVDTParams {
    UINT32  dwToneReportEnable;
    UINT32  dwToneReportType;
    } VOICE_VDT_PARAMS, *PVOICE_VDT_PARAMS;

/* CFGMGR_VOICE_VGR_PARAMS */
typedef struct tagVoiceVGRParams {
    UINT32  dwVRxGain;
    UINT8   cVolumeRxLevel;
    UINT8   cMaxRxLevel;
    UINT8   cMinRxLevel;
    } VOICE_VGR_PARAMS, *PVOICE_VGR_PARAMS;

/* CFGMGR_VOICE_VGT_PARAMS */
typedef struct tagVoiceVGTParams {
    UINT32  dwVTxGain;
    UINT8   cVolumeTxLevel;
    } VOICE_VGT_PARAMS, *PVOICE_VGT_PARAMS;

/* CFGMGR_VOICE_VLS_PARAMS */
typedef struct tagVoiceVLSParams {
    UINT32  bVoiceLocalMode;
    UINT32  bVoicePhoneToDCE;
    UINT32  bMicSpeakerToLine;
    UINT32  bMicSpeakerToDCE;
    UINT32  bMicSpeakerCtl;
    UINT32  dwPreviousRelayConfig;
    UINT32  bVLSOffHook;
    } VOICE_VLS_PARAMS, *PVOICE_VLS_PARAMS;

/* CFGMGR_VOICE_VSD_PARAMS */
typedef struct tagVoiceVSDParams {
    UINT32  dwSilenceTune;
    UINT32  dwSilencePeriod;
    } VOICE_VSD_PARAMS, *PVOICE_VSD_PARAMS;

/* CFGMGR_VOICE_VSM_PARAMS */
typedef struct tagVoiceVSMParams {
    UINT32  dwCompressionMethod;
    UINT32  dwSampleRate;
    UINT32  dwSilenceCompression;
    UINT32  dwSilenceExpansion;
    } VOICE_VSM_PARAMS, *PVOICE_VSM_PARAMS;

/* CFGMGR_VOICE_VTS_PARAMS */
typedef struct tagVoiceVTSParams {
    UINT32  dwFreqTone1;
    UINT32  dwFreqTone2;
    UINT32  dwDTMFDigit;
    UINT32  dwDuration;
    UINT32  dwToneType;
    } VOICE_VTS_PARAMS, *PVOICE_VTS_PARAMS;

typedef enum tagVTSType {
    VTS_END_PLAY_TONE,  /* Mark the end of table */
    VTS_DUAL_TONE,      /* [Freq1, Freq2, Time] */
    VTS_DTMF_TONE,      /* {Digit, Time} */
    VTS_DTMF_DIGIT,     /* Digit */
    } VTS_TYPE;

/* CFGMGR_HANDSET_STATE */
typedef enum tagHandsetState {
    ST_HANDSET_RESET,
    ST_HANDSET_UP,
    ST_HANDSET_DOWN,
    } HANDSET_STATE;


/* CFGMGR_VOICE_MISC_PARAMS */
#pragma pack(1)
typedef struct PACKED_ATTRIB tagVoiceMiscParams {
   UINT32 dwDSVD_VGS_MIN;    /* dummy */
   UINT32 dwDSVD_VGS_MAX;    /* dummy */
   UINT32 dwDSVD_VGS;        /* dummy */
   UINT32 dwDSVD_VGM_MIN;    /* dummy */
   UINT32 dwDSVD_VGM_MAX;    /* dummy */
   UINT32 dwDSVD_VGM;        /* dummy */
   UINT32 dwFDSP_VGS_MIN;    
   UINT32 dwFDSP_VGS_MAX;    
   UINT32 dwFDSP_VGS;    
   UINT32 dwFDSP_VGM_MIN;
   UINT32 dwFDSP_VGM_MAX;
   UINT32 dwFDSP_VGM;
   UINT8  cFDSP_VGR_MIN;
   UINT8  cFDSP_VGR_MAX;
   UINT8  cFDSP_VGR;
   UINT8  cTAM_VGR_MIN;
   UINT8  cTAM_VGR_MAX;
   UINT8  cTAM_VGR;
   UINT8  cVoiceTxLimiter;
   UINT8  cVoiceTeloutLimiter;
   UINT8  cVIAMicGain;
   UINT8  cVIA4DBDrop;
   UINT8  cVIASpeakerGain;
   UINT16 dwVoiceInputScale;
   UINT16 wVoiceLineScale[6];
   UINT16 wFDSPTAGCGAIN;
   UINT16 wFDSPRAGCGAIN;
   UINT16 wVoiceRxLineScale;
   UINT16 wMicAGCAdd;
   UINT16 LECLeakageCtl;
   UINT16 wMinMicGain;
   UINT16 wMaxMicGain;
   UINT16 wMaxMicLvl;
   UINT16 wTxSatAtt;
   UINT8  cFDSPTxLimiter;
   UINT8  cMinVRNAllowed;
   UINT16 wMicGainConcurrent; 
   UINT16 wSpkGainConcurrent;
   UINT8  cLineSilenceDetectMin;
   UINT8  cLineSilenceDetectMed;
   UINT8  cLineSilenceDetectMax;
    } VOICE_MISC_PARAMS, *PVOICE_MISC_PARAMS;
#pragma pack()

#define BOND_OPTIONS_VOICE          0x01
#define BOND_OPTIONS_SPEAKERPHONE   0x02
#define BOND_OPTIONS_RTAM           0x03

#endif /* VOICE_SUPPORT */


#endif      /* #ifndef __CONFIGCODES_H__ */

