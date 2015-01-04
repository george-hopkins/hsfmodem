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


/*************************************************************************

  File Name: sysenv.h

  File Description: System dependence file.
                    This file includes some system files (if OS_DEPENDENT == 1)
                    and defines some system types, calling conventions, etc.

  *************************************************************************/
 
#ifndef __SYSENV_H__
#define __SYSENV_H__

 
#ifndef OS_DEPENDENT
#define OS_DEPENDENT 0
#define UNDEF_OS_DEPENDENT
#endif

#ifdef VERBOSE    /* if defined VERBOSE - need to compile with OS_DEPENDENT. */
#undef OS_DEPENDENT
#define OS_DEPENDENT 1
#endif

#if (CPU_TYPE==CPU_AMD64)
#define lg32 int
#else
#define lg32 long
#endif
 
#ifdef USE_DDK
 
/** if we use DDK (in SessionCtrl compilation) we want to include the igors 
	ostypedefs and typedefs which made for ddk. we close the else in the end 
	of the file - nothing is used in the file in case of DDK compilation .    **/
 
 
#if (OS_DEPENDENT == 1 )
#include "ostypedefs.h"
#else
#include "typedefs.h"
#endif
 
#else /** USE_DDK **/
 
#if ( OS_DEPENDENT == 1 )

/* include dbgfuncs.h for debug printing: */
#include "dbgfuncs.h"

#if ( OS_TYPE == OS_WIN_9X )

#if ( UK_MODE == UK_KERNEL )
 
#ifdef __cplusplus
#include <vtoolscp.h>
#else
#include <vtoolsc.h>
#endif /* __cplusplus */
 
#else /* UK_USER: */

#pragma warning (disable:4115) 
#pragma warning (disable:4214) 
#pragma warning (disable:4201) 
#include <windows.h>
#pragma warning (default:4201)
#pragma warning (default:4214)
#pragma warning (default:4115)

#define DllExport   __declspec( dllexport )

#endif /* UK_MODE */

#if (DEV_TOOL != DT_VTOOLSD)
#if (CPU_TYPE != CPU_AMD64)
typedef int ptrdiff_t;
typedef unsigned int    size_t;
#endif /* (DEV_TOOL != DT_VTOOLSD) */
#endif /* (CPU_TYPE != CPU_AMD64)*/

#endif /* OS_WIN_9X */


#if ( OS_TYPE == OS_LINUX )

typedef void    *HANDLE;
typedef HANDLE  *PHANDLE;
typedef HANDLE  HWND, HHOOK;
typedef void    VOID, *PVOID, **PPVOID;

#endif /* OS_LINUX */
#if ( (OS_TYPE == OS_DOS) && (PRJ_FLAVOUR == PRJFLVR_HSF) )
#include <string.h>
#include <alloc.h>
#endif
#if ( (OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM)) && (UK_MODE == UK_KERNEL) )


#ifndef DBG
#define CANCEL_DBG
#endif

#if (DEV_TOOL == DT_VDW)
#ifdef __cplusplus
#include <vdw.h>
#endif
#elif (DEV_TOOL == DT_MS98DDK)
#ifdef __cplusplus
extern "C"
{
#endif
#include <wdm.h>
#ifdef __cplusplus
}
#endif

typedef void	**PPVOID;

#endif

#if (OS_TYPE == OS_WIN_WDM) && (DEV_TOOL == DT_MS98DDK)
typedef struct
{
    PDMA_ADAPTER    pDmaAdapter;
    PKSPIN_LOCK     SpinLock;
    ULONG           Vector;
    KIRQL           Irql;
    KIRQL           SynchronizeIrql;
    KINTERRUPT_MODE InterruptMode;
    BOOLEAN         ShareVector;
    KAFFINITY       ProcessorEnableMask;
    BOOLEAN         FloatingSave;
	PDEVICE_OBJECT	pDevObj;
	struct _IRP *   Irp;
}WDM_RESOURCES_T;
#endif /* (OS_TYPE == OS_WIN_WDM) && (DEV_TOOL == DT_MS98DDK) */

#ifdef CANCEL_DBG
#undef DBG
#endif


#endif /* OS_WIN_NT4 | OS_WIN_WDM */


#if ( (OS_TYPE == OS_DOS) && (UK_MODE == UK_USER))

typedef char    *HANDLE;
typedef HANDLE  *PHANDLE;
typedef void    VOID, *PVOID;

