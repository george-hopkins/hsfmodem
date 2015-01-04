/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#include "oscompat.h"
#include "osservices.h"
#include "osmemory.h"
#include "osstdio.h"
#include "osresour_ex.h"

#include "comtypes.h"
#include "configtypes.h"
#include "testdebug.h"

#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/unistd.h>
#include <asm/fcntl.h>

#include <linux/ctype.h>

#include <linux/pci.h>

#define caseretstr(x) case CFGMGR_##x: return #x

#ifndef SIMPLE_DAA
#define SIMPLE_DAA      0
#define CAESAR          1
#define SOFT_DAA        2
#define SI3054_DAA      3
#define SI3055_DAA      4
#endif

static int osnvm_debug;
#ifdef FOUND_MODULE_PARAM
module_param(osnvm_debug, int, 0);
#else
MODULE_PARM(osnvm_debug, "i");
#endif

static char* eCodeStr(CFGMGR_CODE eCode)
{
	switch(eCode) {
		caseretstr(MS_PARAMS);
		caseretstr(EC_PARAMS);
		caseretstr(BRK_PARAMS);
		caseretstr(ITF_PARAMS);
		caseretstr(AUTODETECT);
		caseretstr(COMPRESSION_PARAMS);
#if !defined NO_V44_SUPPORT
		caseretstr(V44_PARAMS);
#endif
		caseretstr(RETRAIN_ENABLED);
		caseretstr(CADENCE_PARAMS);
		caseretstr(FORCE_RENEG_UP);
		caseretstr(FORCE_RENEG_DOWN);
#if !defined NO_V34FAX_SUPPORT
		caseretstr(V34FAX_ENABLED);
#endif
		caseretstr(SREG);
		//caseretstr(NRINGS_TO_ANSWER);
		caseretstr(RING_COUNTER);
		caseretstr(ESCAPE_CHAR);
		caseretstr(CR_CHAR);
		caseretstr(LF_CHAR);
		caseretstr(BS_CHAR);
		caseretstr(BLIND_DIAL_WAIT_TIME);
		caseretstr(CARRIER_WAIT_TIME);
		caseretstr(PAUSE_DELAY_TIME);
		caseretstr(CARDET_RESPONSE_TIME);
		caseretstr(HANGUP_DELAY_TIME);
		caseretstr(DTMF_DURATION);
		caseretstr(ESCAPE_PROMPT_DELAY);
		caseretstr(TEST_TIMER);
		caseretstr(FLASH_DELAY_TIME);
		caseretstr(INACTIVITY_TIME);
		caseretstr(DATA_COMPRESSION);
		caseretstr(EXTENDED_RESULT_CODES);
		caseretstr(SREG_LAST);
		caseretstr(DIALSTRING);
		caseretstr(PULSE_DIAL_CONFIG);
		caseretstr(TONE_DIAL_CONFIG);
		caseretstr(DIALTONE_WAIT_TIME);
		caseretstr(DIAL_MODE);
		caseretstr(USE_S7_WHEN_W);
		caseretstr(INVERT_CALLING_TONE);
		caseretstr(CALL_PROGRESS_TONE_DEBOUNCE);
		caseretstr(CALL_MODE);
		caseretstr(CALL_PROGRESS_TIMING);
		caseretstr(CALL_PROGRESS_FLAGS);
		caseretstr(V8BIS_CONTROL);
		caseretstr(V8BIS_RESULT);
		caseretstr(PREFERRED_FLEX_VERSION);
		caseretstr(V90_ENABLED);
		caseretstr(V70_ALLOWED);
		caseretstr(ATMESSAGE_I0);
		caseretstr(ATMESSAGE_I1);
		caseretstr(ATMESSAGE_I2);
		caseretstr(ATMESSAGE_I3);
		caseretstr(ATMESSAGE_I4);
		caseretstr(ATMESSAGE_I5);
		caseretstr(ATMESSAGE_I6);
		caseretstr(ATMESSAGE_I7);
		caseretstr(ATMESSAGE_I8);
		caseretstr(ATMESSAGE_I9);
		caseretstr(ATMESSAGE_I10);
		caseretstr(ATMESSAGE_I11);
		caseretstr(ATMESSAGE_I12);
		caseretstr(DTE_ECHO);
		caseretstr(DTE_CONFIG);
		caseretstr(SYNC_MODE);
		caseretstr(RLSD_BEHAVIOR);
		caseretstr(DTR_BEHAVIOR);
		caseretstr(SPEAKER_VOLUME);
		caseretstr(SPEAKER_CONTROL);
		caseretstr(PULSE_MAKE_BREAK);
		caseretstr(IMMEDIATE_RING_DETECTED);
		caseretstr(ANSWER_TONE_DETECTOR);
		caseretstr(BELL_TONE_DETECTOR);
		caseretstr(DTMF_GENERATOR);
		caseretstr(MODULATION_REPORT);
		caseretstr(MODULATION_INFO);
		caseretstr(BUSY_TONE_CADENCE);
		caseretstr(RINGBACK_TONE_CADENCE);
		caseretstr(RING_CADENCE);
		caseretstr(REORDER_TONE_CADENCE);
		caseretstr(SDIAL_TONE_CADENCE);
		caseretstr(DATA_CALL_TONE_CADENCE);
		caseretstr(FAX_CALL_TONE_CADENCE);
		caseretstr(FAX_AUTO_ANSWER);
		caseretstr(OEM_FLAGS);
		caseretstr(OEM_FLAGS2);
		caseretstr(OEM_FILTERS);
		caseretstr(OEM_SPKR_MUTE_DELAY);
		caseretstr(OEM_READ_MASK);
		caseretstr(OEM_THRESHOLD);
		caseretstr(COUNTRY_CODE);
		caseretstr(PREVIOUS_COUNTRY_CODE);
		caseretstr(COUNTRY_NAME);
		caseretstr(COUNTRY_CODE_LIST);
		caseretstr(COUNTRY_STRUCT);
		caseretstr(COUNTRY_BLIST);
		caseretstr(FILTERS);
		caseretstr(DTMF);
		caseretstr(RING_PARAMS);
		caseretstr(RLSDOFFSET);
		caseretstr(THRESHOLD);
		caseretstr(TXLEVELS);
		caseretstr(RELAYS);
		caseretstr(SPEED_ADJUST);
		caseretstr(SREG_LIMITS);
		caseretstr(PR_IGNORE_TIME);
		caseretstr(USER_SYMRATE);
		caseretstr(COUNTRY_CALLERID1);
		caseretstr(COUNTRY_CALLERID2);
		caseretstr(COUNTRY_CALL_WAITING);
#if !defined NO_CID_SUPPORT
		caseretstr(VCID_FORMAT);
		caseretstr(VRID_FORMAT);
		caseretstr(CALLER_ID_INFO);
		caseretstr(VRID_CRC);
		caseretstr(VRID_TYPE);
		caseretstr(VRID_LENGTH);
		caseretstr(VRID_DATA);
#endif
		caseretstr(VDR_PARAMS);
		caseretstr(CALL_WAITING);
#if !defined NO_MOH_SUPPORT
		caseretstr(MOH);
#endif
#if !defined NO_PIG_SUPPORT
		caseretstr(PCMUPSTREAM);
#endif
#if !defined NO_QC_SUPPORT
		caseretstr(QC);
		caseretstr(QC_PROFILE);
#endif
		caseretstr(V92_CONTROL);
		caseretstr(PROFILE_STORED);
		caseretstr(PROFILE_FACTORY);
#if !defined NO_POUNDUG_SUPPORT
		caseretstr(POUND_UD);
#endif
		caseretstr(ATPARSER_ONLINE);
#if !defined NO_BL_SUPPORT
		caseretstr(BLACK_LIST);
#endif
		caseretstr(TONE_PRIMARY_PARAMS);
		caseretstr(TONE_ALTERNATE_PARAMS);
		caseretstr(TONE_ATBEL_PARAMS);
		caseretstr(TONE_ATV25_PARAMS);
		caseretstr(TONE_1100_PARAMS);
		caseretstr(TONE_2130_PARAMS);
		caseretstr(TONE_2312_PARAMS);
		caseretstr(TONE_2750_PARAMS);
		caseretstr(TONE_2912_PARAMS);
		caseretstr(VOICETONEACALLPROG_PARAMS);
		caseretstr(VOICETONEB_1100_PARAMS);
		caseretstr(VOICETONEC_1300_PARAMS);
		caseretstr(VOICETONEC_2100_PARAMS);
		caseretstr(VOICETONEB_2225_PARAMS);
		caseretstr(V8BTONE_2100_PARAMS);
		caseretstr(V8BTONE_2225_PARAMS);
		caseretstr(CI_CENTER_TONEB_PARAMS);
		caseretstr(CI_SIDE_TONEC_PARAMS);
		caseretstr(VOICEVIEW_TONEC_2912_PARAMS);
		caseretstr(VOICEVIEW_TONEB_2312_PARAMS);
		caseretstr(VOICEVIEW_TONEB_2750_PARAMS);
		caseretstr(VOICEVIEW_TONEA_2130_PARAMS);
		caseretstr(VOICEVIEW_TONEA_NOT_ST_PARAMS);
		caseretstr(TONEC_1650_PARAMS);
		caseretstr(CReSEG1_2002PARAMS);
		caseretstr(TONE_1270_PARAMS);
		caseretstr(TONE_AS);
		caseretstr(TONE_GEN_1650);
		caseretstr(TONE_GEN_V25CALL);
		caseretstr(TONE_GEN_CNG);
		caseretstr(TONE_GEN_CED);
		caseretstr(TONE_GEN_CI);
		caseretstr(BUFFERS_DELAY);
		caseretstr(PCI_DEVICE_ID);
		caseretstr(PCI_VENDOR_ID);
		caseretstr(OEM_HWDATA);
		caseretstr(OEM_DAATYPE);
		caseretstr(OEM_SMARTDAA_PARAMS);
		caseretstr(OEM_SMARTDAA_PARAMSK3);
		caseretstr(OEM_SMARTDAA_PARAMSHAL);
		caseretstr(OEM_SOFT_RING_PARAMS);
		caseretstr(HW_ADAPTER_TYPE);
		caseretstr(YUKON_FIRMWARE_DATA);
		caseretstr(YUKON_FIRMWARE_DELETE);
		caseretstr(CTRY_TONE);
		caseretstr(CTRY_CADENCE);
		caseretstr(CTRY_CALLERID);
		caseretstr(HSFI_TYPE);
		caseretstr(CENTERAL_SITE);
		caseretstr(RXSAMPLES_OUT_FILENAME);
		caseretstr(TXSAMPLES_OUT_FILENAME);
		caseretstr(RXLEN_OUT_FILENAME);
		caseretstr(RXSAMPLES_IN_FILENAME);
		caseretstr(RXLEN_IN_FILENAME);
		caseretstr(PRESOURCES);
		caseretstr(HW_BASE_ADDRESS);
		caseretstr(DSL_HWIO_PARAMS);
		caseretstr(SETTINGS_ATUR_DESCRIPTION);
		caseretstr(SETTINGS_HSTUR_DESCRIPTION);
		caseretstr(SETTINGS_HSTUC_DESCRIPTION);
		caseretstr(SETTINGS_ATUR_PARAMS);
		caseretstr(SETTINGS_HSTUR_PARAMS);
		caseretstr(SETTINGS_HSTUC_PARAMS);
		caseretstr(SETTINGS_NILECTRL_PARAMS);
		caseretstr(SETTINGS_ATUR_DATA);
		caseretstr(SETTINGS_ATUC_DATA);
		caseretstr(SETTINGS_HSTUR_DATA);
		caseretstr(SETTINGS_HSTUC_DATA);
		caseretstr(SETTINGS_NILECTRL_DATA);
		caseretstr(G9922ATU_C_CONFIGURATION);
		caseretstr(G9922ATU_R_CONFIGURATION);
		caseretstr(HSTU_C_CONFIGURATION);
		caseretstr(HSTU_R_CONFIGURATION);
		caseretstr(NILE_CTRL_CONFIGURATION);
		caseretstr(HS_MODULATION_DB);
		caseretstr(CO_HS_MODULATION_DB);
		caseretstr(RT_HS_MODULATION_DB);
		caseretstr(LOWMIPS);
		caseretstr(DSL_HWIO_SAMPLE_RATE);
#if !defined NO_VOICE_SUPPORT
		caseretstr(VOICE_FIRST);
		caseretstr(VOICE_FDSP_PARAMS);
		caseretstr(VOICE_FDSP_PREV_PARAMS);
		caseretstr(VOICE_VBT_PARAMS);
		caseretstr(VOICE_VDID_PARAMS);
		caseretstr(VOICE_VDT_PARAMS);
		caseretstr(VOICE_VGR_PARAMS);
		caseretstr(VOICE_VGT_PARAMS);
		caseretstr(VOICE_VLS_PARAMS);
		caseretstr(VOICE_VSD_PARAMS);
		caseretstr(VOICE_VSM_PARAMS);
		caseretstr(VOICE_VTS_PARAMS);
		caseretstr(VOICE_MISC_PARAMS);
		caseretstr(VOICE_CACHED_MISC_PARAMS);
		caseretstr(VOICE_VIT_PARAMS);
		caseretstr(VOICE_RELAY_CONFIG);
		caseretstr(VOICE_BAUD_RATE);
		caseretstr(VOICE_VRA_PARAMS);
		caseretstr(VOICE_VRN_PARAMS);
		caseretstr(VOICE_VTD_PARAMS);
		caseretstr(VOICE_DEFAULT_ONHOOK_RELAY);
		caseretstr(VOICE_RX_GAIN);
		caseretstr(VOICE_TX_GAIN);
		caseretstr(VOICE_HOOK_CONTROL);
		caseretstr(HANDSET_STATE);
		caseretstr(VOICE_SUPPORTED_VSM_CODE);
		caseretstr(VOICE_BOND_OPTIONS);
		caseretstr(DTMF_REPORTING_ENABLE);
#ifdef VIDEOPHONE
		caseretstr(VOICE_PREVIOUS_MODE);
#endif
		caseretstr(VOICE_DETECTION_MASK);
#endif

		caseretstr(HARDWARE_PROFILE);
		caseretstr(HARDWARE_ID);
		caseretstr(LICENSE_OWNER);
		caseretstr(LICENSE_KEY);
		caseretstr(LICENSE_STATUS);
		caseretstr(LAST);
		default: {
					 static char buf[80];

					 if((eCode & 0xffff) == CFGMGR_COUNTRY_STRUCT) {
					 	sprintf(buf, "COUNTRY_STRUCT_%02x\n", eCode >> 16);
					 } else {
					 	sprintf(buf, "code_%d\n", eCode);
					 }
					 return buf;
				 }
	}
}

