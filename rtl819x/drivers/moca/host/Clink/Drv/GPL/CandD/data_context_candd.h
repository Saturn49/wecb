/*******************************************************************************
*
* GPL/CandD/data_context_candd.h
*
* Description: CandD Driver data definition
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

#ifndef __data_context_candd_h__
#define __data_context_candd_h__


//
// driver kernel context - simulating the way a net driver works
//
struct _driver_kernel_context_
{
    char *name ;
    void *priv ;    // pointer to data context
};
typedef struct _driver_kernel_context_ dk_context_t ;

//
// driver data context - simulating the way an ethernet driver works
//
struct _driver_data_context_
{

    void *p_dg_ctx ;    // pointer to driver gpl context
};
typedef struct _driver_data_context_ dd_context_t ;


#endif /* __data_context_candd_h__ */

