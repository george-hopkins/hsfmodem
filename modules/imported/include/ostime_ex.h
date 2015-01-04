/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * Copyright (c) 2001-2004 Conexant Systems, Inc.
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
 * 6.   Redistribution. Permission to redistribute this software without
 * modification is granted, without prejudice to Linuxant's ability to obtain
 * reparation for any unauthorized distribution of previous versions of this
 * software released under prior LICENSE terms. Modification or redistribution
 * of this software under different terms requires explicit written approval
 * signed by an authorized Linuxant officer.
 * 
 * 7.   Performance. V.92 modems are designed to be capable of receiving data at
 * up to 56Kbps with compatible phone line and server equipment, and transmitting
 * data at up to 31.2Kbps. V.90 modems are designed to be capable of receiving
 * data at up to 56 Kbps from a compatible service provider and transmitting data
 * at up to about 28.8 Kbps. Public networks currently limit download speeds to
 * about 53Kbps. Actual speeds vary and are often less than the maximum possible.
 * 
 * 
 */

#include "osservices.h"
#if 0
#ifndef _OSTIME_EX_H_
#define _OSTIME_EX_H_
 
#include "comtypes.h"
#if (PRJ_FLAVOUR!=PRJFLVR_HSF)
#include "rtmgr_ex.h"
#endif
#ifdef __cplusplus
extern "C"
{
#endif
 
#if (OS_TYPE == OS_VXWORKS)
#define OSCriticalSectionCreate() OSSemaphoreCreate(1)
#define OSCriticalSectionDestroy OSSemaphoreDestroy
#define OSCriticalSectionAcquire OSSemaphoreWait
#define OSCriticalSectionRelease OSSemaphoreSignal
#define OsSetPeriodicTimer OSSetPeriodicTimeOut
#define OsDestroyPeriodicTimer OSDestroyPeriodicTimeOut
#define OsCallOnMyStack OSCallOnMyStack
#endif

#if OS_ONE_OF(OS_MACOSX | OS_STB | OS_LINUX)
#define HLOCK HANDLE
HANDLE OsLockCreate(VOID);
VOID OsLockDestroy(HLOCK hLock); 
VOID OsLockLock(HLOCK hLock);
VOID OsLockUnlock(HLOCK hLock);

#define OSCriticalSectionCreate         OsLockCreate
#define OSCriticalSectionDestroy        OsLockDestroy
#define OSCriticalSectionAcquire        OsLockLock
#define OSCriticalSectionRelease        OsLockUnlock
#define OSSetPeriodicTimer              OsSetPeriodicTimer
#define OSDestroyPeriodicTimer          OsDestroyPeriodicTimer
#define OSCallOnMyStack                 OsCallOnMyStack

#define OSCreateTimer           OsCreateTimer
#define OSSetTimer              OsSetTimer
#define OSCancelTimer           OsCancelTimer
#define OSChangeTimerTimeOut    OsChangeTimerTimeOut
#define OSDestroyTimer          OsDestroyTimer
#endif
	
/**********************************************************************************
Function name :	OsCreatePeriodicTimer
Parameters :	
				InitialTimeOut		- initial interval between 2 timeout callbacks
				pTimeOutCallBack	- Callback to be periodically called back
				pFuncAlloc			- wrapper function for allocations
				pFuncFree			- wrapper function for deallocations
				pRefData			- Reference Data to be passed to wrapper functions
Return value :	Returns a handle to the periodic timeout. This handle is to be
				passed to OS functions acting on periodic timeout. Returns NULL
				on failure.
functionality :	Creates a periodic timeout of InitialTimeOut interval. pTimeOutCallBack
				function will be called on each timer interval. 
**********************************************************************************/
HANDLE	OsCreatePeriodicTimer	(	IN	UINT32			InitialTimeOut,
										IN	PCBFUNC			pTimeOutCallBack,
										IN	PALLOC_FUNC		pFuncAlloc,
										IN	PFREE_FUNC		pFuncFree,
										IN	PVOID			pRefData,
										OUT	HANDLE*		pThreadId);

/**********************************************************************************
Function name :	OSDestroyPeriodicTimer
Parameters :	hTimeOut -	Handle to the periodic time out returned by 
							OSCreatePeriodicTimeOut.
Return value :	none
functionality : destroys a periodic timeout resource.
**********************************************************************************/
GLOBAL	VOID	OSDestroyPeriodicTimer	(	IN	HANDLE			hTimeOut);

/**********************************************************************************
Function name :	OSSetPeriodicTimer
Parameters :	hTimeOut	-	Handle to the periodic timeout
				NewTimeOut	-	New timeout interval
Return value :	returns TRUE when the timeout interval was successfully changed to 
				NewTimeOut
functionality : resets the timeout inteval to a new value.
**********************************************************************************/
GLOBAL	BOOL	OSSetPeriodicTimer	(	IN	HANDLE			hTimeOut,
											IN	UINT32			NewTimeOut);


#if (OS_TYPE != OS_LINUX)
/**********************************************************************************
Function name :	OSCallOnMyStack
Parameters :	pFunc			-	function to be called
				lParam			-	parameter to pass for pFunc
				pTopOfStack		-	Pointer to stack
				StackSizeBytes	-	stack size in bytes.
Return value :	None
functionality :	Temporarily switches a stack to a different locked block of memory.
**********************************************************************************/
GLOBAL	VOID	OSCallOnMyStack(			IN	PCBFUNC			pFunc, 
											IN	UINT32			lParam, 
											IN	PUINT32			pTopOfStack, 
											IN	UINT32			StackSizeBytes);
#else
#define OsCallOnMyStack(pFunc, lParam, pTopOfStack, StackSizeBytes) (pFunc)((PVOID)(lParam))
#endif

/**********************************************************************************
Function name :	OSGetSystemTime
Parameters :	None
Return value :	returns timestamp in milliseconds
functionality	The function returnes a milliseconds timestamp.
**********************************************************************************/
/* returns time stamp in milli-seconds					*/
GLOBAL	UINT32	OSGetSystemTime(			VOID	);

/* initialize epoch to reduce wrap around risk; all values returned by
 * OSGetSystemTime() will be relative to the time OSInitTime() was called */
GLOBAL	VOID	OSInitTime(			VOID	);
 
/**********************************************************************************
Function name :	OSSemaphoreCreate
Parameters :	InitCount - specifies the initial token count for the semaphore
Return value :	Handle to the created semaphore to be passed as parameter for OS 
				functions acting on semaphores. Returns NULL on failure.
functionality	The tokenCount should correspond to the number of clients that can 
				simultaneously acquire the resource the semaphore controls
**********************************************************************************/
GLOBAL	HANDLE	OSSemaphoreCreate(			IN	int				InitCount);

/**********************************************************************************
Function name :	OSSemaphoreDestroy
Parameters :	hSemaphore	-	Handle identifing the semaphore.
Return value :	None
functionality :	Destroys a semaphore. Any tasks currently locked by the semaphore 
				become reschedules.
**********************************************************************************/
GLOBAL	VOID	OSSemaphoreDestroy(			IN	HANDLE			hSemaphore);

/**********************************************************************************
Function name :	OSSemaphoreWait
Parameters :	hSemaphore	-	Handle identifing the semaphore.
Return value :	None
functionality	Blocks the calling run time  if all the tokens of the semaphore are 
				currently claimed.  If the number of clients that currently hold 
				the controlled resource is less than the initial token count, 
				then wait returns immediately and the caller holds the resource.  
				Otherwise, the client is blocked until another client releases 
				the resource.
**********************************************************************************/
GLOBAL	VOID	OSSemaphoreWait(			IN	HANDLE			hSemaphore);

/**********************************************************************************
Function name :	OSSemaphoreSignal
Parameters :	hSemaphore	-	Handle identifing the semaphore.
Return value :	None
functionality :	increments the token count of the semaphore.  If run times are waiting 
				on the semaphore, one of them will be unblocked.  
				The current run time may be rescheduled this function is called.
**********************************************************************************/
GLOBAL	VOID	OSSemaphoreSignal(			IN	HANDLE			hSemaphore);

#if OS_IS_NOT(OS_VXWORKS | OS_MACOSX | OS_STB | OS_LINUX)
 
/**********************************************************************************
Function name :	OSCriticalSectionCreate
Parameters :	None
Return value :	Handle to the created critical section to be passed as parameter for OS 
				functions acting on critical section. Returns NULL on failure.
functionality :	Creates an instance of a critical section. Returns NULL on failure
**********************************************************************************/
#ifdef USE_DDK
GLOBAL HANDLE	OSCriticalSectionCreate();
#else
#if (PROJECT == PRJ_SDSL) || (PROJECT == PRJ_OCTOLOADER)
GLOBAL HANDLE	OSCriticalSectionCreate();
#else
#define OSCriticalSectionCreate() __OSCriticalSectionCreate( __FILE__, __LINE__ )
GLOBAL	HANDLE	__OSCriticalSectionCreate( char *file, int line );
#endif
#endif
/**********************************************************************************
Function name :	OSCriticalSectionDestroy
Parameters :	hMutex	-	Handle identifing the mutex.
Return value :	None
functionality : Destroys a mutex instance. Do not attemt to destroy a mutex while 
				it is acquired.
**********************************************************************************/
GLOBAL	VOID	OSCriticalSectionDestroy(	IN	HANDLE			hMutex);

/**********************************************************************************
Function name :	OSCriticalSectionAcquire
Parameters :	hMutex	-	Handle identifing the mutex.
Return value :	None
functionality:	If the mutex is unowned, the calling run time becomes the owner
				If a run time  other than the calling run time owns the mutex, 
				the calling run time is blocked until the mutex  becomes unowned.  
				If the calling run time already owns the mutex, the run time continues 
				execution.

**********************************************************************************/
GLOBAL	VOID	OSCriticalSectionAcquire(	IN	HANDLE			hMutex);

/**********************************************************************************
Function name :	OSCriticalSectionRelease
Parameters :	hMutex	-	Handle identifing the mutex.
Return value :	None
functionality:	A mutex is initially unowned, and the ownership count is zero.  For 
				each call to acquire, there must be a corresponding call to release.  
				Each time acquire is called, the ownership count  increments, and each 
				time release is called, the ownership count decrements.  Only the run time
				that owns a mutex may leave it.
				When the ownership count falls to zero, the mutex becomes unowned.  At 
				this point, a run time blocked by the mutex, with the highest priority 
				becomes schedulable.
**********************************************************************************/
GLOBAL	VOID	OSCriticalSectionRelease(	IN	HANDLE			hMutex);

#endif /** OS_VXWORKS **/
/**********************************************************************************
Function name : OSSetTimeSensitivity
Parameters :	Interval - timer interrupt interval
Return value :	None
functionality:	Determines the operatin system timeout interval, to enable timer 
				resolution control. For each call to OSSetTimeSensitivity there
				should be a matching call to OSRestoreTimeSensitivity with same Interval 
				value
**********************************************************************************/
GLOBAL	VOID	OSSetTimeSensitivity(	IN	UINT32				Interval);				

/**********************************************************************************
Function name :	OSRestoreTimeSensitivity
Parameters :	Interval - timer interrupt interval
Return value :	None
functionality :	Restores the timer interrupt to the value before OSSetTimeSensitivity
				was called
**********************************************************************************/
GLOBAL	VOID	OSRestoreTimeSensitivity(		IN	UINT32				Interval);				

/**********************************************************************************
Function name :	OSDisableInterrupt
Parameters :	None
Return value :	handle to be passed to OSRestoreInterrupt
functionality :	Disables interrupt.
**********************************************************************************/
GLOBAL	HANDLE	OSDisableInterrupt(void);

/**********************************************************************************
Function name :	OSRestoreInterrupts
Parameters :	hStatus - handle returned by OSDisableInterrupt
Return value :	None
functionality :	Restores the interrupt status as before the OSDisableInterrupt was called
**********************************************************************************/
GLOBAL	VOID	OSRestoreInterrupts(		IN	HANDLE			hStatus);



/**********************************************************************************
Function name :	OsFloatPrefix
Parameters :	None
Return value :	None
functionality :	Treats the FPU in order to enable float operations.
**********************************************************************************/
GLOBAL	int  OsFloatPrefix(void);

/**********************************************************************************
Function name :	OsFloatPrefix
Parameters :	None
Return value :	None
functionality :	Restores the FPU after float operations.
**********************************************************************************/
GLOBAL	BOOL  OsFloatSuffix(int Id);

#if OS_ONE_OF( OS_LINUX | OS_VXWORKS | OS_MACOSX | OS_STB )
HANDLE OSCreateTimer(UINT32 msec, PVOID pCBFunc, PVOID pRefData);
void OSSetTimer(PVOID pTimer);
void OSCancelTimer(PVOID Timer);
void OSChangeTimerTimeOut(PVOID Timer, UINT32 msec);
void OSDestroyTimer(PVOID pTimer);

#endif

#if OS_IS_NOT( OS_MACOSX | OS_STB )
#define OsLockCreate() OSSemaphoreCreate(1)
#endif
#if ( OS_TYPE != OS_STB )
#define OsLockDestroy OSSemaphoreDestroy
#define OsLockLock OSSemaphoreWait
#define OsLockUnlock OSSemaphoreSignal
#define OsGetSystemTime OSGetSystemTime
#else
UINT32	OsGetSystemTime(VOID);
#define OSGetSystemTime OsGetSystemTime
BOOL    OsHookInterrupt     ( HANDLE hDevice, VOID (*InterruptHandler)(VOID*), VOID* pRef);
BOOL    OsUnhookInterrupt   ( HANDLE hDevice);
#endif

GLOBAL  VOID    OSImmediateTimeOut      (	IN	HANDLE			hTimeOut);
 
GLOBAL VOID OSCancelPeriodicTimeOut		(	IN HANDLE			hTimeOut);
 
#if (PROJECT == PRJ_SDSL) || (PROJECT == PRJ_OCTOLOADER)
/**********************************************************************************
Function name :	OSHookInterrupt
Parameters :	pResources  - Different for each Os - includes resources needed for
                              for connecting to interrupt.
                pCallBack   - Interrupt handler 
                pConext     - Contect to be sent to interrupt handler
Return value :	HANDLE to be called to OsUnhookInterrupt
functionality :	Registers an ISR to be called on specified interrupt.
**********************************************************************************/
GLOBAL	HANDLE  OSHookInterrupt(    IN  PVOID   pResources,
                                    IN  PISR    pISRCallBack,
                                    IN  PISR    pDPCCallBack,
                                    IN  HANDLE  pConext);
 
 
/**********************************************************************************
Function name :	OSUnhookInterrupt
Parameters :	hHook  - Hnadle returned by OsHookInterrupt
Return value :	None
functionality :	Unregisters from an interrupt.
**********************************************************************************/
GLOBAL	VOID  OSUnhookInterrupt(    IN  HANDLE   hHook);
 
#endif
/********************************************************************/
/* Utilities                                                        */
/********************************************************************/

BOOL OsAtomicCompareAndSwap (PVOID oldValue, PVOID newValue, PVOID* address);
INT32	OsAtomicAdd   (INT32 amount, INT32* address);
void OsSleep(UINT32 SleepTime);

#if (OS_TYPE == OS_WIN_9X)
#define __FUNCTION__
#endif
 
#if (OS_TYPE == OS_VXWORKS)
int OsTaskSpawn(char *  name,      /* name of new task (stored at pStackBase) */
				int     priority,  /* priority of new task */
				int     options,   /* task option word */
				int     stackSize, /* size (bytes) of stack needed plus name */
				void*   entryPt,   /* entry point of new task */
				int     arg1,      /* 1st of 10 req'd task args to pass to func */
				int     arg2,
				int     arg3,
				int     arg4,
				int     arg5,
				int     arg6,
				int     arg7,
				int     arg8,
				int     arg9,
				int     arg10);
BOOL OsTaskDelete(int TaskId);
#endif

#if (OS_TYPE == OS_LINUX)
typedef unsigned long tMuSecTimeStamp;

#define GlobalProcessorFreq OsGlobalProcessorFreq
#define GetMuSecStartStamp OsGetMuSecStartStamp
#define DeltaTimeMuSec OsDeltaTimeMuSec
#define BusyLoopMilliSec OsBusyLoopMilliSec
#define BusyLoopMuSec OsBusyLoopMuSec
#define PerfCntGuardBusyLoop() {}

extern DWORD GlobalProcessorFreq;  // in MHz

tMuSecTimeStamp OsGetMuSecStartStamp(void);
tMuSecTimeStamp OsDeltaTimeMuSec(tMuSecTimeStamp PrevStamp, tMuSecTimeStamp NextStamp);
void OsBusyLoopMilliSec(DWORD DelayInMilliSecs);
void OsBusyLoopMuSec(DWORD DelayInMicroSecs);
#endif


#ifdef __cplusplus
}
#endif
 
#endif /* _OSTIME_EX_H_ */
#endif
