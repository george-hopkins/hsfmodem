/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */

#ifndef __powerpc__
#define __KERNEL_SYSCALLS__
#endif

#ifndef FOUND_KERNEL_EXECVE
#define STATIC_ERRNO
#endif

#define STATIC_TQUEUE_LOCK

#include "oscompat.h"

#ifdef FOUND_LINUX_SUSPEND
#define UINT8 UINT8_suspend
#define BOOL BOOL_suspend
#define UINT16 UINT16_suspend
#define UINT32 UINT32_suspend
#define INT32 INT32_suspend
#define TRUE TRUE_suspend
#define FALSE FALSE_suspend
#undef INT64
#include <linux/suspend.h>
#undef UINT8
#undef BOOL
#undef UINT16
#undef UINT32
#undef INT32
#undef TRUE
#undef FALSE
#endif /* FOUND_LINUX_SUSPEND */

#include "osservices.h"
#include "oslinux.h"
#include "osmemory.h"
#include "testdebug.h"

#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39))
#include <linux/smp_lock.h>
#endif
#include <linux/file.h>
#include <linux/kmod.h>

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,7) )
#include <linux/completion.h>
#endif

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0) )
#include <linux/kthread.h>
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(3,15,0) )
#include <linux/sched/prio.h>
#else
#include <linux/sched/rt.h>
#endif
#endif

#ifdef for_each_process
#define sigmask_lock sighand->siglock
#define recalc_sigpending_curtask(t) recalc_sigpending()
#else
#define sighand sig
#define recalc_sigpending_curtask(t) recalc_sigpending(t)
#endif

/********************************************************************/
/* Critical Sections (Mutexes) */

typedef struct {
#ifdef CONFIG_SMP
    spinlock_t spinlock;
#else
    volatile unsigned long lock;
#endif
    unsigned long flags;
    unsigned long nestcnt;
    int owner_cpu;
} CRIT_T;

__shimcall__
GLOBAL HCRIT OsCriticalSectionCreate(void)
{
    CRIT_T *mutex;

    ASSERT(OsContextAllowsSleeping());
    mutex  = kmalloc(sizeof(CRIT_T), GFP_KERNEL);
    if(!mutex) {
	ASSERT(mutex);
	return NULL;
    }

#ifdef CONFIG_SMP
    spin_lock_init(&mutex->spinlock);
#else
    mutex->lock = 0;
#endif
    mutex->nestcnt = 0;

    return ((HCRIT)mutex);
}

__shimcall__
GLOBAL VOID OsCriticalSectionDestroy(IN HCRIT hMutex)
{
    if ( NULL == hMutex )
    {
	ASSERT(hMutex);
	return;
    }
    kfree((CRIT_T *)hMutex);
}

__shimcall__
GLOBAL VOID OsCriticalSectionAcquire(IN HCRIT hMutex)
{
    CRIT_T *mutex = (CRIT_T *) hMutex;
    unsigned long flags;
    int cpu;

    ASSERT(hMutex);

    if(!hMutex) /* temporary due to HCF global critical section issue */
	return;

    local_irq_save(flags);

    cpu = smp_processor_id();

#ifdef CONFIG_SMP
    if(!spin_trylock(&mutex->spinlock))
#else
    if(test_and_set_bit(0,(&mutex->lock)))
#endif
    {

	if(cpu == mutex->owner_cpu) {
	    mutex->nestcnt++;
	    local_irq_restore(flags);
	    return;
	}
#ifdef CONFIG_SMP
	spin_lock(&mutex->spinlock);
#else
	mutex->lock = 1;
#endif
    }

    mutex->owner_cpu = cpu;
    mutex->flags = flags;

    return;
}

__shimcall__
GLOBAL VOID OsCriticalSectionRelease(IN HCRIT hMutex)
{
    CRIT_T *mutex = (CRIT_T *) hMutex;

    ASSERT(hMutex);

    if(!hMutex) /* temporary due to HCF global critical section issue */
	return;

    if(mutex->nestcnt) {
	mutex->nestcnt--;
	return;
    }

#ifdef CONFIG_SMP
    spin_unlock(&mutex->spinlock);
#else
    mutex->lock = 0;
#endif
    local_irq_restore(mutex->flags);
}

/********************************************************************/

typedef struct tagLS {
    spinlock_t spinlock;
    struct semaphore semaphore;
    void *owner;
    unsigned long nestcnt;
} LS, *PLS;

__shimcall__
HLOCK OsLockCreate(void)
{
    PLS		pLock = (PLS)OsAllocate(sizeof(LS));

    if(pLock) {
	spin_lock_init(&pLock->spinlock);
	sema_init(&pLock->semaphore, 1);

	pLock->owner = NULL;
	pLock->nestcnt = 0;
    }

    //printk(KERN_DEBUG"%s: %p\n", __FUNCTION__, pLock);
    return (HLOCK)pLock;
}

__shimcall__
void OsLockDestroy(HLOCK hLock)
{
    //printk(KERN_DEBUG"%s: %p\n", __FUNCTION__, hLock);
    if(hLock)
	OsFree((PLS)hLock);
}

__shimcall__
void OsLockLock(HLOCK hLock)
{
    unsigned long flags;

    //printk(KERN_DEBUG"%s: down %p\n", __FUNCTION__, hLock);

    ASSERT(OsContextAllowsSleeping());
    spin_lock_irqsave(&((PLS)hLock)->spinlock, flags);
    if(down_trylock(&((PLS)hLock)->semaphore) != 0) {
	if(((PLS)hLock)->owner == current) {
	    ((PLS)hLock)->nestcnt++;
	    //printk(KERN_DEBUG"%s: ret %p (nestcnt=%lu)\n", __FUNCTION__, hLock, ((PLS)hLock)->nestcnt);
	    spin_unlock_irqrestore(&((PLS)hLock)->spinlock, flags);
	    return;
	}
	spin_unlock_irqrestore(&((PLS)hLock)->spinlock, flags);
	ASSERT(OsContextAllowsSleeping());
	down(&((PLS)hLock)->semaphore);
	spin_lock_irqsave(&((PLS)hLock)->spinlock, flags);
    }
    ((PLS)hLock)->owner = current;
    spin_unlock_irqrestore(&((PLS)hLock)->spinlock, flags);
    //printk(KERN_DEBUG"%s: ret %p\n", __FUNCTION__, hLock);
}

__shimcall__
void OsLockUnlock(HLOCK hLock)
{
    unsigned long flags;

    //printk(KERN_DEBUG"%s: up %p\n", __FUNCTION__, hLock);
    spin_lock_irqsave(&((PLS)hLock)->spinlock, flags);
    if(((PLS)hLock)->nestcnt) {
	((PLS)hLock)->nestcnt--;
	spin_unlock_irqrestore(&((PLS)hLock)->spinlock, flags);
	return;
    }
    ((PLS)hLock)->owner = NULL;
    spin_unlock_irqrestore(&((PLS)hLock)->spinlock, flags);
    up(&((PLS)hLock)->semaphore);
}

