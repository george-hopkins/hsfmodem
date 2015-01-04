/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#ifndef __MODEMEXPERT_H__
#define __MODEMEXPERT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "cnxt_diag_codes_ex.h"

// ModemExpert handle
typedef struct tagHME
{
    int dummy;
} *HME;


typedef enum tagModemExpertEvent
{
    kMEConnected,
    kMEDisconnected,
    kMEPortOpened,
    kMEPortClosed,
    kMECallback,
    kMEChannelCreated,
    kMEChannelDestroyed,
    kMEChannelRestarted

} ModemExpertEvent;


typedef void (*MECALLBACK)(void* pRef, HME hME, ModemExpertEvent event, CNXT_DIAG_CODES code, int subCode, UINT32 timeStamp );


HME    ModemExpertOpen         (char *pDiagDev, MECALLBACK pfnCallback, void *pRef);
int    ModemExpertClose        (HME hME);

int    ModemExpertMonitor      (HME hConnection, CNXT_DIAG_CODES      code, void* pBuf,  UINT32 *pBufSize);
int    ModemExpertControl      (HME hConnection, CNXT_DIAG_CTRL_CODES code, void* pData, UINT32 DataSize);
int    ModemExpertConfigure    (HME hConnection, CNXT_DIAG_CFG_CODES  code, void* pData, UINT32 DataSize);

#ifdef __cplusplus
}
#endif

#endif      // #ifndef __MODEMEXPERT_H__
