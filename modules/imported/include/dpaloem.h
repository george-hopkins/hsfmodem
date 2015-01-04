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

/*----------------------------------------------------------------------------
 *
 * Copyright (C) 1996-2001 Conexant Systems Inc. 
 * All rights reserved.
 *
 *----------------------------------------------------------------------------
 *
 * File: DpalOem.h
 *
 *----------------------------------------------------------------------------
 */
 
#ifndef __DPALOEM_H__
#define __DPALOEM_H__



 
 
/*                              */
/* Struct to hold Caesar params */
/*                              */

#if (USE_PRAGMA_PACK == 1)
#pragma pack(push,1)   /* Packing compatible with WinAC */
#endif

#define DEFAULT_DC_LOOP_POINTS	5
#define DEFAULT_DC_LOOP_ENTRIES	5
#define IMPEDANCE_VALUES_NUM	4
#define NUM_OF_RESISTANCES		5	

typedef struct PACKED_ATTRIB CAESAR_PARAMS_TAG
{
	DWORD	R6;
	DWORD	R26;
	DWORD	R28;
	DWORD	R30_C1;	/* for Caesar1 (20434) */
	DWORD	R30_C2;	/* for Caesar2 (20463) */
	DWORD	R38;

	DWORD	PQac;
	DWORD	PQdc;

	BYTE	ImpedanceValues[IMPEDANCE_VALUES_NUM];

	BYTE	DCLoopVoltagePoint[DEFAULT_DC_LOOP_ENTRIES][DEFAULT_DC_LOOP_POINTS];
	BYTE	DCLoopCurrenPoint[DEFAULT_DC_LOOP_ENTRIES][DEFAULT_DC_LOOP_POINTS];
	BYTE	DCDACInitPoint[DEFAULT_DC_LOOP_ENTRIES];  /* LKS28may99: 5 init point for 5 DCloops (1 value each) */

#if ( PRJ_FLAVOUR == PRJFLVR_CYPRUS )
	/*
		a bug in gnu assembler version 2.9.5 will give a bad value for
		the field below (Flags). This forces alignment of 4 and 
		workaround the bug. It should be removed if future versions of
		GAS fix this (Shai).
	*/
	BYTE	Padding;
#endif

	DWORD	Flags;
	WORD	RingThresh;
	WORD	CIDThresh;
} CAESAR_PARAMS, *PCAESAR_PARAMS;

typedef struct PACKED_ATTRIB HW_DATA_TAG
{
	WORD	VoiceCodecPresent	:   1; /* 0 */
	WORD	ModemMicGain		:   3; /* 1-3 */
	WORD	ModemMicAttenuation	:   1; /* 4 */
	WORD	DACInputSource		:   3; /* 5-7 */
	WORD	ModemSpkDriver		:   1; /* 8 */
	WORD	OSCOff				:   1; /* 9 */
	WORD	Riptide				:   1; /* 10 */
	WORD	WakeUpAfterClose	:   1; /* 11 */
	WORD	EnableInterrupts	:   1; /* 12 */
	WORD	CharlemagneHandset	:   1; /* 13 */
	WORD	PDC_ADPInput		:   1; /* 14 */
	WORD	PME_with_FET		:   1; /* 15 */
	WORD	DC_Adjust			:   1; /* 16 */
	WORD	MuteByAnalogSpeaker	:   1; /* 17 */
	WORD	NT4OpenPortPatch	:   1; /* 18 */
	WORD	ExternalFilter		:   1; /* 19	use Churchill External filter for ring detect */
	WORD	Churchill			:   1; /* 20 True only for Churchill */
	WORD	UpdateConfigSpace	:   1; /* 21 */
	WORD	ExternalIA      	:   1; /* 22 */
	WORD	EnableAudioGPI  	:   1; /* 23 */
	WORD	MoonWOR				:   1; /* 24 */
	WORD	SonyVioThrottle		:   1; /* 25 */
	WORD	MoonDmaWatchdog		:   1; /* 26 */
	WORD	DisableSpeedStep	:   1; /* 27 */
	WORD	HadsetExlusive		:   1; /* 28 */
	WORD    DisableWaveGenerator:   1; /* 29 */
	WORD	DspAtIrql			:	2; /*30-31  0-Always run Dsp at Dpc; 
                                                1-Run Dsp at Irql during retrain;
                                                2-Run Dsp at Irql during retrain and when significant
                                                  Dpc latency is detected;
                                                3-Always run Dsp at Irql
                                     */
    WORD    BanisterWorkaroundDisable:   1; /* 32 */
    WORD    VIA8233                  :   1; /*33 */
    WORD    Yukon                    :   1; /*34       */
	WORD    SpkpDir					 :   2; /* 35-36 */
	WORD	AclinkLatencyHdwEnable   :   1; /* 37 */
    WORD    nVidia                   :   1; /*38 */
	WORD	MaximizeIrqlForIsr		 :	 1; /* 39 */
   	WORD    DisableCpuThrottle       :   1; /*40*/
	WORD    DisableHWPulseDial       :   1; /*41*/
	WORD    EnableUnMuteSpeakers     :   1; /*42*/
    WORD    EnableVoiceRxTxMix       :   1; /*43*/
	WORD	AtiGpioIntSupported		 :	 1; /*44*/
	WORD    EnableAliPlusFifo        :   1; /*45*/
	WORD    Enable48KHFifo           :   1; /*46*/
	WORD    EnableDMARingDetect      :   1; /*47*/
	WORD	EnableSilabOffhookNoiseWA	 :	 1; /*48, byte 6 bit0 for billionton silab codec*/
	WORD	f12KhzSRSupported		 : 1; /*49,byte 6 bit 1 */
	WORD	EnableBrycePLL24Mhz		 : 1; /*50,byte 6 bit 2 */
	WORD	EnableBryceDLL32Khz		 : 1; /*51,byte 6 bit 3 */
	WORD	ESDPciConfigWA			 : 1; /*52,byte 6 bit 4 */
    WORD    VIA_CstateWA             :  1; /* 53 */
    WORD    DisableAliSdataInWOR	 :  1; /* 54 , disable wake on ring from SdataIn for ALI*/
	WORD	ForceTurnOffBitClk		 :  1; /* 55, byte 6 bit 7 force to set PR4 when going to D3 */ 
    WORD    Reserved2                :  8; /* 56-63 */
} HW_DATA, *PHW_DATA;
 
