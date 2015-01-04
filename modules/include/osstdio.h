/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */
#ifndef __OSSTDIO_H__
#define __OSSTDIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if ( UK_MODE == UK_KERNEL )

struct file;

typedef struct file FILE;

#ifndef FOUND_SEEK_MODES
#define SEEK_SET		0x00000000
#define SEEK_CUR		0x00000001
#define SEEK_END		0x00000002
#endif

#define EOF	(-1)

#ifndef OS_NOREDEFS

#define fopen(p, m) OsFOpen(p, m, NULL)
#define fread OsFRead
#define fwrite OsFWrite
#define fclose OsFClose
#define fseek OsFSeek

#endif // OS_NOREDEFS

__shimcall__
FILE * OsFOpen(const char *, const char *, int *);
__shimcall__
size_t OsFRead(void *, size_t, size_t, FILE *, int *);
__shimcall__
size_t OsFWrite(const void *, size_t, size_t, FILE *, int *);
__shimcall__
int OsFClose(FILE *);
__shimcall__
int OsFSeek( FILE *stream, long offset, int origin );

#else // ( UK_MODE == UK_KERNEL )

#include <stdio.h>

#define OsFOpen		fopen
#define OsFRead		fread
#define OsFWrite	fwrite
#define OsFClose	fclose
#define OsFSeek		fseek

#endif // UK_MODE == UK_KERNEL

#ifdef __cplusplus
}
#endif

#endif /* __OSSTDIO_H__ */