__shimcall__
BOOL OsLockTry(HLOCK hLock)
{
    unsigned long flags;

    spin_lock_irqsave(&((PLS)hLock)->spinlock, flags);
    if(down_trylock(&((PLS)hLock)->semaphore) != 0) {
	if(((PLS)hLock)->owner == current) {
	    ((PLS)hLock)->nestcnt++;
	    //printk(KERN_DEBUG"%s: ret %p (nestcnt=%lu) TRUE\n", __FUNCTION__, hLock, ((PLS)hLock)->nestcnt);
	    spin_unlock_irqrestore(&((PLS)hLock)->spinlock, flags);
	    return TRUE;
	}
	//printk(KERN_DEBUG"%s: ret %p FALSE\n", __FUNCTION__, hLock);
	spin_unlock_irqrestore(&((PLS)hLock)->spinlock, flags);
	return FALSE;
    }
    ((PLS)hLock)->owner = current;
    spin_unlock_irqrestore(&((PLS)hLock)->spinlock, flags);
    //printk(KERN_DEBUG"%s: ret %p TRUE\n", __FUNCTION__, hLock);
    return TRUE;
}

__shimcall__
void OsLockTryUnlock(HLOCK hLock)
{
    //printk(KERN_DEBUG"%s: up %p\n", __FUNCTION__, hLock);
    OsLockUnlock(hLock);
}

/********************************************************************/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)
static DEFINE_SEMAPHORE(current_sem);
static DEFINE_SPINLOCK(atomic_lock);
#else
static spinlock_t atomic_lock __attribute__((unused)) = SPIN_LOCK_UNLOCKED;
#endif

/****************************************************************************************
  The OsAtomicCompareAndSwap function compares the value at the specified address with 
  oldVal. The value of newValue is written to the address only if oldValue and the 
  value at the address are equal. OsCompareAndSwap returns true if newValue is written 
  to the address; otherwise, it returns false. 

Params:
oldValue The value to compare at address.
newValue The value to write to address if oldValue compares true.
address The 4-byte aligned address of the data to update atomically.
Result: true if newValue was written to the address. 
 ****************************************************************************************/
__shimcall__
BOOL OsAtomicCompareAndSwapEx (PVOID oldValue, PVOID newValue, PVOID* address, INT size)
{
#if defined(__HAVE_ARCH_CMPXCHG) && !defined(__powerpc__)
    switch(size) {
	case sizeof(u8):
		return (cmpxchg(((u8*)address), (ULONG_PTR)oldValue, (ULONG_PTR)newValue) == (u8)(ULONG_PTR)oldValue);
	case sizeof(u16):
		return (cmpxchg(((u16*)address), (ULONG_PTR)oldValue, (ULONG_PTR)newValue) == (u16)(ULONG_PTR)oldValue);
	case sizeof(u32):
		return (cmpxchg(((u32*)address), (ULONG_PTR)oldValue, (ULONG_PTR)newValue) == (u32)(ULONG_PTR)oldValue);
	case sizeof(u64):
#ifdef cmpxchg64
		return (cmpxchg64(((u64*)address), (ULONG_PTR)oldValue, (ULONG_PTR)newValue) == (u64)(ULONG_PTR)oldValue);
#else
		return (cmpxchg(((u64*)address), (ULONG_PTR)oldValue, (ULONG_PTR)newValue) == (u64)(ULONG_PTR)oldValue);
#endif
	default:
		printk(KERN_ERR"%s: size=%d not supported\n", __FUNCTION__, size);
		return TRUE;
    }
#else
    unsigned long flags;
    BOOL ret = FALSE;

    spin_lock_irqsave(&atomic_lock, flags);

    switch(size) {
	case sizeof(u8):
		if (*(u8*)address == (u8)(ULONG_PTR)oldValue) {
		    *(u8*)address = (u8)(ULONG_PTR)newValue;
		    ret = TRUE;
		}
		break;
	case sizeof(u16):
		if (*(u16*)address == (u16)(ULONG_PTR)oldValue) {
		    *(u16*)address = (u16)(ULONG_PTR)newValue;
		    ret = TRUE;
		}
		break;
	case sizeof(u32):
		if (*(u32*)address == (u32)(ULONG_PTR)oldValue) {
		    *(u32*)address = (u32)(ULONG_PTR)newValue;
		    ret = TRUE;
		}
		break;
	case sizeof(u64):
		if (*(u64*)address == (u64)(ULONG_PTR)oldValue) {
		    *(u64*)address = (u64)(ULONG_PTR)newValue;
		    ret = TRUE;
		}
		break;
	default:
		printk(KERN_ERR"%s: size=%d not supported\n", __FUNCTION__, size);
		ret = TRUE;
		break;
    }

    spin_unlock_irqrestore(&atomic_lock, flags);

//printk(KERN_ERR"%s: size=%d ret=%d\n", __FUNCTION__, size, ret);
    return ret;
#endif
}

__shimcall__
INT32 OsAtomicAdd   (INT32 amount, PINT32 address)
{
    unsigned long flags;
    atomic_t *v = (atomic_t *)address;

    spin_lock_irqsave(&atomic_lock, flags);
    atomic_add(amount, v);
    amount = atomic_read(v);
    spin_unlock_irqrestore(&atomic_lock, flags);

    return amount;
}

__shimcall__
INT32 OsAtomicIncrement (PINT32 address)
{
    unsigned long flags;
    atomic_t *v = (atomic_t *)address;
    INT32 amount;

    spin_lock_irqsave(&atomic_lock, flags);
    atomic_inc(v);
    amount = atomic_read(v);
    spin_unlock_irqrestore(&atomic_lock, flags);

    return amount;
}

__shimcall__
INT32 OsAtomicDecrement (PINT32 address)
{
    unsigned long flags;
    atomic_t *v = (atomic_t *)address;
    INT32 amount;

    spin_lock_irqsave(&atomic_lock, flags);
    atomic_dec(v);
    amount = atomic_read(v);
    spin_unlock_irqrestore(&atomic_lock, flags);

    return amount;
}

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0) )

struct _OSTHRD {
    int pid;
    struct kthread_worker kworker;
    struct task_struct *kworker_task;
};

struct kwork_data {
    struct kthread_work work;
    void (*func)(void *);
    void* data;
};

static void
OsThreadStart(OSTHRD *osthrd, const char *name, BOOLEAN highestprio)
{
    struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };
    
	memset(osthrd, 0, sizeof(*osthrd));
	
	init_kthread_worker(&osthrd->kworker);
	osthrd->kworker_task = kthread_run(kthread_worker_fn, &osthrd->kworker, "k%sd/%s", CNXTTARGET, name);
	
	if (IS_ERR(osthrd->kworker_task)) {
	    return;
    }
    
    osthrd->pid = osthrd->kworker_task->pid;
    
    if(highestprio) {
        sched_setscheduler(osthrd->kworker_task, SCHED_FIFO, &param);
    }
}

