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
 * File: DbgFuncs.h
 *
 * File : DbgFuncs.h
 * 
 * Description : Contains macros for debug printings (to WinICE).
 *
 *----------------------------------------------------------------------------
 */
 
#ifndef __DBGFUNCS_H__
#define __DBGFUNCS_H__



#ifdef VERBOSE

#ifdef DEBUG

#if ( (OS_TYPE == OS_WIN_9X) && (UK_MODE == UK_KERNEL) )
#define DPRINTF(ARGS)	dprintf ARGS
#endif

#if ( OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM) )
#define DPRINTF(ARGS)	{					\
						  DbgPrint ARGS;	\
						  DbgPrint ("\n");	\
						}
#endif

#if ( (OS_TYPE == OS_LINUX) && (UK_MODE == UK_KERNEL) )
#define DPRINTF(ARGS)	{					\
						  UINT32 dwTime = OsGetSystemTime();	\
						  OsDebugPrintf ("%07lu.%03lu: ", dwTime/1000, dwTime%1000);	\
						  OsDebugPrintf ARGS;	\
						  OsDebugPrintf ("\n");	\
						}
#endif

#else /* No DEBUG: */

#define DPRINTF(ARGS)	NPRINTF(ARGS)

#endif

#else  /* No VERBOSE: */

#define DPRINTF(ARGS)

#endif 

#if (OS_TYPE == OS_LINUX)
#define EPRINTF(ARGS)	{					\
						  OsErrorPrintf ARGS;	\
						  OsErrorPrintf ("\n");	\
						}
#else
#define EPRINTF DPRINTF
#endif

#if ( (OS_TYPE == OS_WIN_9X) && (UK_MODE == UK_KERNEL) )

#define NPRINTF(ARGS)	nprintf ARGS

#elif ( OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM) )

#define NPRINTF(ARGS)	{					\
						  DbgPrint ARGS;	\
						  DbgPrint ("\n");	\
						}

#elif ( (OS_TYPE == OS_LINUX) && (UK_MODE == UK_KERNEL) )
#define NPRINTF(ARGS)	{					\
						  OsDebugPrintf ARGS;	\
						  OsDebugPrintf ("\n");	\
						}
#else

/* here we will have to define the printf's for non VXD environments */
#define NPRINTF(ARGS)

#endif



#endif /* __DBGFUNCS_H__ */