#define IN  const

#endif /* OS_DOS && UK_USER */


#if ( UK_MODE == UK_USER )

#if ( OS_TYPE != OS_LINUX )
typedef void	**PPVOID;
#endif

#endif

#if (OS_TYPE == OS_VXWORKS)

#include <vxworks.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memlib.h>
#include <cachelib.h>
#include <loglib.h>

typedef char    *HANDLE;
typedef HANDLE  *PHANDLE;
typedef void    *PVOID, **PPVOID;
#define IN const

#endif /** OS_VXWORKS **/

#if (OS_TYPE == OS_STB)
 
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
 
typedef unsigned long			DWORD,  *PDWORD;
typedef void					VOID  , *PVOID, **PPVOID;
typedef unsigned short			UINT16, *PUINT16;
typedef unsigned long			UINT32, *PUINT32;
typedef long					INT32;
typedef char*					HANDLE;
typedef unsigned char			BYTE,   *PBYTE;
typedef int						BOOL;
typedef char					HAR,   *PCHAR, **PPCHAR;
typedef unsigned short			WORD,   *PWORD;
typedef unsigned int volatile   VOL32, *PVOL32;

#include "hsf4octopus/driver/rio/daa.h"
 
#endif /* OS_STB */
 
 
#else /* not OS_DEPENDENT defines: */
typedef void    *HANDLE;
typedef HANDLE  *PHANDLE;
typedef void     VOID, *PVOID, **PPVOID;
typedef volatile PVOID      *PVPVOID;
 
typedef short    SHORT;

#if ( CPU_TYPE != CPU_AMD64 ) && !defined (_PTRDIFF_T)
typedef int ptrdiff_t;
#if(COMPILER != CMP_XCC) /* #if(PROJECT!=PRJ_SANSIRO) */
#ifndef _SIZE_T
typedef unsigned int    size_t;
#define _SIZE_T
#endif
#endif /* size_t */
#endif /* !AMD64*/
#endif /* OS_DEPENDENT == 1 */

 
#if (OS_TYPE == OS_LINUX)

#if ( UK_MODE == UK_KERNEL )

#ifndef FRAME_WORK_IMPORTED
#define _LOOSE_KERNEL_NAMES
#include <math.h>
#undef _LOOSE_KERNEL_NAMES
#endif /* FRAME_WORK_IMPORTED */
 
#ifdef __cplusplus

#else

#include <stdarg.h>

#endif

#endif /* UK_KERNEL */

#else
#define printk DPRINTF
#endif

#if OS_ONE_OF( OS_LINUX | OS_VXWORKS |  OS_STB )
typedef char    *LPSTR;
typedef const char *LPCSTR;

#ifdef __GNUC__
#define IN
#else
#define IN const
#endif

#endif
 
#if ( UK_MODE == UK_USER )
 
#include <stdio.h>
#include <math.h>
 
#include <stdlib.h>
#include <string.h>
 
#endif /* UK_USER */
 
 
/*****************************************************************************/
/* Check all the next lines!!! they are taken from the SoftK56 commondefs.h  */
#if 0
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define NTAPI __stdcall
#else
#define _cdecl
#define NTAPI
#endif

#if ((defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS))
#define DECLSPEC_IMPORT __declspec(dllimport)
#else
#define DECLSPEC_IMPORT
#endif

#if !defined(_NTSYSTEM_)
#define NTSYSAPI DECLSPEC_IMPORT
#else
#define NTSYSAPI
#endif

#if defined(_M_MRX000) || defined(_M_ALPHA) || defined(_M_PPC)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif

#ifndef CONST
#define CONST               const
#endif
#endif
/*****************************************************************************/

#if ( (OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM)) && (UK_MODE == UK_KERNEL) )

#define _fastcall
#define __fastcall


#ifdef __cplusplus
#if (OVERRIDE_NEW == 1)
extern void* _cdecl operator new( size_t Size, char* FileName, int LineNumber );
#define		NEW		new(__FILE__,__LINE__)
#else
#define NEW new(NonPagedPool)
#endif  /* OVERRIDE_NEW */
#endif  /* __cplusplus */

#ifdef __cplusplus
extern "C"
{
#endif
	int sprintf( char *buffer, const char *format ,... );
#ifdef __cplusplus
}
#endif

#else

#define NEW new

#endif


