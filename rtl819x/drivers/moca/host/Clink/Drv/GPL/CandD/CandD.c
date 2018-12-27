/*******************************************************************************
*
* GPL/CandD/CandD.c
*
* Description: C and D driver main
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


/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18)
#include <linux/autoconf.h>
#else
#include <linux/config.h>
#endif
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/sockios.h>


#include "candd_gpl_hdr.h"


MODULE_LICENSE("Dual BSD/GPL");




// definitions


#define VERSION "CandD Version 1.22"

#define K_BUF_SIZE  512

#define G_IO_START  0xc8004000



#define reverse_endian(b)       ((b) >= 8 ? (b)+8 : (b)+24)
#define reverse_endian_bit(b)   (1 << reverse_endian(b))

//------------------------------------------------------------------------------
// Define Ports used
//------------------------------------------------------------------------------
#define MDIO_PORT           0
#define MDIO_CONFIG_PORT    4
#define MDIO_INPUT_PORT     8

//------------------------------------------------------------------------------
// Define Bits in the GPIO ports
//------------------------------------------------------------------------------
#define MDIO_CLOCK_BIT  reverse_endian_bit(4)
#define MDIO_DATA_BIT   reverse_endian_bit(5)

#define SMI_CLOCK_BIT   reverse_endian_bit(6)
#define SMI_DATA_BIT    reverse_endian_bit(7)

#define YELLOW          reverse_endian_bit(0)
#define GREEN           reverse_endian_bit(1)
#define IXP_MII_DIS     reverse_endian_bit(2)
#define SoC_RESET       reverse_endian_bit(3)
#define SWITCH_RESET    reverse_endian_bit(8)
#define SWITCH_INTR     reverse_endian_bit(9)
#define BUTTON          reverse_endian_bit(10)
#define SOC_XMII_DIS    reverse_endian_bit(11)
#define DIPLEXER        reverse_endian_bit(12)
#define CPU_RESET       reverse_endian_bit(13)


#define __PHY_ADDRESS           0x01


#define MDIO_START_BITS         (0x01 << (30-16))
#define MDIO_OP_READ            (0x02 << (28-16))
#define MDIO_OP_WRITE           (0x01 << (28-16))
#define MDIO_READ_TURN_AROUND   (0x03 << (16-16))
#define MDIO_WRITE_TURN_AROUND  (0x02 << (16-16))

#define MDIO_READ_MASK          (MDIO_START_BITS | MDIO_OP_READ  | PHY_ADDRESS << (23-16))
#define MDIO_WRITE_MASK         (MDIO_START_BITS | MDIO_OP_WRITE | PHY_ADDRESS << (23-16))  
#define MDIO_REG_PLACEMENT      (18-16)

#define ADDRESS_MODE            0x1b
#define ADDRESS_HIGH            0x1c
#define ADDRESS_LOW             0x1d
#define DATA_HIGH               0x1e
#define DATA_LOW                0x1f

#define _BV(n)                  (1 << (n))

#define CLINK_START_WRITE       _BV(0)
#define CLINK_START_READ        _BV(1)
#define CLINK_AUTO_INC          _BV(2)
#define CLINK_BUSY              _BV(3)
#define CLINK_ERROR             _BV(4)

#define PHY_READ    (MDIO_START_BITS | MDIO_OP_READ  | MDIO_READ_TURN_AROUND)
#define PHY_WRITE   (MDIO_START_BITS | MDIO_OP_WRITE | MDIO_WRITE_TURN_AROUND)




//
// structures and definitions
//

/* This structure is used in all SIOCxMIIxxx ioctl calls */
struct mmi_ioctl_data {
        uint16_t                phy_id;
        uint16_t                reg_num;
        uint16_t                val_in;
        uint16_t                val_out;
};

struct ioctl_stuff
{
    char                        name[16];
    union ioctl_data 
    {
        unsigned int            *ptr;
        struct mmi_ioctl_data   mmi;
        unsigned char           MAC[8];
    } dat;
};






/* Global variables of the driver */

/* Major number */
int memory_major = 60;
/* Buffer to store data */
char *memory_buffer;        // allocated
int in_use;
int quiet       = 0;
int ultra_quiet = 0;
int verbose     = 0;    // Decodes and displays Sonics bus traffic

int driver_open = 0 ;   // flag indicating an open call has been made
                        // a close must be called before another open
void *pprivate_data=0;
//------------------------------------------------------------------------------
// This is the location of the gpio registers 
//------------------------------------------------------------------------------
unsigned long G_io;
void *G_base;

unsigned long int seed;
unsigned long int mdio[4];

unsigned long int verbose_addr;
unsigned long int verbose_data;
int               read_state_machine;
unsigned long int verbose_addr_read;
int               auto_count;

int port_select;
int val;

int in_comment;
int reset_command;

int PHY_ADDRESS = __PHY_ADDRESS;






// prototypes

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
static ssize_t memory_write_work(   struct file *filp, 
                                    const char *buf, 
                                    size_t count, 
                                    loff_t *f_pos) ;
int ioctl_operation(struct inode  *fs_inode, 
                    struct file   *filp, 
                    unsigned int  cmd, 
                    unsigned long argument);
int ioctl_operation_work(   struct inode *fs_inode, 
                            struct file *filp, 
                            unsigned int cmd, 
                            unsigned long argument);
