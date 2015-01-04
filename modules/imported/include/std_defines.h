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

/*----------------------------------------------------------------------------
 *
 * Copyright (C) 1996-2006 Conexant Systems Inc.
 * All rights reserved.
 *
 *----------------------------------------------------------------------------
 *
 * File: std_defines.h
 *
 * Define useful values and independent macros.
 *
 *----------------------------------------------------------------------------
 */

#ifndef __STD_DEFINES_H__
#define __STD_DEFINES_H__


#ifndef SCHAR_MIN
#define SCHAR_MIN   (-128)             /* minimum signed char value    */
#endif

#ifndef SCHAR_MAX
#define SCHAR_MAX     127              /* maximum signed char value    */
#endif

#ifndef UCHAR_MAX
#define UCHAR_MAX     0xff             /* maximum unsigned char value  */
#endif

#ifndef SHRT_MIN
#define SHRT_MIN    (-32768)           /* minimum (signed) short value */
#endif

#ifndef SHRT_MAX
#define SHRT_MAX      32767            /* maximum (signed) short value */
#endif

#ifndef USHRT_MAX
#define USHRT_MAX     0xffff           /* maximum unsigned short value */
#endif

#ifndef INT16_MIN
#define INT16_MIN   (-32767 - 1)
#endif

#ifndef INT16_MAX
#define INT16_MAX     32767
#endif

#ifndef UINT16_MAX
#define UINT16_MAX    0xffff
#endif

#ifndef INT32_MIN
#define INT32_MIN   (-2147483647 - 1) /* minimum (signed) 32-bit int value */
#endif

#ifndef INT32_MAX
#define INT32_MAX     2147483647    /* maximum (signed) 32-bit int value */
#endif

#ifndef UINT32_MAX
#define UINT32_MAX    0xffffffff    /* maximum unsigned 32-bit int value */
#endif

#ifndef INT_MIN
#define INT_MIN     (-2147483647 - 1)  /* minimum (signed) int value   */
#endif

#ifndef INT_MAX
#define INT_MAX       2147483647       /* maximum (signed) int value   */
#endif

#ifndef UINT_MAX
#define UINT_MAX      0xffffffff       /* maximum unsigned int value   */
#endif

#ifndef LG32_MIN
#define LG32_MIN    (-2147483647L - 1) /* minimum (signed) long value  */
#endif

#ifndef LG32_MAX
#define LG32_MAX      2147483647L      /* maximum (signed) long value  */
#endif

#ifndef LONG_MIN
#define LONG_MIN    (-2147483647L - 1) /* minimum (signed) long value  */
#endif

#ifndef LONG_MAX
#define LONG_MAX      2147483647L      /* maximum (signed) long value  */
#endif

#ifndef ULONG_MAX
#define ULONG_MAX     0xffffffffUL     /* maximum unsigned long value  */
#endif

#ifndef FLOAT_MAX
#define FLOAT_MAX      3.402823466e+38F   /* maximum (signed) float value */
#endif

#define DWORD_LEN            ( (int) 32 )


#ifndef MAX
#define MAX(a,b)             ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef MIN
#define MIN(a,b)             ( ((a) < (b)) ? (a) : (b) )
#endif

#ifndef BIT_MASK
#define BIT_MASK(bits_count) ( (1 << (bits_count)) - 1 )
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)   (sizeof(a)/sizeof(*(a)))
#endif


#define M_ZERO      ( (float) 0.0 )
#define M_ONE       ( (float) 1.0 )
#ifndef M_PI
#define M_PI        ( 3.1415926535897932384626433832795 )
#endif
#define M_PIF       ( (float) M_PI )
#ifndef M2_PI
#define M2_PI       ( 6.28318530717958647692528676655901 )
#endif
#ifndef M_SQRT2
#define M_SQRT2     ( 1.41421356237309504880 )
#endif

/* Stringification:
 * StringIt(x) will make "x"
 * StringItVal(x) will parse x and put that as a string (at least under gcc):
 * > #define XXX 88
 * > StringIt(XXX) is "XXX"
 * > StringItVal(XXX) is "88" ! */
#define StringIt(x)    #x
#define StringItVal(x) StringIt(x)

#define BYPASS_CONST(dest,type) \
	 *( (type*)(&(dest))  )

#define BYPASS_CONST_FOR_POINTER(dest,type) \
	 ( (type*)(&(dest))  )


/*--------------------------------------------------------------------------*/
enum fx_type
{
    TYPE_REAL_FX16     = 1 ,
    TYPE_COMPLEX_FX16  = 2 ,
    TYPE_REAL_FX32     = 3 ,
    TYPE_COMPLEX_FX32  = 4 ,
    TYPE_REAL_FX64     = 5
} ;

#define FX_TYPE_IS_REAL(_fx_type)  ((int)(_fx_type) & 1)
/*--------------------------------------------------------------------------*/

#endif /* __STD_DEFINES_H__ */
