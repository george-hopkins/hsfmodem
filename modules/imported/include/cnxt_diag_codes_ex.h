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
* $Header:   R:/pvcs68/vm/common/Cider Project/archives/Octopus/include/cnxt_diag_codes_ex.h-arc   1.2   May 14 2003 19:30:58   shay  $
*                                                                                       *
*****************************************************************************************/
 
/*----------------------------------------------------------------------------
 *
 * Copyright (C) 1996-2001 Conexant Systems Inc. 
 * All rights reserved.
 *
 *----------------------------------------------------------------------------
 *
 * File: cnxt_diag_codes_ex.h
 *
 *----------------------------------------------------------------------------
 */
 
#ifndef _CNXT_DIAG_CODES_EX_H_
#define _CNXT_DIAG_CODES_EX_H_

#include "codesdeps.h"

/* define the version of the diagnostic interface */
#define DIAG_API_VERSION	7

#define TMP_LASSO_TESTS_RANGE 10000
 
/* diagnostic codes for Modem Monitor */
typedef enum {
/*0*/	CSDIAG_DRIVER_VERSION				,/*string			Version of device drivers */
		CSDIAG_HARDWARE_PNP_ID				,/*string			For PCI device: vendor, device, subsystem device and vendor IDs */
		CSDIAG_MODEM_FRIENDLY_NAME			,/*string			TAPI name for modem */
		CSDIAG_MODEM_COM_PORT				,/*DWORD			Port number (1, 2, 3, ...) */
		CSDIAG_DTE_EVENT_MASK				,/*DWORD			Event bitmask from COMM API: CD, CTS, DSR, RI */
/*5*/	CSDIAG_LAST_NUMBER_DIALED			,/*string			Last dial string entered */
		CSDIAG_V8_CM						,/*string			v.8 CM octets (field no. 4 in AT#UD) */
		CSDIAG_V8_JM						,/*string			v.8 JM octets (field no. 5 in AT#UD) */
		CSDIAG_V8BIS_FLEX					,/*DWORD			V.8bis negotiation strings, flex version */
		CSDIAG_V8BIS_DSP_VER				,/*DWORD			V.8bis negotiation strings, DSP version */
/*10*/	CSDIAG_V34_INFO						,/*DWORD			V.34 INFO sequence bitmap */
		CSDIAG_MP_SEQ						,/*string			MP sequence */
		CSDIAG_PROTOCOL_NEG_RES				,/*DWORD			v.42 LAPM, MNP4, MNP10,NONE, V.80, v.70 (field no. 40 in AT#UD) */
		CSDIAG_EC_FRAME_SIZE				,/*DWORD			Bytes (field no. 41 in AT#UD) */
		CSDIAG_EC_LINK_TIMEOUTS				,/*DWORD			Number of timeouts in last session (field no. 42 in AT#UD) */
/*15*/	CSDIAG_EC_LINK_NAKS					,/*DWORD			Number of packet retransmit requests in last session (field no. 43 in AT#UD) */
		CSDIAG_CMPRS_NEG_RES				,/*DWORD			v.42bis, MNP5, none (field no. 44 in AT#UD) */
		CSDIAG_CMPRS_DICT_SIZE				,/*DWORD			Compression dictionalry size in bytes (field no. 45 in AT#UD) */
		CSDIAG_SNR							,/*FLOAT			Signal to Noise Ratio (dB) */
		CSDIAG_MSE							,/*FLOAT			Mean Square Error [dB] */
/*20*/	CSDIAG_EQM							,/*FLOAT			 */
		CSDIAG_RX_SCATTER					,/*FLOAT			*	NA */
		CSDIAG_RX_NORM_FACTOR				,/*FLOAT			Gain for scatter plot */
		CSDIAG_ROUND_TRIP_DELAY				,/*DWORD			milliseconds */
		CSDIAG_TX_SIGNAL_POWER				,/*FLOAT			dB */
/*25*/	CSDIAG_RX_SIGNAL_POWER				,/*FLOAT			dB */
		CSDIAG_PAD_DETECTED					,/*DWORD			PAD 0=NORMAL ,3=3dBPad 6=6dBPad	PCM Pad */
		CSDIAG_RBS_DETECTED					,/*DWORD			RBS frame 0 to 63  (1' indicate robbed bit)	Robbed Bits Signaling */
		CSDIAG_NE_ECHO_POWER				,/*FLOAT			Near End Echo [dB] */
		CSDIAG_FE_ECHO_POWER				,/*FLOAT			Far End Echo [dB] */
/*30*/	CSDIAG_TX_SYMBOL_RATE				,/*DWORD			per second */
		CSDIAG_RX_SYMBOL_RATE				,/*DWORD			per second */
		CSDIAG_TX_CARRIER_FREQUENCY			,/*DWORD			Hertz */
		CSDIAG_RX_CARRIER_FREQUENCY			,/*DWORD			Hertz */
		CSDIAG_AGC_MIN						,/*DWORD			dB */
/*35*/	CSDIAG_AGC_MAX						,/*DWORD			dB */
		CSDIAG_AGC_LAST						,/*DWORD			dB */
		CSDIAG_120_HZ_POWER					,/*BOOL				High power level detect in band below 120 Hz */
		CSDIAG_HIGHPASS_FILTER_ENABLED		,/*BOOL				High Pass filter enabled */
		CSDIAG_HIGHEST_RX_STATE				,/*DWORD			TBD	 */
/*40*/	CSDIAG_HIGHEST_TX_STATE				,/*DWORD			TBD	 */
		CSDIAG_MODEM_STATE					,/*DWORD			TBD	 */
		CSDIAG_HANDSHAKE_SEQUENCE			,/*TBD				TBD	 */
		CSDIAG_CALL_SETUP_RES				,/*DWORD			no dialtone, fast busy, busy, voice, data answer tone, data calling tone, fax answer tone, fax calling tone, v.8 bis tone (field no. 1 in AT#UD) */
		CSDIAG_MULTI_MEDIA_MODE				,/*DWORD			data, fax, voice, voice view, v.70, h.324 (field no. 2 in AT#UD) */
/*45*/	CSDIAG_TERMINATION_CAUSE			,/*DWORD			Key abort, Carrier loss, Extension pickup , Call waiting tone detect, Inactivity timer, Link disconnect (normal), GSTN Cleardown, Line current reversal, ATH, DTR drop, Long space disconnect, Excessive retransmissions, Incompatible protocol, Incompatible modulation, S6 timed out, Line current lost, Loss of sync (V.42bis or MNP5), N400 exceeded (field no. 60 in AT#UD) */
		CSDIAG_RATE_RENEG_EV_CNT			,/*DWORD			Number of rate renegotiation events in last session (field no. 31 in AT#UD) */
		CSDIAG_RATE_RENEG_REQ				,/*DWORD			Number requested in last session */
		CSDIAG_RATE_RENEG_GRANTED			,/*DWORD			Number granted in last session */
		CSDIAG_RATE_RENEG_REASON			,/*DWORD			High EQM (fallback), Low EQM (fall forward), Excessive V.42/LAPM retransmits */
/*50*/	CSDIAG_RATE_RENEG_SILENCE_REQ		,/*DWORD			Number requested in last session */
		CSDIAG_RATE_RENEG_SILENCE_GRANTED	,/*DWORD			Number granted in last session */
		CSDIAG_RATE_RENEG_SILENCE_REASON	,/*DWORD			Reason for last rate reneg with silence */
		CSDIAG_RTRN_REQ						,/*DWORD			Number requested in last session (field no. 32 in AT#UD) */
		CSDIAG_RTRN_GRANTED					,/*DWORD			Number granted in last session (field no. 33 in AT#UD) */
/*55*/	CSDIAG_RTRN_REASON					,/*DWORD			Failed rate reneg, High EQM, 2 consecutive rate renegs, Suspected carrier loss, User initiated (ATO1), Excessive V.42/LAPM retransmits */
		CSDIAG_TIME_CONNECTED				,/*DWORD			Seconds, duration of last session */
		CSDIAG_TX_BIT_RATE_FIRST			,/*DWORD			Bits per second */
		CSDIAG_RX_BIT_RATE_FIRST			,/*DWORD			Bits per second */
		CSDIAG_TX_BIT_RATE_MIN				,/*DWORD			Bits per second */
/*60*/	CSDIAG_RX_BIT_RATE_MIN				,/*DWORD			Bits per second */
		CSDIAG_TX_BIT_RATE_MAX				,/*DWORD			Bits per second */
		CSDIAG_RX_BIT_RATE_MAX				,/*DWORD			Bits per second */
		CSDIAG_TX_BIT_RATE					,/*DWORD			Bits per second */
		CSDIAG_RX_BIT_RATE					,/*DWORD			Bits per second */
/*65*/	CSDIAG_TX_NEG_RES					,/*DWORD			Transmit carrier negotiated (field no. 20 in AT#UD) */
		CSDIAG_RX_NEG_RES					,/*DWORD			Receive carrier negotiated (field no. 21 in AT#UD) */
		CSDIAG_CARRIER_LOSS_EV_CNT			,/*DWORD			Number of carrier loss events during last session (field no. 30 in AT#UD) */
		CSDIAG_CALL_WAIT_EV_CNT				,/*DWORD			Number of call waiting events during last session (field no. 61 in AT#UD) */
		CSDIAG_EXTENSION_PICKUP_EV_CNT		,/*DWORD			Number of extension pickup events during last session */
/*70*/	CSDIAG_V34_FALLBACK_REASON			,/*V34_FALLBACK_REASON		Dual PCM detection, Loaded loop detection, ADPCM detection, a-law/mu-law combination, PCM speed too low */
		CSDIAG_TOTAL_TX_BLOCKS				,/*DWORD			Total number of protocol blocks transmitted in last data session */
		CSDIAG_TOTAL_RX_BLOCKS				,/*DWORD			Total number of protocol blocks received in last data session */
		CSDIAG_TOTAL_TX_CHARS				,/*DWORD			Total number of characters transmitted in last data session (field no. 52 in AT#UD) */
		CSDIAG_TOTAL_RX_CHARS				,/*DWORD			Total number of characters received in last data session (field no. 53 in AT#UD) */
/*75*/	CSDIAG_CPU_VENDOR					,/*string			Processor vendor ("Intel", "AMD", "Cyrix", ...) */
		CSDIAG_CPU_SPEED					,/*DWORD			Processor speed, MHz */
		CSDIAG_CACHE_SIZE					,/*DWORD			Level 2 Cache size reported by system */
		CSDIAG_LAST_INT_CPU					,/*DWORD			microseconds in last interrupt */
		CSDIAG_LAST_INT_LATENCY				,/*DWORD			Num of microseconds between last 2 interrupts */
/*80*/	CSDIAG_MAX_INT_CPU					,/*DWORD			Num of microseconds in longest interrupt */
		CSDIAG_MAX_INT_LATENCY				,/*DWORD			Longest microseconds latency between 2 interrupts */
		CSDIAG_SAMPLES_OVERRUNS				,/*DWORD			Num of samples overrun occurred in the past */
		CSDIAG_SAMPLES_UNDERRUNS			,/*DWORD			Num of samples underrun occurred in the past */
		CSDIAG_BUS_OVERRUNS					,/*DWORD			Num of bus overruns occurred in the past */
/*85*/	CSDIAG_BUS_UNDERRUNS				,/*DWORD			Num of bus underruns occurred in the past */
		CSDIAG_LINE_CURRENT					,/*DWORD			Estimated mA drawn on line (SmartDAA only) */
		CSDIAG_LINE_VOLTAGE					,/*DWORD			Estimated mV (SmartDAA only) */
		CSDIAG_LINE_RESISTANCE				,/*FLOAT			Estimated line resistance (SmartDAA only) */
		CSDIAG_MODEM_FAMILY_TYPE			,/*MODEM_FAMILY		HSF, HCF */
/*90*/	CSDIAG_CURRENT_COUNTRY				,/*WORD				T.35 country code for which modem is configured */
		CSDIAG_TX_TRELLIS_STATES			,/*INT				Number of trellis states (16, 32, 64), helps distinguish remote modem vendor */
		CSDIAG_RX_TRELLIS_STATES			,/*INT				Number of trellis states (16, 32, 64), helps distinguish remote modem vendor */
		CSDIAG_TIME_DTR_TO_HANDSHAKE		,/*DWORD			Time (in milliseconds) from off hook to start of handshake */
		CSDIAG_TIME_DTR_TO_PROTOCOL			,/*DWORD			Time (in milliseconds) from off hook to start of normal data transfer after protocol negotiation */
/*95*/	CSDIAG_QC_ENABLED					,/*BOOL				return TRUE if QC is enabled */
		CSDIAG_SERVER_TYPE					,/*SERVER_TYPE_INFO V34, V90 or V92 */
		CSDIAG_INFO0_V92                    ,/* TBD */
		CSDIAG_QC_SYGNAL_MAP				,/* TBD */
		CSDIAG_MH_ENABLED					,/*BOOL				return TRUE if MOH is enabled */
/*100*/	CSDIAG_CID_ENABLED					,/*BOOL				return TRUE if CID is enabled */
		CSDIAG_TIME_ON_HOLD					,/*DWORD			Seconds, time spent with the server on hold */
		CSDIAG_SERVER_HOLD_DURATION			,/*DWORD			seconds, from MHack (which contains maximum server hold time duration) */
		CSDIAG_MOH_REMOTE_RESPONSE			,/*DIAG_MOH_REMOTE_RESPONSE_T	 */
		CSDIAG_CALLER_ID_STRING				,/*STRING */
/*105*/	CSDIAG_MOH_STATUS					,/*MOH_STATE */
		CSDIAG_MOH_REMOTE_REQUEST			,/*DIAG_MOH_REMOTE_REQ_T */
		CSDIAG_MOH_LOCAL_RESPONSE			,/*MOH_LOCAL_DECISION_T */
		CSDIAG_TIMEOUT_FOR_USER_RESPONSE	,/*DWORD */
		CSDIAG_NEGOTIATION_TIME				,/*DWORD		  time elapsed between the beginning of the negotiation and the actual monitor */
/*110*/	CSDIAG_K56_RATES_SUPORTED			,/*BOOL			  TRUE if V90 or FLEX are enabled */
		CSDIAG_PULSE_DIALING_ENABLED		,/*BOOL			  TRUE if Pulse dialing is enabled in ctry params */
		CSDIAG_LAST_DIAL_TYPE				,/*LAST_DIAL_TYPE pulse or tone */
		CSDIAG_FILTER_COEFFICIENTS			,/*MODEM_FILTER_T */
		CSDIAG_REMOTE_MSE					,
/*115*/ CSDIAG_REMOTE_SNR					,
		CSDIAG_REMOTE_RX_SIGNAL_POWER		,
		CSDIAG_REMOTE_RX_BIT_RATE			,
		CSDIAG_REMOTE_TX_BIT_RATE			,
		CSDIAG_CONNECTION_DIRECTION			,/*BOOL (True for originator calls, False for Answer calls)  */
/*120*/	CSDIAG_CID_SETTING					,/*DWORD. (0 - Disabled, 1 - Enabled, string format, 2 - Enabled, binary format) */
		CSDIAG_MOH_METHOD					,/*MOH_METHOD */
		CSDIAG_OGC_ENABLED					,/*BOOL (0 - Disabled,1 - Enabled) */
		CSDIAG_HANDSET_EXCLUSIVE_BIT		,/*BOOL (0 - not handset exclusive,1 - is handset exclusive) */
        CSDIAG_APP_V92_AWARE                ,/*App indicated V.92 support in init sequence, RMY 060302 */

/*Lasso Test temporary range */
		CSDIAG_EEPROM_PNP_IF=TMP_LASSO_TESTS_RANGE ,/*DIAG_EEPROM_PNP_T (structure) */
		CSDIAG_BUS_IF						,/*DIAG_BUS_IF_T	 (structure) */
		CSDIAG_DEVICE_TYPE					,/*DIAG_ASIC_TYPE  //For smart daa only */
		CSDIAG_POWER_TYPE					,/*DIAG_POWER_TYPE */
		CSDIAG_CHECK_PME					,/*OUT: DWORD */
		CSDIAG_GET_GPIO						,/*DIAG_GET_SET_GPIO_T */
		CSDIAG_GET_HW_INPUT					,/*DIAG_GET_SET_GPIO_T */
		CSDIAG_GET_TEST_RX_BUFFER			,/*string */
 
 
/*V42@DC */
		CSDIAG_SESSION_ID = 0x03FF,	
		CSDIAG_PREVIOUS_TERMINATION_CAUSE = CSDIAG_TERMINATION_CAUSE + 0x0400,
		CSDIAG_PREVIOUS_SESSION_ID = 0x07FF,
 
   /* internal debug */
   CSDIAG_CODE_TRACE = 0x6000,
   CSDIAG_CODE_SCR_TRACE,
   CSDIAG_CODE_TRACE_MASK,

	/*v42dc capability code(0x8000-0x87FF) */
		CSDIAG_DEFINED_DIAG_CODE_RANGES = 0x8000,
		CSDIAG_DEFINED_CURRENT_CALL_SDK_DIAG_CODE_RANGES,
		CSDIAG_DEFINED_PREVIOUS_CALL_SDK_DIAG_CODE_RANGES,
	
	CSDIAG_SIGNATURE,
/*V42@DC */
 
	/* send in band AT command for ACF only */
	CSDIAG_ACF_GET_LAST_IN_BAND_AT_RESPONSE = 0xA000,
 
		CSDIAG_LAST                          /*The last enum value, for range check */
 
} CNXT_DIAG_CODES, *PCNXT_DIAG_CODES;