static const char cnxt_nvmdir_static[] = CNXTNVMDIR;
static const char cnxt_nvmdir_dynamic[] = CNXTNVMDIR "/dynamic";

static const UINT16 g_DefaultCountryCode = 0xB5; /* USA */

static const PROFILE_DATA g_FactoryProfile = {
	1,							// Echo
	1,							// Volume
	1,							// Speaker control
	0,							// Pulse
	0,							// Quiet
	1,							// Verbose
	3,							// report Level (ATX3)
	0,							// Connect message
	1,							// AmperC
	2,							// AmperD
	0,							// S0
	0,							// S1;
	43,							// S2;
	13,							// S3;
	10,							// S4;
	8,							// S5;
	2,							// S6;
	50,							// S7;
	2,							// S8;
	14,							// S10;
	95,							// S11;
	50,							// S12;
	0,							// S16;
	0,							// S18;
	70							// S29;
};

enum NVM_FORMAT {
    NVM_FORMAT_HEXBYTES,
    NVM_FORMAT_HEXSHORTS,
    NVM_FORMAT_HEXLONGS,
    NVM_FORMAT_STRING
};

static
enum NVM_FORMAT nvmFormat(CFGMGR_CODE eCode)
{
    switch(eCode) {
	case CFGMGR_COUNTRY_CODE:
	case CFGMGR_PREVIOUS_COUNTRY_CODE:
	case CFGMGR_PCI_VENDOR_ID:
	case CFGMGR_PCI_DEVICE_ID:
	    return NVM_FORMAT_HEXSHORTS;

	case CFGMGR_HARDWARE_PROFILE:
	case CFGMGR_LICENSE_OWNER:
	case CFGMGR_LICENSE_STATUS:
	    return NVM_FORMAT_STRING;

	case CFGMGR_HARDWARE_ID:
	case CFGMGR_LICENSE_KEY:
	    return NVM_FORMAT_HEXLONGS;

	default:
	    return NVM_FORMAT_HEXBYTES;
    }
}