static SYS_INT32 netdev_ethtool_ioctl(SYS_VOID *useraddr);
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





/* Structure that declares the usual file */
/* access functions */
struct file_operations memory_fops = {
  .read    = memory_read,
  .write   = memory_write,
  .open    = memory_open,
  .release = memory_release,
  .ioctl   = ioctl_operation
};

/* Declaration of the init and exit functions */
module_init(memory_init);
module_exit(memory_exit);






/*
*     module entry point
*     initializes the module for the first open
*
*
*
*
*PUBLIC***/
int memory_init(void) 
{
    int result;
    printk(KERN_ALERT "%s\n",VERSION);

  /* Registering device */
    result = register_chrdev(memory_major, "memory", &memory_fops);
    if (result < 0) 
    {
        printk(KERN_ALERT "memory: cannot obtain major number %d\n", memory_major);
        return result;
    }

  /* Allocating memory for the buffer */
    memory_buffer = kmalloc(K_BUF_SIZE, GFP_KERNEL); 
    if (!memory_buffer) 
    { 
        result = -ENOMEM;
        printk(KERN_ALERT "DIAG: Failure to get memory buffer %d\n",memory_major);
        goto fail; 
    } 
    memset(memory_buffer, 0, K_BUF_SIZE);
    in_use=0;
    reset_command = 0;
    G_io = G_IO_START ;
    if (!request_region(G_io,4*4,"GPIO"))
    {
        printk(KERN_ALERT "DIAG: Sorry - memory mapped address is in use\n");
        result = -EBUSY;
        goto fail;
    }
    G_base=ioremap(G_io,4*4);
    
    return 0;

fail: 
    memory_exit(); 
    return result;
}


/*
*    module exit point
*    tears everything down
*
*
*
*
*PUBLIC***/
void memory_exit(void) 
{


    /* Freeing the major number */
    unregister_chrdev(memory_major, "memory");

    /* Freeing buffer memory */
    if (memory_buffer) {
        kfree(memory_buffer);
    }

}



/*
*
*STATIC***/
static void *context_create( void ) 
{
    int size ;
    void *m ;
    dk_context_t *dkcp ;
    dd_context_t *ddcp ;

    size = sizeof(dk_context_t) + sizeof(dd_context_t) ;
    m = HostOS_Alloc( size ) ;
    if( m ) {
        HostOS_Memset( m, 0, size ) ;     
        dkcp = (dk_context_t *)m ;
        ddcp = (dd_context_t *)(((char *)m)+sizeof(dk_context_t)) ;
        dkcp->priv = (void *)ddcp ;
        dkcp->name = MOCA_DRV_NAME ;    // like net_device
    }
    return( m ) ;
}

/*
*
*STATIC***/
static void context_destroy( dk_context_t *dkcp ) 
{
    int size ;

    size = sizeof(dk_context_t) + sizeof(dd_context_t) ;
    HostOS_Free( dkcp, size ) ; 
}



/*
*    module's file open entry point
*
*
*
*
*
*PUBLIC***/
int memory_open(struct inode *inode, struct file *filp) 
{
    int          err ;
    dk_context_t *dkcp ;
    dd_context_t *ddcp ;

    err = 0 ;
    if( !driver_open ) {

        // clink init

        // create dk and dd contexts

        filp->private_data = context_create( ) ; 
        // Sotre the context pointer to a global pointer, so all of users opening this driver can share it.
        pprivate_data = filp->private_data;
        if( filp->private_data ) 
        {
            dkcp = (dk_context_t *)filp->private_data ;
            ddcp = dk_to_dd( dkcp ) ;

            // create dg and dc contexts

            err = Clnk_init_dev( &ddcp->p_dg_ctx, ddcp, dkcp, (unsigned long)G_base );
            if( err ) 
            {
                err = 1 ;    // Error: no memory
            } 

        }
        else 
        {
            err = 1 ;    // Error: no memory
        }

    }
    if(!err)
    {
       driver_open ++;
       if(pprivate_data)
       {
           filp->private_data = pprivate_data;
       }
       else
       {
           err = 1;
       }
    }
    return( err ) ;
}



/*
*    module's file close entry point
*
*
*
*
*
*
*PUBLIC***/
int memory_release(struct inode *inode, struct file *filp) 
{
    int err ;
    dk_context_t *dkcp ;
    dd_context_t *ddcp ;
    void         *dgcp ;

    err = 0 ;
    if( driver_open == 1) {

        // clink exit

        dkcp = (dk_context_t *)filp->private_data ;
        if( dkcp ) {

            // close and free dg and dc contexts 

            ddcp = dk_to_dd( dkcp ) ;
            dgcp = dd_to_dg( ddcp ) ;
            Clnk_exit_dev( dgcp );
            ddcp->p_dg_ctx = 0 ;

            // free dk and dd contexts 

            context_destroy( dkcp ) ;
            filp->private_data = 0 ;
        }
        pprivate_data = 0;
        driver_open = 0 ;
    }
    if(driver_open)
	    driver_open --;

    return( err ) ;
}


