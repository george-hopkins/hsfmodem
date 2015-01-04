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

File Name:			TestDebug.h

File Description:		

*****************************************************************************************/


/****************************************************************************************
*****************************************************************************************
***                                                                                   ***
***                                 Copyright (c) 2000 - 2004                         ***
***                                                                                   ***
***                                Conexant Systems, Inc.                             ***
***                             Personal Computing Division                           ***
***                                                                                   ***
***                                 All Rights Reserved                               ***
***                                                                                   ***
*****************************************************************************************
*****************************************************************************************/


#ifndef __TESTDEBUG_H__
#define __TESTDEBUG_H__


#if (OCT_SW_MODEM == 1)
#include "osservices.h"
#endif /* OCT_SW_MODEM */

#ifdef ENABLE_BLAM_SCR
    #ifdef  NO_BLAM_SUPPORT
        #undef NO_BLAM_SUPPORT
    #endif
#else
    #ifndef NO_BLAM_SUPPORT
        #define NO_BLAM_SUPPORT
    #endif
#endif

#if !defined NO_BLAM_SUPPORT && !defined USE_TRACE
    #define USE_TRACE
#endif


/*#if defined(USE_TRACE) || defined(USE_TRACE_ONLY)*/

#ifdef USE_TRACE_ONLY
#undef ulTraceMask
#undef ulLogThread
#define ulTraceMask (USE_TRACE_ONLY)
#define ulLogThread 0xdeadbeef
#else
    extern unsigned long ulTraceMask;
    extern unsigned long ulLogThread;
