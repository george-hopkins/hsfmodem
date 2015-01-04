/*
 *  oscompat.h: Compatibility defines to handle various Linux versions
 */

/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * 
 * 1.  General Public License. This program is free software, and may
 * be redistributed or modified subject to the terms of the GNU General
 * Public License (version 2) or the GNU Lesser General Public License,
 * or (at your option) any later versions ("Open Source" code). You may
 * obtain a copy of the GNU General Public License at
 * http://www.fsf.org/copyleft/gpl.html and a copy of the GNU Lesser
 * General Public License at http://www.fsf.org/copyleft/less.html,
 * or you may alternatively write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA.
 * 
 * 2.   Disclaimer of Warranties. LINUXANT AND OTHER CONTRIBUTORS MAKE NO
 * REPRESENTATION ABOUT THE SUITABILITY OF THIS SOFTWARE FOR ANY PURPOSE.
 * IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTIES OF ANY KIND.
 * LINUXANT AND OTHER CONTRIBUTORS DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, GOOD TITLE AND AGAINST INFRINGEMENT.
 * 
 * This software has not been formally tested, and there is no guarantee that
 * it is free of errors including, but not limited to, bugs, defects,
 * interrupted operation, or unexpected results. Any use of this software is
 * at user's own risk.
 * 
 * 3.   No Liability.
 * 
 * (a) Linuxant or contributors shall not be responsible for any loss or
 * damage to user, or any third parties for any reason whatsoever, and
 * LINUXANT OR CONTRIBUTORS SHALL NOT BE LIABLE FOR ANY ACTUAL, DIRECT,
 * INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL, OR CONSEQUENTIAL
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED, WHETHER IN CONTRACT, STRICT OR OTHER LEGAL THEORY OF
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 * (b) User agrees to hold Linuxant and contributors harmless from any
 * liability, loss, cost, damage or expense, including attorney's fees,
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
 */
#ifndef __OSCOMPAT_H
#define __OSCOMPAT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "osuniqredef.h"

#include <linux/version.h>
#ifdef FOUND_LINUX_CONFIG
#include <linux/config.h>
#endif
#include <linux/types.h>

#if defined(STATIC_ERRNO)
#ifndef _LINUX_UNISTD_H_
#define _LINUX_UNISTD_H_
#endif
static int errno;
#include <asm/unistd.h>
#endif

#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/kdev_t.h>

#ifdef FOUND_LINUX_BYTEORDER_SWAB
#include <linux/byteorder/swab.h>
#else
#include <linux/swab.h>
#endif

#include <linux/proc_fs.h>

#ifdef FOUND_LINUX_SEMAPHORE
#include <linux/semaphore.h>
#endif

#ifdef FOUND_MODULE_PARAM
#include <linux/moduleparam.h>
#endif

#if LINUX_VERSION_CODE == KERNEL_VERSION(2,4,9)
/* get rid of non-standard min/max macros */
#undef min
#undef max
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
#include <linux/tqueue.h>
#else

#include <linux/spinlock.h>
#include <linux/list.h>
#include <asm/bitops.h>
#include <asm/system.h>

/*
 * New proposed "bottom half" handlers:
 * (C) 1994 Kai Petzke, wpp@marie.physik.tu-berlin.de
 *
 * Advantages:
 * - Bottom halfs are implemented as a linked list.  You can have as many
 *   of them, as you want.
 * - No more scanning of a bit field is required upon call of a bottom half.
 * - Support for chained bottom half lists.  The run_task_queue() function can be
 *   used as a bottom half handler.  This is for example useful for bottom
 *   halfs, which want to be delayed until the next clock tick.
 *
 * Notes:
 * - Bottom halfs are called in the reverse order that they were linked into
 *   the list.
 */

struct tq_struct {
	struct list_head list;		/* linked list of active bh's */
	unsigned long sync;		/* must be initialized to zero */
	void (*routine)(void *);	/* function to call */
	void *data;			/* argument to function */
};

/*
 * Emit code to initialise a tq_struct's routine and data pointers
 */
#define PREPARE_TQUEUE(_tq, _routine, _data)			\
	do {							\
		(_tq)->routine = _routine;			\
		(_tq)->data = _data;				\
	} while (0)

