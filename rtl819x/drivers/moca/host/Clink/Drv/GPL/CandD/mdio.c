/*******************************************************************************
*
* GPL/CandD/mdio.c
*
* Description: High level MDIO access
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

#include "candd_gpl_hdr.h"

#if 0 /* ACTIONTEC_MOCA */
unsigned long ClinkReadFrom(unsigned long addr);
void ClinkWriteTo(unsigned long addr, unsigned long data);
void Turbo_open(unsigned long addr);
void Turbo_close(void);
void Turbo_write(unsigned long data);
unsigned int Turbo_read(void);
#else
#include "aei_mdio.h"

#ifndef phy_reg_read
#define phy_reg_read rtl_gmac_mii_read
#endif

#ifndef phy_reg_write
#define phy_reg_write rtl_gmac_mii_write
#endif

unsigned int ClinkReadMDIOData(int reg_addr)
{
    uint16_t v16=0;

    v16 = phy_reg_read(0, CANDD_PHYID, reg_addr);

    return (unsigned int)v16;
}

void ClinkWriteMDIOData(int reg_addr, unsigned val)
{
    uint16_t v16 = (uint16_t)val;
    phy_reg_write(0, CANDD_PHYID, reg_addr, v16);
}

unsigned long ClinkReadFrom(unsigned long addr)
{
    uint16_t v16_high;
    uint16_t v16_low;
    uint32_t v32;

    //reset_udelay_num();

    v16_high = addr >> 16;
    v16_low = addr & 0xffff;
    phy_reg_write(0, CANDD_PHYID, ADDRESS_HIGH, v16_high);
    phy_reg_write(0, CANDD_PHYID, ADDRESS_LOW, v16_low);
    v16_high = CLINK_START_READ;
    phy_reg_write(0, CANDD_PHYID, ADDRESS_MODE, v16_high);

    v16_high = phy_reg_read(0, CANDD_PHYID, DATA_HIGH);
    v32 = v16_high << 16;
    v16_high = phy_reg_read(0, CANDD_PHYID, DATA_LOW);
    v32 |= v16_high;

    return v32;
}

void ClinkWriteTo(unsigned long addr, unsigned long data)
{
    uint16_t v16_high, v16_low;

    //reset_udelay_num();

    v16_high = addr >> 16;
    v16_low = addr & 0xffff;
    phy_reg_write(0, CANDD_PHYID, ADDRESS_HIGH, v16_high);
    phy_reg_write(0, CANDD_PHYID, ADDRESS_LOW, v16_low);

    v16_high = data >> 16;
    v16_low = data & 0xffff;
    phy_reg_write(0, CANDD_PHYID, DATA_HIGH, v16_high);
    phy_reg_write(0, CANDD_PHYID, DATA_LOW, v16_low);

    v16_high = CLINK_START_WRITE;
    phy_reg_write(0, CANDD_PHYID, ADDRESS_MODE, v16_high);

}

void Turbo_open(unsigned long addr)
{
    ClinkWriteMDIOData(ADDRESS_HIGH, (addr >> 16)  & 0xffff);
    ClinkWriteMDIOData(ADDRESS_LOW, addr);
    ClinkWriteMDIOData(ADDRESS_LOW, addr);
}

void Turbo_close(void)
{
    ClinkWriteMDIOData(ADDRESS_MODE, 0);
}

void Turbo_write(unsigned long data)
{
    ClinkWriteMDIOData(DATA_HIGH, (data >> 16)  & 0xffff);
    ClinkWriteMDIOData(DATA_LOW, data  & 0xffff);
    ClinkWriteMDIOData(ADDRESS_MODE, CLINK_START_WRITE | CLINK_AUTO_INC);
}

unsigned int Turbo_read(void)
{
    unsigned int data = 0;
    unsigned int retVal = 0;

    ClinkWriteMDIOData(ADDRESS_MODE, CLINK_START_READ | CLINK_AUTO_INC);

    data = ClinkReadMDIOData(DATA_HIGH);
    retVal = data << 16;
    data = ClinkReadMDIOData(DATA_LOW);
    retVal |= data & 0xFFFF;

    return retVal;
}
#endif /* ACTIONTEC_MOCA */

/*
*PUBLIC*******************************************************************************/
int clnk_write( void *vctx, SYS_UINT32 addr, SYS_UINT32 data)
{
    ClinkWriteTo( addr, data );

    return(SYS_SUCCESS);
}


/*
*PUBLIC*******************************************************************************/
int clnk_read( void *vctx, SYS_UINT32 addr, SYS_UINT32 *data)
{
    SYS_UINT32      ret;
    
    ret = ClinkReadFrom( addr );
    *data = ret;

    return(SYS_SUCCESS);
}

/*
*P UBLIC*******************************************************************************/
int clnk_write_burst( void *vctx, SYS_UINT32 addr, SYS_UINT32 *data, unsigned int size, int inc )
{
    unsigned int    i;
   
    Turbo_open(addr);

    for(i = 0; i < size; i += sizeof(*data))
    {
        Turbo_write(*data);
        data++;
    }

    Turbo_close();

    return(SYS_SUCCESS);
}

/*
*P UBLIC*******************************************************************************/
int clnk_read_burst( void *vctx, SYS_UINT32 addr, SYS_UINT32 *data, unsigned int size, int inc)
{
    unsigned int    i;

    Turbo_open(addr);

    for(i = 0; i < size; i += sizeof(*data))
    {
        *data = Turbo_read();
        data++;
    }

    Turbo_close();

    return(SYS_SUCCESS);
}

