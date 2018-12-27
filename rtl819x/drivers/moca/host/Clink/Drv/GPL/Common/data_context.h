/*******************************************************************************
*
* GPL/Common/data_context.h
*
* Description: data context selector
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


#ifndef __data_context_h__
#define __data_context_h__

#if defined(PCI_DRVR_SUPPORT)
#include "data_context_pci.h"
#endif
#if defined(E1000_DRVR_SUPPORT) 
#include "data_context_e1000.h"
#endif
#if defined(CANDD_DRVR_SUPPORT) 
#include "data_context_candd.h"
#endif

#endif // __data_context_h__