typedef struct {
    struct list_head link;
    char *pathname;
    PVOID pBuf;
    UINT32 dwSize;
    enum NVM_FORMAT nvmFormat;
} nvmelem_t;

typedef struct {
    struct list_head link;
    char *instname;
} nvmnewinst_t;

/* These lists are protected by nvmelem_writelist_sem */
static LIST_HEAD(nvmelem_writelist);
static LIST_HEAD(nvm_newinst_list);

static DECLARE_MUTEX(nvmelem_writelist_sem);

/* nvmelem_writelist_sem must be held by caller */
static BOOL NVM_WriteListAppend(const char *pathname, PVOID pBuf, UINT32 dwSize, enum NVM_FORMAT nvmFormat)
{
	nvmelem_t *nel;

	nel = OsAllocate(sizeof(nvmelem_t) + dwSize + strlen(pathname) + 1);
	if(!nel) {
	    printk(KERN_ERR"%s: cannot allocate memory for element %s\n", __FUNCTION__, pathname);
	    return FALSE;
	}

	nel->dwSize = dwSize;
	nel->nvmFormat = nvmFormat;
	nel->pBuf = nel + 1;
	memcpy(nel->pBuf, pBuf, dwSize);
	nel->pathname = nel->pBuf + dwSize;
	strcpy(nel->pathname, pathname);

	list_add_tail(&nel->link, &nvmelem_writelist);

	return TRUE;
}


