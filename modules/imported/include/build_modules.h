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

#ifndef __BUILD_MODULES_H__
#define __BUILD_MODULES_H__

#if ( FRAME_WORK == FWK_VXWORKS_MIPS )

#define NO_VOICE_SUPPORT
#define NO_V80_SUPPORT
#define NO_QC_SUPPORT
#define NO_V8BIS_SUPPORT
#define NO_V8_SUPPORT
#define NO_ANSWER_MODE_SUPPORT
#define NO_MOH_SUPPORT
#define NO_CWCID_SUPPORT
#define NO_BL_SUPPORT
/*#define NO_B212_SUPPORT*/
#define NO_DIAGMGR_SUPPORT
#define NO_DUMPMGR_SUPPORT
#define NO_FAX_MODE_SUPPORT
#define NO_TEST_MODE_SUPPORT
#define NO_V34FAX_SUPPORT
#define NO_POUNDUG_SUPPORT
#define NO_SPEAKER_SUPPORT
#define NO_V44_SUPPORT
/*#define NO_CID_SUPPORT */
#define NO_PIG_SUPPORT
#define NO_JAPANESE_CID_SUPPORT
#define NO_FRENCH_CID_SUPPORT
#define NO_BLAM_SUPPORT 
/*#define NO_PULSE_SUPPORT */
#define NO_DTMF_CID_SUPPORT
/*#define NO_DCP_SUPPORT */
#define NO_V90_SUPPORT

#endif /* ( FRAME_WORK == FWK_VXWORKS_MIPS ) */

#if ( FRAME_WORK == FWK_MACOSX )

#define NO_V80_SUPPORT
#define NO_VOICE_SUPPORT
#define NO_V34FAX_SUPPORT
#define NO_BLAM_SUPPORT
#define NO_V8BIS_SUPPORT
#define SHORT_POUND_UG
/*
#define NO_V90_SUPPORT
#define NO_MOH_SUPPORT
#define NO_CWCID_SUPPORT
#define NO_QC_SUPPORT
#define NO_PIG_SUPPORT
#define NO_BL_SUPPORT
#define NO_DTMF_CID_SUPPORT
*/

#endif /* ( FRAME_WORK == FWK_MACOSX ) */

#if ( FRAME_WORK == FWK_LINUX_SOFTK56 )

/* only exclude stuff which isn't supported in all cases;
 * other defines are managed in linux/makefile
 */

#define NO_V80_SUPPORT
#define NO_VOICE_SUPPORT
#define NO_V34FAX_SUPPORT
#define NO_V8BIS_SUPPORT
#if !defined(MT_SUPPORT)
#define NO_MT_SUPPORT
#endif

#endif

#if ( FRAME_WORK == FWK_CORSICA_ADS )
#define NO_VOICE_SUPPORT
#define NO_V80_SUPPORT
#define NO_QC_SUPPORT
#define NO_V8BIS_SUPPORT
/* #define NO_V8_SUPPORT */
#define NO_V90_SUPPORT
#define NO_ANSWER_MODE_SUPPORT
#define NO_MOH_SUPPORT
#define NO_CWCID_SUPPORT
#define NO_BL_SUPPORT
/* #define NO_B212_SUPPORT*/
/* #define NO_DIAGMGR_SUPPORT*/
#define NO_DUMPMGR_SUPPORT
#define NO_FAX_MODE_SUPPORT
#define NO_TEST_MODE_SUPPORT
#define NO_V34FAX_SUPPORT
/* #define NO_POUNDUG_SUPPORT*/
#define NO_SPEAKER_SUPPORT
#define NO_V44_SUPPORT
/* #define NO_CID_SUPPORT */
#define NO_PIG_SUPPORT
#define NO_JAPANESE_CID_SUPPORT
#define NO_FRENCH_CID_SUPPORT
/* #define NO_BLAM_SUPPORT */
/*#define NO_PULSE_SUPPORT */
#define NO_DTMF_CID_SUPPORT
/* #define NO_DCP_SUPPORT */
#define SHORT_POUND_UG
#define BLAM22

#endif

#endif /* __BUILD_MODULES_H__ */
