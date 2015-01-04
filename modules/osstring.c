/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#define OS_NOSTRINGREDEFS

#include "oscompat.h"
#include "osservices.h"

__shimcall__
PVOID OsMemSet(PVOID pBuf, UINT8 c, UINT32 Count)
{
    return memset(pBuf, c, Count);
}

__shimcall__
PVOID OsMemMove(PVOID pDest, PVOID pSrc, UINT32 Count)
{
    return memmove(pDest, pSrc, Count);
}

__shimcall__
PVOID OsMemCpy(PVOID pDest, PVOID pSrc, UINT32 Count)
{
    return memcpy(pDest, pSrc, Count);
}

__shimcall__
int OsMemCmp(PVOID pBuff1, PVOID pBuff2, UINT32 Count)
{
    return memcmp(pBuff1, pBuff2, Count);
}

__shimcall__
PVOID OsStrCpy(LPSTR szDest, LPCSTR szSrc)
{
    return strcpy(szDest, szSrc);
}

__shimcall__
PVOID OsStrnCpy(LPSTR szDest, LPCSTR szSrc, int MaxSize)
{
    return strncpy(szDest, szSrc, MaxSize);
}

__shimcall__
LPSTR OsStrCat(LPSTR szDest, LPCSTR szSrc)
{
    return strcat(szDest, szSrc);
}

__shimcall__
LPSTR OsStrnCat(LPSTR szDest, LPCSTR szSrc, int MaxSize)
{
    return strncat(szDest, szSrc, MaxSize);
}

__shimcall__
int OsStrCmp(LPCSTR szStr1, LPCSTR szStr2)
{
    return strcmp(szStr1, szStr2);
}

__shimcall__
int OsStrnCmp(LPCSTR szStr1, LPCSTR szStr2, UINT32 Count)
{
    return strncmp(szStr1, szStr2, Count);
}

__shimcall__
int OsStrLen(const char * string)
{
    return strlen(string);
}

__shimcall__
int OsToupper(int ch)
{
    return (ch >='a' && ch <= 'z') ? ch-'a'+'A' : ch;
}

__shimcall__
int OsTolower(int ch)
{
    return (ch >='A' && ch <= 'Z') ? ch-'A'+'a' : ch;
}

__shimcall__
int OsIsDigit(int ch)
{
    return (ch >='0' && ch <='9');
}

__shimcall__
int OsVSprintf(LPSTR buffer, LPCSTR format, va_list ap)
{
    return vsprintf(buffer, format, ap);
}

__shimcall__
int OsSprintf(LPSTR buffer, LPCSTR format, ...)
{
    va_list   args;
    int    RetVal;

    va_start (args , format);
    RetVal = vsprintf(buffer,format,args);
    va_end (args);
    return RetVal;
}

__shimcall__
int OsAtoi(LPCSTR szStr)
{
    const char *temp_string = szStr;
    int temp = 0;
    while ( *temp_string ) {
	temp <<= 4;
	if ( OsIsDigit(*temp_string)) {
	    temp += (*temp_string - '0');
	}
	if (*temp_string <= 'f' && *temp_string >= 'a') {
	    temp += ((*temp_string - 'a')+10);
	}
	if (*temp_string <= 'F' && *temp_string >= 'A') {
	    temp += ((*temp_string - 'A')+10);
	}
	temp_string++;  
    }
    return (temp);
}

