/*******************************************************************************
*
* GPL/Common/gpl_context.h
*
* Description: PCI Driver GPL Context
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

#ifndef __gpl_context_h__
#define __gpl_context_h__


/*
    This structure/variable is instantiated at probe time by allocation.
    It is pointed to from the driver data context p_dg_ctx member. 
*/

struct _driver_gpl_context_ {
    void                *p_dd_ctx ; // pointer to driver data    context
    void                *p_dc_ctx ; // pointer to driver control context

#ifdef CLNK_HOST_SEM
    hostos_sema_t       at_lock; 
#else
    hostos_lock_t       at_lock;        // address translator spin lock - referenced in !GPL side
#endif

    hostos_sema_t       ioctl_sem;      // Mutex for ioctl threads, keeps them one at a time 

#ifdef CLNK_HOST_SEM
    hostos_sema_t       mbx_cmd_lock;   // mailbox cmd mutex semphore - referenced in !GPL side
    hostos_sema_t       mbx_swun_lock;  // mailbox sw unsol mutex semphore - referenced in !GPL side
#else
    hostos_lock_t       mbx_cmd_lock;   // mailbox cmd spin lock - referenced in !GPL side
    hostos_lock_t       mbx_swun_lock;  // mailbox sw unsol spin lock - referenced in !GPL side
#endif

};

typedef struct _driver_gpl_context_ dg_context_t ;

#endif  // __gpl_context_h__