#if ( ((OS_DEPENDENT == 0) ||  \
       ( (OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM | OS_LINUX))  ) ) && (UK_MODE == UK_KERNEL) && \
        defined(__cplusplus) )
#if (OS_DEPENDENT == 0) || (OS_ONE_OF(OS_WIN_NT4 | OS_LINUX))
static __inline int abs( int a ) { return ( ( a < 0 ) ? -( a ) : ( a ) ); }
#endif
#endif


#ifndef OK
#define OK 0
#endif


#ifndef TRUE

#define TRUE    1
#define FALSE   0
#if ( OS_TYPE != OS_LINUX )
#if (PROJECT  !=  PRJ_AMD64_WIN_R3 )
typedef int BOOL;
#endif
typedef int *PBOOL;        /* On NT or WDM we allways need the typedef of PBOOL */
#endif

#elif ( OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM) )

typedef int BOOL;        /* On NT or WDM we allways need the typedef of BOOL */
typedef int *PBOOL;        /* On NT or WDM we allways need the typedef of PBOOL */

#endif

#if ( OS_TYPE == OS_LINUX ) || ((PROJECT != PRJ_HSF) && (PROJECT != PRJ_AMD64_WIN_R3))
/* This definition was added for the Nile. It seems to be very problematic, since DDK  & VisualC define
   BOOLEAN as unsigned char and BOOL as int, so they can't match each other. Must check it again!!!! */
#define BOOLEAN BOOL
#endif


#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *) 0)
#endif
#endif

 
#if ((0==OS_DEPENDENT) || (OS_TYPE != OS_WIN_WDM)) && (PROJECT != PRJ_SANSIRO)
#if (CPU_TYPE!=CPU_AMD64)
typedef	unsigned long ULONG_PTR;
typedef          long LONG_PTR;
#ifndef _SIZE_T
typedef unsigned int  size_t;
#define _SIZE_T
#endif
#else
typedef        unsigned long ULONG_PTR;
typedef          long LONG_PTR;
#ifndef _SIZE_T
typedef unsigned long size_t;
#define _SIZE_T
#endif
#endif /*!CPU_AMD64*/
#endif /* ((0==OS_DEPENDENT) || (OS_TYPE != OS_WIN_WDM)) */
 
 
/* the following types are defined only for backwards compatibility and      */
/* should not be used by new code -                                          */
/* DWORD, PDWORD, ULONG, PULONG, BYTE, PBYTE, UCHAR, PUCHAR, WORD, PWORD,    */
/* USHORT, PUSHORT, etc.                                                     */

#if ((DEV_TOOL != DT_VTOOLSD) || (OS_DEPENDENT == 0)) && (OS_TYPE != OS_VXWORKS)
#if (OS_DEPENDENT == 1) && (OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM))
#include <windef.h>
#else  /*(OS_DEPENDENT == 1) && (OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM)) */
 
typedef unsigned int    DWORD,  *PDWORD;
typedef unsigned long   ULONG,  *PULONG;
typedef unsigned long long   ULONGLONG,  *PULONGLONG;
typedef unsigned char   BYTE,   *PBYTE;
typedef unsigned char   UCHAR,  *PUCHAR;
typedef unsigned short  WORD,   *PWORD;
typedef unsigned short  USHORT, *PUSHORT;

typedef unsigned int    UINT,   *PUINT;
typedef int             INT,    *PINT;
typedef long            LONG,   *PLONG;
typedef long long       LONGLONG,   *PLONGLONG;
typedef char            CHAR,   *PCHAR, **PPCHAR;

#if (OS_TYPE == OS_LINUX)
typedef int BOOL;
typedef int *PBOOL;
#endif

typedef LONG NTSTATUS;
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#define STATUS_SUCCESS      ((NTSTATUS)0x00000000L)
typedef size_t SIZE_T;
#endif   /*(OS_DEPENDENT == 1) && (OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM)) */

#endif /* ((DEV_TOOL != DT_VTOOLSD) || (OS_DEPENDENT == 0)) */

#if (OS_TYPE == OS_VXWORKS)
 
typedef unsigned long   DWORD,  *PDWORD;
typedef unsigned short  WORD,   *PWORD;
typedef char            CHAR,   *PCHAR, **PPCHAR;
typedef unsigned char   BYTE,   *PBYTE;
typedef int             INT,    *PINT;
typedef long            LONG,   *PLONG;


