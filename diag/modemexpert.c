/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/queue.h>
#include <string.h>

#include "framewrk.h"
#include "osservices.h"

#include "modemexpert.h"
#include "osdiag_dev.h"

#ifndef NO_SHOW_MODEMEXPERT_NOTIFICATIONS
#define SHOW_MODEMEXPERT_NOTIFICATIONS
#endif

typedef struct tagMECData
{
    LIST_ENTRY(tagMECData) linkage;

    int fd;
    MECALLBACK              pfnCallback;
    void                    *pRef;

} MEData, *PMEData;

#ifdef SHOW_MODEMEXPERT_NOTIFICATIONS
#define codenamestr(p,x) { p##_##x, #x }

struct codename {
    CNXT_DIAG_CODES code;
    char *name;
};

static struct codename csDiagCodeNames[] = {
	codenamestr(CSDIAG,DRIVER_VERSION),
	codenamestr(CSDIAG,HARDWARE_PNP_ID),
	codenamestr(CSDIAG,MODEM_FRIENDLY_NAME),
	codenamestr(CSDIAG,MODEM_COM_PORT),
	codenamestr(CSDIAG,DTE_EVENT_MASK),
	codenamestr(CSDIAG,LAST_NUMBER_DIALED),
	codenamestr(CSDIAG,V8_CM),
	codenamestr(CSDIAG,V8_JM),
	codenamestr(CSDIAG,V8BIS_FLEX),
	codenamestr(CSDIAG,V8BIS_DSP_VER),
	codenamestr(CSDIAG,V34_INFO),
	codenamestr(CSDIAG,MP_SEQ),
	codenamestr(CSDIAG,PROTOCOL_NEG_RES),
	codenamestr(CSDIAG,EC_FRAME_SIZE),
	codenamestr(CSDIAG,EC_LINK_TIMEOUTS),
	codenamestr(CSDIAG,EC_LINK_NAKS),
	codenamestr(CSDIAG,CMPRS_NEG_RES),
	codenamestr(CSDIAG,CMPRS_DICT_SIZE),
	codenamestr(CSDIAG,SNR),
	codenamestr(CSDIAG,MSE),
	codenamestr(CSDIAG,EQM),
	codenamestr(CSDIAG,RX_SCATTER),
	codenamestr(CSDIAG,RX_NORM_FACTOR),
	codenamestr(CSDIAG,ROUND_TRIP_DELAY),
	codenamestr(CSDIAG,TX_SIGNAL_POWER),
	codenamestr(CSDIAG,RX_SIGNAL_POWER),
	codenamestr(CSDIAG,PAD_DETECTED),
	codenamestr(CSDIAG,RBS_DETECTED),
	codenamestr(CSDIAG,NE_ECHO_POWER),
	codenamestr(CSDIAG,FE_ECHO_POWER),
	codenamestr(CSDIAG,TX_SYMBOL_RATE),
	codenamestr(CSDIAG,RX_SYMBOL_RATE),
	codenamestr(CSDIAG,TX_CARRIER_FREQUENCY),
	codenamestr(CSDIAG,RX_CARRIER_FREQUENCY),
	codenamestr(CSDIAG,AGC_MIN),
	codenamestr(CSDIAG,AGC_MAX),
	codenamestr(CSDIAG,AGC_LAST),
	codenamestr(CSDIAG,120_HZ_POWER),
	codenamestr(CSDIAG,HIGHPASS_FILTER_ENABLED),
	codenamestr(CSDIAG,HIGHEST_RX_STATE),
	codenamestr(CSDIAG,HIGHEST_TX_STATE),
	codenamestr(CSDIAG,MODEM_STATE),
	codenamestr(CSDIAG,HANDSHAKE_SEQUENCE),
	codenamestr(CSDIAG,CALL_SETUP_RES),
	codenamestr(CSDIAG,MULTI_MEDIA_MODE),
	codenamestr(CSDIAG,TERMINATION_CAUSE),
	codenamestr(CSDIAG,RATE_RENEG_EV_CNT),
	codenamestr(CSDIAG,RATE_RENEG_REQ),
	codenamestr(CSDIAG,RATE_RENEG_GRANTED),
	codenamestr(CSDIAG,RATE_RENEG_REASON),
	codenamestr(CSDIAG,RATE_RENEG_SILENCE_REQ),
	codenamestr(CSDIAG,RATE_RENEG_SILENCE_GRANTED),
	codenamestr(CSDIAG,RATE_RENEG_SILENCE_REASON),
	codenamestr(CSDIAG,RTRN_REQ),
	codenamestr(CSDIAG,RTRN_GRANTED),
	codenamestr(CSDIAG,RTRN_REASON),
	codenamestr(CSDIAG,TIME_CONNECTED),
	codenamestr(CSDIAG,TX_BIT_RATE_FIRST),
	codenamestr(CSDIAG,RX_BIT_RATE_FIRST),
	codenamestr(CSDIAG,TX_BIT_RATE_MIN),
	codenamestr(CSDIAG,RX_BIT_RATE_MIN),
	codenamestr(CSDIAG,TX_BIT_RATE_MAX),
	codenamestr(CSDIAG,RX_BIT_RATE_MAX),
	codenamestr(CSDIAG,TX_BIT_RATE),
	codenamestr(CSDIAG,RX_BIT_RATE),
	codenamestr(CSDIAG,TX_NEG_RES),
	codenamestr(CSDIAG,RX_NEG_RES),
	codenamestr(CSDIAG,CARRIER_LOSS_EV_CNT),
	codenamestr(CSDIAG,CALL_WAIT_EV_CNT),
	codenamestr(CSDIAG,EXTENSION_PICKUP_EV_CNT),
	codenamestr(CSDIAG,V34_FALLBACK_REASON),
	codenamestr(CSDIAG,TOTAL_TX_BLOCKS),
	codenamestr(CSDIAG,TOTAL_RX_BLOCKS),
	codenamestr(CSDIAG,TOTAL_TX_CHARS),
	codenamestr(CSDIAG,TOTAL_RX_CHARS),
	codenamestr(CSDIAG,CPU_VENDOR),
	codenamestr(CSDIAG,CPU_SPEED),
	codenamestr(CSDIAG,CACHE_SIZE),
	codenamestr(CSDIAG,LAST_INT_CPU),
	codenamestr(CSDIAG,LAST_INT_LATENCY),
	codenamestr(CSDIAG,MAX_INT_CPU),
	codenamestr(CSDIAG,MAX_INT_LATENCY),
	codenamestr(CSDIAG,SAMPLES_OVERRUNS),
	codenamestr(CSDIAG,SAMPLES_UNDERRUNS),
	codenamestr(CSDIAG,BUS_OVERRUNS),
	codenamestr(CSDIAG,BUS_UNDERRUNS),
	codenamestr(CSDIAG,LINE_CURRENT),
	codenamestr(CSDIAG,LINE_VOLTAGE),
	codenamestr(CSDIAG,LINE_RESISTANCE),
	codenamestr(CSDIAG,MODEM_FAMILY_TYPE),
	codenamestr(CSDIAG,CURRENT_COUNTRY),
	codenamestr(CSDIAG,TX_TRELLIS_STATES),
	codenamestr(CSDIAG,RX_TRELLIS_STATES),
	codenamestr(CSDIAG,TIME_DTR_TO_HANDSHAKE),
	codenamestr(CSDIAG,TIME_DTR_TO_PROTOCOL),
	codenamestr(CSDIAG,QC_ENABLED),
	codenamestr(CSDIAG,SERVER_TYPE),
	codenamestr(CSDIAG,INFO0_V92),
	codenamestr(CSDIAG,QC_SYGNAL_MAP),
	codenamestr(CSDIAG,MH_ENABLED),
	codenamestr(CSDIAG,CID_ENABLED),
	codenamestr(CSDIAG,TIME_ON_HOLD),
	codenamestr(CSDIAG,SERVER_HOLD_DURATION),
	codenamestr(CSDIAG,MOH_REMOTE_RESPONSE),
	codenamestr(CSDIAG,CALLER_ID_STRING),
	codenamestr(CSDIAG,MOH_STATUS),
	codenamestr(CSDIAG,MOH_REMOTE_REQUEST),
	codenamestr(CSDIAG,MOH_LOCAL_RESPONSE),
	codenamestr(CSDIAG,TIMEOUT_FOR_USER_RESPONSE),
	codenamestr(CSDIAG,NEGOTIATION_TIME),
	codenamestr(CSDIAG,K56_RATES_SUPORTED),
	codenamestr(CSDIAG,PULSE_DIALING_ENABLED),
	codenamestr(CSDIAG,LAST_DIAL_TYPE),
	codenamestr(CSDIAG,FILTER_COEFFICIENTS),
	codenamestr(CSDIAG,REMOTE_MSE),
	codenamestr(CSDIAG,REMOTE_SNR),
	codenamestr(CSDIAG,REMOTE_RX_SIGNAL_POWER),
	codenamestr(CSDIAG,REMOTE_RX_BIT_RATE),
	codenamestr(CSDIAG,REMOTE_TX_BIT_RATE),
	codenamestr(CSDIAG,CONNECTION_DIRECTION),
	codenamestr(CSDIAG,CID_SETTING),
	codenamestr(CSDIAG,MOH_METHOD),
	codenamestr(CSDIAG,OGC_ENABLED),
	codenamestr(CSDIAG,HANDSET_EXCLUSIVE_BIT),
	codenamestr(CSDIAG,APP_V92_AWARE),
	codenamestr(CSDIAG,EEPROM_PNP_IF),
	codenamestr(CSDIAG,BUS_IF),
	codenamestr(CSDIAG,DEVICE_TYPE),
	codenamestr(CSDIAG,POWER_TYPE),
	codenamestr(CSDIAG,CHECK_PME),
	codenamestr(CSDIAG,GET_GPIO),
	codenamestr(CSDIAG,GET_HW_INPUT),
	codenamestr(CSDIAG,GET_TEST_RX_BUFFER),
	codenamestr(CSDIAG,SESSION_ID),
	codenamestr(CSDIAG,PREVIOUS_TERMINATION_CAUSE),
	codenamestr(CSDIAG,PREVIOUS_SESSION_ID),
	codenamestr(CSDIAG,CODE_TRACE),
	codenamestr(CSDIAG,CODE_SCR_TRACE),
	codenamestr(CSDIAG,CODE_TRACE_MASK),
	codenamestr(CSDIAG,DEFINED_DIAG_CODE_RANGES),
	codenamestr(CSDIAG,DEFINED_CURRENT_CALL_SDK_DIAG_CODE_RANGES),
	codenamestr(CSDIAG,DEFINED_PREVIOUS_CALL_SDK_DIAG_CODE_RANGES),
	codenamestr(CSDIAG,SIGNATURE),
	codenamestr(CSDIAG,ACF_GET_LAST_IN_BAND_AT_RESPONSE),
	codenamestr(CSDIAG,LAST),