static void
OsThreadStop(OSTHRD *osthrd)
{
    if (!IS_ERR(osthrd->kworker_task)) {
        kthread_stop(osthrd->kworker_task);
    }
}

__shimcall__
POSTHRD
OsThreadCreate(const char *name, BOOLEAN highestprio, int *pid)
{
	OSTHRD *osthrd = OsAllocate(sizeof(OSTHRD));

	if(osthrd) {
		OsThreadStart(osthrd, name, highestprio);
		if(pid)
			*pid = osthrd->pid;
	}

	return osthrd;
}

__shimcall__
void
OsThreadDestroy(POSTHRD osthrd)
{
	OsThreadStop(osthrd);
	OsFree(osthrd);
}

static void
unwrap_kwork(struct kthread_work *work)
{
    struct kwork_data *w = container_of(work, struct kwork_data, work);
    w->func(w->data);
}

__shimcall__
void OsThreadScheduleInit(HOSSCHED hWorkStorage, __kernelcall__ void (*func)(void *), void * data)
{
	struct kwork_data *w = (struct kwork_data *)hWorkStorage;
    init_kthread_work(&w->work, unwrap_kwork);
    w->func = func;
    w->data = data;
}

__shimcall__
int OsThreadSchedule(POSTHRD osthrd, HOSSCHED hWorkStorage)
{
	bool ret;
	struct kwork_data *w = (struct kwork_data *)hWorkStorage;

	if(!osthrd || !IS_ERR(osthrd->kworker_task)) {
		printk(KERN_DEBUG "%s: no thread %p\n", __FUNCTION__, osthrd);
		return 0;
	}

	OsModuleUseCountInc();
	ret = queue_kthread_work(&osthrd->kworker, &w->work);
	if(!ret) {
		OsModuleUseCountDec();
	}

	return ret;
}

__shimcall__
void OsThreadScheduleDone(void)
{
    OsModuleUseCountDec();
}

#else

struct _OSTHRD {
	const char *name;
	int pid;
	BOOLEAN highestprio;
	task_queue tq;
	wait_queue_head_t wq;
#ifdef DECLARE_COMPLETION
	struct completion started;
	struct completion exited;
#else
	int running;
#endif
	int stop;

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
	unsigned long stack_overflows;
	struct task_struct ts;
#endif
};

static void handle_sigs(void)
{
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	flush_signals(current); /* must be called without spinlock */
#endif
	spin_lock_irq(&current->sigmask_lock);
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
	flush_signals(current); /* must be called with spinlock held */
#endif
	recalc_sigpending_curtask(current);
	spin_unlock_irq(&current->sigmask_lock);
}

static int cnxt_thread(OSTHRD *osthrd)
{
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
	int i;
#endif

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	daemonize("k%sd/%s", CNXTTARGET, osthrd->name);
#ifdef PF_IOTHREAD
	current->flags |= PF_IOTHREAD;
#endif
#endif

#ifdef PF_NOFREEZE
	current->flags |= PF_NOFREEZE;
#endif

#if defined(TARGET_HSF_LINUX) && defined(CONFIG_SMP)
	/* temporary workaround */
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
	current->cpus_allowed = 1 << cpu_logical_map(0);
#else
	current->cpus_allowed = cpumask_of_cpu(0);
#endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)
	down(&current_sem);
#else
	lock_kernel();
#endif

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
	exit_mm(current);
	exit_files(current);

	current->session = 1;
	current->pgrp = 1;

	snprintf(current->comm, sizeof(current->comm), "k%s%sd", CNXTTARGET, osthrd->name);
	current->comm[sizeof(current->comm) - 1] = '\0';
#endif

	if(osthrd->highestprio) {
		/* set highest priority */
#ifdef DEF_NICE
		current->nice = -20;
#else
		set_user_nice(current, -20);
#endif

#ifndef MAX_RT_PRIO
#define MAX_RT_PRIO 100
#endif
		current->policy = SCHED_FIFO;
		current->rt_priority = MAX_RT_PRIO-1;
	} else {
#ifdef DEF_NICE
		current->nice = -10;
#else
		set_user_nice(current, -10);
#endif
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)
	up(&current_sem);
#else
	unlock_kernel();
#endif

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	flush_signals(current); /* must be called without spinlock */
	allow_signal(SIGTERM);
	allow_signal(SIGKILL);
	allow_signal(SIGSTOP);
#else
	spin_lock_irq(&current->sigmask_lock);
	flush_signals(current); /* must be called with spinlock held */
	{ // flush_signal_handlers
		struct k_sigaction *ka = &current->sighand->action[0];
		for (i = _NSIG ; i != 0 ; i--) {
			ka->sa.sa_handler = SIG_DFL;
			ka->sa.sa_flags = 0;
			sigemptyset(&ka->sa.sa_mask);
			ka++;
		}
	}
	siginitsetinv(&current->blocked, sigmask(SIGTERM) | sigmask(SIGKILL) | sigmask(SIGSTOP));
	recalc_sigpending_curtask(current);
	spin_unlock_irq(&current->sigmask_lock);
#endif

#ifdef DECLARE_COMPLETION
	complete(&osthrd->started);
#else
	osthrd->running = 1;
#endif

	{
		DECLARE_WAITQUEUE(wait, current);

		add_wait_queue(&osthrd->wq, &wait);

		while (!osthrd->stop) {
			set_current_state(TASK_INTERRUPTIBLE);

#if 0// ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
			if(!OsKernelConfig.Stack.Wrap) {
				lock_kernel();
				memcpy(&osthrd->ts.uid, &current->uid, sizeof(osthrd->ts) - (int)(&((struct task_struct *)0)->uid));
				unlock_kernel();
			}
#endif

			run_task_queue(&osthrd->tq);

#if 0// ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
			if(!OsKernelConfig.Stack.Wrap && (current->sighand != osthrd->ts.sighand)) {
				lock_kernel();
				memcpy(&current->uid, &osthrd->ts.uid, sizeof(osthrd->ts) - (int)(&((struct task_struct *)0)->uid));
				unlock_kernel();
				if(!osthrd->stack_overflows++)
					printk(KERN_WARNING"%s: %d stack overflow\n", __FUNCTION__, current->pid);
			}
#endif

			if(signal_pending(current)) {
				handle_sigs();
			} else {
				schedule();
			}
		}
		current->state = TASK_RUNNING;
		remove_wait_queue(&osthrd->wq, &wait);
	}

	handle_sigs();

    osthrd->pid = 0;
#ifdef DECLARE_COMPLETION
#if defined(complete_and_exit) || ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,9) )
    complete_and_exit(&osthrd->exited, 0);
#else
    complete(&osthrd->exited);
    up_and_exit(NULL, 0);
#endif
#else
    osthrd->running = 0;
    return 0;
#endif
}