/* For Modem Control only */
typedef enum 
{
/*0*/	CSDIAG_CTRL_MOH_ENABLE,						/*			IN: BOOL				None */
		CSDIAG_CTRL_CW_DETECTED,					/*			IN: CW_FAKE_MOH_T		NONE */
		CSDIAG_CTRL_MOH_LOCAL_DECISION,				/*			IN: MOH_LOCAL_DECISION_T */
		CSDIAG_CTRL_QC_PROFILE_FLUSH,				/*			TBD */
		CSDIAG_CTRL_QC_PROFILE_LOAD,				/*			TBD */
/*5*/	CSDIAG_CTRL_QC_PROFILE_DELETION,			/*			NONE				 */
		CSDIAG_CTRL_QC_PROFILE_WRITE,				/*			IN: BOOL: ENABLE OR DISABLE WRITING */
		CSDIAG_CTRL_OUTGOING_CALL_REQ,				/*			NONE */
		CSDIAG_CTRL_UPSTREAM_ENABLE,				/*			IN: BOOL				None */
		CSDIAG_CTRL_QC_DISABLE,						/*			IN: BOOL  1 to disable, 0 to enable */
/*10*/	CSDIAG_CTRL_MOH_LOCAL_TIMEOUT,				/*			IN: DWORD  1-13 */
		CSDIAG_CTRL_MOH_CW_SETTINGS,				/*			IN: CW_SET_T */
		CSDIAG_CTRL_V44_COMPRESSION_MODE,			/*			IN: DWORD 0-3 */
		CSDIAG_CTRL_V44_COMPRESSION_NEGOTIATION,	/*			IN: BOOL 0-1 */
		CSDIAG_CTRL_V44_CAPABILITY,					/*			IN: DWORD 0-2 */
/*15*/	CSDIAG_CTRL_V44_TX_DICTIONARY_SIZE,			/*			IN: DWORD 256-65536 */
		CSDIAG_CTRL_V44_RX_DICTIONARY_SIZE,			/*			IN: DWORD 256-65536 */
		CSDIAG_CTRL_V44_MAX_TX_STRING,				/*			IN: DWORD 32-255 */
		CSDIAG_CTRL_V44_MAX_RX_STRING,				/*			IN: DWORD 32-255 */
		CSDIAG_CTRL_V44_MAX_TX_HISTORY,				/*			IN: DWORD 512-65536 */
/*20*/	CSDIAG_CTRL_V44_MAX_RX_HISTORY,				/*			IN: DWORD 512-65536 */

		/* send in band AT command for ACF only */
		CSDIAG_CTRL_ACF_SEND_IN_BAND_AT_CMD,  /* send usual string but without 'AT' or <cr> */
		CSDIAG_CTRL_SET_CID,						/*			IN: BOOL (1 - Enable, 0 - Disable) */
 		CSDIAG_CTRL_MOH_SET_METHOD,					/*			IN: MOH_METHOD */
 		CSDIAG_CTRL_DBG_PRINT,						/*			IN: string */
		
		/*Lasso Test temporary range */
/*10000*/CSDIAG_CTRL_OSCILATOR_SET=TMP_LASSO_TESTS_RANGE, /*		IN: BOOL 1 to start, 0 to stop */
		CSDIAG_CTRL_SET_GPIO,						/*			IN: DIAG_GET_SET_GPIO_T */
		/*W.O.R. Test */
		CSDIAG_CTRL_SET_POWER_TYPE,					/*			IN: DIAG_POWER_TYPE  */
		CSDIAG_CTRL_ARM_FOR_WAKE,					/*			IN: BOOL (1 - Arm, 0 - Disarm) */
		CSDIAG_CTRL_CLEAR_PME,						/*			NONE */
		/*Codec LoopBack */
		CSDIAG_CTRL_START_ANALOG_LOOPBACK,			/*			IN: BOOL	(1 - Start, 0 - Stop)	- RKCTRL */
		CSDIAG_CTRL_BUS_IF_SET_DATA_TRANSFER,		/*			IN: BOOL	(1 - Start, 0 - Stop) */
		CSDIAG_CTRL_TRANSMIT_DATA,					/*			IN: BYTE* (data stream) //Analog LoopBack */
		/*Tone Loopback */
		CSDIAG_CTRL_SINGLE_TONE_GENERATE,			/*			IN: TONE_GENERATOR_TEST_T (BOOL to start/stop, tone description) // */
		CSDIAG_CTRL_SINGLE_TONE_DETECT,				/*			IN:	DWORD timeout			TONE_DETECTOR_TEST_T (tone type, timeout) //Result will be notified by callback CSDIAG_TRAP_TONE_DETECTED */
		CSDIAG_CTRL_DTMF_TONE_GENERATE,				/*			IN: DPDIALDIGITS */
		CSDIAG_CTRL_DTMF_TONE_DETECT,				/*			IN: BOOL (1 - Start, 0 - Stop); */
		CSDIAG_CTRL_START_RING_DETECTOR,			/*          IN: BOOL (1- start, 0 - stop) */
 
		CSDIAG_CTRL_GO_OFF_HOOK,					/*			NONE */
		CSDIAG_CTRL_GO_ON_HOOK,						/*			NONE */
 
		CSDIAG_CTRL_V34_CONNECTION,					/*			IN: BOOL (1- start, 0 - stop) */
		CSDIAG_CTRL_OVP_DETECTION,					/*			IN: DWORD (timeout) */
		CSDIAG_CTRL_CID_DETECTION,					/*			IN: DWORD (timeout) */
		/* A BLAM code. Keep it here for compatibility. */
		CSDIAG_CTRL_CODE_TRACE_MASK,
		CSDIAG_CTRL_REMOTE_DIAGNOSTICS,
		
		CSDIAG_CTRL_CODE_LAST  
}CNXT_DIAG_CTRL_CODES, *PCNXT_DIAG_CTRL_CODES;