	codenamestr(CSDIAG,TRAP_MODEM_STATE),
	codenamestr(CSDIAG,TRAP_TERMINATION_CAUSE),
	codenamestr(CSDIAG,TRAP_CALL_SETUP_RES),
	codenamestr(CSDIAG,TRAP_MOH_REMOTE_RESPONSE),
	codenamestr(CSDIAG,TRAP_MOH_STATUS),
	codenamestr(CSDIAG,TRAP_MOH_LOCAL_RESPONSE),
	codenamestr(CSDIAG,TRAP_APP_V92_AWARE),
	codenamestr(CSDIAG,TRAP_PORT_STATE),
	codenamestr(CSDIAG,TRAP_SINGLE_TONE_DETECTED),
	codenamestr(CSDIAG,TRAP_DTMF_GENERATOR_RESULT),
	codenamestr(CSDIAG,TRAP_DTMF_DETECTOR_RESULT),
	codenamestr(CSDIAG,TRAP_RING_DETECTOR_RESULT),
	codenamestr(CSDIAG,TRAP_RECIEVE_DATA),
	codenamestr(CSDIAG,TRAP_OVP_DETECTED),
	codenamestr(CSDIAG,TRAP_RLSD_DETECTED),
	codenamestr(CSDIAG,TRAP_LAST),
	{ 0, NULL }
};