static void
OsThreadStart(OSTHRD *osthrd, const char *name, BOOLEAN highestprio)
{
	memset(osthrd, 0, sizeof(*osthrd));

	osthrd->name = name;
	osthrd->highestprio = highestprio;

	INIT_LIST_HEAD(&osthrd->tq);
	init_waitqueue_head(&osthrd->wq);

#ifdef DECLARE_COMPLETION
	init_completion(&osthrd->started);
	init_completion(&osthrd->exited);

	osthrd->pid = kernel_thread((int (*)(void *))cnxt_thread, osthrd, 0);
	if(osthrd->pid < 0)
		return;

	wait_for_completion(&osthrd->started);
#else
	osthrd->running = 0;
	osthrd->pid = kernel_thread((int (*)(void *))cnxt_thread, NULL, 0);
	if(osthrd->pid < 0)
		return;
	{
		int i;

		/* wait 5 seconds maximum for thread to start */
		for (i=5000; !osthrd->running && (i > 0); i -= 50) {
			OsSleep(50);
		}
		if(i <= 0) {
			printk(KERN_ERR"%s: giving up on thread\n", __FUNCTION__);
		}
	}
#endif

	//OsDebugPrintf("%s: started pid %d (%s)\n", __FUNCTION__, osthrd->pid, osthrd->name);
}

static void
OsThreadStop(OSTHRD *osthrd)
{
	//OsDebugPrintf("%s: stopping thread=%p pid=%d name=%s\n", __FUNCTION__, osthrd, osthrd->pid, osthrd->name);

	osthrd->stop = 1;
	if(osthrd->pid > 0) {
		int r = kill_proc(osthrd->pid, SIGKILL, 1);
		if(r) {
			printk(KERN_ERR"%s: kill_proc osthrd->pid=%d r=%d\n", __FUNCTION__, osthrd->pid, r);
		}
#ifdef DECLARE_COMPLETION
		wait_for_completion(&osthrd->exited);
#else
		{
			int i;

			/* wait 5 seconds maximum for thread to terminate */
			for (i=5000; osthrd->running && (i > 0); i -= 50) {
				OsSleep(50);
			}
			if(i <= 0) {
				printk(KERN_ERR"%s: giving up on thread\n", __FUNCTION__);
			}
		}
#endif
	}
}

__shimcall__
POSTHRD
OsThreadCreate(const char *name, BOOLEAN highestprio, int *pid)
{
	OSTHRD *osthrd = OsAllocate(sizeof(OSTHRD));

	if(osthrd) {
		OsThreadStart(osthrd, name, highestprio);
		if(pid)
			*pid = osthrd->pid;
	}

	return osthrd;
}

__shimcall__
void
OsThreadDestroy(POSTHRD osthrd)
{
	OsThreadStop(osthrd);
	//OsDebugPrintf("%s: freeing thread=%p\n", __FUNCTION__, osthrd);
	OsFree(osthrd);
}


__shimcall__
void OsThreadScheduleInit(HOSSCHED hWorkStorage, __kernelcall__ void (*func)(void *), void * data)
{
	struct tq_struct *tqs = (struct tq_struct *)hWorkStorage;

    INIT_TQUEUE(tqs, func, data);
}

__shimcall__
int OsThreadSchedule(POSTHRD osthrd, HOSSCHED hWorkStorage)
{
	int ret;
	struct tq_struct *tqs = (struct tq_struct *)hWorkStorage;

	if(!osthrd || osthrd->pid <= 0) {
		printk(KERN_DEBUG "%s: no thread %p?!\n", __FUNCTION__, osthrd);
		return 0;
	}

	OsModuleUseCountInc();
	ret = queue_task(tqs, &osthrd->tq);
	if(!ret)
		OsModuleUseCountDec();
	else
		wake_up(&osthrd->wq);

	return ret;
}

__shimcall__
void OsThreadScheduleDone(void)
{
    OsModuleUseCountDec();
}

#endif

/********************************************************************/

#ifndef FOUND_KERNEL_EXECVE
#ifndef __x86_64__
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 8))
static inline _syscall3(pid_t,waitpid,pid_t,pid,int *,wait_stat,int,options)
#endif

__shimcall__
int OsRunThreadSync(__kernelcall__ int (*func)(void *), void * data)
{
	pid_t pid;
	sigset_t tmpsig;
	int result, status;
   
	//OsDebugPrintf("%s: func=%p(%p)\n", __FUNCTION__, func, data);

	if (!current->fs->root) {
		OsErrorPrintf("%s: Root filesystem is not mounted.\n", __FUNCTION__);
		return -EPERM;
	}

	pid = kernel_thread(func, data, 0);
	if(pid < 0) {
		OsErrorPrintf("%s: Forking of kernel thread failed (%d)\n", __FUNCTION__, pid);
		return pid;
	}

	/* Block signals, everything but SIGKILL/SIGSTOP */
	spin_lock_irq(&current->sigmask_lock);
	tmpsig = current->blocked;
	siginitsetinv(&current->blocked, sigmask(SIGKILL) | sigmask(SIGSTOP));
	recalc_sigpending_curtask(current);
	spin_unlock_irq(&current->sigmask_lock);

	{
		mm_segment_t fs = get_fs ();
		set_fs (get_ds ());
		status = 0; /* SuSE 9.0 workaround */
		result = waitpid(pid, &status, __WCLONE);
		set_fs (fs);
	}

	/* Allow signals again */
	spin_lock_irq(&current->sigmask_lock);
	current->blocked = tmpsig;
	recalc_sigpending_curtask(current);
	spin_unlock_irq(&current->sigmask_lock);

	if (result != pid) {
		OsErrorPrintf("%s: Waiting for pid %d failed (errno=%d).\n", __FUNCTION__, pid, -result);
		return result;
	}

	//OsDebugPrintf("%s: returned status %d\n", __FUNCTION__, (((status) & 0xff00) >> 8));
	return (((status) & 0xff00) >> 8);
}

typedef struct {
	char *path;
	char **argv;
	char **envp;
} OsExecArgs;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)) && defined(__powerpc__)
static int execve_stub(const char *file, char **argv, char **envp)
{
    __syscall_nr(3, int, execve, file, argv, envp);
}
#undef execve
#define execve execve_stub
#endif

static int OsExec(void *execargs)
{
	OsExecArgs *ea = execargs;
	int err;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	err = exec_usermodehelper(ea->path, ea->argv, ea->envp);
#else
	mm_segment_t fs = get_fs ();
	set_fs (get_ds ());
	err = execve(ea->path, ea->argv, ea->envp);
	set_fs (fs);
	//do_exit(0);
#endif

	if(err) {
		printk(KERN_ERR"%s: exec %s returned: %d\n", __FUNCTION__, ea->path, err);
	}

	return err;
}
#endif /* __x86_64__ */
#endif /* !FOUND_KERNEL_EXECVE */

__shimcall__
int
OsForkWait(char *program_path, char *argv[], char *envp[])
{
	int err;
   
	//OsDebugPrintf("%s: %s\n", __FUNCTION__, program_path);

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) )
	err = call_usermodehelper(program_path, argv, envp, 1);
