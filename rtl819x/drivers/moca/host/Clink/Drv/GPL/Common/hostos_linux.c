/*******************************************************************************
*
* GPL/Common/hostos_linux.c
*
* Description: Host OS support for Linux
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

/*******************************************************************************
*                            # I n c l u d e s                                 *
********************************************************************************/

#include "gpl_hdr.h"



#define LOCK_MAGIC      0x4c4f434b       // spells "LOCK"

#define KMALLOC_MAX     65536

extern unsigned long volatile jiffies;


/**
*   Purpose:    Sets size bytes of memory to a given byte value.
*
*   Imports:    pMem - pointer to block of memory to set
*               val  - value to set each byte to
*               size - number of bytes to set
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_Memset(void *pMem, int val, int size)
{
    memset(pMem, val, size);
}

/**
*   Purpose:    Copies size bytes of memory from pFrom to pTo.
*               The memory areas may not overlap.
*
*   Imports:    pTo   - pointer to destination memory area
*               pFrom - pointer to source memory area
*               size  - number of bytes to copy
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_Memcpy(void *pTo, void *pFrom, int size)
{
    memcpy(pTo, pFrom, size);
}

/**
*   Purpose:    Scans input buffer and according to format specified.
*
*   Imports:    buf   - input buffer
*               fmt   - input format
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_Sscanf(const char *buf, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vsscanf(buf, fmt, args);
    va_end(args);
}

/**
*   Purpose:    Allocates DMA-addressable memory.
*
*   Imports:    size - size to allocate
*
*   Exports:    pointer to allocated block
*
*PUBLIC**************************/
void* HostOS_Alloc(int size)
{
    void *pMem;

    if (size < KMALLOC_MAX)
        pMem = kmalloc(size, GFP_KERNEL /* | __GFP_DMA */);
    else
        pMem = (void *)__get_free_pages(GFP_KERNEL, get_order(size));
    // printk("AllocMem %d %p\n", size, pMem);

    /** Assure that system constants defined in Entropic space have harmonious
     *  definitions compared with Unix constants.  This is not the most 
     *  pure place to put this but it does not have any runtime cost since
     *  the compiler optimizes it away. */
    INCTYPES_COMPILE_TIME_ASSERT( EIO       == SYS_INPUT_OUTPUT_ERROR     );
    INCTYPES_COMPILE_TIME_ASSERT( ENOMEM    == SYS_OUT_OF_MEMORY_ERROR    );
    INCTYPES_COMPILE_TIME_ASSERT( EACCES    == SYS_PERMISSION_ERROR       );
    INCTYPES_COMPILE_TIME_ASSERT( EFAULT    == SYS_INVALID_ADDRESS_ERROR  );
    INCTYPES_COMPILE_TIME_ASSERT( EINVAL    == SYS_INVALID_ARGUMENT_ERROR );
    INCTYPES_COMPILE_TIME_ASSERT( ENOSPC    == SYS_OUT_OF_SPACE_ERROR     );
#if 0 /* ACTIONTEC */ 
    INCTYPES_COMPILE_TIME_ASSERT( ENOTEMPTY == SYS_DIR_NOT_EMPTY_ERROR    );
    INCTYPES_COMPILE_TIME_ASSERT( ENOMSG    == SYS_BAD_MSG_TYPE_ERROR     );
    INCTYPES_COMPILE_TIME_ASSERT( ETIMEDOUT == SYS_TIMEOUT_ERROR          );
#endif

    return (pMem);
}

/**
*   Purpose:    Free DMA-addressable memory.
*
*   Imports:    pMem - allocation pointer from get free pages
*               size - Requested block size
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_Free(void* pMem, int size)
{
    if (size < KMALLOC_MAX)
        kfree(pMem);
    else
        free_pages((unsigned long)pMem, get_order(size));
}

/**
*   Purpose:    Delays for about timeInUsec microseconds.
*               The operation of this function is architecture dependent.
*               It is probably a tight loop.
*
*   Imports:    timeInUsec - delay in microseconds
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_Sleep(int timeInUsec)
{
    if( timeInUsec < 1000 ) {
        udelay(timeInUsec);
    } else {
        //HostOS_PrintLog(L_NOTICE, "mdelay delay=%d. jif=%d.\n", timeInUsec/1000, jiffies );
        mdelay(timeInUsec/1000);
        //HostOS_PrintLog(L_NOTICE, "mdelay jif=%d.\n", jiffies );
    }
}

/**
*   Purpose:    Inits a spinlock.
*               Sets the magic code for later testing.
*
*   Imports:    vlk - pointer to an Entropic lock structure
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_lock_init( void *vlk)
{
    hostos_lock_t *lk = (hostos_lock_t *)vlk ;

    if( !lk ) {
        HostOS_PrintLog(L_ERR, "Error: invalid lock pointer\n");
    } else {
        spin_lock_init( &lk->lock_spinlock );
        lk->lock_magic     = LOCK_MAGIC;
    }
}

/**
*   Purpose:    Waits for the lock.
*               The lock must be initialized.
*
*               See the macro:  HostOS_Lock
*
*   Imports:    vlk - pointer to an initialized Entropic lock structure
*                     or pointer to the mutex structure
*
*   Exports:    none - when this returns you have the lock.
*                      Please do an unlock later.
*
*PUBLIC**************************/
void HostOS_Lock(void *vlk)
{
#ifndef CLNK_HOST_SEM    
    hostos_lock_t *lk = (hostos_lock_t *)vlk ;

    BUG_ON((! lk) || (lk->lock_magic != LOCK_MAGIC));

    if( in_irq() ) {
        spin_lock(&lk->lock_spinlock);
    } else {
        spin_lock_irqsave(&lk->lock_spinlock, lk->lock_irq_flags);
    }
#else
    down((struct semaphore *)vlk); 
#endif
}