static struct codename diagMOHRemoteReqNames[] = {
	codenamestr(SDK_MOH_REMOTE_REQ,HOLD),
	codenamestr(SDK_MOH_REMOTE_REQ,RECONNECT),
	codenamestr(SDK_MOH_REMOTE_REQ,DISCONNECT),
	codenamestr(SDK_MOH_REMOTE_REQ,ACCEPT),
	codenamestr(SDK_MOH_REMOTE_REQ,NONE),
	{ 0, NULL }
};

static struct codename diagMOHRemoteRespNames[] = {
	codenamestr(SDK_MOH_REMOTE_RESPONSE,ACK),
	codenamestr(SDK_MOH_REMOTE_RESPONSE,NACK),
	codenamestr(SDK_MOH_REMOTE_RESPONSE,TIMEDOUT),
	codenamestr(SDK_MOH_REMOTE_RESPONSE,SILENCE),
	codenamestr(SDK_MOH_REMOTE_RESPONSE,DISCONNECT),
	codenamestr(SDK_MOH_REMOTE_RESPONSE,NONE),
	{ 0, NULL }
};

static struct codename diagModemStateNames[] = {
	codenamestr(DIAG,INITIALIZING),
	codenamestr(DIAG,IDLE),
	codenamestr(DIAG,OFF_HOOK_IDLE),
	codenamestr(DIAG,DIALTONE_VALIDATE),
	codenamestr(DIAG,DIALING),
	codenamestr(DIAG,RING_ON),
	codenamestr(DIAG,CALL_PROGRESS),
	codenamestr(DIAG,V8),
	codenamestr(DIAG,V8BIS),
	codenamestr(DIAG,TRAINING),
	codenamestr(DIAG,PHYS_LAYER_CONNECT),
	codenamestr(DIAG,PROT_LAYER_CONNECT),
	codenamestr(DIAG,REMOTE_INIT_RATE_RENEG),
	codenamestr(DIAG,LOCAL_INIT_RATE_RENEG),
	codenamestr(DIAG,REMOTE_INIT_RTRN),
	codenamestr(DIAG,LOCAL_INIT_RTRN),
	codenamestr(DIAG,ESCAPE),
	codenamestr(DIAG,CLEARDOWN),
	codenamestr(DIAG,D3),
	codenamestr(DIAG,CALL_WAITING_TONE_DETECTED),
	codenamestr(DIAG,CALLER_ID_IN_PROGRESS),
	codenamestr(DIAG,MOH),
	codenamestr(DIAG,MOH_RESUMING),
	codenamestr(DIAG,MOH_CLEARING_DOWN),
	codenamestr(DIAG,PORT_TO_CLOSE),
	codenamestr(DIAG,MODEM_STATE_LAST),
	{ 0, NULL }
};

