/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

/*
 * cdbgscr.c - c implementation of SCR printing.
 */
 
#include "framewrk.h"
#if (OS_TYPE == OS_LINUX)
#include "oscompat.h"
#endif
#include "ossysenv.h"

#ifdef SCR
#include "osmemory_ex.h"
#include "cdbgscr_ex.h"
#include "scrcommon.h"

#if ( (OS_TYPE == OS_LINUX) && (UK_MODE == UK_KERNEL) )
#include "linux_dbgscr.h"
#include "msgqueue.h"
#include "dbgsrv.h"
#endif


static PDBG_PRINT_FUNC ScrPrintFunc;


static VOID setScrPrintFunc( PVOID func )
{
	ScrPrintFunc = func;

}  /* setScrPrintFunc */


__shimcall__
DWORD C_ScrFormat( IN PCHAR FmtString, ... )
{
	char str[MAX_MESSAGE_LEN];
	va_list ap;
	int nc;

	if ( NULL == ScrPrintFunc )
	{
		LinuxScrSetEntryPointCallback( setScrPrintFunc );
		if ( NULL == ScrPrintFunc )
		{
			return 0;
		}
	}

	va_start( ap, FmtString );
	nc = vsprintf( str, FmtString, ap );
	va_end( ap );

	ScrPrintFunc( str, Q_RECORD_PRIORITY_NORMAL );

	return nc;

}  /* C_ScrFormat */

static PDBG_PRINT_FUNC m_pfnServer;

__shimcall__
BOOL LinuxScrSetEntryPointCallback( PVOID epcbk )
{
#if (OS_TYPE == OS_LINUX)
	m_pfnServer = DBGSRV_Print;

	((PCALLBACK) epcbk)( (PVOID) m_pfnServer );
#endif 
	return (m_pfnServer != NULL);
}
#endif /* SCR */

