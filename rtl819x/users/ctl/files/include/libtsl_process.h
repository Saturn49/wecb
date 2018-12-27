#ifndef LIBTSL_PROCESS_H
#define LIBTSL_PROCESS_H

#include <sys/types.h>
#include <sys/wait.h>
typedef struct tr69_process_s{
    char path[32];
    char argv[128];
	pid_t pid;
	int is_lowerver;
}tr69_process_t;

#define APP_TR69 "./tr69"
#define APP_DATA_CENTER "./data_center"

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_process_start
 *
 *	[DESCRIPTION]:
 *	        start a new process        
 *
 *	[ARGUMENT]:
 *	        char *p_path
 *	        char *p_argv
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tsl_process_start(tr69_process_t *p_process_tb);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_process_stop
 *
 *	[DESCRIPTION]:
 *	        stop the process
 *
 *	[ARGUMENT]:
 *	        char *p_path
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tsl_process_stop(tr69_process_t *p_process_tb);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_process_restart
 *
 *	[DESCRIPTION]:
 *	        restart the process        
 *
 *	[ARGUMENT]:
 *	        char *p_path
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tsl_process_restart(tr69_process_t *p_process_tb);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_process_init
 *
 *	[DESCRIPTION]:
 *	        initinate the process control dameon
 *
 *	[ARGUMENT]:
 *	        struct tr69_process_s *p_process_tb
 *	        int numb
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tsl_process_init(struct tr69_process_s *p_process_tb, int numb);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_process_start
 *
 *	[DESCRIPTION]:
 *	        cleanup the process control daemon 
 *
 *	[ARGUMENT]:
 *	        
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tsl_process_cleanup();

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_app_start
 *
 *	[DESCRIPTION]:
 *	        start a new app       
 *
 *	[ARGUMENT]:
 *	        char *apppath (in)
 *		 char *argv   (in)
 *
 *	[RETURN]
 *              > 0         Success, pid
 *              <=  0          ERROR
 **************************************************************************/
pid_t tsl_app_start(char * apppath,char * argv);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_app_stop
 *
 *	[DESCRIPTION]:
 *	        stop the app
 *
 *	[ARGUMENT]:
		char *apppath
		char * argv
 *	       pid_t stop_pid
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tsl_app_stop(char * apppath,char * argv,pid_t stop_pid);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_app_restart
 *
 *	[DESCRIPTION]:
 *	        restart the app        
 *
 *	[ARGUMENT]:
 *	        char *p_path
		 char *argv
 		 pid_t stop_pid
 *
 *	[RETURN]
 *              > 0          SUCCESS,pid
 *              <=  0          ERROR
 **************************************************************************/
pid_t tsl_app_restart(char * apppath,char * argv,pid_t stop_pid);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tsl_app_stop_all
 *
 *	[DESCRIPTION]:
 *	        stop all app
 *
 *	[ARGUMENT]:
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tsl_app_stop_all();

#endif