static struct codename diagMOHStateNames[] = {
	codenamestr(MOH_STATE,NONE),
	codenamestr(MOH_STATE,AS_DETECTED),
	codenamestr(MOH_STATE,CALLER_ID_PROCESSING),
	codenamestr(MOH_STATE,WAIT_FOR_USER_DECISION),
	codenamestr(MOH_STATE,WAIT_FOR_CALL_RESUME),
	codenamestr(MOH_STATE,REMOTE_REQ_ONHOLD),
	codenamestr(MOH_STATE,WAIT_FOR_CARRIER_SIGNAL),
	codenamestr(MOH_STATE,WAIT_FOR_MODULATION_DISCONNECT),
	codenamestr(MOH_STATE,NOT_SUPPORTED),
	codenamestr(MOH_STATE,WAIT_FOR_REMOTE_RECONNECT),
	codenamestr(MOH_STATE,WAIT_FOR_LOCAL_RECONNECT),
	codenamestr(MOH_STATE,WAIT_FOR_REMOTE_RESUME),
	codenamestr(MOH_STATE,OUTGOING_CALL_REQ),
	codenamestr(MOH_STATE,WAIT_FOR_HANDSET_OFFHOOK),
	codenamestr(MOH_STATE,WAIT_FOR_HANDSET_ONHOOK),
	codenamestr(MOH_STATE,WAIT_FOR_REMOTE_RESPONSE),
	codenamestr(MOH_STATE,LAST),
	{ 0, NULL }
};

