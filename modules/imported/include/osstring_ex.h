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

#ifndef __OS_STRING_EX_H__
#define __OS_STRING_EX_H__
#include "osservices.h"

#ifndef OS_NOREDEFS

#if !(( UK_MODE == UK_USER ) && (OS_TYPE == OS_LINUX))
#if !(defined(__GNUC__) && defined(__i386__) && !defined(OS_NOSTRINGREDEFS) && !defined(_I386_STRING_H_))
#undef memset
#define memset          OsMemSet
#undef memcpy
#define memcpy          OsMemCpy
#undef memcmp
#define memcmp          OsMemCmp
#undef memmove
#define memmove         OsMemMove

#undef strcpy
#define strcpy          OsStrCpy
#undef strncpy
#define strncpy         OsStrnCpy
#undef strcat
#define strcat          OsStrCat
#undef strncat
#define strncat         OsStrnCat
#undef strcmp
#define strcmp          OsStrCmp
#undef strncmp
#define strncmp         OsStrnCmp
#undef strlen
#define strlen          OsStrLen
#endif /* !(defined(__GNUC__) && defined(__i386__) && !defined(OS_NOSTRINGREDEFS) && !defined(_I386_STRING_H_)) */
#undef toupper
#define toupper         OsToupper
#undef tolower
#define tolower         OsTolower
#undef isdigit
#define isdigit         OsIsDigit
#undef sprintf
#define sprintf         OsSprintf
#undef atoi
#define atoi			OsAtoi
#endif

#endif /* OS_NOREDEFS */

#endif /* __OS_STRING_EX_H__ */
#if 0
/*************************************************************************

File Name: OsString.h

File Description: This file implements all the string/memory utilities.

************************************************************************/

/*
 * Copyright (c) 2001 Conexant Systems, Inc.
 * 
 * 1.   Permitted use. Redistribution and use in source and binary forms,
 * with or without modification, are permitted under the terms set forth
 * herein.
 * 
 * 2.   Disclaimer of Warranties. CONEXANT AND OTHER CONTRIBUTORS MAKE NO
 * REPRESENTATION ABOUT THE SUITABILITY OF THIS SOFTWARE FOR ANY PURPOSE.
 * IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTIES OF ANY KIND.
 * CONEXANT AND OTHER CONTRIBUTORS DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, GOOD TITLE AND AGAINST INFRINGEMENT.
 * 
 * This software has not been formally tested, and there is no guarantee that
 * it is free of errors including, but not limited to, bugs, defects,
 * interrupted operation, or unexpected results. Any use of this software is
 * at user's own risk.
 * 
 * 3.   No Liability.
 * 
 * (a) Conexant or contributors shall not be responsible for any loss or
 * damage to user, or any third parties for any reason whatsoever, and
 * CONEXANT OR CONTRIBUTORS SHALL NOT BE LIABLE FOR ANY ACTUAL, DIRECT,
 * INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL, OR CONSEQUENTIAL
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED, WHETHER IN CONTRACT, STRICT OR OTHER LEGAL THEORY OF
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 * (b) User agrees to hold Conexant and contributors harmless from any
 * liability, loss, cost, damage or expense, including attorney's fees,
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
 */
#ifndef __OS_STRING_EX_H__
#define __OS_STRING_EX_H__

#if ( (OS_ONE_OF(OS_WIN_NT4 | OS_WIN_WDM)) && (UK_MODE == UK_KERNEL) )
#define _INTSIZEOF(n)    ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v) ap = (char *)&v + _INTSIZEOF(v)
#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap) ap = (char *)0
typedef char * va_list;

#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if !(( UK_MODE == UK_USER ) && (OS_TYPE == OS_LINUX))

#ifndef OS_NOREDEFS

#undef memset
#define memset          OsMemSet
#undef memcpy
#define memcpy          OsMemCpy
#undef memcmp
#define memcmp          OsMemCmp
#undef memmove
#define memmove         OsMemMove