/**
*   Purpose:    Saves current interrupt context and waits for the lock.
*               The lock must be initialized.
*
*   Imports:    vlk - pointer to an initialized Entropic lock structure
*
*   Exports:    none - when this returns you have the lock.
*                      Please do an unlock later.
*
*PUBLIC**************************/
void HostOS_Lock_Irqsave(void *vlk)
{
    hostos_lock_t *lk = (hostos_lock_t *)vlk ;

    BUG_ON((! lk) || (lk->lock_magic != LOCK_MAGIC));

    spin_lock_irqsave(&lk->lock_spinlock, lk->lock_irq_flags);
}

/**
*   Purpose:    Trys to get the lock.
*               The lock must be initialized.
*
*   Imports:    vlk - pointer to an initialized Entropic lock structure
*
*   Exports:    0 - the lock was NOT acquired. Please try again.
*               1 - the lock is yours. Please ulock it later.
*
*PUBLIC**************************/
int HostOS_Lock_Try(void *vlk)
{
    hostos_lock_t *lk = (hostos_lock_t *)vlk ;
    int locked ;

    BUG_ON((! lk) || (lk->lock_magic != LOCK_MAGIC));

    locked = spin_trylock(&lk->lock_spinlock) ;

    return( locked );
}

/**
*   Purpose:    Unlocks a lock.
*               The lock must be initialized.
*
*               See the macro:  HostOS_Unlock
*
*   Imports:    vlk - pointer to an initialized Entropic lock structure
*
*   Exports:    none - when this returns you have released the lock.
*
*PUBLIC**************************/
void HostOS_Unlock(void *vlk)
{
#ifndef CLNK_HOST_SEM 
    hostos_lock_t *lk = (hostos_lock_t *)vlk ;

    BUG_ON((! lk) || (lk->lock_magic != LOCK_MAGIC));

    if( in_irq() ) {
        spin_unlock(&lk->lock_spinlock);
    } else {
        spin_unlock_irqrestore(&lk->lock_spinlock, lk->lock_irq_flags);
    }
#else
    up((struct semaphore *)vlk); 
#endif
}

/**
*   Purpose:    Unlocks a lock and restores interrupt context.
*               The lock must be initialized.
*
*
*   Imports:    vlk - pointer to an initialized Entropic lock structure
*
*   Exports:    none - when this returns you have released the lock.
*
*PUBLIC**************************/
void HostOS_Unlock_Irqrestore(void *vlk)
{
    hostos_lock_t *lk = (hostos_lock_t *)vlk ;

    BUG_ON((! lk) || (lk->lock_magic != LOCK_MAGIC));

   spin_unlock_irqrestore(&lk->lock_spinlock, lk->lock_irq_flags);
}