/*
*    module's file read entry point
*
*
*
*
*
*
*PUBLIC***/
ssize_t memory_read(struct file *filp, char *buf, 
                    size_t count, loff_t *f_pos) 
{
    int i;

    if( driver_open ) {

        for (i=0;i<4;i++)
        {
            printk(KERN_ALERT "GPIO[%2d] = %04x\n",i,readw(G_base + i*4 + 2));
        }
     
        /* Transfering data to user space */ 
        HostOS_copy_to_user(buf,memory_buffer,1);

        /* Changing reading position as best suits */ 
        if (*f_pos == 0) { 
            *f_pos+=1; 
            return 1; 
        } else { 
            return 0; 
        }

    } else {
        return( 0 ) ;   // Error: not already open
    }
}

/*
*    module's file write entry point
*
*
*
*
*
*
*PUBLIC***/
ssize_t memory_write(   struct file *filp, 
                        const char *buf, 
                        size_t count, 
                        loff_t *f_pos) 
{
    ssize_t s ;

    if( driver_open ) {

        s = memory_write_work(  filp, buf, count, f_pos) ;
        return( s ) ;

    } else {
        return( 0 ) ;   // Error: not already open
    }
}

/*
*    module's file write work entry point
*
*
*
*
*
*
*STATIC***/
static ssize_t memory_write_work(   struct file *filp, 
                                    const char *buf, 
                                    size_t count, 
                                    loff_t *f_pos) 
{
    int j;
    unsigned long int temp;

    HostOS_copy_from_user(memory_buffer,buf,in_use=K_BUF_SIZE < count ? K_BUF_SIZE : count);
    for( j = 0 ; j < in_use ; j++ )
    {
        if (in_comment)
        {
            if (memory_buffer[j] == '\n')
                in_comment=0;
            continue;
        }
        switch (memory_buffer[j])
        {
            case 'P':
                port_select=val;
                val=0;
                break;
            case 'C':
                val=0;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                val=(val << 4);
                val += memory_buffer[j] - '0';
                if (!quiet) printk(KERN_ALERT "DIAG: val=%08x\n",val);
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                val=val << 4;
                val += memory_buffer[j] - 'a' + 10;
                if (!quiet) printk(KERN_ALERT "DIAG: val=%08x\n",val);
                break;
            case '|':
                writel(temp=val | readl(G_base + port_select) ,G_base + port_select);
                if (!quiet) printk(KERN_ALERT "New value at %x = %08lx\n",port_select,temp);
                break;
            case '&':
                writel(temp=val & readl(G_base + port_select) ,G_base + port_select);
                if (!quiet) printk(KERN_ALERT "New value at %x = %08lx\n",port_select,temp);
                break;
            case '^':
                writel(temp=val ^ readl(G_base + port_select) ,G_base + port_select);
                if (!quiet) printk(KERN_ALERT "New value at %x = %08lx\n",port_select,temp);
                break;
            case 'B':
                if (!quiet) printk(KERN_ALERT "DIAG: Write BYTE value %x to port %08x\n",val,(unsigned) G_base + port_select);
                writeb(val,G_base + port_select);
                break;
            case 'W':
                if (!quiet) printk(KERN_ALERT "DIAG: Write WORD value %x to port %08x\n",val,(unsigned) G_base + port_select);
                writew(val,G_base + port_select);
                break;
            case 'L':
                if (!quiet) printk(KERN_ALERT "DIAG: Write LONG value %x to port %08x\n",val,(unsigned) G_base + port_select);
                writel(val,G_base + port_select);
                break;
            case 'V':
                if (val == 1)
                {
                    val=readb(G_base + port_select);
                }
                else if (val == 2)
                {
                    val=readw(G_base + port_select);
                }
                else if (val == 4)
                {
                    val=readl(G_base + port_select);
                }
                if (!quiet) printk(KERN_ALERT "DIAG: Read at %x gives us %x\n",(unsigned) (port_select + G_base), val);
                break;
            case '!':
                setup_gpio_signals();
                break;
            case 'Z':
                ClinkWriteMDIOData(port_select,val);
                if (!quiet) printk(KERN_ALERT "Wrote to Zip2[%2x] = %4x\n",port_select,val);
                break;
            case 'z':
                val=ClinkReadMDIOData(port_select);
                if (!quiet) printk(KERN_ALERT "Read Zip2[%2x] = %04x\n",port_select,val);
                break;
            case 'S':
                ClinkWriteTo(port_select, val);
                if (!quiet) printk(KERN_ALERT "Write to Sonics[%08x] = %08x\n",port_select,val);
                break;
            case 's':
                val=ClinkReadFrom(port_select);
                if (!quiet) printk(KERN_ALERT "Read from Sonics[%08x] = %08x\n",port_select,val);
                break;
            case 'T':
                SMI_operation(PHY_WRITE, port_select,val);
                if (!quiet) printk(KERN_ALERT "Write to Tantos[%04x] = %04x\n",port_select,val);
                break;
            case 't':
                val=SMI_operation(PHY_READ, port_select,val);
                if (!quiet) printk(KERN_ALERT "Read from Tantos[%04x] = %04x\n",port_select,val);
                break;
            case '-':
                PHY_ADDRESS=val;
                break;
            case '=':
                val = reverse_endian_bit(val);
                if (!ultra_quiet) printk(KERN_ALERT "Value = %08x\n",val);
                break;
            case '~':
                val = ~val;
                if (!ultra_quiet) printk(KERN_ALERT "Value = %08x\n",val);
                break;
            case '#':
                in_comment=1;
                break;
            case '+':
                port_select += 4;
                if (!quiet) printk(KERN_ALERT "ADDR = %08x\n",port_select);
                break;
            case 'q':
                quiet ^= 1;
                printk(KERN_ALERT "Quiet mode = %d\n",quiet);
                break;
            case 'Q':
                ultra_quiet ^= 1;
                printk(KERN_ALERT "Ultra quiet mode = %d\n",ultra_quiet);
                break;
            case 'v':
                verbose ^= 1;
                printk(KERN_ALERT "Verbose mode = %d\n",verbose);
                break;
            case '%':
                temp=val;
                if (val==0)
                    rand(1);
                while (temp--)                     
                {
                    ClinkWriteTo(port_select,rand(0));
                    port_select+=4;
                }
                break;
            case '$':
                temp=val;
                while (temp--)
                {
                    if (ClinkReadFrom(port_select) != (val=rand(0)))
                    {
                        if (!quiet) printk(KERN_ALERT "Bad value at %08x (%08lx != %08x)\n",port_select,
                            ClinkReadFrom(port_select),val);
                    }
                    port_select+=4;
                }
                break;
            case 'r':
                reset_command=val;
                break;
            case '?':
                printk(KERN_ALERT "%s\n",VERSION);
                printk(KERN_ALERT "   0-9 a-f = hex values\n");
                printk(KERN_ALERT "   P = Set port to value\n");
                printk(KERN_ALERT "   C = Clean value\n");
                printk(KERN_ALERT "   B = Byte write\n");
                printk(KERN_ALERT "   W = Word write\n");
                printk(KERN_ALERT "   L = Long Word write\n");
                printk(KERN_ALERT "   V = Read word, byte, or long\n");
                printk(KERN_ALERT "   ! = Setup GPIO signals\n");
                printk(KERN_ALERT "   Z = Zip 2 write phy\n");
                printk(KERN_ALERT "   z = Zip 2 read phy\n");
                printk(KERN_ALERT "   S = Sonics bus write\n");
                printk(KERN_ALERT "   s = Sonics bus read\n");
                printk(KERN_ALERT "   T = Tantos Write\n");
                printk(KERN_ALERT "   t = Tantos Read\n");
                printk(KERN_ALERT "   - = Change phy address\n");
                printk(KERN_ALERT "   | = or bits (long write\n");
                printk(KERN_ALERT "   & = and bits (long write\n");
                printk(KERN_ALERT "   ^ = xor bits (long write\n");
                printk(KERN_ALERT "   ~ = Invert bits\n");
                printk(KERN_ALERT "   = = Xlate bit number\n");
                printk(KERN_ALERT "   + = Increment address by 4\n");
                printk(KERN_ALERT "   # = Comment to end of line\n");
                printk(KERN_ALERT "   q = Quiet mode toggle for BIT bang operations\n");
                printk(KERN_ALERT "   Q = Quiet mode toggle for PHY read / write operations\n");
                printk(KERN_ALERT "   v = Verbose mode toggle to see Sonics bus actions\n");
                printk(KERN_ALERT "   %% = Memory test write (val=times, port=addr)\n");
                printk(KERN_ALERT "   $ = Memory test read (val=times, port=addr)\n");
                printk(KERN_ALERT "   r = Set reset command for SoC to current value\n");
                break;
            case ' ':
                break;
            default:
                printk(KERN_ALERT "Character %x (%c) ignored\n",memory_buffer[j] & 0xff,
                    ' ' <= memory_buffer[j] && memory_buffer[j] <= '~' ? memory_buffer[j] : '?' );
                break;
        }
    }
    return in_use;
}


