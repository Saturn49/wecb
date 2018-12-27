/*******************************************************************************
*
* GPL/Common/hostos_linux.h
*
* Description: OS includes and abstracted types
*
*******************************************************************************/

/*******************************************************************************
*                        Entropic Communications, Inc.
*                         Copyright (c) 2001-2008
*                          All rights reserved.
*******************************************************************************/

/*******************************************************************************
* This file is licensed under GNU General Public license.                      *
*                                                                              *
* This file is free software: you can redistribute and/or modify it under the  *
* terms of the GNU General Public License, Version 2, as published by the Free *
* Software Foundation.                                                         *
*                                                                              *
* This program is distributed in the hope that it will be useful, but AS-IS and*
* WITHOUT ANY WARRANTY; without even the implied warranties of MERCHANTABILITY,*
* FITNESS FOR A PARTICULAR PURPOSE, TITLE, or NONINFRINGEMENT. Redistribution, *
* except as permitted by the GNU General Public License is prohibited.         *
*                                                                              *
* You should have received a copy of the GNU General Public License, Version 2 *
* along with this file; if not, see <http://www.gnu.org/licenses/>.            *
*******************************************************************************/

#ifndef _hostos_linux_h_
#define _hostos_linux_h_

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18)
#include <linux/autoconf.h>
#else
#include <linux/config.h>
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/unaligned.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/ctype.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/sched.h>


/**
 *  spinlock structure with added bells and whistles
 */

// for locking purposes
typedef struct hostos_lock
{
    int             lock_magic;        // used to verify the structure is initialized
    unsigned long   lock_irq_flags;    // irq flags for restoring later
    spinlock_t      lock_spinlock;
}
hostos_lock_t ;

// timer operations
// for linux this contains a struct timer_list
typedef struct hostos_timer
{
    struct timer_list ostimer ;
}
hostos_timer_t ;

// tasklet operations
// for linux this contains a struct tasklet_struct
typedef struct hostos_task
{
    struct tasklet_struct ostask ;
}
hostos_task_t ;

// semaphore operations
// for linux this contains a struct semaphore
typedef struct hostos_sema
{
    struct semaphore ossema ;
}
hostos_sema_t ;

typedef struct _hostos_waitq
{
    wait_queue_head_t   wq;
}
hostos_waitq_t ;

// thread operations 
// for linux this contains kernel thread related information
typedef struct hostos_kthread
{
    char name[20];          /* thread name */
    int  threadID;    
    void (*func) (void *);  /* thread internal function */    
    void *arg;              /* argument to pass to kernel thread */
} 
hostos_kthread_t;

#endif /* _hostos_linux_h_ */

