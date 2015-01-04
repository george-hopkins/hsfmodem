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
 * c_dbgscr.h - header file for c implementation of SCR printing.
 */

#ifndef _CDBGSCR_EX_H_
#define _CDBGSCR_EX_H_

#include "sysenv.h"
#if (OS_TYPE == OS_LINUX)
#include "linux_dbgscr.h"
#include <stdarg.h>
#endif


#ifdef SCR

#if ( COMP_ONE_OF(CMP_DJGPP | CMP_GCC) )

extern __shimcall__ DWORD C_ScrFormat( IN PCHAR FmtString, ... )
	__attribute__ ((format (printf, 1, 2)));

#else

extern __shimcall__ DWORD C_ScrFormat( IN PCHAR FmtString, ... );

#endif

#else /* ! SCR */

#if ( COMPILER == CMP_GCC )
#define C_ScrFormat(FmtString, Args...)
#else
/* optimize it out */
#define C_ScrFormat while(0) C_ScrFormat
#endif

#endif /* SCR */

#endif /* _CDBGSCR_EX_H_ */
