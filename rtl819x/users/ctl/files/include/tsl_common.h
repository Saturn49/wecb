/* FILE NAME: tsl_common.h
 * PURPOSE: the header file
 * NOTES:
 * REASON:
 *      DESCRIPTION:
 *      CREATOR: Yunhai Zhu
 *      DATE: 2010/9/1
 */

#ifndef TSL_COMMON_H
#define TSL_COMMON_H

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

#if ( !defined (_BHR2 ) && !defined( SUPPORT_BHR1) )
#include <semaphore.h>
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>


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

#define NAME_SIZE               64
#define MAX_NAME_SIZE           256
#define MIN_BUF_SIZE            128
#define BUF_SIZE                1024
#define MAX_BUF_SIZE            4096
#define MAX_UINT_SIZE           4294967296
#define MAX_INT_SIZE            2147483648
#define MAX_USHORT_SIZE         65536
#define MAX_SHORT_SIZE          32768

    typedef unsigned long long           tsl_u64_t;
    typedef unsigned long           tsl_u32_t;
    typedef unsigned short          tsl_u16_t;
    typedef unsigned char           tsl_u8_t;
    typedef signed   long           tsl_32_t;
    typedef signed   long long          tsl_64_t;
    typedef signed   short          tsl_16_t;
    typedef signed   char           tsl_8_t;

    typedef unsigned int            tsl_uint_t;
    typedef signed   int            tsl_int_t;
    typedef unsigned char           tsl_byte_t;
    typedef unsigned long           tsl_ulong_t;
    typedef signed   long           tsl_long_t;

    typedef char                    tsl_char_t;
    typedef unsigned char           tsl_uchar_t;

    typedef void                    tsl_void_t;
    typedef void                    *tsl_pvoid_t;

    typedef size_t                  tsl_size_t;


#define TSL_B_FALSE 0
#define TSL_B_TRUE  1
    typedef int tsl_bool;

    typedef enum{
        tsl_b_true               = 1,
        tsl_b_false              = 0
    } tsl_bool_t;


#define TSL_RV_SUC               0
#define TSL_RV_ERR               -10
#define TSL_RV_ERR_PARM          -11
#define TSL_RV_FAIL              -20
#define TSL_RV_FAIL_MEM          -21
#define TSL_RV_FAIL_FUNC         -22
#define TSL_RV_FAIL_NONE         -23
#define TSL_RV_PARAM_EXIST       -24 //modified by logan on 2012-06-12
#define TSL_RV_FAIL_NOCHG        -25 //add for skip no used value change event
#ifdef AEI_PARAM_PERSIST
#define TSL_RV_FAIL_PERSIST -26 // add by libby for save some parameter value after factory reset
#endif
typedef int tsl_rv;

typedef char * tr69_oid;

typedef struct {
        tsl_u8_t currentDepth;
        tsl_u32_t instance[6];
}tr69_oid_stack_id;

