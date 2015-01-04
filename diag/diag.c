/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#include <signal.h>
#include <string.h>
#include <asm/byteorder.h>
#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>

#include "framewrk.h"
#include "osservices.h"

#include "modemexpert.h"
#include "osdiag_dev.h"
#include "cnxt_diag_codes_ex.h"

#define ERROR           printf
#define MESSAGE         if (fVerbose) printf

typedef enum _MblamMode {
    ModeBlam,
    ModeScr
} eMode;

HME			g_hModemExpert	= NULL;
volatile int		g_fConnected	= 0;

void EventCallback(void* pRef, HME hConnection, ModemExpertEvent type, CNXT_DIAG_CODES code, int subCode, UINT32 timeStamp);
void SignalHandler(int signal);

char DataBuf[4096];

#ifndef __LITTLE_ENDIAN
#define OsSwapUINT16(x) ((((x) & 0xFF00) >> 8) | (((x) & 0x00FF) << 8))
#define OsSwapUINT32(x) ((OsSwapUINT16(x) << 16) | (OsSwapUINT16((x) >> 16)))
#endif

#define MAX_ZERO_PACKET_NUMBER  256
#define MIN_DATA_SIZE_TO_REPORT_BLAM 4096
#define MIN_DATA_SIZE_TO_REPORT_SCR 4096

int unit;
char szDevName[80];
char szOutputFile[80];
const char *szProgName;
const char kPCIModemDriverDevName[] = "/dev/"CNXTTARGET"diag0";
const char kOutputFileName[] = "/tmp/"CNXTTARGET"diag.log";
BOOL fAppend, fNewOnPortOpen, fJustOpened, fVerbose = FALSE;
int nMinDataSizeToReport = MIN_DATA_SIZE_TO_REPORT_BLAM;

int parse_args(int argc, const char * argv[]);
void print_usage();

FILE	*fp = NULL;
eMode AppMode = ModeBlam;

int main (int argc, const char * argv[]) 
{
    if ((szProgName = strrchr(argv[0], '/')) == NULL)
	szProgName = argv[0];
    else
	szProgName++;

    if (-1 == parse_args(argc, argv)) return -1;

    // Set SIGNAL handler 
    if (SIG_ERR == signal(SIGINT, SignalHandler) )
    {
        ERROR("%s: Couldn't set signal handler\n", szProgName);
    }

    MESSAGE("%s: Output file %s\n", szProgName, szOutputFile);

    if(fVerbose)
	putenv("SHOW_MODEMEXPERT_NOTIFICATIONS=yes");

    g_hModemExpert = ModemExpertOpen(szDevName, EventCallback, NULL);

    if (g_hModemExpert)
    {
        int nZeroSizeCounter = 0, nTotalCounter = 0;

        BOOL fOpenedOnce = FALSE;
        for(;;)
        {
	    struct timeval tmout;

	    tmout.tv_sec = 0;
	    tmout.tv_usec = 200*1000; /* 200 ms */
	    select(FD_SETSIZE, NULL, NULL, NULL, &tmout);

            if (g_fConnected)
            {
                if (fAppend)
                    fp = fopen(szOutputFile, "ab");
                else
                {
                    if (fJustOpened && fNewOnPortOpen)
                    {
                        fJustOpened = FALSE;
                        fp = fopen(szOutputFile, "wb");
                    }
                    else if (!fOpenedOnce)
                        fp = fopen(szOutputFile, "wb");
                    else
                        fp = fopen(szOutputFile, "ab");
                }

                fOpenedOnce = TRUE;

                if (!fp)
                {
                    ERROR("%s: Cannot open output file %s\n", szProgName, szOutputFile);
                    exit(-1);
                }

                for (;;)
                {
                    BOOL fPrint = FALSE;
                    UINT32 dwSize = sizeof(DataBuf);
                    UINT32 *pPtr = (UINT32*)DataBuf;
                    int i;
                
                    if (ModemExpertMonitor(g_hModemExpert, 
                        (AppMode == ModeScr) ? CSDIAG_CODE_SCR_TRACE : CSDIAG_CODE_TRACE, 
                        DataBuf, &dwSize))
                        break;

#ifndef __LITTLE_ENDIAN
                    if (AppMode == ModeBlam)
                        for (i=0; i<dwSize/4; i++, pPtr++)
                            *pPtr = OsSwapUINT32(*pPtr);
#endif

                    if (dwSize == 0)
                    {
                        nZeroSizeCounter++;
                        if (nZeroSizeCounter >= MAX_ZERO_PACKET_NUMBER)
                        {
                            fPrint = TRUE;
                            MESSAGE("%s: got 0 bytes %d times\n", szProgName, nZeroSizeCounter);
                            nZeroSizeCounter = 0;
                        }
                    }
                    else
                    {
                        fwrite(DataBuf, dwSize, 1, fp);
                        nTotalCounter += dwSize;
                        if (nTotalCounter > nMinDataSizeToReport)
                        {
                            MESSAGE("%s: got %d bytes\n", szProgName, nTotalCounter);
                            nTotalCounter = 0;
                        }
                    }

                    if (dwSize < sizeof(DataBuf))
                        break;
                }

                fclose(fp);
                fp = NULL;
            }
        }
    }
    else
    {
        ERROR("%s: Modem Expert open failed\n", szProgName);
    }

    return 0;
}