/*
 * Emit code to initialise all of a tq_struct
 */
#define INIT_TQUEUE(_tq, _routine, _data)			\
	do {							\
		INIT_LIST_HEAD(&(_tq)->list);			\
		(_tq)->sync = 0;				\
		PREPARE_TQUEUE((_tq), (_routine), (_data));	\
	} while (0)

typedef struct list_head task_queue;

#define DECLARE_TASK_QUEUE(q)	LIST_HEAD(q)
#define TQ_ACTIVE(q)		(!list_empty(&q))

extern task_queue tq_timer, tq_immediate, tq_disk;

/*
 * To implement your own list of active bottom halfs, use the following
 * two definitions:
 *
 * DECLARE_TASK_QUEUE(my_tqueue);
 * struct tq_struct my_task = {
 * 	routine: (void (*)(void *)) my_routine,
 *	data: &my_data
 * };
 *
 * To activate a bottom half on a list, use:
 *
 *	queue_task(&my_task, &my_tqueue);
 *
 * To later run the queued tasks use
 *
 *	run_task_queue(&my_tqueue);
 *
 * This allows you to do deferred processing.  For example, you could
 * have a task queue called tq_timer, which is executed within the timer
 * interrupt.
 */

#ifdef STATIC_TQUEUE_LOCK
static spinlock_t tqueue_lock __attribute__((unused)) = SPIN_LOCK_UNLOCKED;
#else
extern spinlock_t tqueue_lock;
#endif

/*
 * Queue a task on a tq.  Return non-zero if it was successfully
 * added.
 */
static inline int queue_task(struct tq_struct *bh_pointer, task_queue *bh_list)
{
	int ret = 0;
	if (!test_and_set_bit(0,&bh_pointer->sync)) {
		unsigned long flags;
		spin_lock_irqsave(&tqueue_lock, flags);
		list_add_tail(&bh_pointer->list, bh_list);
		spin_unlock_irqrestore(&tqueue_lock, flags);
		ret = 1;
	}
	return ret;
}

/*
 * Call all "bottom halfs" on a given list.
 */

static inline void run_task_queue(task_queue *list)
{
    if (TQ_ACTIVE(*list)) {
	struct list_head head, *next;
	unsigned long flags;

	spin_lock_irqsave(&tqueue_lock, flags);
	list_add(&head, list);
	list_del_init(list);
	spin_unlock_irqrestore(&tqueue_lock, flags);

	next = head.next;
	while (next != &head) {
	    void (*f) (void *);
	    struct tq_struct *p;
	    void *data;

	    p = list_entry(next, struct tq_struct, list);
	    next = next->next;
	    f = p->routine;
	    data = p->data;
	    wmb();
	    p->sync = 0;
	    if (f)
		f(data);
	}
    }
}


#define devfs_register_chrdev register_chrdev

#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) && !defined(FOUND_DEVFS)
#define devfs_register_chrdev register_chrdev
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,2)
#include <linux/circ_buf.h>
#else

struct circ_buf {
	char *buf;
	int head;
	int tail;
};

/* Return count in buffer.  */
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

/* Return space available, 0..size-1.  We always leave one free char
   as a completely full buffer has head == tail, which is the same as
   empty.  */
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

/* Return count up to the end of the buffer.  Carefully avoid
   accessing head and tail more than once, so they can change
   underneath us without returning inconsistent results.  */
#define CIRC_CNT_TO_END(head,tail,size) \
	({int end = (size) - (tail); \
	  int n = ((head) + end) & ((size)-1); \
	  n < end ? n : end;})

/* Return space available up to the end of the buffer.  */
#define CIRC_SPACE_TO_END(head,tail,size) \
	({int end = (size) - 1 - (head); \
	  int n = (end + (tail)) & ((size)-1); \
	  n <= end ? n : end+1;})

#endif

#ifdef CONFIG_KDB
#define KDB_ENTER() asm("\tint $129\n")
#else
#define KDB_ENTER() do {} while(0)
#endif

#ifndef minor
#define minor(d) MINOR(d)
#endif

