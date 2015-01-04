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
*	$Header:   R:/pvcs68/vm/common/Cyprus Project/archives/managers/include/targetconfigtypes.h-arc   1.7   24 Dec 2003 18:13:42   shay  $
*
*****************************************************************************************/


/****************************************************************************************

File Name:			TargetConfigTypes.h

File Description:	Configuration parameter data structures specific to softmodem

*****************************************************************************************/
#if !defined __TARGETCONFIGTYPES_H__
#define __TARGETCONFIGTYPES_H__

#include "dpaloem.h"
#include "tonetype.h"

#if OS_TYPE == OS_LINUX
#define DAA_RELAY_TYPE  UINT32 /* for consistency with HCF */
#else
#define DAA_RELAY_TYPE  UINT16
#endif
#define QC_PROFILE_MAX_SIZE 3500
#pragma pack(1)

typedef struct QCProfileStructTag
{
	UINT8	ProfileValid;
	UINT8	Data [QC_PROFILE_MAX_SIZE];
} QCProfileStruct;
 

/* Structure passed with CFGMGR_DTMF_GENERATOR */
typedef struct tagDTMFGenParams {
	UINT16  m_wDigitDuration;		/* in ms */
	UINT16  m_wPauseDuration;		/* in ms */
	INT16	m_nHighBandAmplitude;	/* normed to 32768 */
	INT16	m_nLowBandAmplitude;	/* normed to 32768 */
} DTMF_GEN_PARAMS, *PDTMF_GEN_PARAMS;

#define CntryCallWaitingToneDetector SINGLETONEDETECT

typedef struct 
{

	UINT16  Segment1freq1;	/* Hz */
	UINT16  Segment1freq2;	/* Hz */
	UINT16  Segment2freq;	/* Hz */
	UINT32	Segment1Duration;  /* millisec */
	UINT32	Segment2Duration;  /* millisec */
} CR_TONE_PARAMS, *PCR_TONE_PARAMS;
 
typedef struct CNTRY_TONEPARAMS_TAG 
{
    SINGLETONEDETECT Primary;
    SINGLETONEDETECT Alternate;
    SINGLETONEDETECT VoiceToneACallProgress;
} CNTRY_TONEPARAMS_T, *PCNTRY_TONEPARAMS_T;

typedef struct OEM_TONEPARAMS_TAG
{
    SINGLETONEDETECT Tone1100;
    SINGLETONEDETECT V8bToneC2100;
    SINGLETONEDETECT V8bToneB2225;
    SINGLETONEDETECT VoiceToneB1100;
    SINGLETONEDETECT VoiceToneC1300;
    SINGLETONEDETECT VoiceToneC2100;
    SINGLETONEDETECT VoiceToneB2225;
} OEM_TONEPARAMS_T, *POEM_TONEPARAMS_T;

typedef struct CONSTANT_TONEPARAMS_TAG
{
    SINGLETONEDETECT ToneATBel;
    SINGLETONEDETECT ToneATV25;
    SINGLETONEDETECT CICenterFreq;
    SINGLETONEDETECT CISideBand;
    SINGLETONEDETECT ToneC_2912;
    SINGLETONEDETECT ToneB_2312;
    SINGLETONEDETECT ToneB_2750;
    SINGLETONEDETECT ToneA_2130;
    SINGLETONEDETECT ToneA_NOT_ST;
    SINGLETONEDETECT ToneC_1650;
    SINGLETONEDETECT Tone_CReSEG1_2002;
    SINGLETONEDETECT Tone_1270;
} CONSTANT_TONEPARAMS_T, *PCONSTANT_TONEPARAMS_T;
 
/* Dual/Single Tone generator struct */
typedef struct
{
	UINT16 freq1; /* Hz */
	UINT16 freq2;
	UINT32 OnTime;
	UINT32 OffTime;
    BOOL  bRepeatedTone;
} TONEGEN_PARAMS, *PTONEGEN_PARAMS;
 
typedef struct CONSTTONEGENPARAMS_TAG
{
	TONEGEN_PARAMS	V25CallTone	;
	TONEGEN_PARAMS	CNGTone		;
	TONEGEN_PARAMS	CEDTone		;
	TONEGEN_PARAMS	F1650Tone	;
} CONSTTONEGENPARAMS_T, *PCONSTTONEGENPARAMS_T;

typedef struct PACKED_ATTRIB tagThreshold {
	UINT16		DialThresh;
	UINT16		AltDialThresh;
	UINT16		WDialThresh;
	UINT16		AltWDialThresh;
	UINT16		ProgThresh;
	UINT16		AltProgThresh;
	UINT16		DTMFRxThresh;
	UINT16		RingThreshold;			/* Add soft ring threshold set to 0 if not use */
	INT16		RingBalance;			/* Add soft ring threshold compensation slope, set to 0 if not use*/
	UINT16      CIDThresh;
} CntryThresholdStructure;

#define DEFAULT_DC_LOOP_POINTS	5
#define DEFAULT_DC_LOOP_ENTRIES	5
#define IMPEDANCE_VALUES_NUM	4
#define NUM_OF_RESISTANCES		5	

#pragma pack()

#endif  /* !defined __TARGETCONFIGTYPES_H__ */

