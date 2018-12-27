/*
 *      Log Define
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

/*
  * LOG: OFF(0), DEBUG(1), INFO(2), WARN(3), ERROR(4), TRACE(5)
  * LOG_LINE: 0, 1
  */
#define LOG 4
#define LOG_LINE 0

#define SHOWDEBUG() fprintf(stdout,"[DEBUG] " )	
#define SHOWINFO()  fprintf(stdout,"[*INFO] " )	
#define SHOWWARN()  fprintf(stdout,"[*WARN] " )	
#define SHOWERROR() fprintf(stdout,"[ERROR] " )	
#define SHOWTRACE() fprintf(stdout,"[TRACE] " )	

#define SHOWDATE() \
	do { \
		time_t timer = time(0); \
		char timeStr[80]; \
		strftime(timeStr, sizeof(timeStr), "[%b %d %H:%M:%S] ", gmtime(&timer)); \
		fprintf( stdout, "%s", timeStr); \
	} while(0)

#define SHOWLOG(arg...)\
	do {\
		SHOWDATE();\
		fprintf(stdout,"[%s] [%s] [%d] ", __FILE__, __FUNCTION__, __LINE__);\
		fprintf(stdout,##arg);\
		if(LOG_LINE)\
			fprintf(stdout," [LINE: %d]\n", __LINE__);\
		else\
			fprintf(stdout,"\n");\
	} while(0)

#define kprintf(arg...) do{}while(0)

#define kdebug(arg...) \
	do{ \
		if(LOG == 1){\
			SHOWDEBUG();\
			SHOWLOG(arg);\
		}\
	}while(0)

#define kinfo(arg...) \
	do{ \
		if((LOG == 1) || (LOG == 2)){\
			SHOWINFO();\
			SHOWLOG(arg);\
		}\
	}while(0) 

#define kwarn(arg...) \
	do{ \
		if((LOG == 0) || (LOG == 4))\
			break;\
		SHOWWARN();\
		SHOWLOG(arg);\
	}while(0) 

#define kerror(arg...) \
	do{\
		if(LOG == 0)\
			break;\
		SHOWERROR();\
		SHOWLOG(arg);\
	}while(0) 

#define ktrace(arg...) \
	do{\
		if(LOG == 0)\
			break;\
		SHOWTRACE();\
		SHOWLOG(arg);\
	}while(0)

#endif