/**
*   Purpose:    Terminates a lock.
*
*   Imports:    vlk - pointer to an initialized Entropic lock structure
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_TermLock(void *vlk)
{
}


// Host Print ForMaT string
#define HPFMT "%s: %s"

/**
*   Purpose:    Variable argument OS print.
*
*   Imports:    lev - severity level. See the L_* enum.
*                     See HOST_OS_PRINTLOG_THRESHOLD
*               fmt - printf format string
*               ... - printf format arguments
*
*   Exports:    none
*
*  For reference:  Linux has these
*
*  #define KERN_EMERG      "<0>"   / * system is unusable                   * /
*  #define KERN_ALERT      "<1>"   / * action must be taken immediately     * /
*  #define KERN_CRIT       "<2>"   / * critical conditions                  * /
*  #define KERN_ERR        "<3>"   / * error conditions                     * /
*  #define KERN_WARNING    "<4>"   / * warning conditions                   * /
*  #define KERN_NOTICE     "<5>"   / * normal but significant condition     * /
*  #define KERN_INFO       "<6>"   / * informational                        * /
*  #define KERN_DEBUG      "<7>"   / * debug-level messages                 * /
*
*PUBLIC**************************/
void HostOS_PrintLog(SYS_INT32 lev, const char *fmt, ...)
{
    va_list args;
    int  printed_len;
    static char printk_buf[1024]; /*  Making it static as this is
                                                        * too much data to allocate on the stack. */

    if (lev <= HOST_OS_PRINTLOG_THRESHOLD)
    {   
        /* Emit the output into the temporary buffer */
        va_start(args, fmt);
        printed_len = vsnprintf(printk_buf, sizeof(printk_buf), fmt, args);
        va_end(args);

        switch ( lev )
        {
            case L_EMERG   :
                printk(KERN_EMERG   HPFMT, MOCA_DRV_NAME, printk_buf);
                break ;
            case L_ALERT   :
                printk(KERN_ALERT   HPFMT, MOCA_DRV_NAME, printk_buf);
                break ;
            case L_CRIT    :
                printk(KERN_CRIT    HPFMT, MOCA_DRV_NAME, printk_buf);
                break ;
            case L_ERR     :
                printk(KERN_ERR     HPFMT, MOCA_DRV_NAME, printk_buf);
                break ;
            case L_WARNING :
                printk(KERN_WARNING HPFMT, MOCA_DRV_NAME, printk_buf);
                break ;
            case L_NOTICE  :
                printk(KERN_NOTICE  HPFMT, MOCA_DRV_NAME, printk_buf);
                break ;
            case L_INFO    :
                printk(KERN_INFO    HPFMT, MOCA_DRV_NAME, printk_buf);
                break ;
            case L_DEBUG   :
                printk(KERN_DEBUG   HPFMT, MOCA_DRV_NAME, printk_buf);
                break ;
        }
    }
}

/**
*   Purpose:    register/memory read access from kernel space.
*
*   Imports:    addr - address to read from
*
*   Exports:    32 bit value from given address
*
*PUBLIC**************************/
SYS_UINT32 HostOS_Read_Word( SYS_UINT32 *addr )
{
    SYS_UINT32 rv ;

#if defined(CONFIG_ARCH_ENTROPIC_ECB) && (defined(CONFIG_ARCH_IXP425) || defined(CONFIG_ARCH_IXP4XX))
    rv = *(volatile SYS_UINT32 *)addr ;
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    rv = readl( (const volatile void *)addr );
#else
    rv = readl( addr ) ;
#endif
#endif
    return( rv ) ;
}

/**
*   Purpose:    register/memory write access from kernel space.
*
*   Imports:    val  - 32 bit value to write
*               addr - address to write to
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_Write_Word( SYS_UINT32 val, SYS_UINT32 *addr )
{
#if defined(CONFIG_ARCH_ENTROPIC_ECB) && (defined(CONFIG_ARCH_IXP425) || defined(CONFIG_ARCH_IXP4XX))
    *(volatile SYS_UINT32 *)addr = val ;
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    writel( val, (volatile void *)addr );
#else
    writel( val, addr );
#endif
#endif
}


/**
*   Purpose:    Initialize a timer and register it.
*
*   Imports:    vtmr - pointer to the timer to initialize
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_timer_init( void *vtmr )
{
    struct hostos_timer *tmr = (struct hostos_timer *)vtmr ;

    init_timer( &tmr->ostimer ) ;
}

/**
*   Purpose:    Deactivate a registered timer.
*
*       http://lxr.linux.no/#linux+v2.6.24/kernel/timer.c#L509
*
*   Imports:    vtmr - pointer to the timer to deactivate
*
*   Exports:    1 = the timer was active (not expired)
*               0 = the timer was not active
*
*PUBLIC**************************/
int HostOS_timer_del( void *vtmr )
{
    struct hostos_timer *tmr = (struct hostos_timer *)vtmr ;

    return( del_timer( &tmr->ostimer ) ) ;
}

/**
*   Purpose:    Deactivate a registered timer.
*               And ensure the timer is not running on any CPU
*               Can sleep
*
*   Imports:    vtmr - pointer to the timer to deactivate
*
*   Exports:    1 = the timer was active (not expired)
*               0 = the timer was not active
*
*PUBLIC**************************/
int HostOS_timer_del_sync( void *vtmr )
{
    struct hostos_timer *tmr = (struct hostos_timer *)vtmr ;

    return( del_timer_sync( &tmr->ostimer ) ) ;
}