/*
* Purpose:  Driver's IOCTL entry point.
*           Note that this is a character device.
*
* Imports:  fs_inode - file system inode of the open file
*           filp     - file pointer of the open file
*           cmd      - command to process
*           argument - data for the command
*
* Exports:  0 = success
*
*PUBLIC******************************************************/
int ioctl_operation(struct inode  *fs_inode, 
                    struct file   *filp, 
                    unsigned int  cmd, 
                    unsigned long argument)
{
    int             status;
    dk_context_t    *dkcp ;
    struct ifreq    *ifr   = (struct ifreq *)argument;
    void            *arg   = (void *)ifr->ifr_data ;

    status = 0 ;
    if( driver_open ) {

        dkcp = (dk_context_t *)filp->private_data ;

        switch (cmd)
        {
            // CandD char driver IOCTLs
            case SIOCETHTOOL:       // Ethernet tool
            case SIOCGMIIPHY:       // Get PHY address
            case SIOCGIFHWADDR:     // Get hardware address
            case SIOCHDRCMD:        // Resets the SoC , Control the diplexer switch etc
            case SIOCGMIIREG:       // PHY read request
            case SIOCSMIIREG:       // PHY write request
                status = ioctl_operation_work( fs_inode, filp, cmd, argument ) ;
                break ;
            // en2210 mailbox IOCTLS
            case SIOCCLINKDRV :     // Control plane commands for the driver
                status = clnkioc_driver_cmd( dkcp, arg ) ;
                break ; 
            case SIOCGCLINKMEM :    // Reads registers/memory in c.LINK address space
                status = clnkioc_mem_read( dkcp, arg ) ;
                break ; 
            case SIOCSCLINKMEM :    // Sets registers/memory in c.LINK address space
                status = clnkioc_mem_write( dkcp, arg ) ;
                break ; 
            case SIOCGCLNKCMD :     // mbox cmmds: request with response
                status = clnkioc_mbox_cmd_request( dkcp, arg, 1 ) ;
                break ;
            case SIOCSCLNKCMD :     // mbox cmmds: request with no response
                status = clnkioc_mbox_cmd_request( dkcp, arg, 0 ) ;
                break ;
            case SIOCLNKDRV :       // mbox cmmds: retrieve unsol messages
                status = clnkioc_mbox_unsolq_retrieve( dkcp, arg ) ;
                break ;
            default:
                // here has different with ECB3M-L3 HIRF branch, need confirm
                return -EOPNOTSUPP;
        }
    } else {
        status = 1; // Error: not already open
    }
    return(status);
}