#else
	{
		OsExecArgs *ea = OsAllocate(sizeof(*ea));
		if(!ea)
			return -ENOMEM;

		ea->path = program_path;
		ea->argv = argv;
		ea->envp = envp;

		err = OsRunThreadSync(OsExec, ea);
		OsFree(ea);
	}
#endif

	return err;
}
/********************************************************************/

#define TICKS_TO_MSECS(ticks) (ticks*(1000/HZ))

static inline long MSECS_TO_TICKS(UINT32 msecs)
{
    long ticks = (msecs)/(1000/HZ);

    // round up to next tick
    if ((!ticks && msecs) || (msecs % (1000/HZ)) >= ((1000/HZ) / 2))
	ticks++;

    return ticks;
}

typedef struct TIME_OUT_INSTANCE_TYPE {
    OSSCHED				TaskQueue;
    struct timer_list			Timer;
    UINT32				mSec;
    UINT32				bLocked;
    PFREE_FUNC				pFuncFree;
    PVOID				pRefData;
    PCBFUNC				pTimeOutCallBack;
} TIME_OUT_INSTANCE_T, *PTIME_OUT_INSTANCE_T;

// bit number for delete syncronization
#define TASK_DELETE		1
// bit number indicating that task has been queued
#define TASK_QUEUED		2

POSTHRD OsMdmThread;

static void TimerThreadFunction(void* pData)
{
    PTIME_OUT_INSTANCE_T	pTimeOutInstance = (PTIME_OUT_INSTANCE_T)pData;

    if(test_bit(TASK_DELETE, (void*)&pTimeOutInstance->bLocked)) {
	clear_bit(TASK_QUEUED, (void*)(&pTimeOutInstance->bLocked));
	OsThreadScheduleDone();
	return;
    }

    /* beware: callback functions might use FPU instructions */
    pTimeOutInstance->pTimeOutCallBack(pTimeOutInstance->pRefData);

    clear_bit(TASK_QUEUED, (void*)(&pTimeOutInstance->bLocked));

    /* callback might have set TASK_DELETE */
    if(!test_bit(TASK_DELETE, (void*)&pTimeOutInstance->bLocked))
	mod_timer(&(pTimeOutInstance -> Timer), jiffies + MSECS_TO_TICKS(pTimeOutInstance->mSec));

	OsThreadScheduleDone();
}

static void TimeOutHandler(unsigned long Data)
{
    PTIME_OUT_INSTANCE_T	pTimeOutInstance = (PTIME_OUT_INSTANCE_T)Data;

    if(test_bit(TASK_DELETE, (void*)&pTimeOutInstance->bLocked)) {
	return;
    }

    if(!test_and_set_bit(TASK_QUEUED, (void*)&pTimeOutInstance->bLocked)) {
	if (OsThreadSchedule(OsMdmThread, &pTimeOutInstance->TaskQueue) <= 0) {
	    clear_bit(TASK_QUEUED, (void*)(&pTimeOutInstance->bLocked));
	}
    }
}

static INT32 ntimers;

__shimcall__
HOSTIMER OsCreatePeriodicTimer(IN UINT32 InitialTimeOut, IN PCBFUNC pTimeOutCallBack,
	IN PALLOC_FUNC pFuncAlloc, IN PFREE_FUNC pFuncFree,
	IN PVOID pRefData, OUT HTHREAD* pThreadId)
{
    PTIME_OUT_INSTANCE_T pTimeOutInstance;
    int pid;

    pTimeOutInstance = pFuncAlloc ? pFuncAlloc(sizeof(TIME_OUT_INSTANCE_T), pRefData) : OsAllocate(sizeof(TIME_OUT_INSTANCE_T));
    if(0 == pTimeOutInstance) {
	TRACE(T__ERROR, ("OsCreatePeriodicTimer ERROR, pTimeOutInstance is NULL???\n"));
	return 0;
    }

	OsThreadScheduleInit(&pTimeOutInstance->TaskQueue, TimerThreadFunction, pTimeOutInstance);

    pTimeOutInstance->pFuncFree = pFuncFree;
    pTimeOutInstance->pRefData = pRefData;
    pTimeOutInstance->pTimeOutCallBack = pTimeOutCallBack;
    pTimeOutInstance->bLocked = 0;
    pTimeOutInstance->mSec = InitialTimeOut;

    init_timer(&pTimeOutInstance->Timer);

    pTimeOutInstance->Timer.function = TimeOutHandler;
    pTimeOutInstance->Timer.data = (unsigned long)pTimeOutInstance;
    pTimeOutInstance->Timer.expires = jiffies;

    if(OsAtomicIncrement(&ntimers) == 1) {
#ifdef COMPLETION_INITIALIZER
	static struct completion startup = COMPLETION_INITIALIZER(startup);
#endif

	OsModuleUseCountInc();
	OsMdmThread = OsThreadCreate("modem", TRUE, &pid);
	if(!OsMdmThread || pid <= 0) {
		if(OsMdmThread) {
			OsThreadDestroy(OsMdmThread);
			OsMdmThread = NULL;
		}
	    OsModuleUseCountDec();
	    OsAtomicDecrement(&ntimers);
	    if(pTimeOutInstance->pFuncFree)
		pTimeOutInstance->pFuncFree(pTimeOutInstance, pTimeOutInstance->pRefData);
	    else
		OsFree(pTimeOutInstance);
	    return NULL;
	}
    }

    if(InitialTimeOut) {
	add_timer(&(pTimeOutInstance->Timer));
    }

    if(pThreadId) {
	*pThreadId = (HTHREAD)(unsigned long)(OsMdmThread->pid);
    }

    return (HOSTIMER)pTimeOutInstance;
}

__shimcall__
void OsDestroyPeriodicTimer(IN HOSTIMER hTimeOut)
{
    PTIME_OUT_INSTANCE_T	pTimeOutInstance = (PTIME_OUT_INSTANCE_T)hTimeOut;
    int i;

    if(pTimeOutInstance == 0) {
	TRACE(T__ERROR, ("\nOSLIN ERROR: OsDestroyPeriodicTimer: pTimeOutInstance == NULL???\n"));
	return;
    }

    set_bit(TASK_DELETE, (void*)(&pTimeOutInstance->bLocked));

    del_timer_sync(&pTimeOutInstance->Timer);

    for(i=0; (i < 30) && test_bit(TASK_QUEUED, (void*)&pTimeOutInstance->bLocked); i++) {
	if(!OsMdmThread || (OsMdmThread->pid <= 0)) {
	    printk(KERN_WARNING "%s: modem thread died, canceling queued task(s)\n", __FUNCTION__);
	    clear_bit(TASK_QUEUED, (void*)(&pTimeOutInstance->bLocked));
	    OsModuleUseCountDec();
	    break;
	}
	schedule();
    }

    if(i == 30) {
	printk(KERN_WARNING "%s: task queue stuck?!\n", __FUNCTION__);
    }

    if(!OsMdmThread || (OsMdmThread->pid <= 0)) {
	printk(KERN_WARNING "%s: modem thread exited prematurely\n", __FUNCTION__);
    }

    if(OsAtomicDecrement(&ntimers) == 0) {
	if(OsMdmThread) {
	    OsThreadDestroy(OsMdmThread);
	    OsMdmThread = NULL;
	    OsModuleUseCountDec();
	}
    }

    if(pTimeOutInstance->pFuncFree)
	pTimeOutInstance->pFuncFree(pTimeOutInstance, pTimeOutInstance->pRefData);
    else
	OsFree(pTimeOutInstance);
}