/**
*   Purpose:    Modifies a timer.
*
*       http://lxr.linux.no/#linux+v2.6.24/kernel/timer.c#L480
*
*   Imports:    vtmr    - pointer to the timer to deactivate
*               timeout - timeout value from HostOS_timer_expire_seconds
*                         or HostOS_timer_expire_ticks
*
*   Exports:    1 = the timer was active (not expired)
*               0 = the timer was not active
*
*PUBLIC**************************/
int HostOS_timer_mod( void *vtmr, SYS_ULONG timeout )
{
    struct hostos_timer *tmr = (struct hostos_timer *)vtmr ;

    return( mod_timer( &tmr->ostimer, timeout ) ) ;
}

/**
*   Purpose:    Add a timer.
*               Call this after HostOS_timer_setup
*
*       http://lxr.linux.no/#linux+v2.6.24/kernel/timer.c#L509
*
*   Imports:    vtmr - pointer to the timer to deactivate
*
*   Exports:    1 = the timer was active (not expired)
*               0 = the timer was not active
*
*PUBLIC**************************/
void HostOS_timer_add( void *vtmr )
{
    struct hostos_timer *tmr = (struct hostos_timer *)vtmr ;

    add_timer( &tmr->ostimer ) ;
}

/**
*   Purpose:    Sets up a timer with function and user data.
*
*               See HostOS_timer_set_timeout
*
*   Imports:    vtmr    - pointer to the timer to deactivate
*               func    - callback function to call at timer expiration
*               data    - data to pass to callback
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_timer_setup( void *vtmr, timer_function_t func, SYS_UINTPTR data )
{
    struct hostos_timer *tmr = (struct hostos_timer *)vtmr ;

    tmr->ostimer.function = func ;
    tmr->ostimer.data     = data ;
}

/**
*   Purpose:    Sets a timer's time out.
*
*               See HostOS_timer_setup
*
*   Imports:    vtmr    - pointer to the timer to deactivate
*               timeout - timeout value from HostOS_timer_expire_seconds
*                         or HostOS_timer_expire_ticks
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_timer_set_timeout( void *vtmr, SYS_ULONG timeout )
{
    struct hostos_timer *tmr = (struct hostos_timer *)vtmr ;

    tmr->ostimer.expires  = timeout ;
}

/**
*   Purpose:    Calculate a future expiration point.
*              In seconds.
*
*   Imports:    future - number of seconds into the future
*
*   Exports:    A number for use in the timer expiration member
*
*PUBLIC**************************/
SYS_ULONG HostOS_timer_expire_seconds( SYS_UINT32 future )
{

    return( jiffies + (future * HZ) ) ;
}

/**
*   Purpose:    Calculate a future expiration point.
*              In jiffies.
*
*   Imports:    future - number of ticks into the future
*
*   Exports:    A number for use in the timer expiration member
*
*PUBLIC**************************/
SYS_ULONG HostOS_timer_expire_ticks( SYS_UINT32 future )
{

    return( jiffies + future ) ;
}


/**************************************************************

    wait queue timer - support

    This structure contains everything necessary to
    use timers and wait queues from the !GPL side.

    You allocate one of these wqt_t things and then
    pass its pointer back on the various calls.

***************************************************************/

typedef struct hostos_w_q_t
{
    int                 wqt_allocated ; // 0 = no, 1 = yes
    struct timer_list   wqt_timer;
    wait_queue_head_t   wqt_wq;
}
wqt_t ;


/**
*   Purpose:    Allocates a wqt entry from the heap.
*
*   Imports:
*
*   Exports:    !0 = void pointer to opaque data (really a wqt_t)
*               0 = allocation failure
*
*PUBLIC**************************/
void *HostOS_wqt_alloc( void )
{
    wqt_t       *wqt ;

    wqt = kmalloc( sizeof( wqt_t ), GFP_KERNEL ) ;

    return( (void *)wqt ) ;
}

/**
*   Purpose:    De-allocates a wqt entry from the heap.
*
*   Imports:    vwqt - pointer to the wqt_t to be freed
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_wqt_free( void *vwqt )
{

    kfree( vwqt ) ;
}

/**
*   Purpose:    Initialize a timer and register it.
*
*   Imports:    vwqt - pointer to the wqt_t with timer to initialize
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_wqt_timer_init( void *vwqt )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    wqt->wqt_allocated = 1 ;  // mark the timer inited
    init_timer( &wqt->wqt_timer ) ;
}

/**
*   Purpose:    Deactivate a wqt timer.
*
*      http://lxr.linux.no/#linux+v2.6.24/kernel/timer.c#L509
*
*   Imports:    vwqt - pointer to the wqt_t with timer to deactivate
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_wqt_timer_del( void *vwqt )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    if( wqt->wqt_allocated ) {
        del_timer( &wqt->wqt_timer ) ;
    }
}

/**
*   Purpose:    Deactivate a registered timer
*               And ensure the timer is not running on any CPU.
*               Can sleep.
*
*   Imports:    vwqt - pointer to the wqt_t with timer to deactivate
*
*   Exports:    1 = the timer was active (not expired)
*               0 = the timer was not active
*
*PUBLIC**************************/
int HostOS_wqt_timer_del_sync( void *vwqt )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    return( del_timer_sync( &wqt->wqt_timer ) ) ;
}

