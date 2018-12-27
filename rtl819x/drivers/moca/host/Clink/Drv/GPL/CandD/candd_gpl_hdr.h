/*******************************************************************************
*
* GPL/CandD/candd_gpl_hdr.h
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

#ifndef __candd_gpl_hdr_h__
#define __candd_gpl_hdr_h__


#define SONICS_ADDRESS_OF_sniff_buffer_pa 0


/*******************************************************************************
*                            # I n c l u d e s                                 *
********************************************************************************/

#include "driverversion.h"
#include "HostOS_Spec_candd.h"
#include "linux/errno.h"
#include "inctypes_dvr.h"
#include "hostos_linux.h"

#include "common_dvr.h"
#include "Clnk_ctl_dvr.h"
#include "ClnkMbx_dvr.h"

#include "data_context.h"
#include "gpl_context.h"
#include "drv_ctl_opts.h"
#include "clnkiodefs.h"

/*******************************************************************************
*                            P R O T O T Y P E S                               *
********************************************************************************/

#include "com_abs_proto.h"

#define dc_context_t void // Temporary
#include "CandD_proto.h"
#include "Common_proto.h"
#include "HostOS_proto.h"


#endif // __candd_gpl_hdr_h__