/*
*
*
*
*
*
*
*PUBLIC***/
int ioctl_operation_work(   struct inode *fs_inode, 
                            struct file *filp, 
                            unsigned int cmd, 
                            unsigned long argument)
{
    struct ioctl_stuff  wk;

    HostOS_copy_from_user((char *) &wk,(char *)argument,sizeof(struct ioctl_stuff));
    switch (cmd)
    {
        case SIOCETHTOOL:       // Ethernet tool
			netdev_ethtool_ioctl(wk.dat.ptr);
            HostOS_copy_to_user((char *) argument,(char *) &wk,sizeof(struct ioctl_stuff));
			return 0;
        case SIOCGMIIPHY:       // Get PHY address
            wk.dat.mmi.phy_id=7;
            HostOS_copy_to_user((char *) argument,(char *) &wk,sizeof(struct ioctl_stuff));
            return(0);
        case SIOCGIFHWADDR:     // Get hardware address

            //------------------------------------------------------
            // NOTE: This mac address is hard coded and will have
            // to be modified to read from the file system to get
            // the correct MAC address.
            //------------------------------------------------------
            wk.dat.MAC[0+2]=0x00;
            wk.dat.MAC[1+2]=0x09;
            wk.dat.MAC[2+2]=0x8b;
            wk.dat.MAC[3+2]=0x30;
            wk.dat.MAC[4+2]=0x40;
            wk.dat.MAC[5+2]=0x50;
            HostOS_copy_to_user((char *) argument,(char *) &wk,sizeof(struct ioctl_stuff));
            return(0);
        case SIOCHDRCMD:        // Resets the SoC , Control the diplexer switch etc
            if (!quiet)
                printk(KERN_ALERT "DIAG - IOCTL REQUEST TO CONTROL LOW LEVEL LINES %x (reset_command base=%d)\n",
                       (unsigned int) (wk.dat.ptr),reset_command);
            //------------------------------------------------------
            // Reset line items - we must reset the PLL before SoC. 
            //------------------------------------------------------
            if ((int) (wk.dat.ptr) == reset_command+1) // Hold SoC
            {
                if (!quiet) printk(KERN_ALERT "SoC held in reset\n");
                writel(~0x08000000 & readl(G_base + 0) ,G_base + 0);
            }
            if ((int) (wk.dat.ptr) == reset_command) // Run SoC
            {
                if (!quiet) printk(KERN_ALERT "SoC running\n");
                writel( 0x08000000 | readl(G_base + 0) ,G_base + 0);
            }
            //------------------------------------------------------
            // Diplexer Control items
            //------------------------------------------------------
            if ((int) (wk.dat.ptr) == 2)
                writel(~0x00100000 & readl(G_base + 0) ,G_base + 0);
            if ((int) (wk.dat.ptr) == 3)
                writel( 0x00100000 | readl(G_base + 0) ,G_base + 0);

			if ((int) (wk.dat.ptr) == reset_command+4) // Hold PLL
				{
                if (!quiet) printk("<1>" "PLL held in reset\n");
				writel(~0x00010000 & readl(G_base + 0) ,G_base + 0);
				}

				if ((int) (wk.dat.ptr) == reset_command+5) // Run PLL
				{
                if (!quiet) printk("<1>" "PLL running\n");
				writel( 0x00010000 | readl(G_base + 0) ,G_base + 0);
				}
				
            /*Retrive the Switch Reset Line status value*/
            if((int) (wk.dat.ptr) == 0x80)
            {
                if(readl(G_base + 0)&0x00010000)
                {
                    wk.dat.ptr=(unsigned int *)0x88;
                }
                else
                {
                    wk.dat.ptr=(unsigned int *)0xff;
                }
                HostOS_copy_to_user((char *) argument,(char *) &wk,sizeof(struct ioctl_stuff));
            }
            /*Retrive the Diplexer's GPIO Line status value*/
            if((int) (wk.dat.ptr) == 0x81)
            {
                if(readl(G_base + 0)&0x00100000)
                {
                    wk.dat.ptr=(unsigned int *)0x88;
                }
                else
                {
                    wk.dat.ptr=(unsigned int *)0xff;
                }
                HostOS_copy_to_user((char *) argument,(char *) &wk,sizeof(struct ioctl_stuff));
            }

            return(0);
        case SIOCGMIIREG:       // PHY read request
            if (wk.dat.mmi.reg_num < 0x1000)
                wk.dat.mmi.val_out=ClinkReadMDIOData(wk.dat.mmi.reg_num);
            else
            {
                wk.dat.mmi.val_out=SMI_operation(PHY_READ, wk.dat.mmi.reg_num & 0xfff,0);
                if (!ultra_quiet)
                    printk(KERN_ALERT "DIAG - Tantos Read PHY(%x) REG(%x) (%x)\n",wk.dat.mmi.phy_id,wk.dat.mmi.reg_num,wk.dat.mmi.val_out);
            }
            HostOS_copy_to_user((char *) argument,(char *) &wk,sizeof(struct ioctl_stuff));
            if (!ultra_quiet)
                printk(KERN_ALERT "DIAG - Read PHY(%x) REG(%x) (%x)\n",wk.dat.mmi.phy_id,wk.dat.mmi.reg_num,wk.dat.mmi.val_out);
            return(0);
        case SIOCSMIIREG:       // PHY write request
            if (!ultra_quiet)
                printk(KERN_ALERT "DIAG - Write PHY(%x) REG(%x) (%x)\n",wk.dat.mmi.phy_id,wk.dat.mmi.reg_num,wk.dat.mmi.val_in);
            if (wk.dat.mmi.reg_num < 0x1000)
                ClinkWriteMDIOData(wk.dat.mmi.reg_num,wk.dat.mmi.val_in);
            else
            {
                if (!ultra_quiet)
                    printk(KERN_ALERT "DIAG - Tantos Write PHY(%x) REG(%x) (%x)\n",wk.dat.mmi.phy_id,wk.dat.mmi.reg_num,wk.dat.mmi.val_in);
                SMI_operation(PHY_WRITE, wk.dat.mmi.reg_num & 0xfff,wk.dat.mmi.val_in);
            }
            return(0);

        default:
            printk(KERN_ALERT "DIAG: IOCTL operation %x with argument %lx\n",cmd,argument);
            break ;
    }
    return(0);
}

