#ifndef __OSHDA_H
#define __OSHDA_H

#ifdef __cplusplus
extern "C"
{
#endif

// identical definition in patch_conexant.c must be kept in sync
typedef struct tagHDAOSHAL {
	void *hda_codec;
	int bInSuspendResume;
} HDAOSHAL, *PHDAOSHAL;

extern __shimcall__
unsigned int (*OsHdaCodecGetAddr)(PHDAOSHAL pHdaOsHal);

extern __shimcall__
unsigned int (*OsHdaCodecGetVendorId)(PHDAOSHAL pHdaOsHal);

extern __shimcall__
unsigned int (*OsHdaCodecGetSubsystemId)(PHDAOSHAL pHdaOsHal);

extern __shimcall__
unsigned int (*OsHdaCodecGetRevisionId)(PHDAOSHAL pHdaOsHal);

extern __shimcall__
void (*OsHdaCodecSetEventCallback)(PHDAOSHAL pHdaOsHal, void (*cbHdaEvent)(void *Context, unsigned int res), void *cbHdaEventContext, unsigned char *cbHdaTag);

extern __shimcall__
void (*OsHdaCodecClearEventCallback)(PHDAOSHAL pHdaOsHal, unsigned char cbHdaTag);

extern __shimcall__
unsigned int (*OsHdaCodecRead)(PHDAOSHAL pHdaOsHal, unsigned short nid, int direct, unsigned int verb, unsigned int para);

extern __shimcall__
unsigned int (*OsHdaCodecWallclock)(PHDAOSHAL pHdaOsHal);

extern __shimcall__
int (*OsHdaCodecOpenDMA)(PHDAOSHAL pHdaOsHal, int hw_param_buffer_bytes, void **ppPlaybackStream, void **ppCaptureStream);

extern __shimcall__
void (*OsHdaCodecCloseDMA)(PHDAOSHAL pHdaOsHal, void *pPlaybackStream, void *pCaptureStream);

extern __shimcall__
void (*OsHdaCodecDMAInfo)(PHDAOSHAL pHdaOsHal, void *Stream, unsigned char *StreamID, unsigned long *FifoSize, short unsigned int **ppBufAddr);

// identical definition in patch_conexant.c must be kept in sync
typedef enum {
	OsHdaStreamStateReset = 0,
	OsHdaStreamStateStop  = 1,
	OsHdaStreamStateRun   = 2
} OSHDA_STREAM_STATE;

extern __shimcall__
int (*OsHdaCodecSetDMAState)(PHDAOSHAL pHdaOsHal, OSHDA_STREAM_STATE streamState, void *pPlaybackStream, void *pCaptureStream);

extern __shimcall__
unsigned long (*OsHdaCodecGetDMAPos)(PHDAOSHAL pHdaOsHal, void *Stream);

#ifdef __cplusplus
}
#endif

#endif /* __OSHDA_H */
