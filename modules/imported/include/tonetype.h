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
 * File: tonetype.h
 *
 * tonetype.h - typedefs for DTMFGenerator, DTMFDetector, SingleToneDetector, DualToneGenerator
 *
 *----------------------------------------------------------------------------
 */
 
#ifndef __TONETYPE_H__
#define __TONETYPE_H__
#include "codesdeps.h"

#define NUM_OF_DIGITS	16

#ifndef FRAME_WORK_IMPORTED
#include "statusbase.h"

/*tones.h*/
typedef enum {EVENT_DTMF_0 = TONES_BASE_STATUS, EVENT_DTMF_1 , EVENT_DTMF_2, EVENT_DTMF_3, 
				EVENT_DTMF_4, EVENT_DTMF_5, EVENT_DTMF_6, EVENT_DTMF_7,
					EVENT_DTMF_8, EVENT_DTMF_9,  EVENT_DTMF_STAR, EVENT_DTMF_POUND, 					  
					EVENT_DTMF_A,EVENT_DTMF_B, EVENT_DTMF_C, EVENT_DTMF_D, EVENT_DIAL_DIGIT, 	
					EVENT_TONE_ON, EVENT_TONE_OFF, EVENT_QUEUE_EMPTY, 
					EVENT_TONE_ANSAM, EVENT_TONE_ANS,
					EVENT_TONE_ANSAM_OFF, EVENT_TONE_ANS_OFF,
					EVENT_DTMF_NONE} TONESTATUS, *PTONESTATUS;

typedef enum {		DIGIT_DTMF_0, DIGIT_DTMF_1, DIGIT_DTMF_2, DIGIT_DTMF_3,
					DIGIT_DTMF_4, DIGIT_DTMF_5, DIGIT_DTMF_6, DIGIT_DTMF_7,
					DIGIT_DTMF_8, DIGIT_DTMF_9,  DIGIT_DTMF_STAR, DIGIT_DTMF_POUND, 					  
					DIGIT_DTMF_A, DIGIT_DTMF_B, DIGIT_DTMF_C, DIGIT_DTMF_D, DIGIT_DTMF_NONE} DIGIT_DTMF;
#endif /* FRAME_WORK_IMPORTED */


/* The following struct is used for  single tone
 detection.*/
typedef enum /* sinals generated while looking for tone detection.*/
{
	FILTER_NONE,
	FILTER_1800_NOTCH, 
	FILTER_1400_LOWPASS, /*tones 1650, 2250 2850*/
	FILTER_1600_HIGHPASS,
	FILTER_1100_LOWPASS,
} TONEDET_FILTER; 




typedef enum /* sinals generated while looking for tone detection. */
{
	TX_SIGNAL_NONE,
	TX_1800, 
	TX_USB1, /* tones 1650, 2250 2850 */
	TX_1650,
	TX_980,
	TX_1300,
	TX_1100,
    TX_DATA_MODULATION
} TX_SIGNALS; 

typedef struct tagSingleTone
{
	WORD  CenterFreq;	   /* Hz */
	WORD  Bandwidth;		/* Hz */
	DWORD ResponseTime;	/* Millisec */
	short OnThreshold;	/* dBm */
	short OffThreshold;	/* dBm */
	eTONEDETECT_TYPE ToneID;
   DWORD OnDebounce;    /* Millisec */
   DWORD OffDebounce;   /* Millisec */
} SINGLETONEDETECT, *PSINGLETONEDETECT;
typedef struct 
{
	PSINGLETONEDETECT pSingleToneDetect;
	TX_SIGNALS TxSignalPresent;
} SINGLE_TONE_PARAMS, *PSINGLE_TONE_PARAMS;

typedef struct tagDTMFParamsIn
{
	DWORD	MaxSpeachEnergy; /*@ what units? */
	DWORD	LowGroupFreqDeviation; 
	DWORD	HighGroupFreqDeviation;
	DWORD	NegativeTwistControl;
	DWORD	PositiveTwistControl;
	DWORD	MaxEnergyHitTime;
} DTMFDETECT,*PDTMFDETECT;


typedef struct tagDualTone
{
	WORD freq1; /* Hz */
	WORD freq2;
	WORD amplitude1;
	WORD amplitude2;
	DWORD OnTime;
	DWORD OffTime;
} DUALTONEGEN, *PDUALTONEGEN;