typedef enum{
    tsl_rv_suc               =  0,
    tsl_rv_err               = -10,
    tsl_rv_err_parm          = -11,
    tsl_rv_fail              = -20,
    tsl_rv_fail_mem          = -21,
    tsl_rv_fail_func         = -22,
    //modified by logan on 2012-06-12
	tsl_rv_param_exist	   = -24, /**check parameter value exist or not*/
	
    CMSRET_METHOD_NOT_SUPPORTED = 9000,  /**<Method not supported. */
    CMSRET_REQUEST_DENIED       = 9001,  /**< Request denied (no reason specified). */
    CMSRET_INTERNAL_ERROR       = 9002,  /**< Internal error. */
    CMSRET_INVALID_ARGUMENTS    = 9003,  /**< Invalid arguments. */
    CMSRET_RESOURCE_EXCEEDED    = 9004,  /**< Resource exceeded.  */
    CMSRET_INVALID_PARAM_NAME   = 9005,  /**< Invalid pARAM_TYPE   = 9006,
                                          *  (associated with set/getParameterValues)
                                          *                                          */
    CMSRET_INVALID_PARAM_VALUE  = 9007,  /**< Invalid parameter value.
                                          *  (associated with set/getParameterValues)
                                          *                                          */
    CMSRET_SET_NON_WRITABLE_PARAM = 9008,/**< Attempt to set a non-writable parameter.
                                          *  (associated with setParameterValues)
                                          *                                          */
    CMSRET_NOTIFICATION_REQ_REJECTED = 9009, /**< Notification request rejected.
                                               +------------  2 lines: *  (associated with setParameterAttributes)-----------------------*/

    CMSRET_DOWNLOAD_FAILURE     = 9010,  /**< Download failure.
                                          *  (associated with download or transferComplete)
                                          *                                           */
    CMSRET_UPLOAD_FAILURE       = 9011,  /**< Upload failure.
                                          *  (associated with upload or transferComplete)
                                          */
    CMSRET_FILE_TRANSFER_AUTH_FAILURE = 9012,  /**< File transfer server authentication*/
    CMSRET_UNSUPPORTED_FILE_TRANSFER_PROTOCOL = 9013,/**< Unsupported protocol for file */
    CMSRET_FILE_TRANSFER_NOT_ACCESS_FILESERVER = 9015, /* File transfer failure: unable to contact file server */
    CMSRET_FILE_TRANSFER_INVALID_FILENAME = 9016,   /* File Server Authorization failed */
    CMSRET_FILE_TRANSFER_NOT_COMPLETE = 9017, /* File transfer failure: unable to complete download */
    CMSRET_FILE_UNUSABLE_CORRUPTED = 9018,  /* File transfer failure: file corrupted or otherwise unusable */

    CMSRET_DUPLICATE_INSTANCE_EXIST	= 9101,

    CMSRET_SUCCESS_REBOOT_REQUIRED = 9800, /**< Config successful, but requires reboot to take effect. */
    CMSRET_SUCCESS_UNRECOGNIZED_DATA_IGNORED = 9801,  /**<Success, but some unrecognized data was ignored. */
    CMSRET_SUCCESS_OBJECT_UNCHANGED = 9802,  /**<Success, furthermore object has not changed, returned by STL handl
                                               er functions. */
    CMSRET_FAIL_REBOOT_REQUIRED = 9803,  /**<Config failed, and now system is in a bad state requiring reboot. */
    CMSRET_NO_MORE_INSTANCES = 9804,     /**<getnext operation cannot find any more instances to return. */
    CMSRET_MDM_TREE_ERROR = 9805,         /**<Error during MDM tree traversal */
    CMSRET_WOULD_DEADLOCK = 9806, /**< Caller is requesting a lock while holding the same lock or a different one.
    */
    CMSRET_LOCK_REQUIRED = 9807,  /**< The MDM lock is required for this operation. */
    CMSRET_OP_INTR = 9808,      /**<Operation was interrupted, most likely by a Linux signal. */
    CMSRET_TIMED_OUT = 9809,     /**<Operation timed out. */
    CMSRET_DISCONNECTED = 9810,  /**< Communications link is disconnected. */
    CMSRET_MSG_BOUNCED = 9811,   /**< Msg was sent to a process not running, and the
                                  *   bounceIfNotRunning flag was set on the header.  */
    CMSRET_OP_ABORTED_BY_USER = 9812,  /**< Operation was aborted/discontinued by the user */
    CMSRET_RECURSION_ERROR = 9817,     /**< too many levels of recursion */
    CMSRET_OPEN_FILE_ERROR = 9818,     /**< open file error */
    CMSRET_KEY_GENERATION_ERROR = 9830,     /** certificate key generation error */
    CMSRET_INVALID_CERT_REQ = 9831,     /** requested certificate does not match with issued certificate */
    CMSRET_INVALID_CERT_SUBJECT = 9832,     /** certificate has invalid subject information */
    CMSRET_OBJECT_NOT_FOUND = 9840,     /** failed to find object */

    CMSRET_INVALID_FILENAME = 9850,  /**< filename was not given for download */
    CMSRET_INVALID_IMAGE = 9851,     /**< bad image was given for download */
    CMSRET_INVALID_CONFIG_FILE = 9852,  /**< invalid config file was detected */
    CMSRET_CONFIG_PSI = 9853,         /**< old PSI/3.x config file was detected */
    CMSRET_IMAGE_FLASH_FAILED = 9854, /**< could not write the image to flash */
#ifdef ACTION_TEC
    CMSRET_MESSAGE_NOEXIST = 9900, /**< Requestesd message entry does not exist*/
#endif
    /***********************************************************
     *
     *              Define Common Macro
     *
     ************************************************************/
} tsl_rv_t;
        /*return with rv*/
