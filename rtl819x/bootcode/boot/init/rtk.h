#ifndef __RTK_H__
#define __RTK_H__

// david ----------------------------------------------------------------
#if defined(CONFIG_NFBI)
	#define FW_SIGNATURE		((char *)"csys")	// fw signature
	#define FW_SIGNATURE_WITH_ROOT	((char *)"csro")	// fw signature with root fs
	#define ROOT_SIGNATURE          ((char *)"root")
#elif defined(CONFIG_OSK)
	#define FW_SIGNATURE		((char *)"OSKR")	// fw signature
#elif defined(RTL8196B) && !defined(RTL8196C)
	#define FW_SIGNATURE			((char *)"cs6b")	// fw signature
	#define FW_SIGNATURE_WITH_ROOT	((char *)"cr6b")	// fw signature with root fs
	#define ROOT_SIGNATURE          ((char *)"r6br")
#elif defined(RTL8196B) && defined(RTL8196C)
	#define FW_SIGNATURE			((char *)"cs6c")	// fw signature
	#define FW_SIGNATURE_WITH_ROOT	((char *)"cr6c")	// fw signature with root fs
	#define ROOT_SIGNATURE          ((char *)"r6cr")
#elif defined(RTL8198)
	#define FW_SIGNATURE			((char *)"cs6c")	// fw signature
	#define FW_SIGNATURE_WITH_ROOT	((char *)"cr6c")	// fw signature with root fs
	#define ROOT_SIGNATURE          ((char *)"r6cr")
#else
	#define FW_SIGNATURE			((char *)"csys")	// fw signature
	#define FW_SIGNATURE_WITH_ROOT	((char *)"csro")	// fw signature with root fs
	#define ROOT_SIGNATURE          ((char *)"root")
#endif

#define SQSH_SIGNATURE		((char *)"sqsh")
#define SQSH_SIGNATURE_LE       ((char *)"hsqs")

#if defined(RTL8196B) && !defined(RTL8196C)
#define WEB_SIGNATURE		((char *)"w6bp")
#elif defined(RTL8196B) && defined(RTL8196C)
#define WEB_SIGNATURE		((char *)"w6cp")
#elif defined(RTL8198)
#define WEB_SIGNATURE		((char *)"w6cg")
#else
#define WEB_SIGNATURE		((char *)"webp")
#endif

#define BOOT_SIGNATURE		((char *)"boot")
#define ALL1_SIGNATURE		((char *)"ALL1")
#define ALL2_SIGNATURE		((char *)"ALL2")

#define HW_SETTING_OFFSET		0x6000

// Cyrus ----------------------------------------------------------------
#define HW_SIGNATURE		((char *)"HS")	// hw signature
#define SW_SIGNATURE_D		((char *)"DS")	// sw_default signature
#define SW_SIGNATURE_C		((char *)"CS")	// sw_current signature
#define SIG_LEN			4

#ifdef AEI_WECB

#define AEI_FLASH_SIGNATURE "aeit"
#define ALL_HEADER			((char *)"allp")

#define SOFTWARE_VERSION_MAX 14
typedef struct fw_header {
    unsigned char signature[SIG_LEN];
    unsigned short offset;
    unsigned short force_upgrade; // mark for force upgrade
    unsigned short hdr_chksum;
    unsigned short chksum;
    unsigned int len;
    char version[SOFTWARE_VERSION_MAX];
    unsigned short hw_version;
} FW_HEADER_T;

#define AEI_FLASH_DATA_SIZE 0x1000 //4K

typedef struct _AEI_FLASH_DATA_
{
    unsigned char signature[SIG_LEN]; // should be "aeit"
    unsigned short chksum; // data checksum
    unsigned short len; // Struct size
    char version[SOFTWARE_VERSION_MAX]; // software version
    unsigned short hw_version; // hardware version
    char reserved[AEI_FLASH_DATA_SIZE - 2 - SOFTWARE_VERSION_MAX -2 -2 - SIG_LEN]; // unused
} AEI_FLASH_DATA_T;

#endif

/* Firmware image header */
typedef struct _header_ {
	unsigned char signature[SIG_LEN];
	unsigned long startAddr;
	unsigned long burnAddr;
	unsigned long len;
} IMG_HEADER_T, *IMG_HEADER_Tp;

typedef struct _signature__ {
	unsigned char *signature;
	unsigned char *comment ;
	int sig_len;
	int skip ;
	int maxSize;
	int reboot;
} SIGN_T ;

#ifdef AEI_WECB

static inline unsigned short aei_check_sum_calc(void *addr, unsigned long len)
{
    int i;
    unsigned short sum = 0;
    unsigned short *p = (unsigned short *)addr;

    for (i = 0; i < len/2; i++)
    {
        sum += p[i];
    }

    return sum;
}

#endif

#endif