/* contains the DTMF values for complex impedance. */
typedef struct DTMF_COMP_LEVEL_Tag
{
   int lBand[4];
   int hBand[4];
} OEMDTMFCompLevelStruct;

typedef struct tagDTMFParamsOut
{
	DWORD	Duration;  /* millisec */
	DWORD	InterDigitDelay;  /* millisec */
#ifndef SOFT_PUMP
	long	LowBandPower;   /* dBm */
	long	HighBandPower;  /* dBm */
#else
	OEMDTMFCompLevelStruct* DTMFCompLevel;
#endif
 
	DPDIALDIGITS *pDigits; /* digits to dial buffer */
	DWORD nDigits; /* how many digits in the Digits buffer */
}DTMF_PARAMS_OUT,*PDTMF_PARAMS_OUT;

/* The following struct include the parameters for a dtmf detector. */
typedef struct 
{
	DWORD	MaxSpeachEnergy; 
	DWORD	LowGroupFreqDeviation; 
	DWORD	HighGroupFreqDeviation;
	DWORD	NegativeTwistControl;
	DWORD	PositiveTwistControl;
	DWORD	MaxEnergyHitTime;
} OLD_DTMFDETECT,*POLD_DTMFDETECT;

/* The following struct is used  for single and dual 
 tones generation. In order to describe a single tone, 
 set freq2 to 0  */
typedef struct 
{
	WORD freq1;	/* Hz */
	WORD freq2;	/* Hz */
	short amplitude1;	/* dBm */
	short amplitude2;	/* dBm */
	char MinAmpilitude; /* -dBm */
	char MaxAmpilitude; /* -dBm */
	char defaultAmpilitude; /* -dBm */
	char OffsetAmpilitude; /* -dBm */
	DWORD OnTime;	/* millisec */
	DWORD OffTime;	/* millisec */
} OLD_DUALTONEGEN, *POLD_DUALTONEGEN;

/* The following struct is used  for single and dual
 tones generation when used in configure instead of setup
 so that a single configuration will path the flag together with the struct
 The EXT prefix stands for 'extended' */

typedef struct 
{
	POLD_DUALTONEGEN pDualToneGen;
	BOOL			Repeat;
} OLD_DUALTONEGEN_EXT, *POLD_DUALTONEGEN_EXT;

/* The following struct include the parameters for a dtmf generator. */
typedef struct 
{
	DWORD	Duration;  /* millisec */
	DWORD	InterDigitDelay;  /* millisec */
	long	LowBandPower[NUM_OF_DIGITS];   /* 0.1 -dBm */
	long	HighBandPower[NUM_OF_DIGITS];  /* 0.1 -dBm */
}OLD_DTMF_PARAMS_OUT,*POLD_DTMF_PARAMS_OUT;

/* TONE_OFF and TONE_ON identifiers for single/dual tones, the rest identify dtmf tones
 Generator of ANS/ANSAM tone */
typedef struct 
{
	BOOL AmOn;	/* TRUE when ANSAM */
	BOOL PmOn;  /* TRUE when Phase Modulation */
	char MinAmpilitude; /* -dBm */
	char MaxAmpilitude; /* -dBm */
	char defaultAmpilitude; /* -dBm */
	char OffsetAmpilitude; /* -dBm */
} OLD_ANS_PARAMS_OUT, *POLD_ANS_PARAMS_OUT;

typedef struct 
{
	DWORD	DummyParam; /* for octopus - c compiler does not accept empty structures*/
} OLD_ANS_PARAMS_IN, *POLD_ANS_PARAMS_IN;

typedef struct 
{

	WORD Segment1freq1;	/* Hz */
	WORD Segment1freq2;	/* Hz */
	WORD Segment2freq;	/* Hz */
	DWORD	Segment1Duration;  /* millisec */
	DWORD	Segment2Duration;  /* millisec */
	char MinAmpilitude; /* -dBm */
	char MaxAmpilitude; /* -dBm */
	char defaultAmpilitude; /* -dBm */
	char OffsetAmpilitude; /* -dBm */
} OLD_CR_TONE_PARAMS, *POLD_CR_TONE_PARAMS;

#endif /* __TONETYPE_H__ */