#ifndef unlikely
#define unlikely(x) (x)
#endif

#ifdef MODULE
#ifndef THIS_MODULE
#define THIS_MODULE (&__this_module)
#endif
#ifndef module_init
#define module_init(x)  int init_module(void) { return x(); }
#endif
#ifndef module_exit
#define module_exit(x)  void cleanup_module(void) { x(); }
#endif
#endif

#ifndef MODULE_LICENSE
#define MODULE_LICENSE(x)
#endif

#ifndef MODULE_INFO
#define MODULE_INFO(x,y)
#endif

#ifndef EXPORT_SYMBOL_NOVERS
#define EXPORT_SYMBOL_NOVERS(x) EXPORT_SYMBOL(x)
#endif

#ifndef __exit
#define __exit
#endif

#ifndef __devexit_p
#define __devexit_p(x) x
#endif

#ifndef DECLARE_MUTEX
#define DECLARE_MUTEX(name)                 struct semaphore name=MUTEX
#define init_MUTEX(x)                               *(x)=MUTEX
#endif

#ifndef DECLARE_WAITQUEUE
#define DECLARE_WAITQUEUE(wait, current) struct wait_queue wait = { current, NULL }
#define DECLARE_WAIT_QUEUE_HEAD(wait)       wait_queue_head_t wait
typedef struct wait_queue *wait_queue_head_t;
#define init_waitqueue_head(x)          *(x)=NULL
#endif

#ifndef set_current_state
#define __set_current_state(state_value)	do { current->state = state_value; } while (0)
#ifdef __SMP__
#define set_current_state(state_value)		do { __set_current_state(state_value); mb(); } while (0)
#else
#define set_current_state(state_value)		__set_current_state(state_value)
#endif
#endif // set_current_state

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
#define get_zeroed_page get_free_page
#define try_inc_mod_count(mod) ((mod && !(mod->flags & MOD_DELETED)) ? __MOD_INC_USE_COUNT(mod), 1 : 0)
#endif // LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)

#ifndef PREPARE_TQUEUE
/*
 * Emit code to initialise a tq_struct's routine and data pointers
 */
#define PREPARE_TQUEUE(_tq, _routine, _data)			\
	do {							\
		(_tq)->routine = _routine;			\
		(_tq)->data = _data;				\
	} while (0)
#endif

#ifndef INIT_TQUEUE
/*
 * Emit code to initialise all of a tq_struct
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
#define INIT_TQUEUE(_tq, _routine, _data)			\
	do {							\
		INIT_LIST_HEAD(&(_tq)->list);				\
		(_tq)->sync = 0;				\
		PREPARE_TQUEUE((_tq), (_routine), (_data));	\
	} while (0)
#else
#define INIT_TQUEUE(_tq, _routine, _data)			\
	do {							\
		(_tq)->next = NULL;				\
		(_tq)->sync = 0;				\
		PREPARE_TQUEUE((_tq), (_routine), (_data));	\
	} while (0)
#endif
#endif

#ifndef DECLARE_TASKLET
#define tasklet_struct tq_struct
#define tasklet_init(tlet, func, parm) INIT_TQUEUE(tlet, (void (*)(void *))(func), (void *)(parm))
#define tasklet_schedule(tlet) queue_task(tlet, &tq_immediate); mark_bh(IMMEDIATE_BH)
static inline void tasklet_kill(struct tasklet_struct *t)
{
	unsigned long flags;
	struct tq_struct **pp;

	spin_lock_irqsave(&tqueue_lock, flags);

	if(t->sync) {
		for(pp = &tq_immediate; *pp; pp = &(*pp)->next) {
			if(*pp == t) {
				*pp = t->next;
				t->sync = 0;
				break;
			}
		}
	}

	spin_unlock_irqrestore(&tqueue_lock, flags);
}
#endif

#if defined(EXPORT_SYMTAB)

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)) && defined(memcpy)
/* accomodate references to memcpy caused by gcc */
void * memcpy_for_gcc(void *dest, const void *src, __kernel_size_t n)
{
	return memcpy(dest, src, n);
}
#undef memcpy
void * memcpy(void *dest, const void *src, __kernel_size_t n) __attribute__ ((weak, alias ("memcpy_for_gcc")));
#endif
#endif // EXPORT_SYMTAB