#endif

    #define	T__FATAL_ERROR	0x80000000
    #define T__ERROR		0x40000000
    #define T__ASSERT		0x20000000


	/*arbitrary at the moment */
	#define MAX_NUM_RTRACE_IDS 1024

    enum OFFSET_DEFINITION {
		SCR_CLIENTS_OFFSET	=	0,
		CONTROLLER_OFFSET	=	128,
		FAX_OFFSET			=	256,
		DATA_OFFSET			=	512
	};
    
	/*NOTE:  New trace types can be added into this enum anywhere and tracing should */
	/*       work after a recompile.  However, the BLAM application uses this enum */
	/*       to create its database and has trouble parsing comments within it so */
	/*       at the present time PLEASE ADD NO COMMENTS WITHIN THIS ENUM!!! */
	/*       ALSO: Note that any changes to this enum needs corresponding (order */
	/*       dependent changes tot he BLAM_Filters_Default list below.... */

        /* NEW NOTE:  It is important to keep the order of definitions in this enum in sync
           with the BLAM_FILTERS_DEFAULTS definition of defaults below.  This means you will most likely
           find it EASIER to add any new categrories at the END rather than have to fiddle with adjusting
           the BLAM_FILTERS_DEFAULTS definition
        */

	enum RTRACE_GROUP {
        T__DUMMY,                            /*ALL/DUMMY*/
		T__CALLPROG,                         /*ALL/Controller/Callprogress*/
		T__SESSIONMGR,                       /*ALL/Manager/Session*/
		T__STREAMMGR,                        /*ALL/Manager/Stream*/
		T__DATASESSION,                      /*ALL/Session/Data*/
		T__MNP,	                             /*ALL/EC/MNP*/
		T__V42,	                             /*ALL/EC/V42*/
		T__V42BIS,                           /*ALL/EC/V42B*/
		T__FAXSESSION,	                     /*ALL/Session/Fax*/
		T__FAXPIM,                           /*ALL/Controller/FaxPim*/
		T__ATPARSER,                         /*ALL/Controller/ATParser*/
		T__COMMCTRL,                         /*ALL/Controller/ComCtrl*/
		T__UTILS,                            /*ALL/Controller/Utils*/
		T__RTMGR,                            /*ALL/Manager/RT*/
		T__CONFIGMGR,                        /*ALL/Manager/Config*/
		T__V8BIS,                            /*ALL/Session/V8Bis*/
		T__STATMGR,	                         /*ALL/Manager/Stat*/
		T__V8,                               /*ALL/Session/V8*/
		T__PROFILE,                          /*ALL/Profile*/
		T__NVMMGR,                           /*ALL/Manager/NVM*/
		T__TESTSESSION,                      /*ALL/Session/Test*/
		T__CALLERID,                         /*ALL/V92/CallerID*/
		T__MOH,                              /*ALL/V92/MOH*/
		T__QC,                               /*ALL/V92/QC*/
		T__DIAG,                             /*ALL/Session/Diag*/
    	T__CW,                               /*ALL/V92/CW*/
    	T__DEVMGR,                           /*ALL/Devmgr*/
    	T__AUTOMODE_EL,                      /*ALL/Devmgr/Automode_El*/
    	T__DATA_EL,                          /*ALL/Devmgr/Data_El*/
        T__DEVMGR_PTT,                       /*ALL/Devmgr/PTT*/
    	T__DIALER_EL,                        /*ALL/Devmgr/Dialer_El*/
    	T__RINGDET,                          /*ALL/Devmgr/RingDet*/
    	T__TONEEL,                           /*ALL/Devmgr/Tone_El*/
        T__HDLC,                             /*ALL/HDLC*/
        T__SIGNAL,                           /*ALL/Devmgr/SignalMgr*/
        T__SOFT_FAX_EL,                      /*ALL/FAX_EL*/
        T__CAESAR,                           /*ALL/Caesar*/
                                    

/*****************************************************************************/
/*********************************   F S K   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define T__FSK_CONFIGURE                  "ALL/FSK/Configure"
#else
        T__FSK_CONFIGURE,                /*ALL/FSK/Configure*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__FSK_ANSWER_RX                  "ALL/FSK/Answer/Rx"
#else
        T__FSK_ANSWER_RX,                /*ALL/FSK/Answer/Rx*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__FSK_CALLER_RX                  "ALL/FSK/Caller/Rx"
#else
        T__FSK_CALLER_RX,                /*ALL/FSK/Caller/Rx*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__FSK_ANSWER_TX                  "ALL/FSK/Answer/Tx"
#else
        T__FSK_ANSWER_TX,                /*ALL/FSK/Answer/Tx*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__FSK_CALLER_TX                  "ALL/FSK/Caller/Tx"
#else
        T__FSK_CALLER_TX,                /*ALL/FSK/Caller/Tx*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__ANSDET                         "ALL/AnsDet"
#else
        T__ANSDET,                       /*ALL/AnsDet*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__TONEDET                        "ALL/ToneDet"
#else
        T__TONEDET,                      /*ALL/ToneDet*/
#endif
/*****************************************************************************/
/*********************************   V 2 2   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define T__V22_CONFIGURE                  "ALL/V22/Configure"
#else
        T__V22_CONFIGURE,                /*ALL/V22/Configure*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_RUNTIMECOMMAND             "ALL/V22/RunTimeCommand"
#else
        T__V22_RUNTIMECOMMAND,           /*ALL/V22/RunTimeCommand*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_MESSAGES            "ALL/V22/Answer/Messages"
#else
        T__V22_ANSWER_MESSAGES,          /*ALL/V22/Answer/Messages*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_MESSAGES            "ALL/V22/Caller/Messages"
#else
        T__V22_CALLER_MESSAGES,          /*ALL/V22/Caller/Messages*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_FRED                "ALL/V22/Answer/Fred"
#else
        T__V22_ANSWER_FRED,              /*ALL/V22/Answer/Fred*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_FRED_STATES         "ALL/V22/Answer/Fred/States"
#else
        T__V22_ANSWER_FRED_STATES,       /*ALL/V22/Answer/Fred/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_FRED_SUMMARY        "ALL/V22/Answer/Fred/Summary"
#else
        T__V22_ANSWER_FRED_SUMMARY,      /*ALL/V22/Answer/Fred/Summary*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_RX_STATES           "ALL/V22/Answer/Rx/States"
#else
        T__V22_ANSWER_RX_STATES,         /*ALL/V22/Answer/Rx/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_RX_RATES            "ALL/V22/Answer/Rx/Rates"
#else
        T__V22_ANSWER_RX_RATES,          /*ALL/V22/Answer/Rx/Rates*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_RX_MSE              "ALL/V22/Answer/Rx/Mse"
#else
        T__V22_ANSWER_RX_MSE,            /*ALL/V22/Answer/Rx/Mse*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_RX_EQ              "ALL/V22/Answer/Rx/Eq"
#else
        T__V22_ANSWER_RX_EQ,            /*ALL/V22/Answer/Rx/Eq*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_RX_MISC             "ALL/V22/Answer/Rx/Misc"
#else
        T__V22_ANSWER_RX_MISC,           /*ALL/V22/Answer/Rx/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_RX_TIMEOUTS         "ALL/V22/Answer/Rx/Timeouts"
#else
        T__V22_ANSWER_RX_TIMEOUTS,       /*ALL/V22/Answer/Rx/Timeouts*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_FRED                "ALL/V22/Caller/Fred"
#else
        T__V22_CALLER_FRED,              /*ALL/V22/Caller/Fred*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_FRED_STATES         "ALL/V22/Caller/Fred/States"
#else
        T__V22_CALLER_FRED_STATES,       /*ALL/V22/Caller/Fred/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_FRED_SUMMARY        "ALL/V22/Caller/Fred/Summary"
#else
        T__V22_CALLER_FRED_SUMMARY,      /*ALL/V22/Caller/Fred/Summary*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_RX_STATES           "ALL/V22/Caller/Rx/States"
#else
        T__V22_CALLER_RX_STATES,         /*ALL/V22/Caller/Rx/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_RX_RATES            "ALL/V22/Caller/Rx/Rates"
#else
        T__V22_CALLER_RX_RATES,          /*ALL/V22/Caller/Rx/Rates*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_RX_MSE              "ALL/V22/Caller/Rx/Mse"
#else
        T__V22_CALLER_RX_MSE,            /*ALL/V22/Caller/Rx/Mse*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_RX_EQ              "ALL/V22/Caller/Rx/Eq"
#else
        T__V22_CALLER_RX_EQ,            /*ALL/V22/Caller/Rx/Eq*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_RX_MISC             "ALL/V22/Caller/Rx/Misc"
#else
        T__V22_CALLER_RX_MISC,           /*ALL/V22/Caller/Rx/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_RX_TIMEOUTS         "ALL/V22/Caller/Rx/Timeouts"
#else
        T__V22_CALLER_RX_TIMEOUTS,       /*ALL/V22/Caller/Rx/Timeouts*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_TX_STATES           "ALL/V22/Answer/Tx/States"
#else
        T__V22_ANSWER_TX_STATES,         /*ALL/V22/Answer/Tx/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_ANSWER_TX_RATES            "ALL/V22/Answer/Tx/Rates"
#else
        T__V22_ANSWER_TX_RATES,          /*ALL/V22/Answer/Tx/Rates*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_TX_STATES           "ALL/V22/Caller/Tx/States"
#else
        T__V22_CALLER_TX_STATES,         /*ALL/V22/Caller/Tx/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V22_CALLER_TX_RATES            "ALL/V22/Caller/Tx/Rates"
#else
        T__V22_CALLER_TX_RATES,          /*ALL/V22/Caller/Tx/Rates*/
#endif
/*****************************************************************************/
/*********************************   V 3 2   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define T__V32_CONFIGURE                  "ALL/V32/Configure"
#else
        T__V32_CONFIGURE,                /*ALL/V32/Configure*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_RUNTIMECOMMAND             "ALL/V32/RunTimeCommand"
#else
        T__V32_RUNTIMECOMMAND,           /*ALL/V32/RunTimeCommand*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_MESSAGES            "ALL/V32/Answer/Messages"
#else
        T__V32_ANSWER_MESSAGES,          /*ALL/V32/Answer/Messages*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_EC_MISC             "ALL/V32/Answer/EC/Misc"
#else
        T__V32_ANSWER_EC_MISC,           /*ALL/V32/Answer/EC/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_EC_NEAR             "ALL/V32/Answer/EC/Near"
#else
        T__V32_ANSWER_EC_NEAR,           /*ALL/V32/Answer/EC/Near*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_EC_FAR              "ALL/V32/Answer/EC/Far"
#else
        T__V32_ANSWER_EC_FAR ,           /*ALL/V32/Answer/EC/Far*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_MESSAGES            "ALL/V32/Caller/Messages"
#else
        T__V32_CALLER_MESSAGES,          /*ALL/V32/Caller/Messages*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_EC_MISC             "ALL/V32/Caller/EC/Misc"
#else
        T__V32_CALLER_EC_MISC,           /*ALL/V32/Caller/EC/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_EC_NEAR             "ALL/V32/Caller/EC/Near"
#else
        T__V32_CALLER_EC_NEAR,           /*ALL/V32/Caller/EC/Near*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_EC_FAR              "ALL/V32/Caller/EC/Far"
#else
        T__V32_CALLER_EC_FAR ,           /*ALL/V32/Caller/EC/Far*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_FRED                "ALL/V32/Answer/Fred"
#else
        T__V32_ANSWER_FRED,              /*ALL/V32/Answer/Fred*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_FRED_STATES         "ALL/V32/Answer/Fred/States"
#else
        T__V32_ANSWER_FRED_STATES,       /*ALL/V32/Answer/Fred/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_FRED_SUMMARY        "ALL/V32/Answer/Fred/Summary"
#else
        T__V32_ANSWER_FRED_SUMMARY,      /*ALL/V32/Answer/Fred/Summary*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_RX_STATES           "ALL/V32/Answer/Rx/States"
#else
        T__V32_ANSWER_RX_STATES,         /*ALL/V32/Answer/Rx/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_RX_RATES            "ALL/V32/Answer/Rx/Rates"
#else
        T__V32_ANSWER_RX_RATES,          /*ALL/V32/Answer/Rx/Rates*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_RX_MSE              "ALL/V32/Answer/Rx/Mse"
#else
        T__V32_ANSWER_RX_MSE,            /*ALL/V32/Answer/Rx/Mse*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_RX_EQ              "ALL/V32/Answer/Rx/Eq"
#else
        T__V32_ANSWER_RX_EQ,            /*ALL/V32/Answer/Rx/Eq*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_RX_MISC             "ALL/V32/Answer/Rx/Misc"
#else
        T__V32_ANSWER_RX_MISC,           /*ALL/V32/Answer/Rx/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_RX_TIMEOUTS         "ALL/V32/Answer/Rx/Timeouts"
#else
        T__V32_ANSWER_RX_TIMEOUTS,       /*ALL/V32/Answer/Rx/Timeouts*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_FRED                "ALL/V32/Caller/Fred"
#else
        T__V32_CALLER_FRED,              /*ALL/V32/Caller/Fred*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_FRED_STATES         "ALL/V32/Caller/Fred/States"
#else
        T__V32_CALLER_FRED_STATES,       /*ALL/V32/Caller/Fred/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_FRED_SUMMARY        "ALL/V32/Caller/Fred/Summary"
#else
        T__V32_CALLER_FRED_SUMMARY,      /*ALL/V32/Caller/Fred/Summary*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_RX_STATES           "ALL/V32/Caller/Rx/States"
#else
        T__V32_CALLER_RX_STATES,         /*ALL/V32/Caller/Rx/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_RX_RATES            "ALL/V32/Caller/Rx/Rates"
#else
        T__V32_CALLER_RX_RATES,          /*ALL/V32/Caller/Rx/Rates*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_RX_MSE              "ALL/V32/Caller/Rx/Mse"
#else
        T__V32_CALLER_RX_MSE,            /*ALL/V32/Caller/Rx/Mse*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_RX_EQ              "ALL/V32/Caller/Rx/Eq"
#else
        T__V32_CALLER_RX_EQ,            /*ALL/V32/Caller/Rx/Eq*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_RX_MISC             "ALL/V32/Caller/Rx/Misc"
#else
        T__V32_CALLER_RX_MISC,           /*ALL/V32/Caller/Rx/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_RX_TIMEOUTS         "ALL/V32/Caller/Rx/Timeouts"
#else
        T__V32_CALLER_RX_TIMEOUTS,       /*ALL/V32/Caller/Rx/Timeouts*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_TX_STATES           "ALL/V32/Answer/Tx/States"
#else
        T__V32_ANSWER_TX_STATES,         /*ALL/V32/Answer/Tx/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_ANSWER_TX_RATES            "ALL/V32/Answer/Tx/Rates"
#else
        T__V32_ANSWER_TX_RATES,          /*ALL/V32/Answer/Tx/Rates*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_TX_STATES           "ALL/V32/Caller/Tx/States"
#else
        T__V32_CALLER_TX_STATES,         /*ALL/V32/Caller/Tx/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V32_CALLER_TX_RATES            "ALL/V32/Caller/Tx/Rates"
#else
        T__V32_CALLER_TX_RATES,          /*ALL/V32/Caller/Tx/Rates*/
#endif
/*****************************************************************************/
/*********************************   V 3 4   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define T__V34_CONFIGURE                  "ALL/V34/Configure"
#else
        T__V34_CONFIGURE,                /*ALL/V34/Configure*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_RUNTIMECOMMAND             "ALL/V34/RunTimeCommand"
#else
        T__V34_RUNTIMECOMMAND,           /*ALL/V34/RunTimeCommand*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_MESSAGES            "ALL/V34/Answer/Messages"
#else
        T__V34_ANSWER_MESSAGES,          /*ALL/V34/Answer/Messages*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_MESSAGES_PHASE2     "ALL/V34/Answer/Messages/Phase2"
#else
        T__V34_ANSWER_MESSAGES_PHASE2,   /*ALL/V34/Answer/Messages/Phase2*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_EC_MISC             "ALL/V34/Answer/EC/Misc"
#else
        T__V34_ANSWER_EC_MISC,           /*ALL/V34/Answer/EC/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_EC_NEAR             "ALL/V34/Answer/EC/Near"
#else
        T__V34_ANSWER_EC_NEAR,           /*ALL/V34/Answer/EC/Near*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_EC_FAR              "ALL/V34/Answer/EC/Far"
#else
        T__V34_ANSWER_EC_FAR ,           /*ALL/V34/Answer/EC/Far*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_MESSAGES            "ALL/V34/Caller/Messages"
#else
        T__V34_CALLER_MESSAGES,          /*ALL/V34/Caller/Messages*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_MESSAGES_PHASE2     "ALL/V34/Caller/Messages/Phase2"
#else
        T__V34_CALLER_MESSAGES_PHASE2,   /*ALL/V34/Caller/Messages/Phase2*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_EC_MISC             "ALL/V34/Caller/EC/Misc"
#else
        T__V34_CALLER_EC_MISC,           /*ALL/V34/Caller/EC/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_EC_NEAR             "ALL/V34/Caller/EC/Near"
#else
        T__V34_CALLER_EC_NEAR,           /*ALL/V34/Caller/EC/Near*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_EC_FAR              "ALL/V34/Caller/EC/Far"
#else
        T__V34_CALLER_EC_FAR ,           /*ALL/V34/Caller/EC/Far*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_SUMMARY             "ALL/V34/Caller/Summary"
#else
        T__V34_CALLER_SUMMARY,           /*ALL/V34/Caller/Summary*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX                  "ALL/V34/Caller/Rx"
#else
        T__V34_CALLER_RX,                /*ALL/V34/Caller/Rx*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_FRED                "ALL/V34/Caller/Fred"
#else
        T__V34_CALLER_FRED,              /*ALL/V34/Caller/Fred*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_FRED_STATES         "ALL/V34/Caller/Fred/States"
#else
        T__V34_CALLER_FRED_STATES,       /*ALL/V34/Caller/Fred/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_FRED_SUMMARY        "ALL/V34/Caller/Fred/Summary"
#else
        T__V34_CALLER_FRED_SUMMARY,      /*ALL/V34/Caller/Fred/Summary*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_TIMEOUTS         "ALL/V34/Caller/Rx/Timeouts"
#else
        T__V34_CALLER_RX_TIMEOUTS,       /*ALL/V34/Caller/Rx/Timeouts*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_SUMMARY             "ALL/V34/Answer/Summary"
#else
        T__V34_ANSWER_SUMMARY,           /*ALL/V34/Answer/Summary*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX                  "ALL/V34/Answer/Rx"
#else
        T__V34_ANSWER_RX,                /*ALL/V34/Answer/Rx*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_FRED                "ALL/V34/Answer/Fred"
#else
        T__V34_ANSWER_FRED,              /*ALL/V34/Answer/Fred*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_FRED_STATES         "ALL/V34/Answer/Fred/States"
#else
        T__V34_ANSWER_FRED_STATES,       /*ALL/V34/Answer/Fred/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_FRED_SUMMARY        "ALL/V34/Answer/Fred/Summary"
#else
        T__V34_ANSWER_FRED_SUMMARY,      /*ALL/V34/Answer/Fred/Summary*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_TIMEOUTS         "ALL/V34/Answer/Rx/Timeouts"
#else
        T__V34_ANSWER_RX_TIMEOUTS,       /*ALL/V34/Answer/Rx/Timeouts*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE2_STATES    "ALL/V34/Caller/Rx/Phase2/States"
#else
        T__V34_CALLER_RX_PHASE2_STATES,  /*ALL/V34/Caller/Rx/Phase2/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE2_DPSK      "ALL/V34/Caller/Rx/Phase2/Dpsk"
#else
        T__V34_CALLER_RX_PHASE2_DPSK,    /*ALL/V34/Caller/Rx/Phase2/Dpsk*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE2_PROBING   "ALL/V34/Caller/Rx/Phase2/Probing"
#else
        T__V34_CALLER_RX_PHASE2_PROBING, /*ALL/V34/Caller/Rx/Phase2/Probing*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE2           "ALL/V34/Caller/Rx/Phase2"
#else
        T__V34_CALLER_RX_PHASE2,         /*ALL/V34/Caller/Rx/Phase2*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE2_MISC      "ALL/V34/Caller/Rx/Phase2/Misc"
#else
        T__V34_CALLER_RX_PHASE2_MISC,    /*ALL/V34/Caller/Rx/Phase2/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE2_STATES    "ALL/V34/Answer/Rx/Phase2/States"
#else
        T__V34_ANSWER_RX_PHASE2_STATES,  /*ALL/V34/Answer/Rx/Phase2/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE2_DPSK      "ALL/V34/Answer/Rx/Phase2/Dpsk"
#else
        T__V34_ANSWER_RX_PHASE2_DPSK,    /*ALL/V34/Answer/Rx/Phase2/Dpsk*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE2_PROBING   "ALL/V34/Answer/Rx/Phase2/Probing"
#else
        T__V34_ANSWER_RX_PHASE2_PROBING, /*ALL/V34/Answer/Rx/Phase2/Probing*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE2           "ALL/V34/Answer/Rx/Phase2"
#else
        T__V34_ANSWER_RX_PHASE2,         /*ALL/V34/Answer/Rx/Phase2*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE2_MISC      "ALL/V34/Answer/Rx/Phase2/Misc"
#else
        T__V34_ANSWER_RX_PHASE2_MISC,    /*ALL/V34/Answer/Rx/Phase2/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE34_MSE      "ALL/V34/Caller/Rx/Phase34/Mse"
#else
        T__V34_CALLER_RX_PHASE34_MSE,    /*ALL/V34/Caller/Rx/Phase34/Mse*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE34_MSE      "ALL/V34/Answer/Rx/Phase34/Mse"
#else
        T__V34_ANSWER_RX_PHASE34_MSE,    /*ALL/V34/Answer/Rx/Phase34/Mse*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE34_STATES   "ALL/V34/Caller/Rx/Phase34/States"
#else
        T__V34_CALLER_RX_PHASE34_STATES, /*ALL/V34/Caller/Rx/Phase34/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE34_MP       "ALL/V34/Caller/Rx/Phase34/MP"
#else
        T__V34_CALLER_RX_PHASE34_MP,     /*ALL/V34/Caller/Rx/Phase34/MP*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE34_MISC     "ALL/V34/Caller/Rx/Phase34/Misc"
#else
        T__V34_CALLER_RX_PHASE34_MISC,   /*ALL/V34/Caller/Rx/Phase34/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE34_STATES   "ALL/V34/Answer/Rx/Phase34/States"
#else
        T__V34_ANSWER_RX_PHASE34_STATES, /*ALL/V34/Answer/Rx/Phase34/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE34_MP       "ALL/V34/Answer/Rx/Phase34/MP"
#else
        T__V34_ANSWER_RX_PHASE34_MP,     /*ALL/V34/Answer/Rx/Phase34/MP*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE34_MISC     "ALL/V34/Answer/Rx/Phase34/Misc"
#else
        T__V34_ANSWER_RX_PHASE34_MISC,   /*ALL/V34/Answer/Rx/Phase34/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE34_EQ       "ALL/V34/Caller/Rx/Phase34/Eq"
#else
        T__V34_CALLER_RX_PHASE34_EQ,     /*ALL/V34/Caller/Rx/Phase34/Eq*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_RX_PHASE34_NP       "ALL/V34/Caller/Rx/Phase34/Np"
#else
        T__V34_CALLER_RX_PHASE34_NP,     /*ALL/V34/Caller/Rx/Phase34/Np*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_TX                  "ALL/V34/Caller/Tx"
#else
        T__V34_CALLER_TX,                /*ALL/V34/Caller/Tx*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_TX_ENCODER_PRECODER   "ALL/V34/Caller/Tx/Encoder/Precoder"
#else
        T__V34_CALLER_TX_ENCODER_PRECODER, /*ALL/V34/Caller/Tx/Encoder/Precoder*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_TX                  "ALL/V34/Answer/Tx"
#else
        T__V34_ANSWER_TX,                /*ALL/V34/Answer/Tx*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_TX_ENCODER_PRECODER   "ALL/V34/Answer/Tx/Encoder/Precoder"
#else
        T__V34_ANSWER_TX_ENCODER_PRECODER, /*ALL/V34/Answer/Tx/Encoder/Precoder*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_TX_PHASE2           "ALL/V34/Caller/Tx/Phase2"
#else
        T__V34_CALLER_TX_PHASE2,         /*ALL/V34/Caller/Tx/Phase2*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_TX_PHASE2_STATES    "ALL/V34/Caller/Tx/Phase2/States"
#else
        T__V34_CALLER_TX_PHASE2_STATES,  /*ALL/V34/Caller/Tx/Phase2/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_TX_PHASE2_DPSK      "ALL/V34/Caller/Tx/Phase2/Dpsk"
#else
        T__V34_CALLER_TX_PHASE2_DPSK,    /*ALL/V34/Caller/Tx/Phase2/Dpsk*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_TX_PHASE2           "ALL/V34/Answer/Tx/Phase2"
#else
        T__V34_ANSWER_TX_PHASE2,         /*ALL/V34/Answer/Tx/Phase2*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_TX_PHASE2_STATES    "ALL/V34/Answer/Tx/Phase2/States"
#else
        T__V34_ANSWER_TX_PHASE2_STATES,  /*ALL/V34/Answer/Tx/Phase2/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_TX_PHASE2_DPSK      "ALL/V34/Answer/Tx/Phase2/Dpsk"
#else
        T__V34_ANSWER_TX_PHASE2_DPSK,    /*ALL/V34/Answer/Tx/Phase2/Dpsk*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_TX_PHASE34_STATES   "ALL/V34/Caller/Tx/Phase34/States"
#else
        T__V34_CALLER_TX_PHASE34_STATES, /*ALL/V34/Caller/Tx/Phase34/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_CALLER_TX_PHASE34_MP       "ALL/V34/Caller/Tx/Phase34/MP"
#else
        T__V34_CALLER_TX_PHASE34_MP,     /*ALL/V34/Caller/Tx/Phase34/MP*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_TX_PHASE34_STATES   "ALL/V34/Answer/Tx/Phase34/States"
#else
        T__V34_ANSWER_TX_PHASE34_STATES, /*ALL/V34/Answer/Tx/Phase34/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_TX_PHASE34_MP       "ALL/V34/Answer/Tx/Phase34/MP"
#else
        T__V34_ANSWER_TX_PHASE34_MP,     /*ALL/V34/Answer/Tx/Phase34/MP*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE34_EQ       "ALL/V34/Answer/Rx/Phase34/Eq"
#else
        T__V34_ANSWER_RX_PHASE34_EQ,     /*ALL/V34/Answer/Rx/Phase34/Eq*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V34_ANSWER_RX_PHASE34_NP       "ALL/V34/Answer/Rx/Phase34/Np"
#else
        T__V34_ANSWER_RX_PHASE34_NP,     /*ALL/V34/Answer/Rx/Phase34/Np*/
#endif
/*****************************************************************************/
/*********************************   V 9 2   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define T__V92_RX_PHASE34_MSE             "ALL/V34/Answer/Rx/V92Phase34/Mse"
#else
        T__V92_RX_PHASE34_MSE,           /*ALL/V34/Answer/Rx/V92Phase34/Mse*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V92_RX_PHASE34_STATES          "ALL/V34/Answer/Rx/V92Phase34/States"
#else
        T__V92_RX_PHASE34_STATES,        /*ALL/V34/Answer/Rx/V92Phase34/States*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V92_RX_PHASE34_MP              "ALL/V34/Answer/Rx/V92Phase34/MP"
#else
        T__V92_RX_PHASE34_MP,            /*ALL/V34/Answer/Rx/V92Phase34/MP*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V92_RX_PHASE34_MISC            "ALL/V34/Answer/Rx/V92Phase34/Misc"
#else
        T__V92_RX_PHASE34_MISC,          /*ALL/V34/Answer/Rx/V92Phase34/Misc*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V92_RX_PHASE34_EQ              "ALL/V34/Answer/Rx/V92Phase34/Eq"
#else
        T__V92_RX_PHASE34_EQ,            /*ALL/V34/Answer/Rx/V92Phase34/Eq*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V92_RX_PHASE34_NP              "ALL/V34/Answer/Rx/V92Phase34/Np"
#else
        T__V92_RX_PHASE34_NP,            /*ALL/V34/Answer/Rx/V92Phase34/Np*/
#endif
/*****************************************************************************/
/*********************************    V 8    *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define T__V8_CONFIGURE                   "ALL/V8/Configure"
#else
        T__V8_CONFIGURE,                 /*ALL/V8/Configure*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V8_RUNTIMECOMMAND              "ALL/V8/RunTimeCommand"
#else
        T__V8_RUNTIMECOMMAND,            /*ALL/V8/RunTimeCommand*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V8_ANSWER_MESSAGES             "ALL/V8/Answer/Messages"
#else
        T__V8_ANSWER_MESSAGES,           /*ALL/V8/Answer/Messages*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V8_CALLER_MESSAGES             "ALL/V8/Caller/Messages"
#else
        T__V8_CALLER_MESSAGES,           /*ALL/V8/Caller/Messages*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V8_CALLER_RX                   "ALL/V8/Caller/RX"
#else
        T__V8_CALLER_RX,                 /*ALL/V8/Caller/RX*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V8_ANSWER_RX                   "ALL/V8/Answer/RX"
#else
        T__V8_ANSWER_RX,                 /*ALL/V8/Answer/RX*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V8_CALLER_TX                   "ALL/V8/Caller/TX"
#else
        T__V8_CALLER_TX,                 /*ALL/V8/Caller/TX*/
#endif

#ifdef NO_BLAM_SUPPORT
#define T__V8_ANSWER_TX                   "ALL/V8/Answer/TX"
#else
        T__V8_ANSWER_TX,                 /*ALL/V8/Answer/TX*/
#endif

		Num_Messages
	};


	typedef enum DMP_Enum{
	
        D__DUMMY,                       /*/dummy/dummy.dmp*/

        D__SIGMGR_RX_SAMPLES,           /*/signalmgr/rx.dmp*/
        D__SIGMGR_TX_SAMPLES,           /*/signalmgr/tx.dmp*/


/*****************************************************************************/
/*********************************   F S K   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define D__FSK_SESSION_CFG                     "FskSession.cfg"
#else
        D__FSK_SESSION_CFG,              /*/fsk/FskSession.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__FSK_SESSION_RXI                     "FskSession.rxi"
#else
        D__FSK_SESSION_RXI,              /*/fsk/FskSession.rxi*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__FSK_SESSION_TX8                     "FskSession.tx8"
#else
        D__FSK_SESSION_TX8,              /*/fsk/FskSession.tx8*/
#endif
     
#ifdef NO_BLAM_SUPPORT
#define D__FSK_SESSION_RLN                     "FskSession.rln"
#else
        D__FSK_SESSION_RLN,              /*/fsk/FskSession.rln*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__FSK_SESSION_TLN                     "FskSession.tln"
#else
        D__FSK_SESSION_TLN,              /*/fsk/FskSession.tln*/
#endif
       
#ifdef NO_BLAM_SUPPORT
#define D__FSK_SESSION_CMD                     "FskSession.cmd"
#else
        D__FSK_SESSION_CMD,              /*/fsk/FskSession.cmd*/
#endif
         
/*****************************************************************************/
/*********************************   TONES   *********************************/
/*****************************************************************************/       
#ifdef NO_BLAM_SUPPORT
#define D__TONES_SESSION_CFG                     "TonesSession.cfg"
#else
        D__TONES_SESSION_CFG,            /*/tones/TonesSession.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__ANSDET_RXI                            "Ansdet.rxi"
#else
        D__ANSDET_RXI,                   /*/tones/Ansdet.rxi*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__TONEDET_RXI                           "Tonedet.rxi"
#else
        D__TONEDET_RXI,                  /*/tones/Tonedet.rxi*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__TONEDET_CMD                           "Tonedet.cmd"
#else
        D__TONEDET_CMD,                  /*/tones/Tonedet.cmd*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__TONEGEN_TXO                           "Tonegen.txo"
#else
        D__TONEGEN_TXO,                  /*/tones/Tonegen.txo*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__TONEGEN_CMD                           "Tonegen.cmd"
#else
        D__TONEGEN_CMD,                  /*/tones/Tonegen.cmd*/
#endif

/*****************************************************************************/
/*********************************   V 1 7   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define D__V17_SESSION_CFG                     "V17Session.cfg"
#else
        D__V17_SESSION_CFG,              /*/v17/V17Session.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V17_SESSION_RXI                     "V17Session.rxi"
#else
        D__V17_SESSION_RXI,              /*/v17/V17Session.rxi*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V17_SESSION_TXO                     "V17Session.txo"
#else
        D__V17_SESSION_TXO,              /*/v17/V17Session.txo*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V17_SESSION_RLN                     "V17Session.rln"
#else
        D__V17_SESSION_RLN,              /*/v17/V17Session.rln*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V17_SESSION_TLN                     "V17Session.tln"
#else
        D__V17_SESSION_TLN,              /*/v17/V17Session.tln*/
#endif
 
#ifdef NO_BLAM_SUPPORT
#define D__V17_SESSION_CMD                     "V17Session.cmd"
#else
        D__V17_SESSION_CMD,              /*/v17/V17Session.cmd*/
#endif

/*****************************************************************************/
/*********************************   V 2 2   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define D__V22_SESSION_CFG                     "V22Session.cfg"
#else
        D__V22_SESSION_CFG,              /*/v22/V22Session.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V22_SESSION_RXI                     "V22Session.rxi"
#else
        D__V22_SESSION_RXI,              /*/v22/V22Session.rxi*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V22_SESSION_TX8                     "V22Session.tx8"
#else
        D__V22_SESSION_TX8,              /*/v22/V22Session.tx8*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V22_SESSION_RLN                     "V22Session.rln"
#else
        D__V22_SESSION_RLN,              /*/v22/V22Session.rln*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V22_SESSION_TLN                     "V22Session.tln"
#else
        D__V22_SESSION_TLN,              /*/v22/V22Session.tln*/
#endif
 
#ifdef NO_BLAM_SUPPORT
#define D__V22_SESSION_CMD                     "V22Session.cmd"
#else
        D__V22_SESSION_CMD,              /*/v22/V22Session.cmd*/
#endif
 
/*****************************************************************************/
/*********************************   V 2 7   *********************************/
/*****************************************************************************/
#ifdef NO_BLAM_SUPPORT
#define D__V27_SESSION_CFG                     "V27Session.cfg"
#else
        D__V27_SESSION_CFG,              /*/v27/V27Session.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V27_SESSION_RXI                     "V27Session.rxi"
#else
        D__V27_SESSION_RXI,              /*/v27/V27Session.rxi*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V27_SESSION_TXO                     "V27Session.txo"
#else
        D__V27_SESSION_TXO,              /*/v27/V27Session.txo*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V27_SESSION_RLN                     "V27Session.rln"
#else
        D__V27_SESSION_RLN,              /*/v27/V27Session.rln*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V27_SESSION_TLN                     "V27Session.tln"
#else
        D__V27_SESSION_TLN,              /*/v27/V27Session.tln*/
#endif
 
#ifdef NO_BLAM_SUPPORT
#define D__V27_SESSION_CMD                     "V27Session.cmd"
#else
        D__V27_SESSION_CMD,              /*/v27/V27Session.cmd*/
#endif

/*****************************************************************************/
/*********************************   V 3 2   *********************************/
/*****************************************************************************/    
#ifdef NO_BLAM_SUPPORT
#define D__V32_SESSION_CFG                     "V32Session.cfg"
#else
        D__V32_SESSION_CFG,              /*/v32/V32Session.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V32_SESSION_RXI                     "V32Session.rxi"
#else
        D__V32_SESSION_RXI,              /*/v32/V32Session.rxi*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V32_SESSION_TX8                     "V32Session.tx8"
#else
        D__V32_SESSION_TX8,              /*/v32/V32Session.tx8*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V32_SESSION_RLN                     "V32Session.rln"
#else
        D__V32_SESSION_RLN,              /*/v32/V32Session.rln*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V32_SESSION_TLN                     "V32Session.tln"
#else
        D__V32_SESSION_TLN,              /*/v32/V32Session.tln*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V32_SESSION_CMD                     "V32Session.cmd"
#else
        D__V32_SESSION_CMD,              /*/v32/V32Session.cmd*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V32_SESSION_BIT                     "V32Session.bit"
#else
        D__V32_SESSION_BIT,              /*/v32/V32Session.bit*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__RANGING_AFTER_IIR1_C16              "Ranging_AfterIir1.c16"
#else
        D__RANGING_AFTER_IIR1_C16,       /*/v32/Ranging_AfterIir1.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__RANGING_AFTER_IIR2_C16              "Ranging_AfterIir2.c16"
#else
        D__RANGING_AFTER_IIR2_C16,       /*/v32/Ranging_AfterIir2.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__S_DETECT_R32                        "SDetect.r32"
#else
        D__S_DETECT_R32,                 /*/v32/SDetect.r32*/
#endif

/*****************************************************************************/
/*********************************   V 3 4   *********************************/
/*****************************************************************************/    
#ifdef NO_BLAM_SUPPORT
#define D__V34_SESSION_CFG                     "V34Session.cfg"
#else
        D__V34_SESSION_CFG,              /*/v34/V34Session.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_SESSION_RXI                     "V34Session.rxi"
#else
        D__V34_SESSION_RXI,              /*/v34/V34Session.rxi*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_SESSION_TXO                     "V34Session.txo"
#else
        D__V34_SESSION_TXO,              /*/v34/V34Session.txo*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_SESSION_RLN                     "V34Session.rln"
#else
        D__V34_SESSION_RLN,              /*/v34/V34Session.rln*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_SESSION_TLN                     "V34Session.tln"
#else
        D__V34_SESSION_TLN,              /*/v34/V34Session.tln*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_SESSION_CMD                     "V34Session.cmd"
#else
        D__V34_SESSION_CMD,              /*/v34/V34Session.cmd*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_SESSION_BIT                     "V34Session.bit"
#else
        D__V34_SESSION_BIT,              /*/v34/V34Session.bit*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__DPSK_RX1_SAMPLES_8K_R16             "DpskRx1_Samples_8K.r16"
#else
        D__DPSK_RX1_SAMPLES_8K_R16,      /*/v34/DpskRx1_Samples_8K.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DPSK_RX2_DEMOD_OUT_8K_C16           "DpskRx2_DemodOut_8K.c16"
#else
        D__DPSK_RX2_DEMOD_OUT_8K_C16,    /*/v34/DpskRx2_DemodOut_8K.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DPSK_RX3_DECIMATOR_OUT_9DOT6K_C16   "DpskRx3_DecimatorOut_9dot6K.c16"
#else
        D__DPSK_RX3_DECIMATOR_OUT_9DOT6K_C16, /*/v34/DpskRx3_DecimatorOut_9dot6K.c16*/
#endif
       
#ifdef NO_BLAM_SUPPORT
#define D__DPSK_RX4_DIFF_DET_OUT_9DOT6K_R16    "DpskRx4_DiffDetOut_9dot6K.r16"
#else
        D__DPSK_RX4_DIFF_DET_OUT_9DOT6K_R16, /*/v34/DpskRx4_DiffDetOut_9dot6K.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DPSK_RX5_SLICER_IN_9DOT6K_R16        "DpskRx5_SlicerIn_9dot6K.r16"
#else
        D__DPSK_RX5_SLICER_IN_9DOT6K_R16, /*/v34/DpskRx5_SlicerIn_9dot6K.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DPSK_RX6_SLICER_OUT_9DOT6K_R16        "DpskRx6_SlicerOut_9dot6K.r16"
#else
        D__DPSK_RX6_SLICER_OUT_9DOT6K_R16, /*/v34/DpskRx6_SlicerOut_9dot6K.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DPSK_RX7_BITS_600_R16                 "DpskRx7_Bits_600.r16"
#else
        D__DPSK_RX7_BITS_600_R16,          /*/v34/DpskRx7_Bits_600.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DPSK_RX8_TIMING_9DOT6K_R16            "DpskRx8_Timing_9dot6K.r16"
#else
        D__DPSK_RX8_TIMING_9DOT6K_R16,     /*/v34/DpskRx8_Timing_9dot6K.r16*/
#endif                               

#ifdef NO_BLAM_SUPPORT
#define D__PROBING_L1_POWER_SPECTRUM_R32         "ProbingL1PowerSpectrum.r32"
#else
        D__PROBING_L1_POWER_SPECTRUM_R32,  /*/v34/ProbingL1PowerSpectrum.r32*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__PROBING_L2_POWER_SPECTRUM_R32         "ProbingL2PowerSpectrum.r32"
#else
        D__PROBING_L2_POWER_SPECTRUM_R32,  /*/v34/ProbingL2PowerSpectrum.r32*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__AFTER_IIR_R16                         "AfterIIR.r16"
#else
        D__AFTER_IIR_R16,                  /*/v34/AfterIIR.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_PHASE2_RX_SESSION_R16             "V34Phase2RxSession.r16"
#else
        D__V34_PHASE2_RX_SESSION_R16,      /*/v34/V34Phase2RxSession.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DECODER_UN_C16                        "Decoder_Un.c16"
#else
        D__DECODER_UN_C16,                 /*/v34/Decoder_Un.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DECODER_YN_C16                        "Decoder_Yn.c16"
#else
        D__DECODER_YN_C16,                 /*/v34/Decoder_Yn.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__DECODER_XN_C16                        "Encoder_Xn.c16"
#else
        D__DECODER_XN_C16,                 /*/v34/Encoder_Xn.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__ENCODER_UN_C16                        "Encoder_Un.c16"
#else
        D__ENCODER_UN_C16,                 /*/v34/Encoder_Un.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__ENCODER_YN_C16                        "Encoder_Yn.c16"
#else
        D__ENCODER_YN_C16,                 /*/v34/Encoder_Yn.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__ENCODER_XN_C16                        "Encoder_Xn.c16"
#else
        D__ENCODER_XN_C16,                 /*/v34/Encoder_Xn.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_QAM_TX0_BITS_R16                  "V34QamTx0_Bits.r16"
#else
        D__V34_QAM_TX0_BITS_R16,           /*/v34/V34QamTx0_Bits.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_QAM_TX1_SYMBOLS_C16               "V34QamTx1_Symbols.c16"
#else
        D__V34_QAM_TX1_SYMBOLS_C16,        /*/v34/V34QamTx1_Symbols.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_QAM_TX2_SHAPING_OUT_C16           "V34QamTx2_ShapingOut.c16"
#else
        D__V34_QAM_TX2_SHAPING_OUT_C16,    /*/v34/V34QamTx2_ShapingOut.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_QAM_TX3_MODULATOR_OUT_R16         "V34QamTx3_ModulatorOut.r16"
#else
        D__V34_QAM_TX3_MODULATOR_OUT_R16,  /*/v34/V34QamTx3_ModulatorOut.r16*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V34_QAM_TX4_PREEMPH_OUT_R16           "V34QamTx4_PreemphOut.r16"
#else
        D__V34_QAM_TX4_PREEMPH_OUT_R16,    /*/v34/V34QamTx4_PreemphOut.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V34_QAM_TX5_INTERP_TO_16K_R16         "V34QamTx5_InterpTo16K.r16"
#else
        D__V34_QAM_TX5_INTERP_TO_16K_R16,  /*/v34/V34QamTx5_InterpTo16K.r16*/
#endif
      
#ifdef NO_BLAM_SUPPORT
#define D__V34_QAM_TX6_GAIN_TUNE_R16             "V34QamTx6_GainTune.r16"
#else
        D__V34_QAM_TX6_GAIN_TUNE_R16,      /*/v34/V34QamTx6_GainTune.r16*/
#endif

/*****************************************************************************/
/*********************************   V 9 2   *********************************/
/*****************************************************************************/    
#ifdef NO_BLAM_SUPPORT
#define D__V92_SESSION_CFG                     "V92Session.cfg"
#else
        D__V92_SESSION_CFG,              /*/v90/V92Session.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V92_SESSION_SMP                     "V92Session.smp"
#else
        D__V92_SESSION_SMP,              /*/v90/V92Session.smp*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V92_SESSION_TXO                     "V92Session.txo"
#else
        D__V92_SESSION_TXO,              /*/v90/V92Session.txo*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V92_SESSION_RLN                     "V92Session.rln"
#else
        D__V92_SESSION_RLN,              /*/v90/V92Session.rln*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V92_SESSION_TLN                     "V92Session.tln"
#else
        D__V92_SESSION_TLN,              /*/v90/V92Session.tln*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V92_SESSION_CMD                     "V92Session.cmd"
#else
        D__V92_SESSION_CMD,              /*/v90/V92Session.cmd*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V92_SESSION_BIT                     "V92Session.bit"
#else
        D__V92_SESSION_BIT,              /*/v90/V92Session.bit*/
#endif

/*****************************************************************************/
/*********************************   V 8     *********************************/
/*****************************************************************************/    
#ifdef NO_BLAM_SUPPORT
#define D__V8_SESSION_CFG                     "V8Session.cfg"
#else
        D__V8_SESSION_CFG,               /*/v8/V8Session.cfg*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V8_SESSION_RXI                     "V8Session.rxi"
#else
        D__V8_SESSION_RXI,               /*/v8/V8Session.rxi*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V8_SESSION_TX8                     "V8Session.tx8"
#else
        D__V8_SESSION_TX8,               /*/v8/V8Session.tx8*/
#endif
        
#ifdef NO_BLAM_SUPPORT
#define D__V8_SESSION_RLN                     "V8Session.rln"
#else
        D__V8_SESSION_RLN,               /*/v8/V8Session.rln*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V8_SESSION_TLN                     "V8Session.tln"
#else
        D__V8_SESSION_TLN,               /*/v8/V8Session.tln*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__V8_SESSION_CMD                     "V8Session.cmd"
#else
        D__V8_SESSION_CMD,               /*/v8/V8Session.cmd*/
#endif

/*****************************************************************************/
/*********************************   E C     *********************************/
/*****************************************************************************/   
#ifdef NO_BLAM_SUPPORT
#define D__EC_RX_INPUT_R16                    "EC_RxInput.r16"
#else
        D__EC_RX_INPUT_R16,              /*/ec/EC_RxInput.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__EC_RX_OUTPUT_R16                   "EC_RxOutput.r16"
#else
        D__EC_RX_OUTPUT_R16,             /*/ec/EC_RxOutput.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__EC_TX_R16                          "EC_Tx.r16"
#else
        D__EC_TX_R16,                    /*/ec/EC_Tx.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__EC_NE_COEFFS_R32                   "EC_NeCoeffs.r32"
#else
        D__EC_NE_COEFFS_R32,             /*/ec/EC_NeCoeffs.r32*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__EC_FE_COEFFS_R32                   "EC_FeCoeffs.r32"
#else
        D__EC_FE_COEFFS_R32,             /*/ec/EC_FeCoeffs.r32*/
#endif

/*****************************************************************************/
/*********************************   Q A M   *********************************/
/*****************************************************************************/    
#ifdef NO_BLAM_SUPPORT
#define D__QAM_TX1_SYMBOLS_C16                 "QamTx1_Symbols.c16"
#else
        D__QAM_TX1_SYMBOLS_C16,          /*/qam/QamTx1_Symbols.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__QAM_TX2_SHAPING_OUT_C16             "QamTx2_ShapingOut.c16"
#else
        D__QAM_TX2_SHAPING_OUT_C16,      /*/qam/QamTx2_ShapingOut.c16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__QAM_TX3_MODULATOR_OUT_R16           "QamTx3_ModulatorOut.r16"
#else
        D__QAM_TX3_MODULATOR_OUT_R16,    /*/qam/QamTx3_ModulatorOut.r16*/
#endif

#ifdef NO_BLAM_SUPPORT
#define D__QAM_TX4_TX_SAMPLES_R16              "QamTx4_TxSamples.r16"
#else
        D__QAM_TX4_TX_SAMPLES_R16,       /*/qam/QamTx4_TxSamples.r16*/
#endif


	    DMP_FILES_TOTAL

	} DMP_FILES_T;


