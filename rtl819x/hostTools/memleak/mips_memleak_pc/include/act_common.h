/* FILE NAME: act_common.h
 * PURPOSE: the header file 
 * NOTES:
 * REASON:
 *      DESCRIPTION:  
 *      CREATOR: Yunhai Zhu  
 *      DATE: 2009/3/31
 * Copyright (C), Yunhai Zhu
 */

#ifndef ACT_COMMON_H
#define ACT_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
 *
 *      Define included files according the platform 
 *
 ************************************************************/
#if defined _WIN32
#include <sys/timeb.h>
#include <time.h>
#include <direct.h>
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <limitr.h>

#elif defined _LINUX
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <semaphore.h>

#include <stdio.h>

#include <sys/mman.h>
#include <stdlib.h>
#include "pthread.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <sys/wait.h>
#include <mqueue.h>


#elif defined _VXWORK
#include <vxworks.h>
#include <stdio.h>
#include <string.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <sysLib.h>
#include <stdio.h>
#include <ioLib.h>
#include <usrlib.h>
#include <stat.h>
#include <fcntl.h>
#include <time.h>
#include <tftplib.h>
#include <sys/socket.h>
#include <inetLib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#endif


/***********************************************************
 *
 *                     Define common structure
 *
 ************************************************************/
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifdef _WIN32
#define inline
#define INLINE
#define __FUNCTION__ NULL
#endif

#ifdef _VXWORK
#define inline 
#endif

#ifdef _SOLARIS
#define __FUNCTION__ NULL
#endif

#ifdef _DEBUG
#define DEBUG(act) act
#else
#define DEBUG(act)
#endif

#define NAME_SIZE               64
#define MAX_NAME_SIZE           128
#define MIN_BUF_SIZE            128
#define BUF_SIZE                1024
#define MAX_BUF_SIZE            4096
#define MAX_UINT_SIZE           4294967296
#define MAX_INT_SIZE            2147483648
#define MAX_USHORT_SIZE         65536
#define MAX_SHORT_SIZE          32768

typedef unsigned long           act_u32_t;
typedef unsigned short          act_u16_t;
typedef unsigned char           act_u8_t;
typedef signed   long           act_32_t;
typedef signed   short          act_16_t;
typedef signed   char           act_8_t;

typedef unsigned int            act_uint_t;
typedef signed   int            act_int_t;
typedef unsigned char           act_byte_t;
typedef unsigned long           act_ulong_t;
typedef signed   long           act_long_t;

typedef char                    act_char_t;
typedef unsigned char           act_uchar_t;

typedef void                    act_void_t;
typedef void                    *act_pvoid_t;

typedef size_t                  act_size_t;


#define ACT_B_FALSE 0
#define ACT_B_TRUE  1  
typedef int act_bool;

typedef enum{
        act_b_true               = 1,
        act_b_false              = 0
} act_bool_t;


#define ACT_RV_SUC               0
#define ACT_RV_ERR               -10
#define ACT_RV_ERR_PARM          -11
#define ACT_RV_FAIL              -20
#define ACT_RV_FAIL_MEM          -21   
#define ACT_RV_FAIL_FUNC         -22
typedef int act_rv;


typedef enum{
        act_rv_suc               =  0,
        act_rv_err               = -10,
        act_rv_err_parm          = -11,
        act_rv_fail              = -20,
        act_rv_fail_mem          = -21,
        act_rv_fail_func         = -22      
} act_rv_t;



/***********************************************************
 *
 *              Define Common Macro 
 *
 ************************************************************/