#undef strcpy
#define strcpy          OsStrCpy
#undef strncpy
#define strncpy         OsStrnCpy
#undef strcat
#define strcat          OsStrCat
#undef strncat
#define strncat         OsStrnCat
#undef strcmp
#define strcmp          OsStrCmp
#undef strncmp
#define strncmp         OsStrnCmp
#undef toupper
#define toupper         OsToupper
#undef tolower
#define tolower         OsTolower
#undef isdigit
#define isdigit         OsIsDigit
#undef sprintf
#define sprintf         OsSprintf
#undef strlen
#define strlen          OsStrLen

#endif /* OS_NOREDEFS */

#if ( USE_PRAGMA_PACK == 1 )
#pragma pack(push,1)   /* Packing compatible with WinAC */
#endif


/********************************************************************************
	
Function Name: OsMemSet.

Function Description:The function initializes a buffer to a specified character.

Arguments: Buffer - Pointer to buffer to contain copied characters.
		   c - Character to copy into the buffer.
		   Count - Number of times to copy the character into the buffer.

Return Value: A pointer to Buffer.

********************************************************************************/
StdCall(VOID *, OsMemSet(VOID *Buffer, int c, size_t Count));

/********************************************************************************

Function Name: OsMemCpy.

Function Description:The function copies a specified number of characters from one 
					 buffer to another. This function may not work correctly if 
					 the buffers overlap. 

Arguments: BuffDest - Pointer to buffer to contain copied characters.
		   BuffSrc - Pointer to buffer with the characters to copy.
		   Count - Number of characters to copy.

Return Value: A pointer to BuffDest.

********************************************************************************/
StdCall(VOID *, OsMemCpy(OUT VOID *BuffDest, IN VOID *BuffSrc, size_t Count));
/********************************************************************************

Function Name: OsMemCmp.

Function Description:The function compares a specified number of characters from two buffers.

Arguments: Buff1 - Pointer to first buffer to compare.
		   Buff2 - Pointer to second buffer to compare.
		   Count - Number of characters to compare.

Return Value: Returns one of three values as follows:
													0 if buffers match.
													< 0 if buff1 is less than buff2.
													>0 if buff1 is greater than buff2.

********************************************************************************/
StdCall(int, OsMemCmp(IN VOID *Buff1, IN VOID *Buff2, size_t Count));
/********************************************************************************

Function Name: OsMemMove.

Function Description:The memmove function copies count bytes of characters from 
			src to dest. If some regions of the source area and the destination 
			overlap, memmove ensures that the original source bytes in the 
			overlapping region are copied before being overwritten.
Arguments: dest - Destination object.
		   src - Source object
		   count - NNumber of bytes of characters to copy.

Return Value: memmove returns the value of dest
 
********************************************************************************/
StdCall(VOID *, OsMemMove(OUT VOID *dest, IN VOID *src, size_t count ));


/********************************************************************************

Function Name: OsStrCpy.

Function Description:The function copies a specified a string from one 
					 buffer to another. This function may not work correctly if 
					 the buffers overlap. 

Arguments: BuffDest - Pointer to buffer to contain copied string.
		   BuffSrc - Pointer to buffer with the string to copy.

Return Value: A pointer to BuffDest.

********************************************************************************/
StdCall(VOID *, OsStrCpy(OUT CHAR *BuffDest, IN CHAR *BuffSrc));

/********************************************************************************

Function Name: OsStrnCpy.

Function Description:The function copies a specified a string from one 
					 buffer to another. The function does not copy more 
					 than MaxSize bytes.The behavior of strncpy is 
					 undefined if the source and destination strings overlap

Arguments: BuffDest - Pointer to buffer to contain copied string.
		   BuffSrc - Pointer to buffer with the string to copy.
		   MaxSize - Max chars to copy

Return Value: A pointer to BuffDest.

********************************************************************************/
StdCall(CHAR *, OsStrnCpy(OUT CHAR *BuffDest, IN CHAR *BuffSrc, size_t MaxSize));

/********************************************************************************

Function Name: OsStrCat.

Function Description:The function copies a specified a string from one 
					 buffer to another, at its end. This function may not work correctly if 
					 the buffers overlap. 

Arguments: BuffDest - Pointer to buffer to contain copied string.
		   BuffSrc - Pointer to buffer with the string to copy.

Return Value: A pointer to BuffDest.

********************************************************************************/
StdCall(CHAR *, OsStrCat(OUT CHAR *BuffDest, IN CHAR *BuffSrc));