#define SIG_CHECK(x)
#if (OS_DEPENDENT == 0)
typedef unsigned int    UINT,   *PUINT;
#endif

#endif /** OS_VXWORKS **/

#if ( COMP_ONE_OF(CMP_DJGPP | CMP_GCC | CMP_XCC) )


#define __int64 long long

#define _cdecl
#define __cdecl
#define _stdcall
#define __stdcall
#define _fastcall
#define __fastcall

#if ( OS_TYPE == OS_LINUX ) && defined(__i386__)
#define __shimcall__ __attribute__((regparm(0)))
#define __kernelcall__
#define __kernelcallregparm__ __attribute__((regparm(3)))
#define __kernelcallstkparm__ __attribute__((regparm(0)))
#else
#define __shimcall__
#define __kernelcall__
#define __kernelcallregparm__
#define __kernelcallstkparm__
#endif

#ifdef USE_GCC_CALLING_CONVENTION

/* Usage: CDecl( int *, f( int arg1, char *arg2, etc ) ); */
#define CDecl( __ret_type__, __decl__ ) \
  __ret_type__ __decl__ __attribute__ ((cdecl))

/* Usage: StdCall( int *, f( int arg1, char *arg2, etc ) ); */
#define StdCall( __ret_type__, __decl__ ) \
  __ret_type__ __decl__ __attribute__ ((stdcall))

/* Usage: FastCall( int *, f( int arg1, char *arg2, etc ) ); */
#define FastCall( __ret_type__, __decl__ ) \
  __ret_type__ __decl__ __attribute__ ((regparm(3)))

#else /* No calling conventions: */

/* Usage: CDecl( int *, f( int arg1, char *arg2, etc ) ); */
#define CDecl( __ret_type__, __decl__ ) __ret_type__ __decl__ 

/* Usage: StdCall( int *, f( int arg1, char *arg2, etc ) ); */
#define StdCall( __ret_type__, __decl__ ) __ret_type__ __decl__ 

/* Usage: FastCall( int *, f( int arg1, char *arg2, etc ) ); */
#define FastCall( __ret_type__, __decl__ ) __ret_type__ __decl__ 

#endif /* USE_GCC_CALLING_CONVENTION */

 
/* To make structures packed (with no padding), do:
   typedef struct PACKED_ATTRIB st_TAG { ... } st_T; */
#define PACKED_ATTRIB __attribute__ ((packed))
 
 
#define Naked

#define _inline __inline

#define __null 0

#define FARPROC


#elif ( COMPILER == CMP_MSDEV )


/* Usage: CDecl( int *, f( int arg1, char *arg2, etc ) ); */
#define CDecl( __ret_type__, __decl__ ) __ret_type__ __cdecl __decl__

/* Usage: StdCall( int *, f( int arg1, char *arg2, etc ) ); */
#define StdCall( __ret_type__, __decl__ ) __ret_type__ __stdcall __decl__

/* Usage: FastCall( int *, f( int arg1, char *arg2, etc ) ); */
#define FastCall( __ret_type__, __decl__ ) __ret_type__ __fastcall __decl__
 
 
/* To make structures packed (with no padding), do:
   typedef struct PACKED_ATTRIB st_TAG { ... } st_T; */
#define PACKED_ATTRIB
 

/* Usage: Naked int *f( int arg1, char *arg2, etc ); */
#define Naked __declspec( naked ) 


#elif ( COMPILER == CMP_BORLAND )
#define PACKED_ATTRIB
 
#elif ( COMPILER == CMP_ADS )
 
#define _cdecl
#define __cdecl
#define _stdcall
#define __stdcall
#define _fastcall
#define __fastcall
 
/* Usage: CDecl( int *, f( int arg1, char *arg2, etc ) ); */
#define CDecl( __ret_type__, __decl__ ) __ret_type__ __decl__
 
/* Usage: StdCall( int *, f( int arg1, char *arg2, etc ) ); */
#define StdCall( __ret_type__, __decl__ ) __ret_type__ __decl__
 
/* Usage: FastCall( int *, f( int arg1, char *arg2, etc ) ); */
#define FastCall( __ret_type__, __decl__ ) __ret_type__ __decl__
 
 
/* To make structures packed (with no padding), do:
   typedef struct PACKED_ATTRIB st_TAG { ... } st_T; */