/**
*   Purpose:    Modifies a timer.
*
*      http://lxr.linux.no/#linux+v2.6.24/kernel/timer.c#L480
*
*   Imports:    vwqt    - pointer to the wqt_t with timer to modify
*               timeout - timeout value from HostOS_timer_expire_seconds
*                        or HostOS_timer_expire_ticks
*
*   Exports:    1 = the timer was active (not expired)
*               0 = the timer was not active
*
*PUBLIC**************************/
int HostOS_wqt_timer_mod( void *vwqt, SYS_ULONG timeout )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    return( mod_timer( &wqt->wqt_timer, timeout ) ) ;
}

/**
*   Purpose:    Add a timer
*               Call this after HostOS_timer_setup.
*
*      http://lxr.linux.no/#linux+v2.6.24/kernel/timer.c#L509
*
*   Imports:    vwqt    - pointer to the wqt_t with timer to add
*
*   Exports:    1 = the timer was active (not expired)
*               0 = the timer was not active
*
*PUBLIC**************************/
void HostOS_wqt_timer_add( void *vwqt )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    add_timer( &wqt->wqt_timer ) ;
}

/**
*   Purpose:    Sets up a timer with function and user data.
*
*               See HostOS_timer_set_timeout
*
*   Imports:    vwqt    - pointer to the wqt_t with timer to  init
*               func    - callback function to call at timer expiration
*               data    - data to pass to callback
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_wqt_timer_setup( void *vwqt, timer_function_t func, SYS_UINTPTR data )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    wqt->wqt_timer.function = func ;
    wqt->wqt_timer.data     = data ;
}

/**
*   Purpose:    Sets a timer's time out.
*
*               See HostOS_timer_setup
*
*   Imports:    vwqt    - pointer to the wqt_t with timer to set
*               timeout - timeout value from HostOS_timer_expire_seconds
*                         or HostOS_timer_expire_ticks
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_wqt_timer_set_timeout( void *vwqt, SYS_ULONG timeout )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    wqt->wqt_timer.expires  = timeout ;
}

/**
*   Purpose:    Inits a wait queue head in a wqt_t.
*
*   Imports:    vwqt    - pointer to the wqt_t with wait q to init
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_wqt_waitq_init( void *vwqt )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    init_waitqueue_head( &wqt->wqt_wq );
}

/**
*   Purpose:    Wakes up a wait q in a wqt_t.
*
*   Imports:    vwqt    - pointer to the wqt_t with wait q to wake
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_wqt_waitq_wakeup_intr( void *vwqt )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    if( wqt )
    {
        wake_up_interruptible( &wqt->wqt_wq );
    }
}

/**
*   Purpose:    Waits for a wait q event in a wqt_t.
*
*   Imports:    vwqt    - pointer to the wqt_t with wait q to wait on
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_wqt_waitq_wait_event_intr( void *vwqt, HostOS_wqt_condition func, void *vp )
{
    wqt_t       *wqt = (wqt_t *)vwqt ;

    wait_event_interruptible( wqt->wqt_wq, /* pass by value */
                              func(vp) );

}