static inline void OsModuleUseCountInc(void)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	MOD_INC_USE_COUNT;
#else
	(void)try_module_get(THIS_MODULE);
#endif
}

static inline void OsModuleUseCountDec(void)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	MOD_DEC_USE_COUNT;
#else
	module_put(THIS_MODULE);
#endif
}

#if defined(DEBUG_CHECK_STACK) && (defined(EXPORT_SYMTAB) || defined(CNXTSERIAL_INCLUDE_CORE))

/* inspired by Benjamin LaHaise's CONFIG_X86_STACK_CHECK patch */

static void __attribute__ ((unused, used))
_OSUNIQDEF(stack_overflow)(void *esp, void *eip)
{
	printk(KERN_ERR "%s: from %p.  esp: %p\n", __FUNCTION__, eip, esp);
	KDB_ENTER();
}

void mcount(void) __attribute__ ((alias (_OSUNIQDEF_STR(mcount))));

static int __attribute__ ((unused)) stack_danger_zone = 
		sizeof(struct task_struct) + 512;

asm(
"\n"
"THREAD_SIZE       = 8192\n" /* 2 * PAGE_SIZE */
"\n"
".data\n"
"\n"
"stack_overflowed:\n"
"	.long   0\n"
"\n"
".text\n"
"\n"
"	.align 16\n"
"\n"
"	.type    "_OSUNIQDEF_STR(mcount)",@function\n"
"	.globl "_OSUNIQDEF_STR(mcount)"\n"
"\n"
_OSUNIQDEF_STR(mcount)":\n"
"	push %eax\n"
"	movl $(THREAD_SIZE - 1),%eax\n"
"	andl %esp,%eax\n"
"	cmpl stack_danger_zone, %eax\n"
"	jle 1f\n"
"2:\n"
"	popl %eax\n"
"	ret\n"
"\n"
"1:\n"
"	movl	%esp,%eax\n"
"\n"
"	lock; btsl $0,stack_overflowed\n" /* prevent re-entry */
"	jc 2b\n"
"\n"
"	pushf\n"
"	cli\n"
"\n"
	/* push eip then esp of error for stack_overflow */
"	pushl	4(%eax)\n"
"	pushl	%eax\n"
"	call	"_OSUNIQDEF_STR(stack_overflow)"\n"
"	addl	$8,%esp\n"
"\n"
"	popf\n"
"\n"
"	popl	%eax\n"
"	movl    $0,stack_overflowed\n"
"	ret\n"
);

#warning stack check enabled

#endif

#ifndef irqs_disabled
#define irqs_disabled() \
({ \
 	unsigned long flags;\
	__asm__ __volatile__("pushfl ; popl %0":"=g" (flags): /* no input */); \
	!(flags & (1<<9)); \
})
#endif

static inline int OsContextAllowsSleeping(void)
{
    if(irqs_disabled())
        return 0;
    
    if(in_irq())
        return 0;
            
    if((in_interrupt())
#ifdef in_atomic
            || (in_atomic())
#endif  
            || (in_softirq())
      ) 
        return 0;
        
    return 1;
}

#if defined(CONFIG_ALL_PPC) && !defined(CONFIG_PPC_PMAC) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
#define CONFIG_PPC_PMAC
#endif

#ifdef __cplusplus
}
#endif

#ifdef FOUND_DEV_NAME
#define PCI_SLOT_NAME(x)  dev_name(&(x)->dev)
#else
#ifdef FOUND_PCI_DEV_SLOT_NAME
#define PCI_SLOT_NAME(x)  (x)->slot_name
#else
#define PCI_SLOT_NAME(x)  (x)->dev.bus_id
#endif
#endif

