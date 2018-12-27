/*******************************************************************************
*
* GPL/Common/gpl_ctx_setup.c
*
* Description: context setup support
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



/*******************************************************************************
*            S t a t i c   M e t h o d   P r o t o t y p e s                   *
********************************************************************************/

/*******************************************************************************
*           
* Purpose:  Allocates and clears a DG
*    
* Inputs:
*
* Outputs:  the context
*
*PUBLIC***************************************************************************/
void *ctx_alloc_dg_context( void )
{
    dg_context_t *dgcp ;

    dgcp = (dg_context_t *)HostOS_Alloc(sizeof(dg_context_t));
    if( dgcp )
    {
        HostOS_Memset(dgcp, 0, sizeof(dg_context_t));
    }

    return(dgcp);
}

/*******************************************************************************
*           
* Purpose:  Frees a DG
*    
* Inputs:   dgcp - the DG context to free
*
* Outputs:  
*
*PUBLIC***************************************************************************/
void ctx_free_dg_context( void *dgcp )
{

    HostOS_Free( dgcp, sizeof(dg_context_t) ) ;

}

/*******************************************************************************
*           
* Purpose:  links a DG somewhere
*    
* Inputs:   vdgcp - void pointer to DG context 
*           ddcp  - void pointer to DD context
*           dccp  - void pointer to DC context
*
* Outputs:  
*
*PUBLIC***************************************************************************/
void ctx_link_dg_context( void *vdgcp, void *ddcp, void *dccp )
{
    dg_context_t *dgcp = vdgcp ;

    dgcp->p_dd_ctx = (void *)ddcp ;  // link dg to dd
    dgcp->p_dc_ctx = (void *)dccp;   // link dg to dc
}

/*******************************************************************************
*
* Description:
*       Initializes the OS context.
*
* Inputs:
*       vdgcp        - void Pointer to the OS context.
*
* Outputs:
*       0 = SYS_SUCCESS
*
*PUBLIC***************************************************************************/
void Clnk_init_os_context( void *vdgcp )
{
    dg_context_t *dgcp = vdgcp ;

#ifdef CLNK_HOST_SEM
    HostOS_mutex_init(   &dgcp->at_lock);       // referenced from !GPL side
#else
    HostOS_lock_init(   &dgcp->at_lock);        // referenced from !GPL side
#endif

    HostOS_mutex_init(  &dgcp->ioctl_sem);

#ifdef CLNK_HOST_SEM
    HostOS_mutex_init(   &dgcp->mbx_cmd_lock);   // referenced from !GPL side
    HostOS_mutex_init(   &dgcp->mbx_swun_lock);  // referenced from !GPL side
#else
    HostOS_lock_init(   &dgcp->mbx_cmd_lock);   // referenced from !GPL side
    HostOS_lock_init(   &dgcp->mbx_swun_lock);  // referenced from !GPL side
#endif
}

/*******************************************************************************
*
* Description:
*       Initializes the control context.
*
* Inputs:
*       dccp        - Pointer to the control context.
*       dkcp        - Pointer to the kernel context.
*       dev_base    - Device base address
*
* Outputs:
*       0 = SYS_SUCCESS
*
*PUBLIC***************************************************************************/
void ctx_linkage_lister( void *dkcp, unsigned int **vp, int len )
{
    dg_context_t *dgcp = dk_to_dg( dkcp ) ;

    *vp++ = (unsigned int *)&dgcp->at_lock;
    *vp++ = (unsigned int *)&dgcp->ioctl_sem;
    *vp++ = (unsigned int *)&dgcp->mbx_cmd_lock;
    *vp++ = (unsigned int *)&dgcp->mbx_swun_lock;
}