/* STATIC */
static SYS_INT32 netdev_ethtool_ioctl(SYS_VOID *useraddr)
{
    u32 ethcmd;

    if (HostOS_copy_from_user(&ethcmd, useraddr, sizeof(ethcmd)))
	{
		printk(KERN_ALERT "ETHTOOL: failed to copy data from user space\n");
        return -EFAULT;
	}

    switch (ethcmd)
    {
        case ETHTOOL_GDRVINFO:
        {
            struct ethtool_drvinfo info = {ETHTOOL_GDRVINFO};
            HostOS_Memcpy(info.driver, MOCA_DRV_NAME, sizeof(MOCA_DRV_NAME));
            HostOS_Memcpy(info.version, MOCA_DRV_VERSION, sizeof(MOCA_DRV_VERSION));

			printk(KERN_INFO "Driver name: %s, version: %s\n", info.driver, info.version);

            if (HostOS_copy_to_user(useraddr, &info, sizeof(info)))
			{
				printk(KERN_ALERT "ETHTOOL: failed to copy data to user space\n");
                return -EFAULT;
			}
            return 0;
        }
    } /* end switch */

//	printk(KERN_ALERT "ETHTOOL: not supported eth command: 0x%x\n", ethcmd);
    return -EOPNOTSUPP;
}


/******************************************************

        End of MODULE functions

        Start of Utility functions

*******************************************************/



/*
*   Purpose:    random number generator
*
*   Imports:    init - 1 to reseed the seed and generate
*                      0 to generate
*
*   Exports:    random number, sort of
*
*PUBLIC***/
unsigned long int rand(int init)
{
    if (init)
        seed=0x12345678;
    seed=(seed << 1) | (((seed >> 31) ^ (seed >> 27)) & 1);
    return(seed);
}


/*
*
*
*
*
*
*
*PUBLIC***/
void setup_MDIOClockOut16Bit(void)
{
    mdio[0]=readl(G_base+MDIO_PORT) & ~(MDIO_DATA_BIT | MDIO_CLOCK_BIT);
    mdio[1]=mdio[0] | MDIO_CLOCK_BIT;
    mdio[2]=mdio[0] | MDIO_DATA_BIT;
    mdio[3]=mdio[1] | mdio[2];
}

/*
*         send 16 bits to MDIO bus
*
*         Send 8 bits of data to the MDIO bus for transmission to the target.
*
*
*
*PUBLIC***/
void MDIOClockOut16Bits(unsigned int val)
{
    int i;
    for (i=15;i>=0;i--)
    {
        if (val & (1 << i))
        {
            writel(mdio[2],G_base + MDIO_PORT); /* Clock low, data high */
            if (!quiet)
            printk(KERN_ALERT "OUT=%08x (i=%d)\n",readl(G_base + MDIO_PORT),i);
            writel(mdio[3],G_base + MDIO_PORT); /* Clock high, data high */
            if (!quiet)
            printk(KERN_ALERT "OUT=%08x\n",readl(G_base + MDIO_PORT));
        }
        else
        {
            writel(mdio[0],G_base + MDIO_PORT); /* Clock low, data low */

            if (!quiet)
            printk(KERN_ALERT "OUT=%08x (i=%d)\n",readl(G_base + MDIO_PORT),i);
            writel(mdio[1],G_base + MDIO_PORT); /* Clock high, data low */
            if (!quiet)
            printk(KERN_ALERT "OUT=%08x\n",readl(G_base + MDIO_PORT));
        }
    }
}

/*
*
*
*
*
*
*
*PUBLIC***/
void MDIOClockOutPreamble(void)
{
    MDIOClockOut16Bits(0xffff);
    MDIOClockOut16Bits(0xffff);
}