#if defined(PCI_DRVR_SUPPORT)
/**
*   Purpose:    PCI device register access.
*               These functions read or write the PCI bus device.
*
*               Linux uses nested macros to define these.
*               See: lxr.linux.no/#linux+v2.6.24/drivers/pci/access.c
*
*   Imports:    ddev           - context structure with device pointer
*               reg            - PCI device bus address/register address
*               pVal or val    - where to put or get the data
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_ReadPciConfig_Word(void* ddev, SYS_UINT32 reg, SYS_UINT16* pVal)
{
    struct pci_dev *pdev = ((dd_context_t *)ddev)->pdev;

    pci_read_config_word(pdev, reg, pVal);
}

/**
*   Purpose:    PCI device register access.
*              These functions read or write the PCI bus device.
*
*              Linux uses nested macros to define these.
*              See: lxr.linux.no/#linux+v2.6.24/drivers/pci/access.c
*
*   Imports:    ddev           - context structure with pci_dev pointer
*               reg            - PCI device bus address/register address
*               pVal or val    - where to put or get the data
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_ReadPciConfig(void* ddev, SYS_UINT32 reg, SYS_UINT32* pVal)
{
    struct pci_dev *pdev = ((dd_context_t *)ddev)->pdev;

    pci_read_config_dword(pdev, reg, pVal);
}

/**
*   Purpose:    PCI device register access.
*               These functions read or write the PCI bus device.
*
*               Linux uses nested macros to define these.
*               See: lxr.linux.no/#linux+v2.6.24/drivers/pci/access.c
*
*   Imports:    ddev           - context structure with device pointer
*               reg            - PCI device bus address/register address
*               pVal or val    - where to put or get the data
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_WritePciConfig_Word(void *ddev, SYS_UINT32 reg, SYS_UINT16 val)
{
    struct pci_dev *pdev = ((dd_context_t *)ddev)->pdev;

    pci_write_config_word(pdev, reg, val);
}

/**
*   Purpose:    PCI device register access.
*               These functions read or write the PCI bus device.
*
*               Linux uses nested macros to define these.
*               See: lxr.linux.no/#linux+v2.6.24/drivers/pci/access.c
*
*   Imports:    ddev           - context structure with device pointer
*               reg            - PCI device bus address/register address
*               pVal or val    - where to put or get the data
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_WritePciConfig(void* ddev, SYS_UINT32 reg, SYS_UINT32 val)
{
    struct pci_dev *pdev = ((dd_context_t *)ddev)->pdev;

    pci_write_config_dword(pdev, reg, val);
}

/**
*   Purpose:    Allocate DMA-addressable memory.
*
*   Imports:    ddev    - pci_dev pointer
*               size    - Requested block size
*               ppMemPa - Pointer to place to return the Physical memory address
*
*   Exports:    Virtual memory address (and Phys address), or NULL on failure
*
*PUBLIC**************************/
void *HostOS_AllocDmaMem(void *ddev, int size, void **ppMemPa)
{
    struct pci_dev *pdev = ((dd_context_t *)ddev)->pdev;
    dma_addr_t     dma_buf;
    void           *pMemVa;

    pMemVa   = pci_alloc_consistent(pdev, size, &dma_buf);
    *ppMemPa = (void *)dma_buf;

    // printk("AllocDmaMem %d %x %x\n", size, pMemVa, dma_buf);

    return (pMemVa);
}

/**
*   Purpose:    Free DMA-addressable memory.
*
*   Imports:    ddev   - pci_dev pointer
*               size   - Requested block size
*               pMemVa - Virtual memory address returned by HostOS_AllocDmaMem()
*               pMemPa - Physical memory address returned by    ditto
*
*   Exports:    Virtual memory address (and Phys address), or NULL on failure
*
*PUBLIC**************************/
void HostOS_FreeDmaMem(void *ddev, int size, void *pMemVa, void *pMemPa)
{
    struct pci_dev *pdev = ((dd_context_t *)ddev)->pdev;

    pci_free_consistent(pdev, (size_t) size, pMemVa, (dma_addr_t)pMemPa);
}
#endif

/**
*   Purpose:    Initialize a tasklet.
*
*               Prototype:
*               void tasklet_init(struct tasklet_struct *t,
*                                 void (*func)(unsigned long),
*                                 unsigned long data)
*
*   Imports:    vtl  - pointer to the tasklet structure
*               func - tasklet function
*               data - tasklet context data
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_task_init( void *vtl, void *func, unsigned long data )
{

    tasklet_init((struct tasklet_struct *)vtl, func, data ) ; // abstraction
}

/**
*   Purpose:    Schedule a tasklet.
*               Tasklet must already be initialized.
*
*   Imports:    vtl - pointer to the tasklet structure
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_task_schedule( void *vtl )
{

    tasklet_schedule( (struct tasklet_struct *)vtl ) ; // abstraction
}

/**
*   Purpose:    Enable a tasklet.
*
*   Imports:    vtl - pointer to the tasklet structure
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_task_enable( void *vtl )
{

    tasklet_enable( (struct tasklet_struct *)vtl ) ;  // abstraction
}

/**
*   Purpose:    Disable a tasklet.
*
*   Imports:    vtl - pointer to the tasklet structure
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_task_disable( void *vtl )
{

    tasklet_disable( (struct tasklet_struct *)vtl ) ;  // abstraction
}

/**
*   Purpose:    Kill a tasklet.
*
*   Imports:    vtl - pointer to the tasklet structure
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_task_kill( void *vtl )
{

    tasklet_kill( (struct tasklet_struct *)vtl ) ; // abstraction
}

/**
*   Purpose:    Initialize a mutex.
*
*   Imports:    vmt - pointer to the mutex structure
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_mutex_init( void *vmt )
{

    init_MUTEX( (struct semaphore *)vmt ) ;  // abstraction
}

/**
*   Purpose:    Release (up) a mutex.
*
*               Do NOT call this from an ISR!
*
*               Do NOT call this if you are not the current
*               holder of the mutex.
*
*   Imports:    vmt - pointer to the mutex structure
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_mutex_release( void *vmt )
{

    up( (struct semaphore *)vmt ) ; // abstraction
}

/**
*   Purpose:    Acquire (down) a mutex.
*
*               Might sleep.
*               When this returns you have the mutex.
*
*               This is NOT the preferred method. See
*               HostOS_mutex_acquire_intr().
*
*               Do NOT call this from an ISR!
*
*   Imports:    vmt - pointer to the mutex structure
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_mutex_acquire( void *vmt )
{

    down( (struct semaphore *)vmt ) ; // abstraction
}

/**
*   Purpose:    Acquire (down) a mutex. Interruptible
*
*               Might sleep.
*               When this returns 0 you have the mutex.
*               When this returns -EINTR you have been interrupted
*               and must bail out. You do NOT have the mutex. The
*               purpose here is to allow the caller, probably a
*               user space app, to be interrupted and die quickly
*               and quietly; rather than becoming a zombie because
*               the driver won't let go.
*
*               This is the preferred method.
*
*               Do NOT call this from an ISR!
*
*   Imports:    vmt - pointer to the mutex structure
*
*   Exports:    0      - you got the mutex
*               -EINTR - you've been interrupted, no mutex
*
*PUBLIC**************************/
int HostOS_mutex_acquire_intr( void *vmt )
{
    int rc ;

    rc = down_interruptible( (struct semaphore *)vmt ) ; // abstraction
    return( rc ) ;
}