typedef enum 
{
	CSDIAG_TRAP_MODEM_STATE		     = CSDIAG_MODEM_STATE,
	CSDIAG_TRAP_TERMINATION_CAUSE    = CSDIAG_TERMINATION_CAUSE,
	CSDIAG_TRAP_CALL_SETUP_RES	     = CSDIAG_CALL_SETUP_RES,
	CSDIAG_TRAP_MOH_REMOTE_RESPONSE	 = CSDIAG_MOH_REMOTE_RESPONSE,
	CSDIAG_TRAP_MOH_STATUS			 = CSDIAG_MOH_STATUS,
    CSDIAG_TRAP_MOH_LOCAL_RESPONSE   = CSDIAG_MOH_LOCAL_RESPONSE,
    CSDIAG_TRAP_APP_V92_AWARE        = CSDIAG_APP_V92_AWARE,
	CSDIAG_TRAP_PORT_STATE			 = 109,						/*DIAG_NOTIFY_PORT_STATE_T  */

 
	CSDIAG_TRAP_SINGLE_TONE_DETECTED = TMP_LASSO_TESTS_RANGE,	/*DIAG_SINGLE_TONE_DETECTOR_RESULT */
	CSDIAG_TRAP_DTMF_GENERATOR_RESULT,							/*BOOL */
	CSDIAG_TRAP_DTMF_DETECTOR_RESULT,							/*DIAG_DTMF_DETECTOR_RESULT  */
	CSDIAG_TRAP_RING_DETECTOR_RESULT,							/*RING_DET_PARAMS (0 - Timed Out, 1 - Detected) */
	CSDIAG_TRAP_RECIEVE_DATA,									/*byte * stream */
	CSDIAG_TRAP_OVP_DETECTED,									/*BOOL (0 - Timed Out, 1 - Detected) */
	CSDIAG_TRAP_RLSD_DETECTED,									/*BOOL (0 - Timed Out, 1 - Detected) */
 
	CSDIAG_TRAP_LAST,
}CNXT_DIAG_TRAP_CODES, *PCNXT_DIAG_TRAP_CODES;