/*
*         read 16 bits from the mdio bus
*
*
*
*
*
*PUBLIC***/
unsigned int MDIOClockIn16Bit(int data_bit)
{
    int bit;
    int rc;

    rc=0;
    writel(readl(G_base + MDIO_CONFIG_PORT) | data_bit,G_base + MDIO_CONFIG_PORT);

    if (!quiet)
        printk(KERN_ALERT "CTL=%08x\n",readl(G_base + MDIO_CONFIG_PORT));

    for (bit=15;bit>=0;--bit)
    {
        writel(mdio[2],G_base + MDIO_PORT); /* Clock low */

        if (!quiet)
            printk(KERN_ALERT "OUT=%08x\n",readl(G_base + MDIO_PORT));

        if (readl(G_base + MDIO_INPUT_PORT) & data_bit)
            rc=(rc << 1) | 1;
        else
            rc=(rc << 1) | 0;

        if (!quiet)
            printk(KERN_ALERT "IN =%08x (i.e. %08x -> %04x)\n", readl(G_base + MDIO_INPUT_PORT),
                   readl(G_base + MDIO_INPUT_PORT) & data_bit,rc);

        writel(mdio[3],G_base + MDIO_PORT); /* Clock high */

        if (!quiet)
            printk(KERN_ALERT "OUT=%08x\n",readl(G_base + MDIO_PORT));
    }

    /* Enable MDIO output */
    writel(readl(G_base + MDIO_CONFIG_PORT) & ~data_bit,G_base + MDIO_CONFIG_PORT);

    if (!quiet)
        printk(KERN_ALERT "CTL=%08x\n",readl(G_base + MDIO_CONFIG_PORT));

    writel(mdio[2],G_base + MDIO_PORT);
    writel(mdio[3],G_base + MDIO_PORT);
    writel(mdio[2],G_base + MDIO_PORT);
    writel(mdio[3],G_base + MDIO_PORT);
    return(rc);
}        

/*
*
*
*
*
*
*
*PUBLIC***/
unsigned int ClinkReadMDIOData(int reg_addr)
{
    unsigned int rc;

    setup_MDIOClockOut16Bit();
    MDIOClockOutPreamble();
    MDIOClockOut16Bits(MDIO_READ_MASK | MDIO_READ_TURN_AROUND | (reg_addr << MDIO_REG_PLACEMENT));
    rc=MDIOClockIn16Bit(MDIO_DATA_BIT);
    if (verbose)
    {
        if (reg_addr == DATA_HIGH)
        {
            read_state_machine |= 2;
            verbose_data = (verbose_data & 0xffff) | ((unsigned long int) rc << 16);
        }
        if (reg_addr == DATA_LOW)
        {
            read_state_machine |= 1;
            verbose_data = (verbose_data & 0xffff0000L) | ((unsigned long int) rc);
        }
        if (read_state_machine == 3)
        {
            printk(KERN_ALERT "SONICS: READ  %08lX = %08lX\n",verbose_addr_read,verbose_data);
        }
    }
    return(rc);
}



/*
*
*
*
*
*
*
*PUBLIC***/
void ClinkWriteMDIOData(int reg_addr,unsigned val)
{
    setup_MDIOClockOut16Bit();
    MDIOClockOutPreamble();
    MDIOClockOut16Bits(MDIO_WRITE_MASK | MDIO_WRITE_TURN_AROUND | (reg_addr << MDIO_REG_PLACEMENT));
    MDIOClockOut16Bits(val);
    if (verbose)
    {
        if ((reg_addr != ADDRESS_HIGH) &&
            (reg_addr != ADDRESS_LOW) &&
            (reg_addr != DATA_HIGH) &&
            (reg_addr != DATA_LOW) &&
            (reg_addr != ADDRESS_MODE))
        {
            printk(KERN_ALERT "PHY: WRITE %04X %04X\n",reg_addr,val & 0xffff);
        }
        if (reg_addr == ADDRESS_HIGH)
            verbose_addr = (verbose_addr & 0xffff) | ((unsigned long int) val << 16);
        if (reg_addr == ADDRESS_LOW)
            verbose_addr = (verbose_addr & 0xffff0000L) | ((unsigned long int) val);
        if (reg_addr == DATA_HIGH)
            verbose_data = (verbose_data & 0xffff) | ((unsigned long int) val << 16);
        if (reg_addr == DATA_LOW)
            verbose_data = (verbose_data & 0xffff0000L) | ((unsigned long int) val);
        if (reg_addr == ADDRESS_MODE)
        {
            verbose_addr_read=verbose_addr;
            if (val & CLINK_START_WRITE)
            {
                if (auto_count < 100)
                {
                    printk(KERN_ALERT "SONICS: WRITE %08lX = %08lX%s\n",verbose_addr,verbose_data,
                        val & CLINK_AUTO_INC ? " (auto)" : "");
                }
            }
            if (val & CLINK_START_READ)
                read_state_machine=0;
            if (val & CLINK_AUTO_INC)
            {
                verbose_addr += 4L;
                auto_count++;
            }
            else
                auto_count=0;
        }
    }
}