#define PACKED_ATTRIB
 
 
/* Usage: Naked int *f( int arg1, char *arg2, etc ); */
#define Naked __declspec( naked ) 
 
#endif


/* New typedefs that should be used: */

#if (OS_TYPE != OS_VXWORKS) || (OS_DEPENDENT == 0)
typedef unsigned short      UINT16;
 
typedef unsigned char       UINT8;
 
typedef signed short        INT16;
 
#if !((OS_TYPE == OS_WIN_WDM) && (OS_DEPENDENT == 1))
typedef signed char         INT8;
#endif
#endif

typedef unsigned short    *PUINT16;
typedef unsigned char     *PUINT8;

typedef signed short      *PINT16;
 
#if (OS_TYPE != OS_WIN_WDM)
typedef signed char       *PINT8;
#endif

#if (OS_TYPE == OS_LINUX)

typedef unsigned int       UINT32;
 
typedef unsigned int           *PUINT32;
typedef volatile unsigned int *PVUINT32;

typedef int        INT32;
 
typedef int        *PINT32;

typedef char * STRING;

#elif ( (OS_DEPENDENT == 0) ||  ((OS_TYPE == OS_WIN_9X) && (UK_MODE == UK_KERNEL) ) || (OS_TYPE == OS_DOS)  ) 
typedef unsigned long       UINT32;
 
typedef unsigned long           *PUINT32;
typedef volatile unsigned long *PVUINT32;

typedef long                INT32;
 
typedef long              *PINT32;

typedef char              * STRING;

#endif

#if   ((OS_TYPE == OS_WIN_NT4) && (UK_MODE == UK_KERNEL) )
typedef unsigned long       UINT32;
typedef unsigned long     *PUINT32;

typedef long                INT32;
typedef long              *PINT32;

#endif

#if (OS_DEPENDENT == 1) && (OS_TYPE == OS_VXWORKS)
typedef unsigned long     *PUINT32;

typedef long              *PINT32;

typedef char              * STRING;
#endif /** OS_VXWORKS **/


#define INT64               __int64
#define UINT64              unsigned __int64


typedef enum OBJECT_INIT_STATE_TAG
{
	VALID_STATE = 0x1357,
	INITIALIZED_STATE 
} OBJECT_INIT_STATE_T;


/* The following definitions should be used for each parameter in a          */
/* function's parameters list.                                               */

#if ( OS_DEPENDENT == 0 ) && !defined(IN)
#define IN  const
#endif

#define OUT
#define IO
#define I_O
#define MODULAR							/* Used only in the current module */

/* The following definitions should be used with each function's definition: */
#define GLOBAL
#define STATIC      static

#define INT3

#ifdef UNDEF_OS_DEPENDENT
#undef OS_DEPENDENT
#undef UNDEF_OS_DEPENDENT
#endif
 
#ifndef DPRINTF
#define DPRINTF(x) 
#endif

 
#ifndef NOASSERTS
 
#if ( UK_MODE == UK_USER )
 
#include <assert.h>
 
#ifdef ASSERT
#undef ASSERT
#endif

#define ASSERT(x)	assert(x)
 
#else
 
#if ( OS_TYPE == OS_LINUX )
#undef ASSERT
#define ASSERT(assert)						\
do {										\
	if (!(assert)) {						\
	    UINT32 dwTime = OsGetSystemTime();	\
		OsDebugPrintf (						\
			"%07lu.%03lu: Assertion failure in %s() at %s:%d: \"%s\"\n",	\
			 dwTime/1000, dwTime%1000, __FUNCTION__, __FILE__, __LINE__, # assert);	\
		/*OsDebugBreakpoint("");*/				\
	}										\
} while (0);
 
#define assert ASSERT
#define osAssert ASSERT
#endif /* OS_LINUX */
 
#endif /* UK_USER */
 
#endif /* NO_ASSERTS */
 
#if ( defined(NOASSERTS) || !defined(ASSERT) )
#undef ASSERT
#define ASSERT(a)
#endif
 
#if ( defined(NOASSERTS) || !defined(assert) )
#undef assert
#define assert(a)
#endif
 
#if ( defined(NOASSERTS) || !defined(osAssert) )
#undef osAssert
#define osAssert(a)
#endif

#endif	/** USE_DDK **/

#include "std_defines.h"
#include "osservices.h"

#endif /* __SYSENV_H__ */