/* Write pBuf to file as sequence of two-digit hex bytes */
static BOOL
NVM_WriteFileData(FILE *file, const char *pathname, PVOID pBuf, UINT32 dwSize, enum NVM_FORMAT nvmFormat)
{
    char buf[MAX_OEM_STR_LEN + 4], *dp;
    unsigned char *p;
    UINT32 size;
    int errno;

    p = pBuf;
    dp = buf;

    if(nvmFormat == NVM_FORMAT_HEXBYTES) {
	for (size = 1; size < dwSize; size++) {
	    sprintf (dp, "%02X,", *p++);
	    dp += 3;
	    if(((size) % 16) == 0) {
		*dp++ = '\n';
		if (OsFWrite(buf, 1, dp - buf, file, &errno) != (dp - buf)) {
		    printk(KERN_ERR "%s: write error to %s errno=%d\n", __FUNCTION__, pathname, errno);
		    goto err;
		}
		dp = buf;
	    }
	}
	sprintf (dp, "%02X\n", *p++);
	dp += 3;

    } else if((nvmFormat == NVM_FORMAT_HEXSHORTS) && !(dwSize % sizeof(UINT16))) {
	for (size = sizeof(UINT16); size < dwSize; size += sizeof(UINT16)) {
	    sprintf (dp, "%04X,", *(UINT16*)p);
	    p += sizeof(UINT16);
	    dp += 5;
	    if(((size) % 16) == 0) {
		*dp++ = '\n';
		if (OsFWrite(buf, 1, dp - buf, file, &errno) != (dp - buf)) {
		    printk(KERN_ERR "%s: write error to %s errno=%d\n", __FUNCTION__, pathname, errno);
		    goto err;
		}
		dp = buf;
	    }
	}
	sprintf (dp, "%04X\n", *(UINT16*)p);
	p += sizeof(UINT16);
	dp += 5;

    } else if((nvmFormat == NVM_FORMAT_HEXLONGS) && !(dwSize % sizeof(UINT32))) {
	for (size = sizeof(UINT32); size < dwSize; size += sizeof(UINT32)) {
	    sprintf (dp, "%08X,", *(UINT32*)p);
	    p += sizeof(UINT32);
	    dp += 9;
	    if(((size) % 16) == 0) {
		*dp++ = '\n';
		if (OsFWrite(buf, 1, dp - buf, file, &errno) != (dp - buf)) {
		    printk(KERN_ERR "%s: write error to %s errno=%d\n", __FUNCTION__, pathname, errno);
		    goto err;
		}
		dp = buf;
	    }
	}
	sprintf (dp, "%08X\n", *(UINT32*)p);
	p += sizeof(UINT32);
	dp += 9;

    } else if(nvmFormat == NVM_FORMAT_STRING) {
	size = (dwSize < MAX_OEM_STR_LEN) ? dwSize : MAX_OEM_STR_LEN;
	buf[0] = '"';
	strncpy(&buf[1], pBuf, size);
	buf[1 + size ] = '\0';
	size = strlen(buf);
	buf[size++] = '"';
	buf[size++] = '\n';
	buf[size] = '\0';
	dp += size;

    } else {
	printk(KERN_ERR "%s: invalid NVM format (%d)\n", __FUNCTION__, nvmFormat);
	goto err;
    }

    if(dp - buf) {
	if (OsFWrite(buf, 1, dp - buf, file, &errno) != (dp - buf)) {
	    printk(KERN_ERR "%s: write error to %s errno=%d\n", __FUNCTION__, pathname, errno);
	    goto err;
	}
    }

    OsFClose(file);

    if(osnvm_debug)
    	printk(KERN_DEBUG"%s: wrote %u bytes to %s\n", __FUNCTION__, dwSize, pathname);

    return TRUE;

err:
    if(file)
	OsFClose(file);
    return FALSE;
}


static int
NVM_NewInstance(char *instname)
{
    char *argv[] = { CNXTSBINDIR"/"CNXTTARGET"config", "--auto", "--newinstance", instname, NULL };
    char *envp[] = { "HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin:"CNXTSBINDIR, NULL };

    //printk(KERN_DEBUG"%s: instname=%s\n", __FUNCTION__, instname);

    return OsForkWait(argv[0], argv, envp);
}