#if !defined NO_BLAM_SUPPORT
/*///////////////////////////////////////////////////////////////////////////// */
/*///////////////////////////////////////////////////////////////////////////// */
/*///////////////////////////////////////////////////////////////////////////// */
/*///////////////////////////////////////////////////////////////////////////// */
/*///////////////////////////////////////////////////////////////////////////// */
/*   Background Logging Definitions .... */
/* */
/*		NOTE:	PLEASE, any changes made in TRACE message mask definitions should */
/*				be made also in enum and defaults defined below.  */
/* */
/*///////////////////////////////////////////////////////////////////////////// */
/*///////////////////////////////////////////////////////////////////////////// */
/*///////////////////////////////////////////////////////////////////////////// */
/*///////////////////////////////////////////////////////////////////////////// */
/*///////////////////////////////////////////////////////////////////////////// */

#ifndef FILEIDNUM
#warning FILEIDNUM not defined
#define FILEIDNUM 1000
#endif

#if defined BLAM22

	#include "rpt_io.h"


	/* */
	/*used to initialize Global Severity filter at port open */
	/* */
	#define BLAM_SEVERITY_DEFAULT (T__FATAL_ERROR|T__ERROR)



    /*The FOllowing list is used to initialize trace ON/OFF defaults */
	/*for all of the above enum elements at port open time.  This list may */
	/*be overridden by soft defaults (such as registry....).   */
	/* */
	/*NOTE:		Any change to the above list MUST have a corresponding change to  */
	/*			this list...and THIS list IS order dependent -- it is used */
	/*          to initialize an array of filter criteria values...!!!!! */



	/*This macro is used to initialize the default tracing behavior.  It MUST be kept */
	/*current with the enum above and is order dependent.  It initialized each bit for each */
	/*trace category. */
	#define BLAM_FILTERS_DEFAULTS 	{						\
					/*00*/	DO_LOG_MSG_INIT(T__CALLPROG)		\
					/*01*/|	DO_LOG_MSG_INIT(T__SESSIONMGR)		\
					/*02*/|	DO_LOG_MSG_INIT(T__STREAMMGR)		\
					/*03*/|	DO_LOG_MSG_INIT(T__DATASESSION)		\
					/*04*/|	DO_LOG_MSG_INIT(T__MNP)			\
					/*05*/|	DO_LOG_MSG_INIT(T__V42)			\
					/*06*/|	DO_LOG_MSG_INIT(T__V42BIS)		\
					/*07*/|	DO_LOG_MSG_INIT(T__FAXSESSION)		\
					/*08*/|	DO_LOG_MSG_INIT(T__FAXPIM)		\
					/*10*/|	DO_LOG_MSG_INIT(T__ATPARSER)		\
					/*11*/|	DO_LOG_MSG_INIT(T__COMMCTRL)		\
					/*12*/|	NO_LOG_MSG_INIT(T__UTILS)		\
					/*13*/|	NO_LOG_MSG_INIT(T__RTMGR)		\
					/*14*/|	NO_LOG_MSG_INIT(T__CONFIGMGR)		\
					/*15*/|	NO_LOG_MSG_INIT(T__V8BIS)		\
					/*16*/|	NO_LOG_MSG_INIT(T__STATMGR)		\
					/*17*/|	NO_LOG_MSG_INIT(T__HAL)			\
					/*18*/|	NO_LOG_MSG_INIT(T__HALOS)		\
					/*19*/|	NO_LOG_MSG_INIT(T__HALSCRIPT)		\
					/*20*/|	NO_LOG_MSG_INIT(T__PORT)		\
					/*21*/|	NO_LOG_MSG_INIT(T__V8)			\
					/*22*/|	NO_LOG_MSG_INIT(T__PROFILE)		\
					/*23*/|	NO_LOG_MSG_INIT(T__NVMMGR)		\
					/*24*/|	NO_LOG_MSG_INIT(T__TESTSESSION)		\
					/*25*/|	NO_LOG_MSG_INIT(T__DRIVER)		\
					/*26*/|	NO_LOG_MSG_INIT(T__CALLERID)		\
					/*27*/|	NO_LOG_MSG_INIT(T__MOH)			\
					/*28*/|	NO_LOG_MSG_INIT(T__QC)			\
					/*29*/|	NO_LOG_MSG_INIT(T__DIAG)		\
          			/*30*/| NO_LOG_MSG_INIT(T__CW)			\
                    /*31*/| DO_LOG_MSG_INIT(T__DEVMGR)          \
              ,						\
                    /*00*/	DO_LOG_MSG_INIT(T__AUTOMODE_EL) 	\
                    /*01*/|	DO_LOG_MSG_INIT(T__DATA_EL) 	\
                    /*02*/|	DO_LOG_MSG_INIT(T__DEVMGR_PTT) \
                    /*03*/|	DO_LOG_MSG_INIT(T__DIALER_EL) \
                    /*04*/|	DO_LOG_MSG_INIT(T__RINGDET) \
                    /*05*/|	DO_LOG_MSG_INIT(T__TONEEL)  \
                    /*06*/|	NO_LOG_MSG_INIT(T__HDLC)     	\
                    /*07*/|	NO_LOG_MSG_INIT(T__FAX_EL)     	\
                    /*08*/| DO_LOG_MSG_INIT(T__CAESAR)\
                    /*09*/| DO_LOG_MSG_INIT(T__ERROR)\
                    /*11*/|	DO_LOG_MSG_INIT(T__SIGNAL)		\
                    \
					}


		#define DMP_BLAM_FILTERS_DEFAULTS 	{						\
					/*00*/	DO_LOG_MSG_INIT(DEV_MGR_RX_SAMPLES)		\
					/*01*/|	DO_LOG_MSG_INIT(DEV_MGR_TX_SAMPLES)		\
					/*02*/|	DO_LOG_MSG_INIT(V17_SESSION_CONFIG)		\
					/*03*/|	DO_LOG_MSG_INIT(V17_SESSION_COMMANDS)		\
					/*04*/|	DO_LOG_MSG_INIT(V17_SESSION_RX_SAMPLES)			\
					/*05*/|	DO_LOG_MSG_INIT(V17_SESSION_TX_SAMPLES)			\
					/*06*/|	DO_LOG_MSG_INIT(V17_SESSION_RX_BUFLEN)		\
					/*07*/|	DO_LOG_MSG_INIT(V17_SESSION_TX_BUFLEN)		\
					/*08*/|	DO_LOG_MSG_INIT(V17_SESSION_TX_BITS)		\
					/*09*/|	DO_LOG_MSG_INIT(V32_SESSION_CONFIG)		\
					/*10*/|	DO_LOG_MSG_INIT(V32_SESSION_COMMANDS)		\
					/*11*/|	DO_LOG_MSG_INIT(V32_SESSION_RX_BUFLEN)		\
					/*12*/|	DO_LOG_MSG_INIT(V32_SESSION_RX_BUFLEN)		\
					/*13*/|	DO_LOG_MSG_INIT(V32_SESSION_TX_BUFLEN)		\
					/*14*/|	DO_LOG_MSG_INIT(V32_SESSION_TX_BITS)		\
					}

    #define RTRACE0(TypeAndSeverity, unusedLoggingThread, str)  RPT_Write(                      \
					(TypeAndSeverity),        \
					((__LINE__) & LINE_NUM_MASK) | (FILEIDNUM << 13), \
					(UINT32)0)


	#define RTRACE1(TypeAndSeverity, unusedLoggingThread, str, arg1)  RPT_Write(                         \
					(TypeAndSeverity),                     \
					(((__LINE__) & LINE_NUM_MASK) | (FILEIDNUM << 13) | (1 <<29)), \
					(UINT32)arg1)
	
	#define RTRACE2(TypeAndSeverity, unusedLoggingThread, str, arg1, arg2)  RPT_Write(                   \
					(TypeAndSeverity),                     \
					(((__LINE__) & LINE_NUM_MASK) | (FILEIDNUM << 13) | (2 <<29)), \
					(UINT32)arg1, (UINT32)arg2)


	#define RTRACE3(TypeAndSeverity, unusedLoggingThread, str, arg1, arg2, arg3)  RPT_Write(              \
					(TypeAndSeverity),                      \
					(((__LINE__) & LINE_NUM_MASK) | (FILEIDNUM << 13) | (3 <<29)),  \
					(UINT32)arg1, (UINT32)arg2, (UINT32)arg3)


	#define RTRACE4(TypeAndSeverity, unusedLoggingThread, str, arg1, arg2, arg3, arg4)  RPT_Write(       \
					(TypeAndSeverity),                     \
					(((__LINE__) & LINE_NUM_MASK) | (FILEIDNUM << 13) | (4 <<29)), \
					(UINT32)arg1, (UINT32)arg2, (UINT32)arg3, (UINT32)arg4)


	#define RTRACE5(TypeAndSeverity, unusedLoggingThread, str, arg1, arg2, arg3, arg4, arg5)  RPT_Write(        \
					(TypeAndSeverity),                            \
					(((__LINE__) & LINE_NUM_MASK) | (FILEIDNUM << 13) | (5 <<29)),        \
					(UINT32)arg1, (UINT32)arg2, (UINT32)arg3, (UINT32)arg4, (UINT32)arg5)						


	#define RTRACE6(TypeAndSeverity, unusedLoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6)  RPT_Write(                \
					(TypeAndSeverity),                                          \
					(((__LINE__) & LINE_NUM_MASK) | (FILEIDNUM << 13) | (6 <<29)),                      \
					(UINT32)arg1, (UINT32)arg2, (UINT32)arg3, (UINT32)arg4, (UINT32)arg5, (UINT32)arg6)


	#define RTRACE7(TypeAndSeverity, unusedLoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6, arg7)  RPT_Write(                        \
					(TypeAndSeverity),                                                        \
					(((__LINE__) & LINE_NUM_MASK) | (FILEIDNUM << 13) | (7 <<29)),                                    \
					(UINT32)arg1, (UINT32)arg2, (UINT32)arg3, (UINT32)arg4, (UINT32)arg5, (UINT32)arg6, (UINT32)arg7)

    #define RTRACE_DUMP(TypeAndSeverity, unusedLoggingThread, BufferAddress, Size)  RPT_Write( \
					(TypeAndSeverity),       \
					(RTRACE_DUMP_CODE << 13),                        \
					(UINT32)BufferAddress,                           \
					(UINT32)Size)

    #define RDUMP(TypeAndSeverity, BufferAddress, Size)  RPT_Write( \
			    (TypeAndSeverity),                                  \
			    (RDUMP_CODE << 13),                                 \
			    (UINT32)BufferAddress,                              \
			    (UINT32)Size)


    #define RTRACE_EOS(TypeAndSeverity, unusedLoggingThread)  RPT_Write(     \
			    (TypeAndSeverity), \
			    (RTRACE_EOS_CODE << 13))


    #define RDUMP_EOS(TypeAndSeverity, unusedLoggingThread)  RPT_Write(      \
			    (TypeAndSeverity), \
			    (RDUMP_EOS_CODE << 13))

    #define TRACE( x, y )