typedef enum
{
		/*+ms params */
/*0*/	CSDIAG_CFG_MODULATION,						/*			IN: Modulation Type: RK_DP_IDS (First parameter of at+ms) */
		CSDIAG_CFG_BIT_RATE_RX_MAX,					/*			INT - Bit Rate		None */
		CSDIAG_CFG_BIT_RATE_TX_MAX,					/*			INT - Bit Rate		None */
		CSDIAG_CFG_BIT_RATE_RX_MIN,					/*			INT - Bit Rate		None */
		CSDIAG_CFG_BIT_RATE_TX_MIN,					/*			INT - Bit Rate		None */
/*5*/	CSDIAG_CFG_AUTOMODE_ENABLE,					/*			BOOL - 1/0			None */
		/*QC configs	 */
		CSDIAG_CFG_QC_PROFILE_DELETION,				/*			NONE				 */
		CSDIAG_CFG_QC_PROFILE_WRITE,				/*			BOOL: ENABLE OR DISABLE WRITING */
		CSDIAG_CFG_QC_DISABLE,						/*			BOOL  1 to disable, 0 to enable */
		/*Upstream configs */
		CSDIAG_CFG_UPSTREAM_ENABLE,					/*			BOOL				None */
		/*Moh Configs */
/*10*/	CSDIAG_CFG_MOH_ENABLE,						/*			IN: BOOL				None */
		CSDIAG_CFG_MOH_LOCAL_TIMEOUT,				/*			DWORD  1-13 */
		CSDIAG_CFG_MOH_CW_SETTINGS,					/*			CW_SET_T */
		/*ds44 params */
		CSDIAG_CFG_V44_COMPRESSION_MODE,			/*			DWORD 0-3 */
		CSDIAG_CFG_V44_COMPRESSION_NEGOTIATION,		/*			BOOL 0-1 */
/*15*/	CSDIAG_CFG_V44_CAPABILITY,					/*			DWORD 0-2 */
		CSDIAG_CFG_V44_TX_DICTIONARY_SIZE,			/*			DWORD 256-65536 */
		CSDIAG_CFG_V44_RX_DICTIONARY_SIZE,			/*			DWORD 256-65536 */
		CSDIAG_CFG_V44_MAX_TX_STRING,				/*			DWORD 32-255 */
		CSDIAG_CFG_V44_MAX_RX_STRING,				/*			DWORD 32-255 */
/*20*/	CSDIAG_CFG_V44_MAX_TX_HISTORY,				/*			DWORD 512-65536 */
		CSDIAG_CFG_V44_MAX_RX_HISTORY,				/*			DWORD 512-65536 */
		CSDIAG_CFG_MOH_SET_METHOD,					/*			MOH_METHOD */
		CSDIAG_CFG_MOH_RESUME_OVERRIDE,
		CSDIAG_CFG_MOH_REJECT_OVERRIDE,
/*25*/	CSDIAG_CFG_MOH_ACCEPT_OVERRIDE,
		CSDIAG_CFG_SET_CID,
		CSDIAG_CFG_CODE_LAST  
} CNXT_DIAG_CFG_CODES;

/* The overloaded structure for pOutBuffer parameter. */
/* It allows a timestamp could be sent back. */
/*typedef struct 
{
	PVOID		pOutBuf;
	DWORD		dwTimeStamp;
} OVERLOADED_OUTBUF, *POVERLOADED_OUTBUF;
 */
#endif /* _CNXT_DIAG_CODES_EX_H_ */
 
 