__shimcall__
void
NVM_WriteFlushList(BOOL write)
{
    down(&nvmelem_writelist_sem);

    while(!list_empty(&nvm_newinst_list)) {
	int err;

	nvmnewinst_t *ni = list_entry(nvm_newinst_list.next, nvmnewinst_t, link);
	list_del(&ni->link);
	up(&nvmelem_writelist_sem);

	err = write ? NVM_NewInstance(ni->instname) : 0;
	if (err) {
	    //printk(KERN_ERR "%s: cannot create instance %s: err=%d failed again!\n", __FUNCTION__, ni->instname, err);
	    down(&nvmelem_writelist_sem);
	    list_add(&ni->link, &nvm_newinst_list);
	    goto done;
	}

	printk(KERN_WARNING "%s: instance %s successfully created\n", __FUNCTION__, ni->instname);

	OsFree(ni->instname);
	OsFree(ni);

	down(&nvmelem_writelist_sem);
    }

    while(!list_empty(&nvmelem_writelist)) {

	nvmelem_t *nel = list_entry(nvmelem_writelist.next, nvmelem_t, link);

	if(write) {
	    FILE *file;
	    int err;

	    file = OsFOpen(nel->pathname, "w", &err);
	    if (!file) {
		if(err == -EROFS)
		    goto done;

		printk(KERN_ERR"%s: cannot open %s (errno=%d)\n", __FUNCTION__, nel->pathname, err);

	    } else {
		//printk(KERN_DEBUG"%s: writing %s\n", __FUNCTION__, nel->pathname);
		NVM_WriteFileData(file, nel->pathname, nel->pBuf, nel->dwSize, nel->nvmFormat);
	    }
	}

	list_del(&nel->link);

	OsFree(nel);
    }

done:
    up(&nvmelem_writelist_sem);
}

static BOOL
NVM_WriteFile(const char *pathname, PVOID pBuf, UINT32 dwSize, enum NVM_FORMAT nvmFormat, BOOL suspendInProgress)
{
    int err;
    FILE *file;

    down(&nvmelem_writelist_sem);
    if(!list_empty(&nvm_newinst_list) || !list_empty(&nvmelem_writelist) || suspendInProgress) {
	if(!NVM_WriteListAppend(pathname, pBuf, dwSize, nvmFormat)) {
	    up(&nvmelem_writelist_sem);
	    return FALSE;
	}

	up(&nvmelem_writelist_sem);

	NVM_WriteFlushList(TRUE);

	return TRUE;
    }

    file = OsFOpen(pathname, "w", &err);
    if (!file) {
	if(err == -EROFS) {
	    BOOL res;

	    //printk(KERN_DEBUG"%s: delaying write %s\n", __FUNCTION__, pathname);
	    res = NVM_WriteListAppend(pathname, pBuf, dwSize, nvmFormat);
	    up(&nvmelem_writelist_sem);
	    return res;
	}

	up(&nvmelem_writelist_sem);
	printk(KERN_ERR"%s: cannot open %s (errno=%d)\n", __FUNCTION__, pathname, err);
	return FALSE;
    }

    up(&nvmelem_writelist_sem);

    //printk(KERN_DEBUG"%s: opened %s\n", __FUNCTION__, pathname);

    return NVM_WriteFileData(file, pathname, pBuf, dwSize, nvmFormat);
}

/* Read in file containing either:
 * - Sequence of two-digit hex bytes
 * - String enclosed in ""
 */
static BOOL
NVM_ReadFile(const char *pathname, PVOID pBuf, UINT32 *pdwSize)
{
    int err;
    FILE *file;
    char buf[100], *p, *dp;
    BOOL stringMode = FALSE;
    UINT32 l;
    int n;
    struct list_head *lh;
    int errno;

    down(&nvmelem_writelist_sem);
    for(lh = nvmelem_writelist.prev; lh != &nvmelem_writelist; lh = lh->prev) {
	nvmelem_t *nel = list_entry(lh, nvmelem_t, link);

	if(!strcmp(pathname, nel->pathname)) {
	    l = *pdwSize;
	    if(l > nel->dwSize)
	       l = nel->dwSize;

	    memcpy(pBuf, nel->pBuf, l);
	    memset(pBuf + l, 0, *pdwSize - l); /* zero pBuf remainder */
	    *pdwSize = l;

	    if(osnvm_debug)
		printk(KERN_DEBUG"%s: returning %s from writelist\n", __FUNCTION__, pathname);

	    up(&nvmelem_writelist_sem);
	    return TRUE;
	}
    }
    up(&nvmelem_writelist_sem);

    file = OsFOpen(pathname, "r", &err);
    dp = pBuf;
    l = *pdwSize;

    if (file) {

	if(osnvm_debug)
	    printk(KERN_DEBUG"%s: opened %s\n", __FUNCTION__, pathname);

	while(l > 0) {
	    n = OsFRead(buf, 1, sizeof(buf)-1, file, &errno);
	    if(n <= 0)
		break;

	    buf[n] = '\0';

	    for(p = buf; (l > 0) && (p < &buf[n]); p++) {

		if(*p == '"') {
		    stringMode = !stringMode;
		    continue;
		}

		if(stringMode) {
		    *dp++ = *p;
		    l--;
		    continue;
		}

		if(!isxdigit(*p))
		    continue;

		/* hack to read COUNTRY_CODE as UINT16 without swapping: */
		if((p == buf) && (n == 5) && (l >= *pdwSize) && (*pdwSize == sizeof(UINT16)) &&
			isxdigit(p[1]) && isxdigit(p[2]) &&
			isxdigit(p[3]) && p[4] == '\n') {
		    *((UINT16*)dp) = simple_strtoul (p, &p, 16);
		    dp += 2;
		    l -= 2;
		    continue;
		}

		/* hack to read LICENSE_KEY as UINT32 without swapping: */
		if((p == buf) && (n == 9) && (l >= *pdwSize) && (*pdwSize == sizeof(UINT32)) &&
			isxdigit(p[1]) && isxdigit(p[2]) &&
			isxdigit(p[3]) && isxdigit(p[4]) &&
			isxdigit(p[5]) && isxdigit(p[6]) &&
			isxdigit(p[7]) && p[8] == '\n') {
		    *((UINT32*)dp) = simple_strtoul (p, &p, 16);
		    dp += 4;
		    l -= 4;
		    continue;
		}

		if(p == &buf[sizeof(buf)-1-1]) {
		    /* value split accross buffer boundary, read next chunk */
		    buf[0] = *p;
		    p = buf;
		    n = OsFRead(buf+1, 1, sizeof(buf)-1-1, file, &errno);
		    if(n < 0) {
			break;
		    }
		    n++;
		    buf[n] = '\0';
		}

		*dp++ = simple_strtoul (p, &p, 16);
		l--;
	    }
	}
	OsFClose(file);

	if(osnvm_debug && l)
		printk(KERN_DEBUG"%s: read %u bytes (%u missing) from %s\n", __FUNCTION__, *pdwSize - l, l, pathname);
    } else {
	if(osnvm_debug)
	    printk(KERN_DEBUG"%s: cannot open %s (errno=%d)\n", __FUNCTION__, pathname, err);
    }

    *pdwSize -= l;

    memset(dp, 0, l); /* zero pBuf remainder */

    return file ? TRUE : FALSE;
}