/*
*
*
*
*
*
*
*PUBLIC***/
void setup_SMIClockOut16Bit(void)
{
    mdio[0]=readl(G_base+MDIO_PORT) & ~(SMI_DATA_BIT |  SMI_CLOCK_BIT);
    mdio[1]=mdio[0] | SMI_CLOCK_BIT; /* Clock high, data low  */
    mdio[2]=mdio[0] | SMI_DATA_BIT;  /* Clock low, data high  */
    mdio[3]=mdio[1] | mdio[2];       /* Clock high, data high */
}


/*
*
*
*
*
*
*
*PUBLIC***/
unsigned int SMI_operation(int operation, int addr,unsigned int val)
{
    setup_SMIClockOut16Bit();
    MDIOClockOutPreamble();
    MDIOClockOut16Bits(operation | (addr << MDIO_REG_PLACEMENT));
    if (operation == PHY_READ)
        return(MDIOClockIn16Bit(SMI_DATA_BIT));
    else
        MDIOClockOut16Bits(val);
    return(val);
}

/*
*           wait for SoC bus to say idle
*
*
*
*
*
*PUBLIC***/
void ClinkWaitMDIOReady(void)
{
    unsigned int rc;
    for (;;) {          // If there is problem in SoC, Watchdog interrupt will reset me !!!
        rc=ClinkReadMDIOData(ADDRESS_MODE);
        if ((rc & 0xff) == 0)
            return;
        if (rc & CLINK_BUSY)
        {
            continue;
        }
        if (rc & CLINK_ERROR)
        {
            continue;
        }
        return;
    }
}


/*
*            read the soncis bus
*
*
*
*
*
*PUBLIC***/
unsigned long ClinkReadFrom(unsigned long addr)
{
    unsigned long data;

    ClinkWaitMDIOReady();
    ClinkWriteMDIOData(ADDRESS_HIGH, addr >> 16);
    ClinkWriteMDIOData(ADDRESS_LOW, addr & 0xffff);
    ClinkWriteMDIOData(ADDRESS_MODE, CLINK_START_READ);
    ClinkWaitMDIOReady();
    data=ClinkReadMDIOData(DATA_HIGH);
    data = data << 16;
    data |= (ClinkReadMDIOData(DATA_LOW) & 0xffff);
    return data;
}


/*
*          read the soncis bus
*
*
*
*
*
*PUBLIC***/
void ClinkWriteTo(unsigned long addr, unsigned long data)
{
    ClinkWaitMDIOReady();
    ClinkWriteMDIOData(ADDRESS_HIGH, addr >> 16);
    ClinkWriteMDIOData(ADDRESS_LOW, addr & 0xffff);
    
    ClinkWriteMDIOData(DATA_HIGH, data >> 16);
    ClinkWriteMDIOData(DATA_LOW, data & 0xffff);
    ClinkWriteMDIOData(ADDRESS_MODE, CLINK_START_WRITE);
}

/*
*
*
*
*
*
*
*PUBLIC***/
void Turbo_open(unsigned long addr)
{
    ClinkWaitMDIOReady();
    ClinkWriteMDIOData(ADDRESS_HIGH, addr >> 16);
    ClinkWriteMDIOData(ADDRESS_LOW, addr);
    ClinkWriteMDIOData(ADDRESS_LOW, addr);
}

/*
*
*
*
*
*
*
*PUBLIC***/
void Turbo_write(unsigned long data)
{
    ClinkWriteMDIOData(DATA_HIGH, data >> 16);
    ClinkWriteMDIOData(DATA_LOW, data);
    ClinkWriteMDIOData(ADDRESS_MODE, CLINK_START_WRITE | CLINK_AUTO_INC);
}

/*
*
*
*
*
*
*
*PUBLIC***/
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

/*
*
*
*
*
*
*
*PUBLIC***/
void Turbo_close(void)
{
    ClinkWriteMDIOData(ADDRESS_MODE, 0);
}



/*
*
*
*
*
*
*
*PUBLIC***/
void setup_gpio_signals(void)
{
    if (!quiet)
    {
        printk(KERN_ALERT "MDIO_CLOCK_BIT = %08x\n",MDIO_CLOCK_BIT);
        printk(KERN_ALERT "MDIO_DATA_BIT = %08x\n",MDIO_DATA_BIT);
        printk(KERN_ALERT "SoC_RESET = %08x\n",SoC_RESET);
        printk(KERN_ALERT "SWITCH_RESET = %08x\n",SWITCH_RESET);
        printk(KERN_ALERT "SWITCH_INTR = %08x\n",SWITCH_INTR);
        printk(KERN_ALERT "BUTTON = %08x\n",BUTTON);
        printk(KERN_ALERT "DIPLEXER = %08x\n",DIPLEXER);
        printk(KERN_ALERT "YELLOW = %08x\n",YELLOW);
        printk(KERN_ALERT "GREEN = %08x\n",GREEN);
    }
        
    /* Set pin state (1 = high) */
    writel(
           MDIO_CLOCK_BIT | MDIO_DATA_BIT |
           SMI_CLOCK_BIT | SMI_DATA_BIT |
           SoC_RESET | SOC_XMII_DIS |
           SWITCH_INTR |
           BUTTON |
           DIPLEXER |
           YELLOW | GREEN
           ,G_base + MDIO_PORT);

    /* Set output enables (1 = input/tristate, 0 = output) */
    writel(BUTTON|SWITCH_INTR ,G_base + MDIO_CONFIG_PORT);
}

/*****************************************************

        End of utility functions

******************************************************/





