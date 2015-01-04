/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */
#include "oscompat.h"

#include "framewrk.h"
#include "osservices.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
#include <asm/i387.h>
#endif

#if defined(cpu_has_xmm) && !defined(load_mxcsr)
#define load_mxcsr( val ) do { \
	unsigned long __mxcsr = ((unsigned long)(val) & 0xffbf); \
	asm volatile( "ldmxcsr %0" : : "m" (__mxcsr) ); \
} while (0)
#endif

#define NFPSTATES 10

typedef struct {
	unsigned long used;
#ifdef FOUND_THREAD_XSTATE
	union thread_xstate i387;
#else
	union i387_union i387;
#endif
	unsigned int cr0;
} fpstate_t;

static fpstate_t fpstates[NFPSTATES];

static spinlock_t fpstates_lock __attribute__((unused)) = SPIN_LOCK_UNLOCKED;

static inline void
hsf_kernel_fpu_begin(fpstate_t *fpstate)
{
	fpstate->cr0 = read_cr0();
	clts();
#ifdef __x86_64__
	asm volatile( "rex64/fxsave %0 ; fnclex"
		: "=m" (fpstate->i387.fxsave) );
#else
#ifdef cpu_has_fxsr
	if ( cpu_has_fxsr ) {
		asm volatile( "fxsave %0 ; fnclex"
		      	: "=m" (fpstate->i387.fxsave) );
	} else {
		asm volatile( "fnsave %0 ; fwait"
		      	: "=m" (fpstate->i387.fsave) );
	}
#else
	asm volatile( "fnsave %0 ; fwait"
	      	: "=m" (fpstate->i387) );
#endif
#endif /* __x86_64__ */

    __asm__("fninit");
#ifdef cpu_has_xmm
	if ( cpu_has_xmm )
		load_mxcsr(0x1f80);
#endif
}

static inline void
hsf_kernel_fpu_end(fpstate_t *fpstate)
{
	clts();
#ifdef __x86_64__
	asm volatile( "rex64/fxrstor %0; fwait"
		      : : "m" (fpstate->i387.fxsave) );
#else
#ifdef cpu_has_fxsr
	if ( cpu_has_fxsr ) {
		asm volatile( "fxrstor %0; fwait"
			      : : "m" (fpstate->i387.fxsave) );
	} else {
		asm volatile( "frstor %0; fwait"
			      : : "m" (fpstate->i387.fsave) );
	}
#else
	asm volatile( "frstor %0; fwait"
		      : : "m" (fpstate->i387) );
#endif
#endif /* __x86_64__ */
	write_cr0(fpstate->cr0);
}

/***************************************************************************
Function name :	OsFloatPrefix
Parameters :	None
Return value :	None
functionality :	Treats the FPU in order to enable float operations.
***************************************************************************/
 
__shimcall__
GLOBAL	int OsFloatPrefix(void)
{
	unsigned long flags;
	static int Id;

#if 0
	if(!in_interrupt()) {
		kernel_fpu_begin();
		return;
	}

	ASSERT(in_interrupt());
#endif

#ifdef CONFIG_PREEMPT
	preempt_disable(); /* might not be necessary since we are in interrupt context */
#endif

	spin_lock_irqsave(&fpstates_lock, flags);
	while(test_and_set_bit(0, &fpstates[Id].used)) {
		Id++;
		if(Id == NFPSTATES)
			Id = 0;
	}
	spin_unlock_irqrestore(&fpstates_lock, flags);

	hsf_kernel_fpu_begin(&fpstates[Id]);

	/* not allowed to sleep until OsFloatSuffix().
	 * moot point, since we're in interrupt context anyway..
	 * */
	return Id;
}

/***************************************************************************
Function name :	OsFloatPrefix
Parameters :	None
Return value :	None
functionality :	Restores the FPU after float operations.
***************************************************************************/
__shimcall__
GLOBAL	BOOL OsFloatSuffix(int Id)
{
#if 0
	if(!in_interrupt()) {
		kernel_fpu_end();
		return;
	}
#endif
	hsf_kernel_fpu_end(&fpstates[Id]);
	if(!test_and_clear_bit(0, &fpstates[Id].used)) {
#ifdef CONFIG_PREEMPT
		preempt_enable();
#endif
		printk(KERN_ERR "%s: Id=%d used bit was FALSE !\n", __FUNCTION__, Id);
		return FALSE;
	}

#ifdef CONFIG_PREEMPT
	preempt_enable();
#endif
	return TRUE;
}