/* added for dlg support */
typedef struct PACKED_ATTRIB DLG_PARAMS_Tag {
	BYTE Enabled;
	BYTE Polarity;
	BYTE PinIndex;
	WORD Debounce;
} DLG_PARAMS, *PDLG_PARAMS; 
 
typedef struct PACKED_ATTRIB HandSet_PARAMS_Tag
{
	BYTE Enabled;
	BYTE Polarity;
	BYTE GpioNumber;
	WORD Lcs_Debounce_Time;
} HandSet_PARAMS, *PHandSet_PARAMS; 

/* VIA CoreLogic Parameters */
typedef struct PACKED_ATTRIB VIA_CORELOGIC_PARAMS_TAG
{
	WORD CheckDataValidBit;
	WORD CodecAccessTimeOut;
	WORD Reserved1;
	WORD Reserved2;
} VIA_CORELOGIC_PARAMS,*PVIA_CORELOGIC_PARAMS;

typedef struct PACKED_ATTRIB CODEC_ID_FORMAT_TAG
{
	WORD VendorId1;
	WORD VendorId2;
	WORD IdType;
	WORD Reserved1;
	WORD Reserved2;
	WORD Reserved3;
} CODEC_ID_FORMAT,*PCODEC_ID_FORMAT;

typedef struct PACKED_ATTRIB CODEC_ID_PARAMS_TAG
{
	DWORD				NumberOfIds;
	PCODEC_ID_FORMAT	pCodecLists;
} CODEC_ID_PARAMS,*PCODEC_ID_PARAMS;
/*                                         */
/* struct to hold board related papameters */
/*                                         */
typedef struct PACKED_ATTRIB BOARD_PARAMS_TAG
{
	BOOL			f3VoltsIA;			/* TRUE when it's 3.3 volt (should add 4 dB) */
	BOOL			fSpkMuteHWPresent;	/* speaker can be muted */
	BOOL			fSpkMuteHWPolarity;	/* spekar mute polarity. */
										/* when true - we should set a GPIO to mute */
	DWORD			OEMSpkrMuteDelay;	/* in ms */
 
	WORD			wDAAType;			/* what DAA is it ? ( soft,smart...) */
	CAESAR_PARAMS	CaesarParams;		/* Specific parameters for Caesar */
	CAESAR_PARAMS	CaesarParamsK3;		/* Specific parameters for Caesar on Kublai3 (Endurance) */
	CAESAR_PARAMS	HalParams;			/* Specific parameters Hal */
	BOOL			fPmeWithInt;		/* Generate PME with/without interrupt. */
	BYTE			VoiceCodecMicGain;
	BOOL			fClearPME;			/* Always clear PME status bit. */
	
	HW_DATA			HwData;

	DLG_PARAMS		DlgParams;			/* Parameters for activation of digital line detection */
	HandSet_PARAMS	HandSetParams;
 
#ifdef WINTEST
	DWORD			DeviceId;		/* for ModemHelper uses. */
	DWORD			VendorId;
	DWORD			HwType;
#endif
	BOOL			fLookForExternalAudioStreams;	/* Specifies to CSampler whether it needs to evan try external source for audiostreams.																		*/

	VIA_CORELOGIC_PARAMS ViaParams;
	DWORD			PllStartupDelay;
	CODEC_ID_PARAMS CodecIdParams;

} BOARD_PARAMS, *PBOARD_PARAMS;


typedef struct PACKED_ATTRIB CAESAR_CONTROL_PARAMS_TAG
{
	WORD	OH		:	1;
	WORD	TXM		:	1;
	WORD	EICO	:	1;
	WORD	RD_CID	:	1;
	WORD	HS		:	1;
	WORD	GPIO	:	3;
	WORD	RMake	:	2;
	WORD	DCLoop	:	3;
	WORD	IMP		:	2;
	WORD	EXT		:	1;
} CAESAR_CONTROL_PARAMS, *PCAESAR_CONTROL_PARAMS;

#define SIMPLE_DAA		0
#define CAESAR			1
#define SOFT_DAA		2
#define SI3054_DAA		3
#define SI3055_DAA		4

typedef struct{
	DWORD DCoffset;
	DWORD CalibrationTime; /*mSec */
	DWORD EstimationTime;  /*mSec */
} DC_CALC_PARAMS, *PDC_CALC_PARAMS;

typedef struct {
   LONG dwAmplificationLevel; /* The desired level should be value between 0-255 */
   /* Those 2 value define the linear 0-255 scale in dBs */
   LONG dwMin_dB;			/* the Maximum amplification in dB */
   LONG dwMax_dB;			/* the Minimum amplification in dB */
} VOLUME_AMPLIFICATION_PARAMS, *PVOLUME_AMPLIFICATION_PARAMS; 


#define DCCALC_IDLE_TIME     10000	/** We want it to start the calculation immediatly (Liat). **/

#if (USE_PRAGMA_PACK == 1)
#pragma pack(pop)   /* Packing compatible with our VXDs */
#endif
 
#endif /* __DPALOEM_H__ */