static BOOL
NVM_ReadStaticParm(POS_DEVNODE pDevNode, const char *name, PVOID pBuf, UINT32 * pdwSize)
{
    char pathname[100];

    snprintf(pathname, sizeof(pathname), "%s/%s/%s", cnxt_nvmdir_static, pDevNode->hwProfile, name);
    return NVM_ReadFile(pathname, pBuf, pdwSize);
}

static BOOL
NVM_ReadDynamicParm(POS_DEVNODE pDevNode, const char *name, PVOID pBuf, UINT32 * pdwSize)
{
    char pathname[100];

    snprintf(pathname, sizeof(pathname), "%s/%d-%s/%s", cnxt_nvmdir_dynamic, pDevNode->hwInstNum, pDevNode->hwInstName, name);
    return NVM_ReadFile(pathname, pBuf, pdwSize);
}

static BOOL
NVM_ReadCountryParm(POS_DEVNODE pDevNode, int countryt35, UINT8 reference, const char *name, PVOID pBuf, UINT32 * pdwSize)
{
    char pathname[100];
    BOOL r;
    UINT32 Size = pdwSize ? *pdwSize : 0;

    snprintf(pathname, sizeof(pathname), "%s/%s/Region/%04X%s", cnxt_nvmdir_static, pDevNode->hwProfile, countryt35, name);
    r = NVM_ReadFile(pathname, pBuf, pdwSize);
    if(!r && reference != 0xFF) {
	snprintf(pathname, sizeof(pathname), "%s/%s/Profile/%04X%s", cnxt_nvmdir_static, pDevNode->hwProfile, reference, name);
	if(pdwSize)
	    *pdwSize = Size;
	return NVM_ReadFile(pathname, pBuf, pdwSize);
    } else
	return r;
}

static BOOL
NVM_ReadCountry(POS_DEVNODE pDevNode, BYTE daaType, int countryt35, CtryPrmsStruct *ctryPrms)
{
    UINT32 dwSize;
    UINT8 Reference = 0xFF;
	char *nameTxLevel, *nameRelays;

    memset(ctryPrms, 0, sizeof(ctryPrms));

    //printk(KERN_DEBUG "%s: pDevNode=%p countryt35=%x daaType=%d\n", __FUNCTION__, pDevNode, countryt35, daaType);

    ctryPrms->T35Code = countryt35;

#if !TARGET_HCF_FAMILY /* HSF */
    dwSize = sizeof (Reference);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/REFERENCE", &Reference, &dwSize);
#endif

    dwSize = sizeof (ctryPrms->cInter);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "_NAME", &ctryPrms->cInter, &dwSize);

    dwSize = sizeof (ctryPrms->cIntCode);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/INTCODE", &ctryPrms->cIntCode, &dwSize);

#if !TARGET_HCF_FAMILY
	if((daaType == SI3054_DAA) || (daaType == SI3055_DAA)) {
		nameRelays = "/SILABRELAYS";
		nameTxLevel = "/SMART_TXLEVEL";
	} else if(daaType == CAESAR) {
		nameRelays = "/SMART_RELAYS";
		nameTxLevel = "/SMART_TXLEVEL";
	} else