int parse_args(int argc, const char * argv[]) 
{
    int i;
    fAppend = FALSE;
    fNewOnPortOpen = FALSE;
    strcpy(szDevName, kPCIModemDriverDevName);
    strcpy(szOutputFile, kOutputFileName);

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'a':
                fAppend = TRUE;
                break;
            case 'i':
                fNewOnPortOpen = TRUE;
                break;
            case 'c':
                strcpy(szDevName, &argv[i][2]);
                break;
            case 'o':
                strcpy(szOutputFile, &argv[i][2]);
                break;
            case 'v':
                fVerbose = TRUE;
                break;
            case 's':
                MESSAGE("SCR mode\n");
                AppMode = ModeScr;
                nMinDataSizeToReport = MIN_DATA_SIZE_TO_REPORT_SCR;
                break;
            default:
                print_usage();
                return -1;
            }
        }
        else
        {
            print_usage();
            return -1;
        }
    }
    if (fNewOnPortOpen) fAppend = FALSE;

    MESSAGE("Using class %s; output file %s\n", szDevName, szOutputFile);
    return 0;
}

void print_usage()
{
    printf("%s [-cDevName] [-oOutputFile] [-aivs]\n", szProgName);
    printf("Where -a - append\n");
    printf("      -v - verbose\n");
    printf("      -i - new file on each port open\n");
    printf("      -s - SCR mode\n");
    printf("Defaults:\n");
    printf("%s -c%s -o%s\n", szProgName, kPCIModemDriverDevName, kOutputFileName);
}

void SignalHandler(int signal)
{
    // Clean up
    MESSAGE("Cleaning up...\n");
    if(g_hModemExpert)
        ModemExpertClose(g_hModemExpert);

    if (fp)
    {
        fflush(fp);
        fclose(fp);
        fp = NULL;
    }
    exit(0);
}

void EventCallback(void* pRef, HME hConnection, ModemExpertEvent type, CNXT_DIAG_CODES code, int subCode, UINT32 timeStamp)
{
    switch(type)
    {
        case kMEConnected:
            MESSAGE("%s: Modem Connected\n", szProgName);
            g_fConnected = 1;
            break;
        case kMEDisconnected:
            MESSAGE("%s: Modem Disconnected\n", szProgName);
            g_fConnected = 0;
	    SignalHandler(SIGHUP);
            break;
        case kMEPortOpened:
            MESSAGE("%s:  Port opened\n", szProgName);
            fJustOpened = TRUE;
            break;
        case kMEPortClosed:
            MESSAGE("%s:  Port closed\n", szProgName);
            break;
        default:
            break;
    }
}