static struct codename diagCallSetupResNames[] = {
	codenamestr(UD_CallSetupInfo,None),
	codenamestr(UD_CallSetupInfo,NoDialTone),
	codenamestr(UD_CallSetupInfo,FastBusy),
	codenamestr(UD_CallSetupInfo,Busy),
	codenamestr(UD_CallSetupInfo,UnknownSignal),
	codenamestr(UD_CallSetupInfo,Voice),
	codenamestr(UD_CallSetupInfo,DataAns),
	codenamestr(UD_CallSetupInfo,DataCall),
	codenamestr(UD_CallSetupInfo,FaxAns),
	codenamestr(UD_CallSetupInfo,FaxCall),
	codenamestr(UD_CallSetupInfo,V8bis),
	{ 0, NULL }
};

static struct codename diagTerminationCauseNames[] = {
	codenamestr(UD_CallTermination,CauseUndefined),
	codenamestr(UD_CallTermination,NoPreviousCall),
	codenamestr(UD_CallTermination,CallInProgress),
	codenamestr(UD_CallTermination,CallWaiting),
	codenamestr(UD_CallTermination,Delayed),
	codenamestr(UD_CallTermination,InacTimerExpired),
	codenamestr(UD_CallTermination,DTRDrop),
	codenamestr(UD_CallTermination,BlackListed),
	codenamestr(UD_CallTermination,ExtOffhook),
	codenamestr(UD_CallTermination,S7Expired),
	codenamestr(UD_CallTermination,LoopCurrentInterrupted),
	codenamestr(UD_CallTermination,NoDialTone),
	codenamestr(UD_CallTermination,Voice),
	codenamestr(UD_CallTermination,ReorderTone),
	codenamestr(UD_CallTermination,CarrierLost),
	codenamestr(UD_CallTermination,TrainingFailed),
	codenamestr(UD_CallTermination,NoModinCommon),
	codenamestr(UD_CallTermination,RetrainedFail),
	codenamestr(UD_CallTermination,GSTNCleardown),
	codenamestr(UD_CallTermination,FaxDetected),
	codenamestr(UD_CallTermination,AnykeyAbort),
	codenamestr(UD_CallTermination,ATH),
	codenamestr(UD_CallTermination,ATZ),
	codenamestr(UD_CallTermination,FrameReject),
	codenamestr(UD_CallTermination,NoECEstablished),
	codenamestr(UD_CallTermination,ProtViolation),
	codenamestr(UD_CallTermination,n400Exceeded),
	codenamestr(UD_CallTermination,NegotiatFailed),
	codenamestr(UD_CallTermination,DiscontFrame),
	codenamestr(UD_CallTermination,SabmeFrame),
	codenamestr(UD_CallTermination,LostSync),
	codenamestr(UD_CallTermination,DLG),
	codenamestr(UD_CallTermination,LineInUse),
	codenamestr(UD_CallTermination,MOHTimeout),
	codenamestr(UD_CallTermination,PulseAbort),
	codenamestr(UD_CallTermination,LineMissing),
	codenamestr(UD_CallTermination,MOHDisconnect),
	{ 0, NULL }
};

static struct codename diagModNegResNames[] = {
	codenamestr(UD_MOD,V17),
	codenamestr(UD_MOD,V21),
	codenamestr(UD_MOD,V22),
	codenamestr(UD_MOD,V22bis),
	codenamestr(UD_MOD,V23CC),
	codenamestr(UD_MOD,V23SC),
	codenamestr(UD_MOD,V27ter),
	codenamestr(UD_MOD,V29HD),
	codenamestr(UD_MOD,V32),
	codenamestr(UD_MOD,V32bis),
	codenamestr(UD_MOD,V34),
	codenamestr(UD_MOD,V90),
	codenamestr(UD_MOD,V92),
	codenamestr(UD_MOD,K56),
	codenamestr(UD_MOD,B212A),
	codenamestr(UD_MOD,B103),
	codenamestr(UD_MOD,Unknown),
	{ 0, NULL }
};