#endif
	{
		nameRelays = "/RELAYS";
		nameTxLevel = "/TXLEVEL";
	}

    dwSize = sizeof (ctryPrms->Txlevel);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, nameTxLevel, &ctryPrms->Txlevel, &dwSize);

	if((daaType == SI3054_DAA) || (daaType == SI3055_DAA)) {
		int LowDialLevel = ctryPrms->Txlevel.LowDialLevel;
		int HighDialLevel = ctryPrms->Txlevel.HighDialLevel;

		if(ctryPrms->Relays[1] < 0x04000) {
			ctryPrms->Txlevel.LowDialLevel  = LowDialLevel-9;
			ctryPrms->Txlevel.HighDialLevel = HighDialLevel-9;
		}
		else
		{
			ctryPrms->Txlevel.LowDialLevel  = LowDialLevel-4;
			ctryPrms->Txlevel.HighDialLevel = HighDialLevel+10;
		}
	}

    dwSize = sizeof (ctryPrms->Relays);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, nameRelays, &ctryPrms->Relays, &dwSize);

    dwSize = sizeof (ctryPrms->Pulse);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/PULSE", &ctryPrms->Pulse, &dwSize);

    dwSize = sizeof (ctryPrms->Ring);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/RING", &ctryPrms->Ring, &dwSize);

    dwSize = sizeof (ctryPrms->SRegLimits);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/SREG", &ctryPrms->SRegLimits, &dwSize);

    dwSize = sizeof (ctryPrms->DTMF);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, (daaType == CAESAR) ? "/SMART_DTMF" : "/DTMF", &ctryPrms->DTMF, &dwSize);

    /* FILTER: was commented out on HSF */
	/* MARCXXX: Filter.Primary, Filter.Alternate, Filter.VoiceToneACallProgress/FilterVoiceToneA */
    dwSize = sizeof (ctryPrms->Filter);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/FILTER", &ctryPrms->Filter, &dwSize);

    dwSize = sizeof (ctryPrms->Threshold);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, (daaType == CAESAR) ? "/SMART_THRESHOLD" : "/THRESHOLD", &ctryPrms->Threshold, &dwSize);

    dwSize = sizeof (ctryPrms->RLSD);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/RLSD", &ctryPrms->RLSD, &dwSize);

    dwSize = sizeof (ctryPrms->Tone);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/TONE", &ctryPrms->Tone, &dwSize);

    dwSize = sizeof (ctryPrms->Timing);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/TIMING", &ctryPrms->Timing, &dwSize);

    dwSize = sizeof (ctryPrms->Cadence);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/CADENCE", &ctryPrms->Cadence, &dwSize);

    dwSize = sizeof (ctryPrms->Blacklisting);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/BLACKLISTING", &ctryPrms->Blacklisting, &dwSize);

    dwSize = sizeof (ctryPrms->CallerID);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/CALLERID", &ctryPrms->CallerID, &dwSize);

    dwSize = sizeof (ctryPrms->CallerID2);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/CALLERID2", &ctryPrms->CallerID2, &dwSize);

    dwSize = sizeof (ctryPrms->Flags);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/FLAGS", &ctryPrms->Flags, &dwSize);

    dwSize = sizeof (ctryPrms->AgressSpeedIndex);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/SPEEDADJUST", &ctryPrms->AgressSpeedIndex, &dwSize);

#if 0
    dwSize = sizeof (ctryPrms->PulseAbort);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/PULSE_ABORT", &ctryPrms->PulseAbort, &dwSize);
#endif

    dwSize = sizeof (ctryPrms->CallWaitingParms);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/CALL_WAITING", &ctryPrms->CallWaitingParms, &dwSize);

    dwSize = sizeof (ctryPrms->V92Control);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/V92_CONTROL", &ctryPrms->V92Control, &dwSize);

#if 0
    dwSize = sizeof (ctryPrms->ToneParams);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/TONEPARAMS", &ctryPrms->ToneParams, &dwSize);
#endif

#if !TARGET_HCF_FAMILY
    dwSize = sizeof (ctryPrms->OgcParams);
    NVM_ReadCountryParm(pDevNode, countryt35, Reference, "/V92_OGC", &ctryPrms->OgcParams, &dwSize);
#endif

    return TRUE;
}


static
BOOL isDynamicParm(CFGMGR_CODE eCode)
{
    switch(eCode) {
	case CFGMGR_PROFILE_STORED:
	case CFGMGR_POUND_UD:
	case CFGMGR_BLACK_LIST:
	case CFGMGR_COUNTRY_CODE:
	case CFGMGR_PREVIOUS_COUNTRY_CODE:
	case CFGMGR_VRID_FORMAT:
	case CFGMGR_VRID_CRC:
	case CFGMGR_VRID_TYPE:
	case CFGMGR_VRID_LENGTH:
	case CFGMGR_VRID_DATA:
#if !defined NO_QC_SUPPORT
	case CFGMGR_QC_PROFILE:
#endif
	case CFGMGR_PCI_VENDOR_ID:
	case CFGMGR_PCI_DEVICE_ID:
	case CFGMGR_HARDWARE_PROFILE:
	case CFGMGR_HARDWARE_ID:
	case CFGMGR_LICENSE_OWNER:
	case CFGMGR_LICENSE_KEY:
	case CFGMGR_LICENSE_STATUS:
	    return TRUE;
	default:
	    return FALSE;
    }
}