__shimcall__
BOOL OsSetPeriodicTimer(IN HOSTIMER hTimeOut, IN UINT32 NewTimeOut)
{
    PTIME_OUT_INSTANCE_T	pTimeOutInstance = (PTIME_OUT_INSTANCE_T)hTimeOut;

    if(pTimeOutInstance == 0) {
	TRACE(T__ERROR, ("\nERROR!!! OsSetPeriodicTimer: pTimeOutInstance == NULL\n"));
	return FALSE;
    }

    if(NewTimeOut == pTimeOutInstance->mSec) {
	/* no change, don't touch it */
	return TRUE;
    }
    pTimeOutInstance->mSec = NewTimeOut;
    if(NewTimeOut) {
	clear_bit(TASK_DELETE, (void*)(&pTimeOutInstance->bLocked));
	mod_timer(&pTimeOutInstance -> Timer, jiffies + MSECS_TO_TICKS(pTimeOutInstance -> mSec));
	return TRUE;
    } else {
	set_bit(TASK_DELETE, (void*)(&pTimeOutInstance->bLocked));
	del_timer(&pTimeOutInstance -> Timer);
	return FALSE;
    }
}

__shimcall__
void OsImmediateTimeOut(IN HOSTIMER hTimeOut)
{
    PTIME_OUT_INSTANCE_T    pTimeOutInstance = (PTIME_OUT_INSTANCE_T)hTimeOut;

    mod_timer(&pTimeOutInstance -> Timer, jiffies);
}

static time_t epoch = 0;

__shimcall__
UINT32 OsGetSystemTime(void)
{
    struct timeval		timestamp;

    do_gettimeofday(&timestamp);

    return ((timestamp.tv_sec-epoch)*1000 + timestamp.tv_usec/1000);
}

__shimcall__
void OsSleep(UINT32 ms)
{
    /* we must schedule() for sleeps >= 10ms since HCF calls us in loops
     * waiting for asynchronous events.
     */
    if(ms >= TICKS_TO_MSECS(1)) {
	UINT32 start, end;
	long timeout = MSECS_TO_TICKS(ms);
	//long ort = timeout;

	if(!OsContextAllowsSleeping()) {
	    if(ms >= 100) {
	    	printk(KERN_ERR"%s(%u): shouldn't sleep that long in this context!\n", __FUNCTION__, ms);
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20) )
		{static int onlyonce; if(!onlyonce) { onlyonce=1; dump_stack(); } }
#endif
	    }
//	    ASSERT(OsContextAllowsSleeping());
	    mdelay(ms);
	    return;
	}

	start = OsGetSystemTime();
	do {
	    set_current_state(TASK_UNINTERRUPTIBLE);
	} while ((timeout = schedule_timeout(timeout)) > 0);
	end = OsGetSystemTime() - start;
	if(end < ms) {
	    //printk(KERN_ERR"OsSleep: short timeout; wanted %ld got %ldms ort=%ld\n", ms, end, ort);
	    OsSleep(ms - end);
	}
	//else if(end > ms) {
	//	printk(KERN_ERR"OsSleep: timeout %ldms (%ldms too much) ort=%ld\n", ms, end - ms, ort);
	//}
    } else {
	// short delay
	mdelay(ms);
    }
}

#if ! TARGET_HCF_FAMILY

typedef struct TIMER_TAG
{
    UINT32 msec;
    struct timer_list timer;
} TIMER_T, *PTIMER_T;

__shimcall__
HANDLE OsCreateTimer(UINT32 msec, __kernelcall__ PVOID pCBFunc, PVOID pRefData)
{
    PTIMER_T pTimer;

    if ( pCBFunc == NULL )
    {
	ASSERT(pCBFunc);
	return (NULL);
    }

    ASSERT(OsContextAllowsSleeping());
    pTimer = kmalloc(sizeof(TIMER_T),GFP_KERNEL);
    if ( NULL == pTimer )
    {
	ASSERT(pTimer);
	return (NULL);
    }

    memset(pTimer,0,sizeof(TIMER_T));

    init_timer(&(pTimer->timer));
    pTimer->timer.function = pCBFunc ;
    pTimer->timer.data = (unsigned long)pRefData;
    pTimer->timer.expires = jiffies + MSECS_TO_TICKS(msec);
    pTimer->msec = msec;
    return ((HANDLE)pTimer);
}

__shimcall__
void OsSetTimer(PVOID pTimer)
{
    struct timer_list*	pTimerH;
    PTIMER_T pTimerS = (PTIMER_T)pTimer;
    if ( pTimer == NULL )
    {
	ASSERT(pTimer);
	return;
    }
    pTimerH = &(pTimerS->timer);
    mod_timer(pTimerH, jiffies + MSECS_TO_TICKS(pTimerS->msec));
}

__shimcall__
void OsCancelTimer(PVOID pTimer)
{
    struct timer_list*	pTimerH;
    if ( pTimer == NULL )
    {
	ASSERT(pTimer);
	return;
    }
    pTimerH = &((PTIMER_T)pTimer)->timer;

    del_timer(pTimerH);
}

__shimcall__
void OsChangeTimerTimeOut(PVOID pTimer, UINT32 msec)
{
    PTIMER_T pTimerS = (PTIMER_T)pTimer;
    if ( pTimer == NULL )
    {
	ASSERT(pTimer);
	return;
    }
    pTimerS->msec = msec;
}