#ifdef FOUND_CLASS_SIMPLE
#define CLASS_DEVICE_CREATE(class, dev, device, fmt, rest...) class_simple_device_add(class, dev, device, fmt, ## rest)
#define CLASS_DESTROY(class) class_simple_destroy(class)
#define CLASS_DEVICE_DESTROY(class, dev) class_simple_device_remove(dev)
#define CLASS_CREATE(owner, name) class_simple_create(owner, name)
#else
#ifdef FOUND_CLASS_DEVICE
#ifdef FOUND_CLASS_DEVICE_PARENT
#define CLASS_DEVICE_CREATE(class, dev, device, fmt, rest...) class_device_create(class, NULL, dev, device, fmt, ## rest)
#else
#define CLASS_DEVICE_CREATE(class, dev, device, fmt, rest...) class_device_create(class, dev, device, fmt, ## rest)
#endif
#define CLASS_DEVICE_DESTROY(class, dev) class_device_destroy(class, dev)
#else
#ifdef FOUND_DEVICE_CREATE_DRVDATA
#define CLASS_DEVICE_CREATE(class, dev, device, fmt, rest...) device_create_drvdata(class, device, dev, NULL, fmt, ## rest)
#else
#ifdef FOUND_DEVICE_CREATE_WITH_DRVDATA
#define CLASS_DEVICE_CREATE(class, dev, device, fmt, rest...) device_create(class, device, dev, NULL, fmt, ## rest)
#else
#define CLASS_DEVICE_CREATE(class, dev, device, fmt, rest...) device_create(class, device, dev, fmt, ## rest)
#endif
#endif
#define CLASS_DEVICE_DESTROY(class, dev) device_destroy(class, dev)
#endif
#define CLASS_DESTROY(class) class_destroy(class)
#define CLASS_CREATE(owner, name) class_create(owner, name)
#endif

#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,12) )
#define USB_MS_TIMEOUT(ms) ((ms / 1000) * HZ)
#else
#define USB_MS_TIMEOUT(ms) (ms)
#endif

#ifdef FOUND_PM_MESSAGE_STRUCT
#define GET_PM_MESSAGE_EVENT(pm_message) (pm_message.event)
#else
#define GET_PM_MESSAGE_EVENT(pm_message) ((u32)(pm_message))
#endif

#ifndef FOUND_PCI_CHOOSE_STATE
#define pci_choose_state(pdev, state) (state)
#endif

#if !defined(FOUND_PM_MESSAGE_T) && !defined(FOUND_PM_MESSAGE_STRUCT)
typedef u32 pm_message_t;
#endif

#ifdef FOUND_PCI_REGISTER_DRIVER
#define PCI_REGISTER_DRIVER(pci_driver) pci_register_driver(pci_driver)
#else
#define PCI_REGISTER_DRIVER(pci_driver) pci_module_init(pci_driver)
#endif

#ifdef FOUND_TOUCH_ATIME
#define TOUCH_ATIME(file) touch_atime((file)->f_vfsmnt,(file)->f_dentry);
#else
#define TOUCH_ATIME(file) update_atime((file)->f_dentry->d_inode);
#endif

#ifdef FOUND_PCI_GET_DEVICE
#define PCI_GET_DEVICE pci_get_device
#else
#define PCI_GET_DEVICE pci_find_device
#endif

#ifndef __user
#define __user
#endif

#ifdef IRQF_SHARED
#define CNXT_IRQ_SHARED IRQF_SHARED
#else
#define CNXT_IRQ_SHARED SA_SHIRQ
#endif

#ifndef proc_root_driver
#define proc_root_driver NULL
#define PROC_PREFIX "driver/"
#else
#define PROC_PREFIX ""
#endif

#ifndef FOUND_KILL_PROC
static inline struct pid *find_pid(int nr)
{
	return find_pid_ns(nr, &init_pid_ns);
}

static inline int
kill_proc(pid_t pid, int sig, int priv)
{
	int ret;

	rcu_read_lock();
	ret = kill_pid(find_pid(pid), sig, priv);
	rcu_read_unlock();
	return ret;
}
#endif

#ifdef FOUND_STRUCT_TTY_PORT
#define UART_INFO_TO_TTY(ui) (ui->port.tty)
#else
#define UART_INFO_TO_TTY(ui) (ui->tty)
#endif

#ifdef FOUND_NO_STRUCT_UART_INFO
typedef struct uart_state uart_info_t;
#else
typedef struct uart_info uart_info_t;
#endif

#endif /* __OSCOMPAT_H */