__shimcall__
COM_STATUS
NVM_Read (HANDLE hNVM, CFGMGR_CODE eCode, PVOID pBuf, UINT32 * pdwSize)
{
    POS_DEVNODE pDevNode = (POS_DEVNODE)hNVM;
    BOOL res;
    UINT32 Size;

    //printk(KERN_DEBUG "%s: pDevNode=%p CfgCode=%s Size=%p\n", __FUNCTION__, pDevNode, eCodeStr(eCode), pdwSize);

    if(isDynamicParm(eCode)) {
	ASSERT(pdwSize);

	Size = *pdwSize;

	res = NVM_ReadDynamicParm(pDevNode, eCodeStr(eCode), pBuf, pdwSize);
	if(!res) {
	    if (eCode == CFGMGR_COUNTRY_CODE) {
		ASSERT(Size == sizeof(g_DefaultCountryCode));
		if(Size == sizeof(g_DefaultCountryCode)) {
			memcpy(pBuf, &g_DefaultCountryCode, sizeof(g_DefaultCountryCode));
			*pdwSize = sizeof(g_DefaultCountryCode);
			res = TRUE;
		}
	    } else if (eCode == CFGMGR_PROFILE_STORED) {
		ASSERT(Size == sizeof(g_FactoryProfile));
		if(Size == sizeof(g_FactoryProfile)) {
			memcpy(pBuf, &g_FactoryProfile, sizeof(g_FactoryProfile));
			*pdwSize = sizeof(g_FactoryProfile);
			res = TRUE;
		}
	    }
	}
    } else if(eCode == CFGMGR_COUNTRY_STRUCT) {
#if TARGET_HCF_FAMILY
	res = NVM_ReadCountry(pDevNode, SIMPLE_DAA, (int)pdwSize, pBuf);
#else
	BYTE daaType = SIMPLE_DAA;

	Size = sizeof(daaType);
	NVM_Read (hNVM, CFGMGR_OEM_DAATYPE, &daaType, &Size);
	res = NVM_ReadCountry(pDevNode, daaType, (int)(unsigned long)pdwSize, pBuf);
#endif
    } else {
	Size = *pdwSize;

	res = NVM_ReadStaticParm(pDevNode, eCodeStr(eCode), pBuf, pdwSize);
	if(!res && (eCode == CFGMGR_PROFILE_FACTORY)) {
	    ASSERT(Size == sizeof(g_FactoryProfile));
	    if(Size == sizeof(g_FactoryProfile)) {
		memcpy(pBuf, &g_FactoryProfile, sizeof(g_FactoryProfile));
		*pdwSize = sizeof(g_FactoryProfile);
		res = TRUE;
	    }
	}
    }

    return res ? COM_STATUS_SUCCESS : COM_STATUS_VALUE_NOT_FOUND;
}

__shimcall__
COM_STATUS
NVM_Write (HANDLE hNVM, CFGMGR_CODE eCode, PVOID pBuf, PUINT32 pdwSize)
{
    POS_DEVNODE pDevNode = (POS_DEVNODE)hNVM;
    char pathname[100];

    //printk(KERN_DEBUG "%s: pDevNode=%p CfgCode=%s Size=%ld\n", __FUNCTION__, pDevNode, eCodeStr(eCode), *pdwSize);
	if(pDevNode->hwSuspendInProgress)
		return COM_STATUS_FAIL;

    if(!isDynamicParm(eCode)) {
    	printk(KERN_WARNING "%s: pDevNode=%p: saving static parameter, CfgCode=%s Size=%d\n", __FUNCTION__, pDevNode, eCodeStr(eCode), *pdwSize);
    }

    snprintf(pathname, sizeof(pathname), "%s/%d-%s/%s", cnxt_nvmdir_dynamic, pDevNode->hwInstNum, pDevNode->hwInstName, eCodeStr(eCode));

    return NVM_WriteFile(pathname, pBuf, *pdwSize, nvmFormat(eCode), pDevNode->hwSuspendInProgress) ? COM_STATUS_SUCCESS : COM_STATUS_FAIL;
}

__shimcall__
HANDLE
NVM_Open (ULONG_PTR dwDevNode)
{
    POS_DEVNODE pDevNode = (POS_DEVNODE)dwDevNode;
    char instname[100];
    int err;
    char hwProfile[sizeof(pDevNode->hwProfile)];
    UINT32 uiSize;
    nvmnewinst_t *newinst;
    
    snprintf(instname, sizeof(instname), "%d-%s", pDevNode->hwInstNum, pDevNode->hwInstName);

    err = NVM_NewInstance(instname);

    if(err) {
    	printk(KERN_WARNING "%s: cannot create instance %s: err=%d will retry later...\n", __FUNCTION__, instname, err);
	newinst = OsAllocate(sizeof(nvmnewinst_t));
	if (!newinst)
		return NULL;
	newinst->instname = OsAllocate(strlen(instname) + 1);
	if (!newinst->instname) {
		OsFree(newinst);
		return NULL;
	}
	strcpy(newinst->instname, instname);

	down(&nvmelem_writelist_sem);
	list_add_tail(&newinst->link, &nvm_newinst_list);
	up(&nvmelem_writelist_sem);
    }

    uiSize = sizeof(hwProfile);
    if(NVM_Read(pDevNode, CFGMGR_HARDWARE_PROFILE, hwProfile, &uiSize) == COM_STATUS_SUCCESS) {
	strncpy(pDevNode->hwProfile, hwProfile, sizeof(pDevNode->hwProfile));
	pDevNode->hwProfile[sizeof(pDevNode->hwProfile)-1] = '\0';
    }

    if(!strncmp(pDevNode->hwInstName, "PCI", 3)) {
	UINT16 pci_id;
	UINT32 size;

    	size = sizeof(pci_id);
    	pci_id = ((struct pci_dev *)pDevNode->hwDev)->device;
    	NVM_Write(pDevNode, CFGMGR_PCI_DEVICE_ID, &pci_id, &size);

    	size = sizeof(pci_id);
    	pci_id = ((struct pci_dev *)pDevNode->hwDev)->vendor;
    	NVM_Write(pDevNode, CFGMGR_PCI_VENDOR_ID, &pci_id, &size);
    }
    return pDevNode;
}

__shimcall__
COM_STATUS
NVM_Close (HANDLE hNVM)
{
    //printk(KERN_DEBUG "%s: hNVM=%p\n", __FUNCTION__, hNVM);

    NVM_WriteFlushList(TRUE);

    return COM_STATUS_SUCCESS;
}

void
OsNvmExit(void)
{
    //printk(KERN_DEBUG "%s: hNVM=%p\n", __FUNCTION__, hNVM);

    NVM_WriteFlushList(FALSE);
}