#define TSL_VASSERT_RV(cond, rv)                                 \
        {                                                                \
            if (!(cond)){                                            \
                printf("ASSERT:(%s),%s %s line %d\n",            \
#cond, __FUNCTION__, __FILE__, __LINE__); \
                return rv;                                       \
            }                                                        \
        }

#define TSL_VASSERT_RV_ACT(cond, rv, action)                     \
        {                                                                \
            if (!(cond)){                                            \
                printf("ASSERT:(%s),%s %s line %d\n",            \
#cond, __FUNCTION__, __FILE__, __LINE__); \
                {action;}                                        \
                return rv;                                       \
            }                                                        \
        }

        /*return with void*/
#define TSL_VASSERT_VRV(cond)                                    \
        {                                                                \
            if (!(cond)){                                            \
                printf("ASSERT:(%s),%s %s line %d\n",             \
#cond, __FUNCTION__, __FILE__, __LINE__); \
                return ;                                          \
            }                                                        \
        }

#define TSL_VASSERT_VRV_ACT(cond, action)                        \
        {                                                                \
            if (!(cond)){                                            \
                printf("ASSERT:(%s),%s %s line %d\n",            \
#cond, __FUNCTION__, __FILE__, __LINE__); \
                {action;}                                        \
                return ;                                         \
            }                                                        \
        }

        /* do not return*/
#define TSL_VASSERT_NRV(cond)                                    \
        {                                                                \
            if (!(cond)){                                            \
                printf("ASSERT:(%s),%s %s,line %d\n",            \
#cond, __FUNCTION__, __FILE__, __LINE__); \
            }                                                        \
        }

#define TSL_VASSERT_NRV_ACT(cond, action)                        \
        {                                                                \
            if (!(cond)){                                            \
                printf("ASSERT:(%s),%s %s,line %d\n",            \
#cond, __FUNCTION__, __FILE__, __LINE__); \
                {action;}                                        \
            }                                                        \
        }

        /* break*/
#define TSL_VASSERT_BR(cond)                                    \
        {                                                                \
            if (!(cond)){                                            \
                printf("ASSERT:(%s),%s %s,line %d\n",            \
#cond, __FUNCTION__, __FILE__, __LINE__); \
                break;                                           \
            }                                                        \
        }

#define TSL_VASSERT_BR_ACT(cond, action)                        \
        {                                                                \
            if (!(cond)){                                            \
                printf("ASSERT:(%s),%s %s line %d\n",            \
#cond, __FUNCTION__, __FILE__, __LINE__); \
                {action;}                                        \
                break ;                                         \
            }                                                        \
        }

        /*default TSL_VASSERT macro used to judge the parameter is correct or not*/
#define TSL_VASSERT(cond)                    TSL_VASSERT_RV(cond, TSL_RV_ERR_PARM)
#define TSL_VASSERT_ACT(cond, action)        TSL_VASSERT_RV_ACT(cond, TSL_RV_ERR_PARM, action)

        /*default TSL_VASSERT macro used to judge the function called is correct or not*/
#define TSL_FVASSERT(cond)                   TSL_VASSERT_RV(cond, TSL_RV_FAIL_FUNC)
#define TSL_FVASSERT_RV(cond, rv)            TSL_VASSERT_RV(cond, rv)
#define TSL_FVASSERT_ACT(cond, act)          TSL_VASSERT_RV_ACT(cond, TSL_RV_FAIL_FUNC, act)
#define TSL_FVASSERT_RV_ACT(cond, rv, act)   TSL_VASSERT_RV_ACT(cond, rv, act)
#define TSL_FVASSERT_NRV(cond)               TSL_VASSERT_NRV(cond)
#define TSL_FVASSERT_NRV_ACT(cond, act)      TSL_VASSERT_NRV_ACT(cond, act)
#define TSL_FVASSERT_VRV(cond)               TSL_VASSERT_VRV(cond)
#define TSL_FVASSERT_VRV_ACT(cond, act)      TSL_VASSERT_VRV_ACT(cond, act)


#define _MALLOC(size)            malloc(size)
#define _CALLOC(size, size_t)    calloc(size, size_t)
#define _FREE(ptr)               free(ptr)


#define TSL_SMALLOC_RV(ptr, type, size, rv)                                        \
        {                                                                                  \
            TSL_VASSERT_RV((ptr = (type *)_MALLOC(size)) != NULL, rv);                 \
            memset(ptr, 0, size);                                                      \
            TSL_VASSERT_RV(ptr != NULL, rv);                                           \
        }

#define TSL_SMALLOC_RV_ACT(ptr, type, size, rv, act)                               \
        {                                                                                  \
            TSL_VASSERT_RV_ACT((ptr = (type *)_MALLOC(size)) != NULL, rv, act);        \
            memset(ptr, 0, size);                                                      \
            TSL_VASSERT_RV_ACT(ptr != NULL, rv, act);                                  \
        }
#define TSL_SMALLOC(ptr, type, size) TSL_SMALLOC_RV(ptr, type, size, TSL_RV_FAIL_MEM)


#define TSL_MALLOC_RV(ptr, type, rv)                                               \
        {                                                                                  \
            TSL_VASSERT_RV((ptr = (type *)_MALLOC(sizeof(type))) != NULL, rv);         \
            memset(ptr, 0, sizeof(type));                                              \
            TSL_VASSERT_RV(ptr != NULL, rv);                                           \
        }
#define TSL_MALLOC_RV_ACT(ptr, type, rv, act)                                       \
        {                                                                                  \
            TSL_VASSERT_RV_ACT((ptr = (type *)_MALLOC(sizeof(type))) != NULL, rv, act);\
            memset(ptr, 0, sizeof(type));                                              \
            TSL_VASSERT_RV_ACT(ptr != NULL, rv, act);                                  \
        }
#define TSL_MALLOC_ACT(ptr, type, act)        TSL_MALLOC_RV_ACT(ptr, type, TSL_RV_FAIL_MEM, act)
#define TSL_MALLOC(ptr, type)                 TSL_MALLOC_RV(ptr, type, TSL_RV_FAIL_MEM)

#define TSL_FREE(ptr) {_FREE(ptr); ptr=NULL;}

#ifndef _DEBUG

#define printf(format, arg...)
#define tsl_printf(format, arg...) 

#else

#define tsl_printf(format, arg...)              \
        do{\
            fprintf(stdout, format, ##arg);\
        }while(0)
#endif

#define MEM_ALIGN(size, boundary) \
        (((size) + ((boundary) - 1)) & ~((boundary) - 1))


#define TSL_CURRENT_TIME() \
({ \
        struct timespec _t0; \
        clock_gettime(CLOCK_MONOTONIC, &_t0); \
        _t0.tv_sec; \
    })
#define TSL_DIFF_TIME(t) \
({ \
        struct timespec _t1; \
        clock_gettime(CLOCK_MONOTONIC, &_t1); \
        _t1.tv_sec-(t); \
    })


#ifdef __cplusplus
}
#endif

#include "ctl_log.h"

#endif