__shimcall__
void OsDestroyTimer(PVOID pTimer)
{
    if ( pTimer == NULL )
    {
	ASSERT(pTimer);
	return;
    }

    del_timer_sync(&((PTIMER_T)pTimer)->timer);
    kfree(pTimer);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
#define PROCFREQ_FROM_KERNEL
#endif

//#define PROCFREQ_FROM_KERNEL_CONSTANT

#if !defined(PROCFREQ_FROM_KERNEL) || defined(PROCFREQ_FROM_KERNEL_CONSTANT)
static DWORD dwProcessorFreq;

__shimcall__
GLOBAL DWORD  OsGetProcessorFreq(void)
{
    return dwProcessorFreq;
}
#endif

#ifndef PROCFREQ_FROM_KERNEL
static unsigned long OsCalcCpuRate(void)
{
    struct timeval tv;
    unsigned long flags;
    unsigned long time1, time2, cpurate;
    unsigned int target_usec;

    // first ensure that the tv_usec will not wraparound
    do_gettimeofday( &tv );

    if ( tv.tv_usec > 990000 ) {
	do
	{
	    do_gettimeofday( &tv );
	} while ( tv.tv_usec > 990000 );
    }

    local_irq_save(flags);
    // calc the cpu rate 
    do_gettimeofday( &tv );
    rdtscl(time1);
    target_usec = tv.tv_usec + 1000;
    do
    {
	do_gettimeofday( &tv );
    } while ( tv.tv_usec < target_usec );
    rdtscl(time2);

    local_irq_restore(flags);

    cpurate = ( (time2-time1+500) /1000 ) * 1000000; // to get whole number 

#ifdef DEBUG
    printk(KERN_DEBUG"%s: %lu\n", __FUNCTION__, cpurate);
#endif

    return cpurate;
}
#else
__shimcall__
GLOBAL DWORD  OsGetProcessorFreq(void)
{
    return cpu_khz / 1000;
}
#endif

__shimcall__
UINT32 OsReadCpuCnt(void)
{
	UINT32 tsc;

	rdtscl(tsc);

	return tsc;
}
#endif

__shimcall__
int OsInit(void)
{
    struct timeval timestamp;
    do_gettimeofday(&timestamp);

    epoch = timestamp.tv_sec;

#if ! TARGET_HCF_FAMILY
#ifdef PROCFREQ_FROM_KERNEL_CONSTANT
	dwProcessorFreq =  cpu_khz / 1000;
#elif !defined(PROCFREQ_FROM_KERNEL)
    dwProcessorFreq = OsCalcCpuRate()/1000000;
#endif
#endif

#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0) )
    if(sizeof(OSSCHED) <= sizeof(struct kwork_data)) {
        OsErrorPrintf("OSSCHED too small (%d < %d)\n", sizeof(OSSCHED), sizeof(struct kwork_data));
#else
    if(sizeof(OSSCHED) <= sizeof(struct tq_struct)) {
        OsErrorPrintf("OSSCHED too small (%d < %d)\n", sizeof(OSSCHED), sizeof(struct tq_struct));
#endif
        return -ENOSPC;
    }

    return 0;
}

/********************************************************************/

__shimcall__
HTHREAD OsGetCurrentThread(void)
{
    return (HTHREAD)((in_interrupt()) ? 0 : (unsigned long)current->pid);
}

/********************************************************************/


__shimcall__
void    OsRawVPrintf       (LPCSTR szFormat, PVOID args, BOOL addnl)
{
    char printk_buf[1024];

    printk_buf[0]= '\0';
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,4,10) )
    vsprintf(printk_buf, szFormat, args);
#else
    vsnprintf(printk_buf, sizeof(printk_buf), szFormat, args);
#endif

    printk_buf[sizeof(printk_buf)-1]= '\0';

    printk(addnl ? "%s\n" : "%s", printk_buf);
}

__shimcall__
void    OsRawPrintf       (LPCSTR szFormat, ...)
{
    va_list args;

    va_start(args, szFormat);
    OsRawVPrintf(szFormat, args, FALSE);
    va_end(args);
}

__shimcall__
void    OsRawNLPrintf       (LPCSTR szFormat, ...)
{
    va_list args;

    va_start(args, szFormat);
    OsRawVPrintf(szFormat, args, TRUE);
    va_end(args);
}

__shimcall__
void    OsErrorVPrintf       (LPCSTR szFormat, PVOID args)
{
    char printk_buf[1024];

    printk_buf[0]= '\0';
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,4,10) )
    vsprintf(printk_buf, szFormat, args);
#else
    vsnprintf(printk_buf, sizeof(printk_buf), szFormat, args);
#endif

    printk_buf[sizeof(printk_buf)-1]= '\0';

#if 0
    {
	static int lasthadnl=1;
	if(lasthadnl)
	    printk(KERN_ERR"%s", printk_buf);
	else
	    printk("%s", printk_buf);

	lasthadnl = printk_buf[strlen(printk_buf) - 1] == '\n';
    }
#else
    printk(KERN_ERR"%s", printk_buf);
#endif
}

__shimcall__
void    OsErrorPrintf       (LPCSTR szFormat, ...)
{
    va_list args;

    va_start(args, szFormat);
    OsErrorVPrintf(szFormat, args);
    va_end(args);
}

__shimcall__
void    OsDebugVPrintf       (LPCSTR szFormat, PVOID args)
{
    char printk_buf[1024];

    printk_buf[0]= '\0';
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,4,10) )
    vsprintf(printk_buf, szFormat, args);
#else
    vsnprintf(printk_buf, sizeof(printk_buf), szFormat, args);
#endif

    printk_buf[sizeof(printk_buf)-1]= '\0';

    printk(KERN_DEBUG"%s", printk_buf);
}

__shimcall__
void    OsDebugPrintf       (LPCSTR szFormat, ...)
{
    va_list args;

    va_start(args, szFormat);
    OsDebugVPrintf(szFormat, args);
    va_end(args);
}

__shimcall__
void    OsDebugBreakpoint   ( LPCSTR szMsg )
{
    OsDebugPrintf("%s: %s\n", __FUNCTION__, szMsg);
    //KDB_ENTER();
}

#if 0
void OsDebugDumpData(PUINT8 data, UINT32 len)
{
    OsDebugPrintf("%s: data=%p len=%ld\n", __FUNCTION__, data, len);
}
#endif


typedef struct {
	wait_queue_head_t wq;
	int flag;
	const char *name;
} EVENT, *PEVENT;

__shimcall__
void OsEventInit(HOSEVENT hEvent, const char* name)
{
	PEVENT pEvent = (PEVENT)hEvent;

	//OsDebugPrintf("%s: %p\n", __FUNCTION__, pEvent);

	if(pEvent) {
		init_waitqueue_head(&pEvent->wq);
		pEvent->flag = 0;
		pEvent->name = name;
	}
}

__shimcall__
HOSEVENT OsEventCreate(const char* name)
{
	HOSEVENT hEvent = OsAllocate(sizeof(EVENT));
   
	OsEventInit(hEvent, name);

	return hEvent;
}

__shimcall__
void OsEventDestroy(HOSEVENT hEvent)
{
	OsFree(hEvent);
}

#ifndef wq_write_lock_irqsave
#define wq_write_lock_irqsave spin_lock_irqsave
#endif
#ifndef wq_write_unlock_irqrestore
#define wq_write_unlock_irqrestore spin_unlock_irqrestore
#endif

__shimcall__
void OsEventWait(HOSEVENT hEvent)
{
	PEVENT pEvent = (PEVENT)hEvent;
	DECLARE_WAITQUEUE(wait, current);
	unsigned long flags;

	//OsDebugPrintf("%s: %p\n", __FUNCTION__, hEvent);

	if(!OsContextAllowsSleeping()) {
	    	printk(KERN_ERR"%s: can't sleep in this context!\n", __FUNCTION__);
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20) )
		{static int onlyonce; if(!onlyonce) { onlyonce=1; dump_stack(); } }