static struct codename diagProtResNames[] = {
	codenamestr(UD_Protocol,NONE),
	codenamestr(UD_Protocol,LAPM),
	codenamestr(UD_Protocol,ALT),
	{ 0, NULL }
};

static struct codename diagCmprsResNames[] = {
	codenamestr(UD_Compression,NONE),
	codenamestr(UD_Compression,V42BIS),
	codenamestr(UD_Compression,ALT),
	codenamestr(UD_Compression,V44),
	{ 0, NULL }
};

static struct codename diagMultimediaModeNames[] = {
	codenamestr(UD_MultimediaMode,DataOnly),
	codenamestr(UD_MultimediaMode,FaxOnly),
	codenamestr(UD_MultimediaMode,VoiceOnly),
	codenamestr(UD_MultimediaMode,VoiceView),
	codenamestr(UD_MultimediaMode,DSVD),
	codenamestr(UD_MultimediaMode,H324),
	codenamestr(UD_MultimediaMode,OtherV80),
	{ 0, NULL }
};

static struct codename diagDteDceModeNames[] = {
	codenamestr(UD_DteDceModes,AsyncData),
	codenamestr(UD_DteDceModes,V80TransSync),
	codenamestr(UD_DteDceModes,V80FramedSync),
	{ 0, NULL }
};

static struct {
    CNXT_DIAG_CODES code;
    struct codename *names;
} codenametables[] = {
    { CSDIAG_MODEM_STATE, diagModemStateNames },
    { CSDIAG_TERMINATION_CAUSE, diagTerminationCauseNames },
    { CSDIAG_MOH_STATUS, diagMOHStateNames },
    { CSDIAG_MOH_REMOTE_REQUEST, diagMOHRemoteReqNames },
    { CSDIAG_MOH_REMOTE_RESPONSE, diagMOHRemoteRespNames },
    { CSDIAG_CALL_SETUP_RES, diagCallSetupResNames },
    { CSDIAG_TX_NEG_RES, diagModNegResNames },
    { CSDIAG_RX_NEG_RES, diagModNegResNames },
    { CSDIAG_PROTOCOL_NEG_RES, diagProtResNames },
    { CSDIAG_CMPRS_NEG_RES, diagCmprsResNames },
    { CSDIAG_MULTI_MEDIA_MODE, diagMultimediaModeNames },
    { 0, NULL },
};

static char *codeNameStr(CNXT_DIAG_CODES code, struct codename *names)
{
    int i;

    for(i=0; names[i].name && (names[i].code != code); i++);
    if(names[i].name)
	return names[i].name;
    else {
	static char buf[80];

	sprintf(buf, "code_%d\n", code);
	return buf;
    }
}
#endif /* SHOW_MODEMEXPERT_NOTIFICATIONS */

static LIST_HEAD(listMEInstances, tagMECData) MEInstanceList;

static void sigio_handler(int sig)
{
    char buf[256];
    SDK2K_NOTIFICATION Notification;
    int n, i;
    PMEData         pData;

    for (pData = MEInstanceList.lh_first; pData != NULL; pData = pData->linkage.le_next) {
	while(1) {
	    n = read(pData->fd, &Notification, sizeof(Notification));

	    if(n <= 0) {
		if(!((n < 0) && (errno == EAGAIN))) {
		    if(errno == ENODEV) {
			if(pData->pfnCallback) {
			    pData->pfnCallback(pData->pRef, (HME)pData, kMEDisconnected, 0, 0, 0);
			}
			break;
		    }
		    sprintf(buf, "read %d bytes, errno=%d\n", n, errno);
		    write(2, buf, strlen(buf));
		}
		break;
	    }

	    if(n != sizeof(Notification)) {
		sprintf(buf, "read %d bytes, not multiple of Notification size\n", n);
		write(2, buf, strlen(buf));
		continue;
	    }

#ifdef SHOW_MODEMEXPERT_NOTIFICATIONS
	    if(getenv("SHOW_MODEMEXPERT_NOTIFICATIONS")) {
		for(i=0; codenametables[i].names && (codenametables[i].code != Notification.Code); i++);

		if(codenametables[i].names) {
		    sprintf(buf, "TimeStamp=%d %s %s\n",
			    Notification.TimeStamp,
			    codeNameStr(Notification.Code, csDiagCodeNames),
			    codeNameStr(Notification.Data, codenametables[i].names));
		} else {
		    sprintf(buf, "TimeStamp=%d Code=%s Data=%ld\n",
			    Notification.TimeStamp,
			    codeNameStr(Notification.Code, csDiagCodeNames),
			    Notification.Data);
		}
		write(2, buf, strlen(buf));
	    }
#endif /* SHOW_MODEMEXPERT_NOTIFICATIONS */

	    if(pData->pfnCallback) {
		pData->pfnCallback( pData->pRef, (HME)pData, kMECallback,
			Notification.Code, Notification.Data, Notification.TimeStamp);
	    }
	}
    }

}