#endif /* defined BLAM22 */


#else     /* !defined NO_BLAM_SUPPORT */

#if ((defined SCR) && (OS_TYPE == OS_STB))
	#include "rpt_io.h"
	#define DW_CLIENT_ID ((UINT32)0)
	#define Num_Messages 0
	#define BLAM_SEVERITY_DEFAULT 0

	#include "octopus/include/dbgscr_wrapper.h"

    #define RTRACE0(TypeAndSeverity, LoggingThread, str) DbgScrWrap_Write( str )
	#define RTRACE1(TypeAndSeverity, LoggingThread, str, arg1) DbgScrWrap_Write( str, arg1 )
	#define RTRACE2(TypeAndSeverity, LoggingThread, str, arg1, arg2) DbgScrWrap_Write( str, arg1, arg2 )
	#define RTRACE3(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3) DbgScrWrap_Write( str, arg1, arg2, arg3 )
	#define RTRACE4(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4) DbgScrWrap_Write( str, arg1, arg2, arg3, arg4 )
	#define RTRACE5(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5) DbgScrWrap_Write( str, arg1, arg2, arg3, arg4, arg5 )
	#define RTRACE6(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6) DbgScrWrap_Write( str, arg1, arg2, arg3, arg4, arg5, arg6 )
	#define RTRACE7(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6, arg7) DbgScrWrap_Write( str, arg1, arg2, arg3, arg4, arg5, arg6, arg7 )

    #define SCR_RTL_PRINT0(TypeAndSeverity, LoggingThread, str) DbgScrWrap_Write( str )
	#define SCR_RTL_PRINT1(TypeAndSeverity, LoggingThread, str, arg1) DbgScrWrap_Write( str, arg1 )
	#define SCR_RTL_PRINT2(TypeAndSeverity, LoggingThread, str, arg1, arg2) DbgScrWrap_Write( str, arg1, arg2 )
	#define SCR_RTL_PRINT3(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3) DbgScrWrap_Write( str, arg1, arg2, arg3 )
	#define SCR_RTL_PRINT4(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4) DbgScrWrap_Write( str, arg1, arg2, arg3, arg4 )
	#define SCR_RTL_PRINT5(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5) DbgScrWrap_Write( str, arg1, arg2, arg3, arg4, arg5 )
	#define SCR_RTL_PRINT6(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6) DbgScrWrap_Write( str, arg1, arg2, arg3, arg4, arg5, arg6 )
	#define SCR_RTL_PRINT7(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6, arg7) DbgScrWrap_Write( str, arg1, arg2, arg3, arg4, arg5, arg6, arg7 )