/********************************************************************************

Function Name: OsStrnCat.

Function Description:The function copies a specified a string from one 
					 buffer to another, at its end. The function does not copy
					 more than MaxSize bytes.The behavior of strncpy is 
					 undefined if the source and destination strings overlap

Arguments: BuffDest - Pointer to buffer to contain copied string.
		   BuffSrc - Pointer to buffer with the string to copy.
		   MaxSize - Max chars to copy

Return Value: A pointer to BuffDest.

********************************************************************************/
StdCall(CHAR *, OsStrnCat(OUT CHAR *BuffDest, IN CHAR *BuffSrc, size_t MaxSize));

/********************************************************************************

Function Name: OsStrCmp.

Function Description:The strcmp function compares string1 and string2 
		lexicographically and returns a value indicating their relationship

Arguments: string1 ,string2 - Null terminated strings to compare

Return Value: lexicographic relation of string1 to string2.

********************************************************************************/
StdCall(int, OsStrCmp(IN CHAR *string1, IN CHAR *string2));
/********************************************************************************

Function Name: OsStrnCpy.

Function Description:The strcmp function compares string1 and string2 
		lexicographically and returns a value indicating their relationship
Arguments: string1 ,string2 - Null terminated strings to compare
					MaxSize	- Number of characters to compare

Return Value: lexicographic relation of string1 to string2.

********************************************************************************/
StdCall(int, OsStrnCmp(IN CHAR *string1, IN CHAR *string2, size_t MaxSize));
/********************************************************************************

Function Name: OsToupper.

Function Description:converts a charecter c to upper case  

Arguments: c - charected to be converted to upper case.

Return Value: upper case charecter of c.

********************************************************************************/
StdCall(int, OsToupper(int c));
/********************************************************************************

Function Name: OsTolower.

Function Description:converts a charecter c to lower case  

Arguments: c - charected to be converted to lower case.

Return Value: lower case charecter of c.

********************************************************************************/
StdCall(int, OsTolower(int c));
/********************************************************************************

Function Name: OsIsDigit.

Function tests if a charecter is a decimal digit. 

Arguments: c - integer to test.

Return Value: non zero value if c is a decimal digit('0' - '9').

********************************************************************************/
StdCall(int, OsIsDigit(int c));

/********************************************************************************

Function Name: OsSprintf.

Function Description:The OsSprintf function formats and stores a series of 
						characters and values in buffer. 

Arguments: buffer - Pointer to buffer to contain copied string.
		   format - control string


Return Value: OsSprintf returns the number of bytes stored in buffer, 
				not counting the terminating null character. 

********************************************************************************/
int	__cdecl  OsSprintf(OUT CHAR *buffer, IN CHAR *format_string, ...);

/********************************************************************************

Function Name: OSSprintfFixedArgs.

Function Description:The OSSprintfFixedArgs function formats and stores a series of 
						characters and values in buffer. The function recieves the 
						format parameters not on the stack, but using the va_list
						parameter

Arguments: buffer - Pointer to buffer to contain copied string.
		   format - control string.
		   StartedVaList - pointer to va_list that has been started to include the format arguments


Return Value: OsSprintf returns the number of bytes stored in buffer, 
				not counting the terminating null character. 

********************************************************************************/
StdCall(int, OSSprintfFixedArgs(OUT CHAR *buffer, IN CHAR *format, IN CHAR *StartedVaList));

/********************************************************************************

Function Name: OsStrLen.

Function Description:The function returns the length of a null-terminated string.

Arguments: String - Pointer to buffer to contain copied string.

Return Value: int - the length of a null-terminated string.

********************************************************************************/
StdCall(size_t, OsStrLen(IN CHAR *String));

 
int Osatoi(IN PCHAR String);
 
#if ( USE_PRAGMA_PACK == 1 )
#pragma pack(pop)   /* Packing compatible with our VXDs */
#endif

#endif /* UK_MODE == UK_KERNEL */

#ifdef __cplusplus
}
#endif

#endif /* __OS_STRING_EX_H__ */
#endif
