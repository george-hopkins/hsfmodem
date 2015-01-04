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

File Name:			ConfigCodes.h	

File Description:	Configuration Manager Enumerated Code Types

*****************************************************************************************/


#ifndef __CONFIGCODES_H__
#define __CONFIGCODES_H__

typedef enum tagCFGMGR_CODE
{

    CFGMGR_MS_PARAMS,                               /* +ms parameters */
    CFGMGR_EC_PARAMS,                               /* PEC_USER_CONFIG EC parameters */
/*#$YS$ Added for Break handling */
	CFGMGR_BRK_PARAMS,								/* +eb Break handling	*/
/*YS@001130 */
	CFGMGR_ITF_PARAMS,								/* +itf Tx Flow Control Thresholds */
/*#$YS$ Added for AT+IPR command. */
    CFGMGR_AUTODETECT,								/* +ipr DTE rate (0: Autodetect, non-zero: Fixed rate */

    CFGMGR_COMPRESSION_PARAMS,                      /* PDS_USER_CONFIG compression parameters */
/*YS@001130 */
#if !defined NO_V44_SUPPORT
    CFGMGR_V44_PARAMS,								/* PV44_USER_CONFIG  V.44 compression parameters */
#endif
    CFGMGR_RETRAIN_ENABLED,                         /* */

    CFGMGR_CADENCE_PARAMS,                          /* CADENCE_CFG * - params of the cadence */

    /* AT-parser to LMON */
    CFGMGR_FORCE_RENEG_UP,                          /* UINT8            */
    CFGMGR_FORCE_RENEG_DOWN,                        /* UINT8            */
/*#$YS$ */
#if !defined NO_V34FAX_SUPPORT
    CFGMGR_V34FAX_ENABLED,                          /* UINT8            */
#endif

    /* S-Registers  - UINT8 */
	CFGMGR_SREG,									/* S0 */
	CFGMGR_NRINGS_TO_ANSWER     = CFGMGR_SREG+0,	/* Number of rings before modem answers the call */
	CFGMGR_RING_COUNTER         = CFGMGR_SREG+1,    /* Number of rings detected */
	CFGMGR_ESCAPE_CHAR          = CFGMGR_SREG+2,    /* Escape character (+) */
	CFGMGR_CR_CHAR              = CFGMGR_SREG+3,    /* Line Termination character (CR) */
	CFGMGR_LF_CHAR              = CFGMGR_SREG+4,    /* Response Formatting character (LF) */
	CFGMGR_BS_CHAR              = CFGMGR_SREG+5,	/* Command Line Editing character (BS) */
	CFGMGR_BLIND_DIAL_WAIT_TIME = CFGMGR_SREG+6,    /* Time, in seconds, the modem waits before starting to dial 
													   after going off hook when blind dialing. */
	CFGMGR_CARRIER_WAIT_TIME    = CFGMGR_SREG+7,    /* Time, in seconds, the modem waits for carrier.
													   If enabled by country options, time, in seconds, 
													   the modem waits for dialtone after 'W' dial modifier */
	CFGMGR_PAUSE_DELAY_TIME     = CFGMGR_SREG+8,    /* Time, in seconds, the modem waits after ',' dial modifier */
	CFGMGR_CARDET_RESPONSE_TIME = CFGMGR_SREG+9,    /* Time, in 0.1 seconds modem waits before reporting carrier */
	CFGMGR_HANGUP_DELAY_TIME    = CFGMGR_SREG+10,	/* Time, in tenths of a second, the modem waits before 
													   hanging up after a loss of carrier */
	CFGMGR_DTMF_DURATION        = CFGMGR_SREG+11,   /* Duration, in milliseconds, of tones in DTMF dialing */
	CFGMGR_ESCAPE_PROMPT_DELAY  = CFGMGR_SREG+12,   /* Time, in fiftieths of a second, between receiving +++
													   and sending OK to DTE */
	CFGMGR_TEST_TIMER           = CFGMGR_SREG+18,   /* Time, in seconds, that the modem conducts a test (&T1) */
	CFGMGR_FLASH_DELAY_TIME     = CFGMGR_SREG+29,   /* Time, in hundredths of a second, the modem goes on-hook 
													   after flash (!) modifier */
	CFGMGR_INACTIVITY_TIME      = CFGMGR_SREG+30,   /* Time, in tens of seconds, the modem stays online when no
													   data is sent or received */
    CFGMGR_DATA_COMPRESSION     = CFGMGR_SREG+46,   /* Selection of compression */
	CFGMGR_EXTENDED_RESULT_CODES = CFGMGR_SREG+95,/* Bits to override some of the W control options */
    CFGMGR_SREG_LAST = CFGMGR_SREG+255,				/* S-register window end */


	/* Call Progress control codes */
	CFGMGR_DIALSTRING,								/* Pointer to a dial string     LPCSTR */
	CFGMGR_PULSE_DIAL_CONFIG,						/* Pulse dial parameters    */
	CFGMGR_TONE_DIAL_CONFIG,						/* Tone dial parameters     */
	CFGMGR_DIALTONE_WAIT_TIME,						/* Time in tenths of a second the modem waits for dialtone */
	CFGMGR_DIAL_MODE,								/* Dial mode (tone or pulse      DIAL_MODE */
	CFGMGR_USE_S7_WHEN_W,							/* if TRUE, use S7 for timeout after W modifier in dial string
													   if FALSE, use CFGMGR_DIALTONE_WAIT_TIME value */
    CFGMGR_INVERT_CALLING_TONE,                     /* If TRUE - invert calling tone fro this call */

    CFGMGR_CALL_PROGRESS_TONE_DEBOUNCE,				/* Debounce times for call progress tones */
	CFGMGR_CALL_MODE,     				            /* CALL_MODE*                   */
    CFGMGR_CALL_PROGRESS_TIMING,					/* TIMING_CONFIG*. Delays       */
	CFGMGR_CALL_PROGRESS_FLAGS,						/* CALL_PROGRESS_FLAGS*         */
	CFGMGR_V8BIS_CONTROL,							/* V8BIS_OPERATION_CONTROL*		*/
	CFGMGR_V8BIS_RESULT,							/* PV8BIS_RESULT				*/
    CFGMGR_PREFERRED_FLEX_VERSION,                  /* Preferred Flex version       */
    CFGMGR_V90_ENABLED,                             /* V.90 is enabled              */
    CFGMGR_V70_ALLOWED,                             /* V.70 is allowed              */


    /* ATIn Messages - LPCSTR */
    CFGMGR_ATMESSAGE_I0,
    CFGMGR_ATMESSAGE_I1,
    CFGMGR_ATMESSAGE_I2,
    CFGMGR_ATMESSAGE_I3,
    CFGMGR_ATMESSAGE_I4,
    CFGMGR_ATMESSAGE_I5,
    CFGMGR_ATMESSAGE_I6,
    CFGMGR_ATMESSAGE_I7,
    CFGMGR_ATMESSAGE_I8,
    CFGMGR_ATMESSAGE_I9,
    CFGMGR_ATMESSAGE_I10,
    CFGMGR_ATMESSAGE_I11,
    CFGMGR_ATMESSAGE_I12,

    /* DTE configuration    */
    CFGMGR_DTE_ECHO,                    /* UINT8            */
    CFGMGR_DTE_CONFIG,                  /* DTE_USER_CONFIG  */

    CFGMGR_SYNC_MODE,                   /* AT&Q             */

    CFGMGR_RLSD_BEHAVIOR,               /* UINT8            */
    CFGMGR_DTR_BEHAVIOR,                /* UINT8            */

    /* Speaker Control */
    CFGMGR_SPEAKER_VOLUME,              /* UINT8            */
    CFGMGR_SPEAKER_CONTROL,             /* UINT8            */

    /* Pulse Dial Make/Break Ratio */
    CFGMGR_PULSE_MAKE_BREAK,            /* UINT8            */

    /* Ring burst frequencies */
    CFGMGR_IMMEDIATE_RING_DETECTED,     /* immediate ring used in French CID */

    /* Answer tone detector params                                  */
    CFGMGR_ANSWER_TONE_DETECTOR,        /* PANSWER_TONE_PARAMS      */
    /* Bell answer (2225) tone detector params                      */
    CFGMGR_BELL_TONE_DETECTOR,          /* PANSWER_TONE_PARAMS      */
    /* DTMF generator parameters                                    */
    CFGMGR_DTMF_GENERATOR,              /* PDTMF_GEN_PARAMS         */

    /* Modulation Report                                            */
    CFGMGR_MODULATION_REPORT,           /* UINT8                    */

    /* to replace STATUS_CP_CARRIER */
    CFGMGR_MODULATION_INFO,

    /* Cadences                                                     */
    CFGMGR_CADENCE_BASE,
    CFGMGR_BUSY_TONE_CADENCE = CFGMGR_CADENCE_BASE,
	/*#$YS$ Added for Voice support. */
	CFGMGR_RINGBACK_TONE_CADENCE,
    CFGMGR_RING_CADENCE,
    CFGMGR_REORDER_TONE_CADENCE,
    CFGMGR_SDIAL_TONE_CADENCE,
	/*#$YS$ Added for Voice support. */
	CFGMGR_DATA_CALL_TONE_CADENCE,
    CFGMGR_FAX_CALL_TONE_CADENCE,

    /* FAX Service Class 1 Configuration Values                     */
    CFGMGR_FAX_AUTO_ANSWER,             /* UINT8                    */

    /*  OEM flags                                                   */
    CFGMGR_OEM_FLAGS,                   /* OEMFlagStructure         */
	CFGMGR_OEM_FLAGS2,					/* OEMFlag2Structure        */
    CFGMGR_OEM_FILTERS,                 /* OEMFilterStructure       */
    CFGMGR_OEM_SPKR_MUTE_DELAY,         /* UINT16 SpkrMuteDelay     */
	CFGMGR_OEM_READ_MASK,				/* UINT16 OEMReadMask       */
    CFGMGR_OEM_THRESHOLD,				/* OEMThresholdStruct       */

    CFGMGR_COUNTRY_CODE,                /* current country code     */
                                        /* Note: DO NOT USE THIS CODE TO CHANGE THE COUNTRY !!! */
                                        /* USE CONFIGMGR_CONTROL_CHANGE_COUNTRY CONTROL CODE INSTEAD */
    CFGMGR_PREVIOUS_COUNTRY_CODE,       /* previous country code    */
    CFGMGR_COUNTRY_NAME,                /* name of current country  */
    CFGMGR_COUNTRY_CODE_LIST,           /* list of supported country codes */
    CFGMGR_COUNTRY_STRUCT,              /* read the whole country structure */
    CFGMGR_COUNTRY_BLIST,              
	CFGMGR_FILTERS,						/* CntryTxlevelStructure    */
	CFGMGR_DTMF,						/* CntryDTMFStructure       */
	CFGMGR_RING_PARAMS,					/* Ring Detector timing		*/
	CFGMGR_RLSDOFFSET,					/* RLSDXrthOffset           */
	CFGMGR_THRESHOLD,
	CFGMGR_TXLEVELS,					/* CntryTxlevelStructure    */
	CFGMGR_RELAYS,						/* Relays[LAST_RELAY_ENTRY] */
	CFGMGR_SPEED_ADJUST,				/* CntryAgressSpeedIndexStructure a.k.a CountrySpeedAdjust */
    CFGMGR_SREG_LIMITS,                 /* limits for S-registers   */
    CFGMGR_PR_IGNORE_TIME,              /* ignore polarity reversal for specified non-zero time */
	CFGMGR_USER_SYMRATE,				/* user-specified symbol rate limits */
    CFGMGR_COUNTRY_CALLERID1,           /* CID type */
	CFGMGR_COUNTRY_CALLERID2,			/* */
	CFGMGR_COUNTRY_CALL_WAITING,		/* */
#if !defined NO_CID_SUPPORT
	CFGMGR_VCID_FORMAT,					/* +VCID settings */
	CFGMGR_VRID_FORMAT,					/* +VRID settings */
	CFGMGR_CALLER_ID_INFO,				/* current caller id state  */
	CFGMGR_VRID_CRC,					/* stored CID CRC			*/
	CFGMGR_VRID_TYPE,
	CFGMGR_VRID_LENGTH,
	CFGMGR_VRID_DATA,
#endif
/*#$YS$ Added for Distinctive Ring support. */
    CFGMGR_VDR_PARAMS,

	CFGMGR_CALL_WAITING,				/* call waiting info		*/
	CFGMGR_MOH,							/* MOH info					*/
#if !defined NO_PIG_SUPPORT
	CFGMGR_PCMUPSTREAM,					/* PCM upstream configuration */
#endif
#if !defined NO_QC_SUPPORT
	CFGMGR_QC,							/* quick connect info		*/
	CFGMGR_QC_PROFILE,						/* QCPROFILE */
#endif
	CFGMGR_V92_CONTROL,					/* Liat ??? */

    CFGMGR_PROFILE_STORED,        		/* PPROFILE_DATA            */
    CFGMGR_PROFILE_FACTORY,        		/* PPROFILE_DATA            */

#if !defined NO_POUNDUG_SUPPORT
    CFGMGR_POUND_UD,                    /* #UD/#UG structure        */
#endif
    CFGMGR_ATPARSER_ONLINE,              /* UINT8                   */
#if !defined NO_BL_SUPPORT
    CFGMGR_BLACK_LIST,                  /*  Black List structure    */
#endif

    /* Soft Modem Tone configurations */
	CFGMGR_TONE_PRIMARY_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_TONE_ALTERNATE_PARAMS,			/* SINGLETONEDETECT*	*/	
	CFGMGR_TONE_ATBEL_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_TONE_ATV25_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_TONE_1100_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_TONE_2130_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_TONE_2312_PARAMS,				/* SINGLETONEDETECT*	(390)*/	
	CFGMGR_TONE_2750_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_TONE_2912_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICETONEACALLPROG_PARAMS,		/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICETONEB_1100_PARAMS,			/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICETONEC_1300_PARAMS,			/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICETONEC_2100_PARAMS,			/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICETONEB_2225_PARAMS,			/* SINGLETONEDETECT*	*/	
	CFGMGR_V8BTONE_2100_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_V8BTONE_2225_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_V8BTONEC_2100_PARAMS,			/* SINGLETONEDETECT*	*/	
	CFGMGR_V8BTONEB_2225_PARAMS,			/* SINGLETONEDETECT*	*/	
	CFGMGR_CI_CENTER_TONEB_PARAMS,			/* SINGLETONEDETECT*	(400)*/	
	CFGMGR_CI_SIDE_TONEC_PARAMS,			/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICEVIEW_TONEC_2912_PARAMS,		/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICEVIEW_TONEB_2312_PARAMS,		/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICEVIEW_TONEB_2750_PARAMS,		/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICEVIEW_TONEA_2130_PARAMS,		/* SINGLETONEDETECT*	*/	
	CFGMGR_VOICEVIEW_TONEA_NOT_ST_PARAMS,	/* SINGLETONEDETECT*	*/	
	CFGMGR_TONEC_1650_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_CReSEG1_2002PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_TONE_1270_PARAMS,				/* SINGLETONEDETECT*	*/	
	CFGMGR_TONEB_980_PARAMS,				
	CFGMGR_TONE_CAS,						
	CFGMGR_TONE_AS,							/* SINGLETONEDETECT*	*/
	CFGMGR_TONE_DTMF,						

	/* Tone generator - constant configurations saved in Configuration manager */
	CFGMGR_TONE_GEN_1650,					/* TONEGEN_PARAMS*		*/
	CFGMGR_TONE_GEN_V25CALL,				/* TONEGEN_PARAMS*		*/
	CFGMGR_TONE_GEN_CNG,					/* TONEGEN_PARAMS*			(410)	*/
	CFGMGR_TONE_GEN_CED,					/* TONEGEN_PARAMS*				*/
	CFGMGR_TONE_GEN_CI,						/* TONEGEN_PARAMS*				*/
 
    /** HW - buffer switch delay. **/
    CFGMGR_BUFFERS_DELAY,                   /* UINT16*. */
   	CFGMGR_PCI_DEVICE_ID,	/*(414) */
	CFGMGR_PCI_VENDOR_ID,
 
    CFGMGR_OEM_HWDATA,                   /* HW_DATA */
    CFGMGR_OEM_DAATYPE,                   /* BYTE */
	CFGMGR_OEM_SMARTDAA_PARAMS         ,   /*  CAESAR_PARAMS */
	CFGMGR_OEM_SMARTDAA_PARAMSK3       ,   /*  CAESAR_PARAMS for Kublai3 */
	CFGMGR_OEM_SMARTDAA_PARAMSHAL      ,   /*  CAESAR_PARAMS for Hal */
    CFGMGR_OEM_SOFT_RING_PARAMS,           /* RING_PARAMS */
	CFGMGR_HW_ADAPTER_TYPE,                
 	CFGMGR_YUKON_FIRMWARE_DATA,            /* Yukon firmware */
	CFGMGR_YUKON_FIRMWARE_DELETE,          /* Delete Yukon memory allocation */

	/* country parametrs for CtryPrmsStruct */
/*  	CFGMGR_CTRY_INTCODE          ,     Country Name                   (420)*/ 
/*  	CFGMGR_CTRY_PULSE            ,     CntryPulseStructure            */ 
	CFGMGR_CTRY_TONE             ,    /* CntryToneStructure             */
	CFGMGR_CTRY_CADENCE          ,    /* CntryCadenceStructure          */
	CFGMGR_CTRY_CALLERID         ,    /* CntryCallerIDStructure         */ 
 
	CFGMGR_HSFI_TYPE         ,    /* BOOL (TRUE for SmartHSFi, FALSE for HSFi)         */ 
	/************************************************/
    /************Common configuration codes *********/
    /************************************************/
	

    CFGMGR_CENTERAL_SITE,						    /* BOOL - TRUE - for sentral site emulation */
	CFGMGR_RXSAMPLES_OUT_FILENAME,					/* char[CFGMGR_MAX_FILENAME] File name for dumping RxSamples			*/
	CFGMGR_TXSAMPLES_OUT_FILENAME,					/* char[CFGMGR_MAX_FILENAME] File name for dumping TxSamples			*/
	CFGMGR_RXLEN_OUT_FILENAME,					    /* char[CFGMGR_MAX_FILENAME]File name for dumping Rx Length			*/

	CFGMGR_RXSAMPLES_IN_FILENAME,					/* char[CFGMGR_MAX_FILENAME] File name for user RxSamples			*/
	CFGMGR_RXLEN_IN_FILENAME,					/* char[CFGMGR_MAX_FILENAME] File name for user TxSamples			*/

	CFGMGR_PRESOURCES,                              /* PVOID - pResources ( used for interrupt hook, and DMA allocations*/
    CFGMGR_HW_BASE_ADDRESS,                         /* IO/Memory Base Address                                           */
    
	/************************************************/
    /************Soft DSL configuration codes *******/
    /************************************************/	
    CFGMGR_DSL_HWIO_PARAMS,/* NILE_CFMGR_CODE_BASE, HW_IO_SOFTDSL_SET_PARAMS_T */
	CFGMGR_SETTINGS_ATUR_DESCRIPTION,
	CFGMGR_SETTINGS_HSTUR_DESCRIPTION,
	CFGMGR_SETTINGS_HSTUC_DESCRIPTION,
	CFGMGR_SETTINGS_NILECTRL_DESCRIPTION,
	CFGMGR_SETTINGS_ATUR_PARAMS,
	CFGMGR_SETTINGS_HSTUR_PARAMS,
	CFGMGR_SETTINGS_HSTUC_PARAMS,
	CFGMGR_SETTINGS_NILECTRL_PARAMS,
	CFGMGR_SETTINGS_ATUR_DATA,
	CFGMGR_SETTINGS_ATUC_DATA,
	CFGMGR_SETTINGS_HSTUR_DATA,
	CFGMGR_SETTINGS_HSTUC_DATA,
	CFGMGR_SETTINGS_NILECTRL_DATA,

	CFGMGR_G9922ATU_C_CONFIGURATION,
	CFGMGR_G9922ATU_R_CONFIGURATION,
	CFGMGR_HSTU_C_CONFIGURATION,
	CFGMGR_HSTU_R_CONFIGURATION,
	CFGMGR_NILE_CTRL_CONFIGURATION,
	CFGMGR_HS_MODULATION_DB,							/* HS_MODULATION_DB_T * */
	CFGMGR_CO_HS_MODULATION_DB,							/* CO side HS_MODULATION_DB_T */
	CFGMGR_RT_HS_MODULATION_DB,							/* RT side HS_MODULATION_DB_T */
	CFGMGR_LOWMIPS,									/* RT side HS_MODULATION_DB_T */
	CFGMGR_DSL_HWIO_SAMPLE_RATE,

/*#$YS$ */
#if !defined NO_VOICE_SUPPORT
    CFGMGR_VOICE_FIRST = 0x400,
    CFGMGR_VOICE_FDSP_PARAMS, 
    CFGMGR_VOICE_FDSP_PREV_PARAMS,
    CFGMGR_VOICE_VBT_PARAMS,
    CFGMGR_VOICE_VDID_PARAMS,
    CFGMGR_VOICE_VDT_PARAMS,
    CFGMGR_VOICE_VGR_PARAMS,
    CFGMGR_VOICE_VGT_PARAMS,
    CFGMGR_VOICE_VLS_PARAMS,
    CFGMGR_VOICE_VSD_PARAMS,
    CFGMGR_VOICE_VSM_PARAMS,
    CFGMGR_VOICE_VTS_PARAMS,
    CFGMGR_VOICE_MISC_PARAMS,
    CFGMGR_VOICE_CACHED_MISC_PARAMS,
/*    CFGMGR_VOICE_VCID_PARAMS, */
    CFGMGR_VOICE_VIT_PARAMS,
    CFGMGR_VOICE_RELAY_CONFIG,
    CFGMGR_VOICE_BAUD_RATE,
    CFGMGR_VOICE_VRA_PARAMS,
    CFGMGR_VOICE_VRN_PARAMS,
    CFGMGR_VOICE_VTD_PARAMS,
    CFGMGR_VOICE_DEFAULT_ONHOOK_RELAY,
    CFGMGR_VOICE_RX_GAIN,
    CFGMGR_VOICE_TX_GAIN,
    CFGMGR_VOICE_HOOK_CONTROL,
    CFGMGR_HANDSET_STATE,
    CFGMGR_VOICE_SUPPORTED_VSM_CODE,
    CFGMGR_VOICE_BOND_OPTIONS,
    CFGMGR_DTMF_REPORTING_ENABLE,
	#ifdef VIDEOPHONE
    CFGMGR_VOICE_PREVIOUS_MODE,
	#endif
    CFGMGR_VOICE_DETECTION_MASK,
#endif /* VOICE_SUPPORT */
#if !defined NO_BLAM_SUPPORT
    CFGMGR_BLAM_STATE,
#endif

	CFGMGR_HARDWARE_PROFILE,
	CFGMGR_HARDWARE_ID,
	CFGMGR_LICENSE_OWNER,
	CFGMGR_LICENSE_KEY,
	CFGMGR_LICENSE_STATUS,

    CFGMGR_LAST

} CFGMGR_CODE;

typedef enum tagHW_TYPE{ /* do not change the order for linux inf file sake */
	HW_TYPE_NONE = 0, 
	HW_TYPE_BASIC2, 
	HW_TYPE_ICH, 
	HW_TYPE_YUKON, 
	HW_TYPE_VIA,
	HW_TYPE_ALI,
    HW_TYPE_QUAKE,
    HW_TYPE_ELMA,
	HW_TYPE_RIO,
	HW_TYPE_CADMUS2, 
	HW_TYPE_ATI,
	HW_TYPE_SIS,
	HW_TYPE_HDA, 
	HW_TYPE_BASIC3, 
	HW_TYPE_LAST,
}HW_TYPE;
 

#endif      /* #ifndef __CONFIGCODES_H__ */