#ifdef DPRINTF
#undef DPRINTF
#endif
	#define DPRINTF( x ) DbgScrWrap_Write x
	#define TRACE(x, y ) DbgScrWrap_Write y
#else  /* ((defined SCR) && (OS_TYPE == OS_STB)) */

#if ( OS_TYPE == OS_STB )
	#define TRACE(x, y ) 
#else
    #define TRACE(mask,printf)  {                                           \
                                    if ((mask) & ulTraceMask)               \
                                    {                                       \
                                        UINT32 dwTime = OsGetSystemTime();  \
                                        OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000); \
                                        OsDebugPrintf printf;               \
                                    }                                       \
                                }
#endif

#endif   /* !defined NO_BLAM_SUPPORT */

#endif  /* ((defined SCR) && (OS_TYPE == OS_STB)) */
	
	/*This lets us use either ulTraceMask or dwDmTraceMask assuming the  */
	/*os_x.h file is always included before the Testdebug.h file... */
	#ifndef NOBLAMTRACEMASK
		#define NOBLAMTRACEMASK  ulTraceMask
	#endif
    #define NOBLAMLOGGINGTHREAD ulLogThread

#if (OS_TYPE != OS_STB)
	/*These are here to let code converted to BLAM trace via WinICE */
	#define RTRACE0(TypeAndSeverity, LoggingThread, str) {										\
                    if (((TypeAndSeverity) & NOBLAMTRACEMASK) ||	(LoggingThread == NOBLAMLOGGINGTHREAD)) \
                                {																\
                                    UINT32 dwTime = OsGetSystemTime();							\
                                    OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000);    \
                                    OsDebugPrintf (str);										\
                                }																\
                            }


	#define RTRACE1(TypeAndSeverity, LoggingThread, str, arg1) {								\
                    if (((TypeAndSeverity) & NOBLAMTRACEMASK) ||	(LoggingThread == NOBLAMLOGGINGTHREAD)) \
                                {																\
                                    UINT32 dwTime = OsGetSystemTime();							\
                                    OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000);    \
                                    OsDebugPrintf (str, arg1);									\
                                }																\
                            }


	#define RTRACE2(TypeAndSeverity, LoggingThread, str, arg1, arg2) {						\
                    if (((TypeAndSeverity) & NOBLAMTRACEMASK) ||	(LoggingThread == NOBLAMLOGGINGTHREAD)) \
							{																\
                                UINT32 dwTime = OsGetSystemTime();							\
                                OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000);    \
                                OsDebugPrintf (str, arg1, arg2);							\
                            }																\
                        }


	#define RTRACE3(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3) {	        \
                    if (((TypeAndSeverity) & NOBLAMTRACEMASK) ||	(LoggingThread == NOBLAMLOGGINGTHREAD)) \
						{												                \
                            UINT32 dwTime = OsGetSystemTime();					        \
                            OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000);    \
                            OsDebugPrintf (str, arg1, arg2, arg3);                      \
						}															    \
                    }


	#define RTRACE4(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4) {	    \
                    if (((TypeAndSeverity) & NOBLAMTRACEMASK) ||	(LoggingThread == NOBLAMLOGGINGTHREAD)) \
					{												                    \
                        UINT32 dwTime = OsGetSystemTime();					            \
                        OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000);        \
                        OsDebugPrintf (str, arg1, arg2, arg3, arg4);                    \
                    }																	\
                }


	#define RTRACE5(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5) {	\
                    if (((TypeAndSeverity) & NOBLAMTRACEMASK) ||	(LoggingThread == NOBLAMLOGGINGTHREAD)) \
					{																		\
                        UINT32 dwTime = OsGetSystemTime();									\
                        OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000);			\
                        OsDebugPrintf (str, arg1, arg2, arg3, arg4, arg5);                  \
                    }																		\
                }


	#define RTRACE6(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6) {	\
                    if (((TypeAndSeverity) & NOBLAMTRACEMASK) ||	(LoggingThread == NOBLAMLOGGINGTHREAD)) \
					{																			\
                        UINT32 dwTime = OsGetSystemTime();										\
                        OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000);				\
                        OsDebugPrintf (str, arg1, arg2, arg3, arg4, arg5, arg6);                \
                    }																			\
                }



	#define RTRACE7(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6, arg7) {	\
                    if (((TypeAndSeverity) & NOBLAMTRACEMASK) ||	(LoggingThread == NOBLAMLOGGINGTHREAD)) \
						{																				\
                            UINT32 dwTime = OsGetSystemTime();											\
                            OsDebugPrintf ("%07u.%03u: ", dwTime/1000, dwTime%1000);					\
                            OsDebugPrintf (str, arg1, arg2, arg3, arg4, arg5, arg6, arg7);              \
                        }																				\
                    }

    #define RDUMP(TypeAndSeverity, BufferAddress, Size)  

    #define RTRACE_EOS(TypeAndSeverity, LoggingThread)  

