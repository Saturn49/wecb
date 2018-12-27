#ifndef _CTL_LOG_H
#define _CTL_LOG_H

#include <sys/time.h>
#include <time.h>

#if 0
#define SHOWDATE()	do { \
	time_t the_time; \
	tm *tm_ptr;  \
	(void) time(&the_time); \
	tm_ptr = localtime(&the_time); \
	fprintf( stdout, "[%02d:%02d:%02d] ", \
			tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec ); \
} while(0)
#else
#define SHOWDATE()	
#endif

enum CmsLogLevel{
    LOG_LEVEL_NULL,
    LOG_LEVEL_ERR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_NOTICE,
    LOG_LEVEL_DEBUG
};

extern int CTL_LOG_LEVEL;

#define ctllog_debug(arg...)	do { \
                    if(CTL_LOG_LEVEL < LOG_LEVEL_DEBUG)\
                        break;\
					SHOWDATE(); \
					fprintf(stdout,"DEBUG %s:%d ",__FILE__,__LINE__);\
					fprintf(stdout,##arg);\
					fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_notice(arg...)	do { \
                    if(CTL_LOG_LEVEL < LOG_LEVEL_NOTICE)\
                        break;\
					SHOWDATE(); \
					fprintf(stdout,"NOTICE %s:%d ",__FILE__,__LINE__);\
					fprintf(stdout,##arg);\
					fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_warn(arg...)	do { \
                    if(CTL_LOG_LEVEL < LOG_LEVEL_WARN)\
                        break;\
					SHOWDATE(); \
					fprintf(stdout,"WARNING %s:%d ",__FILE__,__LINE__);\
					fprintf(stdout,##arg);\
					fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_error(arg...)	do { \
                    if(CTL_LOG_LEVEL < LOG_LEVEL_ERR)\
                        break;\
					SHOWDATE(); \
					fprintf(stdout,"ERROR %s:%d ",__FILE__,__LINE__);\
					fprintf(stdout,##arg);\
					fprintf(stdout,"\n" );\
				} while(0)
				
#define V6_DEBUG	

#define CTL_MODULE_NCS_CFG_SYNC 0X00000001
#define CTL_MODULE_WAN_UPGRADE 0X00000002
#define ALL_CTL_MODULE (CTL_MODULE_WAN_UPGRADE)
#define ctldbg_module(module_mask ,arg...)        do { \
        if(!(module_mask & ALL_CTL_MODULE))\
            break;\
        SHOWDATE(); \
        fprintf(stdout,"%s:%d :%s() ",__FILE__,__LINE__,__func__);\
        fprintf(stdout,##arg);\
        fprintf(stdout,"\n" );\
    } while(0)

#endif
