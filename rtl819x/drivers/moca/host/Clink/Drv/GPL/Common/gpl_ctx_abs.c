/*******************************************************************************
*
* GPL/Common/gpl_ctx_abs.c
*
* Description: context abstraction
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
*                                N O T E                                       *
********************************************************************************/



/*******************************************************************************
*                            # I n c l u d e s                                 *
********************************************************************************/

#include "gpl_hdr.h"


/*
    The parameter  CONTEXT_DEBUG   must be defined to enable context
    linkage checking. With it defined you'll get error checking and
    messages about null context linkage pointers.
*/
#define CONTEXT_DEBUG       0   // define 1 for context linkage checking






/**
*  Purpose:    Converts a driver kernel context pointer to
*              a driver data context pointer.
*
*  Imports:    dkcp - pointer to a device kernel context
*
*  Exports:    pointer to the driver data context
*
*PUBLIC**************************/
void *dk_to_dd( void *dkcp )
{
    dd_context_t *ddcp ;

#if CONTEXT_DEBUG
    if( dkcp ) {
        ddcp = ((dk_context_t *)dkcp)->priv;
    } else {
        HostOS_PrintLog(L_INFO, "Error - context linkage dk_to_dd\n" );
        ddcp = 0 ;
    }
#else

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 31)
    ddcp = netdev_priv(dkcp);
#else
    ddcp = ((dk_context_t *)dkcp)->priv;
#endif

#endif
    return( ddcp ) ;
}

/**
*  Purpose:    Converts a driver data context pointer to
*              a driver gpl context pointer.
*
*  Imports:    ddcp - pointer to a device data context
*
*  Exports:    pointer to the driver gpl context
*
*PUBLIC**************************/
void *dd_to_dg( void *ddcp )
{
    dg_context_t *dgcp ;

#if CONTEXT_DEBUG
    if( ddcp ) {
        dgcp = ((dd_context_t *)ddcp)->p_dg_ctx;
    } else {
        HostOS_PrintLog(L_INFO, "Error - context linkage dd_to_dg\n" );
        dgcp = 0 ;
    }
#else
    dgcp = ((dd_context_t *)ddcp)->p_dg_ctx ;
#endif

    return( dgcp ) ;
}

/**
*  Purpose:    Converts a driver gpl context pointer to
*              a driver control context pointer.
*
*  Imports:    dgcp - pointer to a device gpl context
*
*  Exports:    pointer to the driver control context
*
*PUBLIC**************************/
void *dg_to_dc( void *dgcp )
{
    void *dccp ;

#if CONTEXT_DEBUG
    if( dgcp ) {
        dccp = ((dg_context_t *)dgcp)->p_dc_ctx;
    } else {
        HostOS_PrintLog(L_INFO, "Error - context linkage dg_to_dc\n" );
        dccp = 0 ;
    }
#else
    dccp = ((dg_context_t *)dgcp)->p_dc_ctx ;
#endif

    return( dccp ) ;
}

/**
*  Purpose:    Converts a driver gpl context pointer to
*              a driver data context pointer.
*
*  Imports:    dgcp - pointer to a device gpl context
*
*  Exports:    pointer to the driver data context
*
*PUBLIC**************************/
void *dg_to_dd( void *dgcp )
{
    dd_context_t *ddcp ;

#if CONTEXT_DEBUG
    if( dgcp ) {
        ddcp = ((dg_context_t *)dgcp)->p_dd_ctx;
    } else {
        HostOS_PrintLog(L_INFO, "Error - context linkage dg_to_dd\n" );
        ddcp = 0 ;
    }
#else
    ddcp = ((dg_context_t *)dgcp)->p_dd_ctx ;
#endif

    return( ddcp ) ;
}



////////////////////////////////////////////////////////////
// === combo functions
////////////////////////////////////////////////////////////

/**
 *  Purpose:    Converts a driver kernel context pointer to
 *              a driver control context pointer.
 *
 *  Imports:    dkcp - pointer to a device kernel context
 *
 *  Exports:    pointer to the driver control context
 *
*PUBLIC**************************/
void *dk_to_dc( void *dkcp )
{
    void *ddcp ;
    void *dgcp ;
    void *dccp ;

#if CONTEXT_DEBUG
    dccp = 0 ;
    if( dkcp ) {
        ddcp = dk_to_dd( dkcp ) ;
        if( ddcp ) {
            dgcp = dd_to_dg( ddcp ) ;
            if( dgcp ) {
                dccp = dg_to_dc( dgcp ) ;
            }
        }
    }
    if( !dccp ) {
        HostOS_PrintLog(L_INFO, "Error - context linkage dk_to_dc\n" );
    }
#else
    ddcp = dk_to_dd( dkcp ) ;
    dgcp = dd_to_dg( ddcp ) ;
    dccp = dg_to_dc( dgcp ) ;
#endif

    return( dccp ) ;
}

/**
 *  Purpose:    Converts a driver data context pointer to
 *              a driver control context pointer.
 *
 *  Imports:    dkcp - pointer to a device kernel context
 *
 *  Exports:    pointer to the driver control context
 *
*PUBLIC**************************/
void *dd_to_dc( void *ddcp )
{
    void *dgcp ;
    void *dccp ;

#if CONTEXT_DEBUG
    dccp = 0 ;
    if( ddcp ) {
        dgcp = dd_to_dg( ddcp ) ;
        if( dgcp ) {
            dccp = dg_to_dc( dgcp ) ;
        }
    }
    if( !dccp ) {
        HostOS_PrintLog(L_INFO, "Error - context linkage dd_to_dc\n" );
    }
#else
    dgcp = dd_to_dg( ddcp ) ;
    dccp = dg_to_dc( dgcp ) ;
#endif

    return( dccp ) ;
}

/**
 *  Purpose:    Converts a driver kernel context pointer to
 *              a driver gpl context pointer.
 *
 *  Imports:    dkcp - pointer to a device kernel context
 *
 *  Exports:    pointer to the driver gpl context
 *
*PUBLIC**************************/
void *dk_to_dg( void *dkcp )
{
    void *ddcp ;
    void *dgcp ;

#if CONTEXT_DEBUG
    dgcp = 0 ;
    if( dkcp ) {
        ddcp = dk_to_dd( dkcp ) ;
        if( ddcp ) {
            dgcp = dd_to_dg( ddcp ) ;
        }
    }
    if( !dgcp ) {
        HostOS_PrintLog(L_INFO, "Error - context linkage dk_to_dg\n" );
    }
#else
    ddcp = dk_to_dd( dkcp ) ;
    dgcp = dd_to_dg( ddcp ) ;
#endif

    return( dgcp ) ;
}

/**
 *  Purpose:    Converts a driver control context pointer to
 *              a driver data context pointer.
 *
 *  Imports:    dccp - pointer to a device control context
 *
 *  Exports:    pointer to the driver data context
 *
*PUBLIC**************************/
void *dc_to_dd( void *dccp )
{
    void *dgcp ;
    void *ddcp ;

#if CONTEXT_DEBUG
    ddcp = 0 ;
    if( dccp ) {
        dgcp = dc_to_dg( dccp ) ;
        if( dgcp ) {
            ddcp = dg_to_dd( dgcp ) ;
        }
    }
    if( !ddcp ) {
        HostOS_PrintLog(L_INFO, "Error - context linkage dc_to_dd\n" );
    }
#else
    dgcp = dc_to_dg( dccp ) ;
    ddcp = dg_to_dd( dgcp ) ;
#endif

    return( ddcp ) ;
}