HME ModemExpertOpen (char *pDiagDev, MECALLBACK pfnCallback, void *pRef)
{
    PMEData         pData;
    sigset_t sset, oset;

    if(!pfnCallback)
        return NULL;

    pData = malloc(sizeof(MEData));
    if(!pData)
        return NULL;

    memset(pData, 0, sizeof(MEData));

    pData->fd = open(pDiagDev, O_RDWR | O_NONBLOCK);
    if(pData->fd < 0) {
	perror(pDiagDev);
	free(pData);
	return NULL;
    }

    sigset(SIGIO, sigio_handler);
    fcntl(pData->fd, F_SETOWN, getpid());
    fcntl(pData->fd, F_SETFL, fcntl(pData->fd, F_GETFL, 0) | O_ASYNC);

    pData->pfnCallback  = pfnCallback;
    pData->pRef         = pRef;

    sigemptyset(&sset);
    sigaddset(&sset, SIGIO);
    sigprocmask(SIG_BLOCK, &sset, &oset);
    LIST_INSERT_HEAD(&MEInstanceList, pData, linkage);
    sigprocmask(SIG_SETMASK, &oset, NULL);

    if(pData->pfnCallback) {
        pData->pfnCallback(pData->pRef, (HME)pData, kMEConnected, 0, 0, 0);
    }

    return (HME)pData;
}

int ModemExpertClose(HME hME)
{
    PMEData pData =(PMEData)hME;

    if(pData) {
	sigset_t sset, oset;

	sigaddset(&sset, SIGIO);
	sigprocmask(SIG_BLOCK, &sset, &oset);
	LIST_REMOVE(pData, linkage);
	sigprocmask(SIG_SETMASK, &oset, NULL);

	close(pData->fd);
    	free(pData);
    }
}


int ModemExpertMonitor(HME hME, CNXT_DIAG_CODES code, void* pBuf, UINT32 *pBufSize)
{
    PMEData pData =(PMEData)hME;
    SDK2K_IOCTLARGS ioctlArgs;
    int r;

    ioctlArgs.ControlCode = code;
    ioctlArgs.Buffer = pBuf;
    ioctlArgs.BufferSize = *pBufSize;

    r = ioctl(pData->fd, SDK2K_IOC_MONITOR, &ioctlArgs);
    if(r >= 0) {
	*pBufSize = ioctlArgs.BufferSize;
    }

    return r;
}

int ModemExpertControl(HME hME, CNXT_DIAG_CTRL_CODES code, void* pBuf, UINT32 DataSize)
{
    PMEData pData =(PMEData)hME;
    SDK2K_IOCTLARGS ioctlArgs;

    ioctlArgs.ControlCode = code;
    ioctlArgs.Buffer = pBuf;
    ioctlArgs.BufferSize = DataSize;

    return ioctl(pData->fd, SDK2K_IOC_CONTROL, &ioctlArgs);
}


int ModemExpertConfigure(HME hME, CNXT_DIAG_CFG_CODES code, void* pBuf, UINT32 DataSize)
{
    PMEData pData =(PMEData)hME;
    SDK2K_IOCTLARGS ioctlArgs;

    ioctlArgs.ControlCode = code;
    ioctlArgs.Buffer = pBuf;
    ioctlArgs.BufferSize = DataSize;

    return ioctl(pData->fd, SDK2K_IOC_CONFIGURE, &ioctlArgs);
}

