/*******************************************************************************
*
* GPL/Common/gpl_hdr.h
*
* Description: GPL headers
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

#ifndef __gpl_hdr_h__
#define __gpl_hdr_h__

/*******************************************************************************
*                            # I n c l u d e s                                 *
********************************************************************************/

#include "linux/errno.h"
#include "inctypes_dvr.h"
#include "hostos_linux.h"

#include "common_dvr.h"

#if defined(PCI_DRVR_SUPPORT) 
#include "HostOS_Spec_pci.h"
#include "eth_dvr.h"
#endif

#if defined(CANDD_DRVR_SUPPORT) 
#include "HostOS_Spec_candd.h"
#endif

#if defined(E1000_DRVR_SUPPORT) 
#include "HostOS_Spec_e1000.h"
#include "e1000.h"
#endif

#include "data_context.h"
#include "gpl_context.h"

/*******************************************************************************
*                            P R O T O T Y P E S                               *
********************************************************************************/

#if defined(PCI_DRVR_SUPPORT) 
#include "pci_gpl_proto.h"
#endif
#include "com_abs_proto.h"

#include "HostOS_proto.h"


#endif // __gpl_hdr_h__