/**
*   Purpose:    Copies a block from user space to kernel space.
*
*   Imports:    to     - pointer to a kernel space buffer to receive the data
*               from   - pointer to a user space buffer to source the data
*               nbytes - number of bytes to copy
*
*   Exports:    number of bytes NOT copied
*
*PUBLIC**************************/
unsigned long HostOS_copy_from_user( void *to, const void *from, unsigned long nbytes )
{
    unsigned long n ;

    n = copy_from_user( to, from, nbytes );
    return( n ) ;
}

/**
*   Purpose:    Copies a block from kernel space to user space.
*
*   Imports:    to     - pointer to a user space buffer to receive the data
*               from   - pointer to a kernel space buffer to source the data
*               nbytes - number of bytes to copy
*
*   Exports:    number of bytes NOT copied
*
*PUBLIC**************************/
unsigned long HostOS_copy_to_user( void *to, const void *from, unsigned long nbytes )
{
    unsigned long n ;

    n = copy_to_user( to, from, nbytes );
    return( n ) ;
}

#if defined(PCI_DRVR_SUPPORT)
/**
*   Purpose:    Check if the carrier is ok.
*
*   Imports:    kdev - context structure with device pointer
*
*   Exports:    1 if carrier is ok, 0 if not
*
*PUBLIC**************************/
unsigned long HostOS_netif_carrier_ok( void *kdev )
{

    return( netif_carrier_ok((struct net_device *)kdev) ) ;
}

/**
*   Purpose:    Sets net carrier on.
*
*   Imports:    kdev - context structure with device pointer
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_netif_carrier_on( void *kdev )
{

    netif_carrier_on((struct net_device *)kdev);
    netif_wake_queue((struct net_device *)kdev);
}

/**
*   Purpose:    Sets net carrier off.
*
*   Imports:    kdev - context structure with device pointer
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_netif_carrier_off( void *kdev )
{

    netif_carrier_off((struct net_device *)kdev);
}

/**
*   Purpose:    Sets the MAC address in the OS.
*
*   Imports:    mac_hi - Bytes 0-3 of MAC address
*               mac_lo - Bytes 4-5 of MAC address
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_set_mac_address( void *kdev, SYS_UINT32 mac_hi, SYS_UINT32 mac_lo )
{
    struct net_device *dev = (struct net_device *)kdev;

    dev->dev_addr[0] = (SYS_UCHAR) ((mac_hi >> 24) & 0xff);
    dev->dev_addr[1] = (SYS_UCHAR) ((mac_hi >> 16) & 0xff);
    dev->dev_addr[2] = (SYS_UCHAR) ((mac_hi >>  8) & 0xff);
    dev->dev_addr[3] = (SYS_UCHAR) ((mac_hi      ) & 0xff);
    dev->dev_addr[4] = (SYS_UCHAR) ((mac_lo >> 24) & 0xff);
    dev->dev_addr[5] = (SYS_UCHAR) ((mac_lo >> 16) & 0xff);
}

/**
*   Purpose:    Brings up the Ethernet interface.
*
*   Imports:    kdev - context structure with device pointer
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_open( void *kdev )
{
    struct net_device *dev = (struct net_device *)kdev;

    if ((dev->flags & IFF_UP) == IFF_UP)
    {
        clnketh_open(dev);
    }
}

/**
*   Purpose:    Shuts down the Ethernet interface.
*
*   Imports:    kdev - context structure with device pointer
*
*   Exports:
*
*PUBLIC**************************/
void HostOS_close( void *kdev )
{
    struct net_device *dev = (struct net_device *)kdev;

    if ((dev->flags & IFF_UP) == IFF_UP)
    {
        clnketh_close(dev);
    }
}
#endif