#ifdef _DEBUG
#define act_printf(format, arg...)  \
do{\
    extern FILE *g_log_fd;\
    if (g_log_fd == NULL){\
        g_log_fd = fopen("memleak.log", "wa+");\
    }\
    fprintf(stdout, format, ##arg);\
    fprintf(g_log_fd, format, ##arg);\
    fflush(g_log_fd);\
}while(0)
#define printf(format, arg...) 
#define act_color_printf(color_type, format, arg...)\
do{\
    extern FILE *g_log_fd;\
    if (g_log_fd == NULL){\
        g_log_fd = fopen("memleak.log", "wa+");\
    }\
    fprintf(stdout, "\033["#color_type"m"format"\033[0m", ##arg);\
    fprintf(g_log_fd, format, ##arg);\
    fflush(g_log_fd);\
}while(0)
#define act_lcolor_printf(color_type, format, arg...)\
do{\
    extern FILE *g_log_fd;\
    if (g_log_fd == NULL){\
        g_log_fd = fopen("memleak.log", "wa+");\
    }\
    fprintf(stdout, "\033[1;"#color_type"m"format"\033[0m", ##arg);\
    fprintf(g_log_fd, format, ##arg);\
    fflush(g_log_fd);\
}while(0)
#else
#define act_printf(format, arg...) printf(format, ##arg)
#define act_color_printf(color_type, format, arg...)  fprintf(stdout, "\033["#color_type"m"format"\033[0m", ##arg);
#define act_lcolor_printf(color_type, format, arg...) fprintf(stdout, "\033[1;"#color_type"m"format"\033[0m", ##arg);
#endif  

#define act_black_printf(format, arg...)    act_color_printf(30, format, ##arg)
#define act_red_printf(format, arg...)      act_color_printf(31, format, ##arg)
#define act_green_printf(format, arg...)    act_color_printf(32, format, ##arg)
#define act_yellow_printf(format, arg...)   act_color_printf(33, format, ##arg)
#define act_blue_printf(format, arg...)     act_color_printf(34, format, ##arg)
#define act_zred_printf(format, arg...)     act_color_printf(35, format, ##arg)
#define act_qblu_printf(format, arg...)     act_color_printf(36, format, ##arg)
#define act_white_printf(format, arg...)    act_color_printf(37, format, ##arg)

#define act_lblack_printf(format, arg...)    act_lcolor_printf(30, format, ##arg)
#define act_lred_printf(format, arg...)      act_lcolor_printf(31, format, ##arg)
#define act_lgreen_printf(format, arg...)    act_lcolor_printf(32, format, ##arg)
#define act_lyellow_printf(format, arg...)   act_lcolor_printf(33, format, ##arg)
#define act_lblue_printf(format, arg...)     act_lcolor_printf(34, format, ##arg)
#define act_lzred_printf(format, arg...)     act_lcolor_printf(35, format, ##arg)
#define act_lqblu_printf(format, arg...)     act_lcolor_printf(36, format, ##arg)
#define act_lwhite_printf(format, arg...)    act_lcolor_printf(37, format, ##arg)

/*return with rv*/
#define ACT_VASSERT_RV(cond, rv)                                 \
{                                                                \
        if (!(cond)){                                            \
               printf("ASSERT:(%s),%s %s line %d\n",             \
                       #cond, __FUNCTION__, __FILE__, __LINE__); \
                return rv;                                       \
        }                                                        \
}

#define ACT_VASSERT_RV_ACT(cond, rv, action)                     \
{                                                                \
        if (!(cond)){                                            \
               printf("ASSERT:(%s),%s %s line %d\n",             \
                       #cond, __FUNCTION__, __FILE__, __LINE__); \
                {action;}                                        \
                return rv;                                       \
        }                                                        \
}

/*return with void*/
#define ACT_VASSERT_VRV(cond)                                    \
{                                                                \
        if (!(cond)){                                            \
               printf("ASSERT:(%s),%s %s line %d\n",             \
                       #cond, __FUNCTION__, __FILE__, __LINE__); \
               return ;                                          \
        }                                                        \
}

#define ACT_VASSERT_VRV_ACT(cond, action)                        \
{                                                                \
        if (!(cond)){                                            \
               printf("ASSERT:(%s),%s %s line %d\n",             \
                       #cond, __FUNCTION__, __FILE__, __LINE__); \
                {action;}                                        \
                return ;                                         \
        }                                                        \
}

/* do not return*/
#define ACT_VASSERT_NRV(cond)                                    \
{                                                                \
        if (!(cond)){                                            \
               printf("ASSERT:(%s),%s %s,line %d\n",             \
                       #cond, __FUNCTION__, __FILE__, __LINE__); \
        }                                                        \
}

#define ACT_VASSERT_NRV_ACT(cond, action)                        \
{                                                                \
        if (!(cond)){                                            \
               printf("ASSERT:(%s),%s %s,line %d\n",             \
                       #cond, __FUNCTION__, __FILE__, __LINE__); \
                {action;}                                        \
        }                                                        \
}


/*default ACT_VASSERT macro used to judge the parameter is correct or not*/
#define ACT_VASSERT(cond)                    ACT_VASSERT_RV(cond, ACT_RV_ERR_PARM)
#define ACT_VASSERT_ACT(cond, action)        ACT_VASSERT_RV_ACT(cond, ACT_RV_ERR_PARM, action)

/*default ACT_VASSERT macro used to judge the function called is correct or not*/
#define ACT_FVASSERT(cond)                   ACT_VASSERT_RV(cond, ACT_RV_FAIL_FUNC)
#define ACT_FVASSERT_RV(cond, rv)            ACT_VASSERT_RV(cond, rv)
#define ACT_FVASSERT_ACT(cond, act)          ACT_VASSERT_RV_ACT(cond, ACT_RV_FAIL_FUNC, act)
#define ACT_FVASSERT_RV_ACT(cond, rv, act)   ACT_VASSERT_RV_ACT(cond, rv, act)
#define ACT_FVASSERT_NRV(cond)               ACT_VASSERT_NRV(cond)
#define ACT_FVASSERT_NRV_ACT(cond, act)      ACT_VASSERT_NRV_ACT(cond, act)
#define ACT_FVASSERT_VRV(cond)               ACT_VASSERT_VRV(cond)
#define ACT_FVASSERT_VRV_ACT(cond, act)      ACT_VASSERT_VRV_ACT(cond, act)

/*#define _MALLOC_MMAP*/
#ifdef _MALLOC_MMAP
#define _MALLOC(size)            mmap(0, size,PROT_READ|PROT_WRITE , MAP_SHARED|MAP_ANONYMOUS, -1, 0)
#define _CALLOC(size, size_t)    _MALLOC(size)
#define _FREE(ptr)               munmap(ptr,0) 
#else
#define _MALLOC(size)            malloc(size) 
#define _CALLOC(size, size_t)    calloc(size * size_t)
#define _FREE(ptr)               free(ptr)
#endif

#define ACT_SMALLOC_RV(ptr, type, size, rv)                                        \
{                                                                                  \
        ACT_VASSERT_RV((ptr = (type *)_MALLOC(size)) != NULL, rv);                 \
        memset(ptr, 0, size);                                                      \
        ACT_VASSERT_RV(ptr != NULL, rv);                                           \
}

#define ACT_SMALLOC_RV_ACT(ptr, type, size, rv, act)                               \
{                                                                                  \
        ACT_VASSERT_RV_ACT((ptr = (type *)_MALLOC(size)) != NULL, rv, act);        \
        memset(ptr, 0, size);                                                      \
        ACT_VASSERT_RV_ACT(ptr != NULL, rv, act);                                  \
}
#define ACT_SMALLOC(ptr, type, size) ACT_SMALLOC_RV(ptr, type, size, ACT_RV_FAIL_MEM)
                 

#define ACT_MALLOC_RV(ptr, type, rv)                                               \
{                                                                                  \
        ACT_VASSERT_RV((ptr = (type *)_MALLOC(sizeof(type))) != NULL, rv);         \
        memset(ptr, 0, sizeof(type));                                              \
        ACT_VASSERT_RV(ptr != NULL, rv);                                           \
}                                          
#define ACT_MALLOC_RV_ACT(ptr, type, rv, act)                                       \
{                                                                                  \
        ACT_VASSERT_RV_ACT((ptr = (type *)_MALLOC(sizeof(type))) != NULL, rv, act);\
        memset(ptr, 0, sizeof(type));                                              \
        ACT_VASSERT_RV_ACT(ptr != NULL, rv, act);                                  \
}
#define ACT_MALLOC_ACT(ptr, type, act)        ACT_MALLOC_RV_ACT(ptr, type, ACT_RV_FAIL_MEM, act)                    
#define ACT_MALLOC(ptr, type)                 ACT_MALLOC_RV(ptr, type, ACT_RV_FAIL_MEM)                    

#define ACT_FREE(ptr) _FREE(ptr)

                                              
        
#ifdef __cplusplus
}
#endif


#endif