#elif ( !defined SCR )
	/*These are here to let code converted to BLAM trace via WinICE */
	#define RTRACE0(TypeAndSeverity, LoggingThread, str) 

	#define RTRACE1(TypeAndSeverity, LoggingThread, str, arg1) 
        
	#define RTRACE2(TypeAndSeverity, LoggingThread, str, arg1, arg2) 

	#define RTRACE3(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3) 

	#define RTRACE4(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4) 

	#define RTRACE5(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5)
        
	#define RTRACE6(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6)

	#define RTRACE7(TypeAndSeverity, LoggingThread, str, arg1, arg2, arg3, arg4, arg5, arg6, arg7) 

    #define RDUMP(TypeAndSeverity, BufferAddress, Size)  

    #define RTRACE_EOS(TypeAndSeverity, LoggingThread)  

#endif /* MACOSX */


    #define DUMP(mask,printf)  {                                           \
                                    if ((mask) & ulTraceMask)               \
                                    {                                       \
                                        OsDebugPrintf printf;               \
                                    }                                       \
                                }

	#define DUMPDATA(mask, data, len) /*{                                           \
	void DumpData(PUINT8 data, UINT32 len);					\
                                    if ((mask) & ulTraceMask)               \
                                    {                                       \
                                        DumpData(data, len);               \
                                    }                                       \
                                }
*/	




#ifdef USE_BREAKPOINT

    #define CNXT_BREAKPOINT(msg)     { OsDebugBreakpoint(msg);   }

#else

    #define CNXT_BREAKPOINT(msg)

#endif



#ifdef NO_BLAM_SUPPORT
    typedef char *              ScrNodeLocationT    ;
    typedef char *              DmpBaseNameT        ;
#else
    typedef enum RTRACE_GROUP   ScrNodeLocationT    ;
    typedef enum DMP_Enum       DmpBaseNameT        ;
#endif


/* XXX: temporary */
#if 0
#define T__DEVMGR T__DRIVER
#define T__DEVMGR_PTT T__DRIVER
#define T__TONEEL T__DRIVER
#define T__AUTOMODE_EL T__DRIVER
#define T__SIGNAL T__DRIVER
#endif





#endif  /* #ifndef __DEBUG_H__ */
