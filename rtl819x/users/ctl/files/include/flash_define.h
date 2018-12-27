#ifndef __FLASH_DEFINE_H__
#define __FLASH_DEFINE_H__

#define FLASH_SECTION_MAGIC	0xFEEDBABE
#define BUF_SIZE	4096
#define MAX_FILENAME	128

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;

typedef struct {
	u32 magic;
	u32 head_size; // == sizeof(section_header_t) + sizeof(tag)
	u32 file_size;
	u32 file_chksum;
} factory_header_t;

static inline void chksum_update(u32 *sum, u8 *buf, u32 size)
{
    register u32 chksum = 0;
    register u32 tmp;

    /* The buffer may begin at an unaligned address, so process the unaligned
     * part one byte at a time */
    while (((u32)buf & 3) && size)
    {
		chksum += *(buf++);
		size--;
    }

    /* By the time we're here, 'buf' is 32-bit aligned so we can start reading 
     * from memory 32-bits at a time, thus minimizing access to memory */
    while(size >= 4)
    {
		tmp = *((u32 *)buf);
		chksum += (tmp & 0xff) + ((tmp >> 8) & 0xff) + ((tmp >> 16) & 0xff) + ((tmp >> 24) & 0xff);
		
		size -= 4;
		buf += 4;
    }

    /* Process unaligned leftovers one byte at a time */
    while(size--)
    {
		chksum += *(buf++);
	}
	
    *sum += chksum;
}

#endif
