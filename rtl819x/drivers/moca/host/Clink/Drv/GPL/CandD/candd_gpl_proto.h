/*******************************************************************************
*
* GPL/CandD/candd_gpl_proto.h
*
* Description: Linux Ethernet Driver types
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



/* Do Not Edit! The following contents produced by script.  Thu Jan 22 16:27:49 HKT 2009  */


/*** public prototypes from GPL/CandD/CandD.c ***/
int memory_init(void);
void memory_exit(void);
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, 
                    size_t count, loff_t *f_pos);
ssize_t memory_write(   struct file *filp, 
                        const char *buf, 
                        size_t count, 
                        loff_t *f_pos);
int ioctl_operation(struct inode  *fs_inode, 
                    struct file   *filp, 
                    unsigned int  cmd, 
                    unsigned long argument);
int ioctl_operation_work(   struct inode *fs_inode, 
                            struct file *filp, 
                            unsigned int cmd, 
                            unsigned long argument);
unsigned long int rand(int init);
void setup_MDIOClockOut16Bit(void);
void MDIOClockOut16Bits(unsigned int val);
void MDIOClockOutPreamble(void);
unsigned int MDIOClockIn16Bit(int data_bit);
unsigned int ClinkReadMDIOData(int reg_addr);
void ClinkWriteMDIOData(int reg_addr,unsigned val);
void setup_SMIClockOut16Bit(void);
unsigned int SMI_operation(int operation, int addr,unsigned int val);
void ClinkWaitMDIOReady(void);
unsigned long ClinkReadFrom(unsigned long addr);
void ClinkWriteTo(unsigned long addr, unsigned long data);
void Turbo_open(unsigned long addr);
void Turbo_write(unsigned long data);
unsigned int Turbo_read(void);
void Turbo_close(void);
void setup_gpio_signals(void);

/*** public prototypes from GPL/CandD/mdio.c ***/
int clnk_write( void *vctx, SYS_UINT32 addr, SYS_UINT32 data);
int clnk_read( void *vctx, SYS_UINT32 addr, SYS_UINT32 *data);