#endif
		return;
	}

	//OsDebugPrintf("bef AWQ\n");
	add_wait_queue(&pEvent->wq, &wait);
	while(1) {
		wq_write_lock_irqsave(&pEvent->wq.lock, flags);
		if(pEvent->flag) {
			pEvent->flag = 0;
			//OsDebugPrintf("ret OK\n");
			wq_write_unlock_irqrestore(&pEvent->wq.lock, flags);
			break;
		}
		wq_write_unlock_irqrestore(&pEvent->wq.lock, flags);
		rmb();
		//OsDebugPrintf("UNINT");
		set_current_state(TASK_UNINTERRUPTIBLE);
		//OsDebugPrintf("ST");
		schedule();
	}

	current->state = TASK_RUNNING;
	//OsDebugPrintf("TRUN");
	remove_wait_queue(&pEvent->wq, &wait);
	//OsDebugPrintf("aft RWQ");
}

// sets event to signalled state
__shimcall__
long OsEventSet(HOSEVENT hEvent)
{
	PEVENT pEvent = (PEVENT)hEvent;
	unsigned long flags;
	int previous_state;

	//OsDebugPrintf("%s: %p\n", __FUNCTION__, hEvent);
	mb();
	wq_write_lock_irqsave(&pEvent->wq.lock, flags);
	previous_state = pEvent->flag;
	pEvent->flag = 1;
	wq_write_unlock_irqrestore(&pEvent->wq.lock, flags);
	wmb();
	//OsDebugPrintf("BWU\n");
	wake_up(&pEvent->wq);
	//OsDebugPrintf("AWU\n");

	return previous_state;
}

// waits until event is set by OsEventSet() call or timeout (ms) has passed
__shimcall__
OSEVENT_WAIT_RESULT OsEventWaitTime(HOSEVENT hEvent, UINT32 timeout)
{
	PEVENT pEvent = (PEVENT)hEvent;
	DECLARE_WAITQUEUE(wait, current);
	unsigned long flags;
	long tmticks = MSECS_TO_TICKS(timeout);
	OSEVENT_WAIT_RESULT res = OSEVENT_WAIT_TIMEOUT;

	//OsDebugPrintf("%s: %p timeout=%ld\n", __FUNCTION__, hEvent, timeout);

	if(!timeout) {
		wq_write_lock_irqsave(&pEvent->wq.lock, flags);
		if(pEvent->flag) {
			pEvent->flag = 0;
			res = OSEVENT_WAIT_OK;
		}
		wq_write_unlock_irqrestore(&pEvent->wq.lock, flags);
		return res;
	}

	if(!OsContextAllowsSleeping()) {
	    	printk(KERN_ERR"%s: can't sleep in this context!\n", __FUNCTION__);
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20) )
		{static int onlyonce; if(!onlyonce) { onlyonce=1; dump_stack(); } }
#endif
		return OSEVENT_WAIT_ERROR;
	}

	//OsDebugPrintf("bef AWQ\n");
	add_wait_queue(&pEvent->wq, &wait);
	do {
		wq_write_lock_irqsave(&pEvent->wq.lock, flags);
		if(pEvent->flag) {
			pEvent->flag = 0;
			//OsDebugPrintf("ret OK\n");
			wq_write_unlock_irqrestore(&pEvent->wq.lock, flags);
			res = OSEVENT_WAIT_OK;
			break;
		}
		wq_write_unlock_irqrestore(&pEvent->wq.lock, flags);
		rmb();
		//OsDebugPrintf("UNINT");
		set_current_state(TASK_UNINTERRUPTIBLE);
		//OsDebugPrintf("ST");
	} while ( tmticks && (tmticks = schedule_timeout(tmticks)) );

	current->state = TASK_RUNNING;
	//OsDebugPrintf("TRUN");
	remove_wait_queue(&pEvent->wq, &wait);
	//OsDebugPrintf("aft RWQ");

#if 0
	if(res == OSEVENT_WAIT_TIMEOUT) {
		printk(KERN_DEBUG"%s(%p/%s, %u): returning OSEVENT_WAIT_TIMEOUT\n", __FUNCTION__, hEvent, pEvent->name, timeout);
		//{static int onlyonce; if(!onlyonce) { onlyonce=1; dump_stack(); } }
	}
#endif
	return res;
}

// sets event to non-signalled state
__shimcall__
long OsEventClear(HOSEVENT hEvent)
{
	PEVENT pEvent = (PEVENT)hEvent;
	unsigned long flags;
	int previous_state;

	//OsDebugPrintf("%s: %p\n", __FUNCTION__, hEvent);

	wq_write_lock_irqsave(&pEvent->wq.lock, flags);
	previous_state = pEvent->flag;
	pEvent->flag = 0;
	wq_write_unlock_irqrestore(&pEvent->wq.lock, flags);
	wmb();

	return previous_state;
}

__shimcall__
long OsEventState(HOSEVENT hEvent)
{
	PEVENT pEvent = (PEVENT)hEvent;
	unsigned long flags;
	int state;

	//OsDebugPrintf("%s: %p\n", __FUNCTION__, hEvent);

	wq_write_lock_irqsave(&pEvent->wq.lock, flags);
	state = pEvent->flag;
	wq_write_unlock_irqrestore(&pEvent->wq.lock, flags);

	return state;
}

#ifdef TARGET_HCF_PCI_LINUX
typedef struct tasklet_struct OSDPC, *POSDPC;

__shimcall__
HOSDPC OsDpcCreate(__kernelcall__ void (*func)(void *), void * data)
{
    POSDPC pDpc = (POSDPC) kmalloc(sizeof(OSDPC), GFP_KERNEL);

    if(pDpc) {
	tasklet_init(pDpc, (void (*)(unsigned long))func, (unsigned long)data);
    }

    //	DPRINTF(("%s: %p", __FUNCTION__, pDpc));
    return (HOSDPC)pDpc;
}

__shimcall__
void OsDpcDestroy(HOSDPC hDpc)
{
    POSDPC pDpc = (POSDPC)hDpc;

    tasklet_disable(pDpc);

    //	DPRINTF(("%s: %p", __FUNCTION__, hDpc));
    kfree(pDpc);
}

__shimcall__
void OsDpcSchedule(HOSDPC hDpc)
{
    tasklet_schedule((POSDPC)hDpc);
}

__shimcall__
void OsDpcEnable(HOSDPC hDpc)
{
    tasklet_enable((POSDPC)hDpc);
}

__shimcall__
void OsDpcDisable(HOSDPC hDpc)
{
    tasklet_disable((POSDPC)hDpc);
}
#endif /* TARGET_HCF_PCI_LINUX */

__kernelcall__
#if (__GNUC__ == 3 && __GNUC_MINOR__ > 1) || __GNUC__ > 3
__attribute__((used))
#endif
static void regParmUsed(BOOL *regParm)
{
#if defined(__i386__)
	register void **esp asm("esp") = esp;

	*regParm = ((void *)&regParm == esp);
#else
	*regParm = FALSE;
#endif
}

__shimcall__
BOOL
OsKernelUsesRegParm(void)
{
	BOOL regParm;

	regParmUsed(&regParm);

	return regParm;
}