#if defined(E1000_DRVR_SUPPORT) || defined(CANDD_DRVR_SUPPORT)

/* declaring mutexes for kernel thread start/stop */
DECLARE_MUTEX (HostOS_thread_mutex);

hostos_kthread_t hostosThread;

/**
*   Purpose:    Check pending signal of task.
*
*   Imports:    vtask - pointer to the task
*
*   Exports:    0 - no pending signal
*               1 - has pendign signal
*
*PUBLIC**************************/
int HostOS_signal_pending(void *vtask)
{
    struct task_struct *p;
    
    if(vtask== SYS_NULL)
        p = current;
    else
        p = (struct task_struct *)vtask;

    return signal_pending(p);
}

/**
*   Purpose:    Convert  jiffies to milliseconds.
*
*   Imports:    j- jiffies
*
*   Exports:    none
*
***************************/
static inline unsigned int HostOS_jiffies_to_msecs(const unsigned long j)
{
#if HZ <= 1000 && !(1000 % HZ)
    return (1000 / HZ) * j;
#elif HZ > 1000 && !(HZ % 1000)
    return (j + (HZ / 1000) - 1)/(HZ / 1000);
#else
    return (j * 1000) / HZ;
#endif
}

/**
*   Purpose:    Convert milliseconds to jiffies.
*
*   Imports:    m - milliseconds
*
*   Exports:    none
*
***************************/
static inline unsigned long HostOS_msecs_to_jiffies(const unsigned int m)
{
    if (m > HostOS_jiffies_to_msecs(MAX_JIFFY_OFFSET))
        return MAX_JIFFY_OFFSET;
#if HZ <= 1000 && !(1000 % HZ)
    return (m + (1000 / HZ) - 1) / (1000 / HZ);
#elif HZ > 1000 && !(HZ % 1000)
    return m * (HZ / 1000);
#else
    return (m * HZ + 999) / 1000;
#endif
}

/**
*   Purpose:    Sleep waiting for waitqueue interruptions.
*
*   Imports:    msecs - Time in milliseconds to sleep for
*
*   Exports:    none
*
*PUBLIC**************************/
void HostOS_msleep_interruptible(unsigned int msecs)
{
    unsigned long timeout = HostOS_msecs_to_jiffies(msecs) + 1;

    while (timeout && !signal_pending(current)) {
        __set_current_state(TASK_INTERRUPTIBLE);
        timeout = schedule_timeout(timeout);
    }
}

/**
*   Purpose:    Kernel thread internal function.
*
*   Imports:    pdata - pointer to thread struct
*
*   Exports:    none
*
***************************/
static void HostOS_thread_inter_func(void *pdata)
{
    hostos_kthread_t *pThread = (hostos_kthread_t *)pdata;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
    daemonize(hostosThread.name, 0);
    allow_signal(SIGKILL);
    allow_signal(SIGTERM);    
#else
    daemonize();
#endif

    /* kernel thread main function */
    pThread->func(pThread->arg);

    return ;
}

/**
*   Purpose:    Create new kernel thread.
*
*   Imports:    pThreadID - pointer to new kernel threadID
*               pName     - pointer to thread name string
*               func      - pointer to thread internal function
*               arg       - argument to pass to thread function
*
*   Exports:    0  - OK
*               -1 - FAILED
*
*PUBLIC**************************/
int HostOS_thread_start(unsigned int *pThreadID, char *pName, void (*func)(void *), void *arg)
{
    down(&HostOS_thread_mutex);

    strncpy(hostosThread.name, pName,16);
    hostosThread.func = func;
    hostosThread.arg = arg;

    /* Create kernel thread */
    hostosThread.threadID = kernel_thread((int (*)(void *))HostOS_thread_inter_func, &hostosThread, 0);
    *pThreadID = hostosThread.threadID;

    if(hostosThread.threadID < 0)
    {
        up (&HostOS_thread_mutex);
        return -1;
    }
    
    up(&HostOS_thread_mutex);
    return 0;
}

/**
*   Purpose:    Stop a kernel thread.
*
*   Imports:    threadID - kernel threadID that to be stopped
*
*   Exports:    0   - OK
*               -1  - FAILED
*
*PUBLIC**************************/
int HostOS_thread_stop(unsigned long threadID)
{
    int ret = 0;

    down(&HostOS_thread_mutex);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)    
    if((ret = kill_pid(find_vpid(threadID), SIGKILL, 1)))
#else
    if((ret = kill_proc(threadID, SIGKILL, 1)))
#endif
    {
        up(&HostOS_thread_mutex);
        return -1;
    }

    up(&HostOS_thread_mutex);
    return 0;
}

#endif
