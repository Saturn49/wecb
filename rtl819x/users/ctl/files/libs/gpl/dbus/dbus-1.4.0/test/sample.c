#include <dbussend_msg.h>
#include <dbusrecv_msg.h>
#ifndef DBUS_COMPILATION
#define DBUS_COMPILATION /* Cheat and use private stuff */
#endif
#include <dbus/dbus.h>
#include <dbus/dbus-mainloop.h>
#include <dbus/dbus-internals.h>
#undef DBUS_COMPILATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>   //gettimeofday
#include <pthread.h>

//#define COST_TIME_TEST
#ifdef COST_TIME_TEST
#include <time.h>
#endif

#include "ctl_msg.h"

#define TEST_DESTNAME       "org.actiontec.Test"
#define TEST_INTERFACE      "org.actiontec.interface"

static DBusHandlerResult filter_func(DBusConnection *connection, DBusMessage *message, void *user_data)
{
    char *p_buf = NULL;
    int length = 0;
    
    dbusrecv_getmsg(message, &p_buf, &length);
    if (length > 0)
    {
        fprintf(stdout, "length=%d\n", length);
    }
    if (dbus_message_is_signal (message,
                              "org.actiontec.PMD",
                              "Disconnected"))
    {
        fprintf(stdout, "%s: dbus_message_is_signal disconnected!!!\n", __FILE__);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else
    {
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
}

int dbussend_msg_test(void)
{
    dbussend_hdl_st *p_dbussend_hdl = NULL;
    char buffer[1024] = {0};
    dbussend_msg_st *p_dbussend_msg = (dbussend_msg_st *)buffer;
#ifdef COST_TIME_TEST
    int testcount = 0;
    struct timeval tm_start;
    struct timeval tm_end;
    int interval = 0;
#endif

    fprintf(stdout, "Test-sendst entry!\n");    
    p_dbussend_hdl = dbussend_init();
    if (NULL == p_dbussend_hdl)
    {
        fprintf(stdout, "TEST: dbussend_init failed!\n");
        return -1;
    }

#ifdef COST_TIME_TEST
    gettimeofday(&tm_start, NULL);
    for (testcount=0; testcount<50; testcount++)
    {
#endif
        p_dbussend_msg->data_length = sizeof(buffer) - sizeof(dbussend_msg_st);
        memcpy(p_dbussend_msg->buffer, "It is a test case of dbussend!\n", strlen("It is a test case of dbussend!\n"));        
        if (0 == dbussend_sendmsg(p_dbussend_hdl, 
                                    TEST_INTERFACE, 
                                    NULL, 
                                    p_dbussend_msg, 
                                    sizeof(dbussend_msg_st)+p_dbussend_msg->data_length))
            ;//fprintf(stdout, "Successfully to send msg!\n");
        else
            fprintf(stdout, "Failed to send msg!\n");
#ifdef COST_TIME_TEST        
    }
    gettimeofday(&tm_end, NULL);
    interval = (tm_end.tv_sec - tm_start.tv_sec) * 1000 +
        (tm_end.tv_usec - tm_start.tv_usec)/1000;
    fprintf(stdout, "Send_signal: interval=%d ms\n", interval);
#endif
   
    dbussend_uninit(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    fprintf(stdout, "Test-sendst exit!\n");
    
    return 0;
}

int dbussend_msg_methodcall_test(void)
{
    dbussend_hdl_st *p_dbussend_hdl = NULL;
    char buffer[1024] = {0};
    dbussend_msg_st *p_dbussend_msg = (dbussend_msg_st *)buffer;    
#ifdef COST_TIME_TEST
    int testcount = 0;
    struct timeval tm_start;
    struct timeval tm_end;
    int interval = 0;
#endif

    fprintf(stdout, "Test-sendst entry!\n");    
    p_dbussend_hdl = dbussend_init();
    if (NULL == p_dbussend_hdl)
    {
        fprintf(stdout, "TEST: dbussend_init failed!\n");
        return -1;
    }

#ifdef COST_TIME_TEST
    gettimeofday(&tm_start, NULL);
    for (testcount=0; testcount<50; testcount++)
    {
#endif
        p_dbussend_msg->data_length = sizeof(buffer) - sizeof(dbussend_msg_st);
        memcpy(p_dbussend_msg->buffer, "It is a test case of dbussend!\n", strlen("It is a test case of dbussend!\n"));
        if (0 == dbussend_sendmsg_methodcall(p_dbussend_hdl, 
            TEST_DESTNAME,
            TEST_INTERFACE,
            NULL, 
            p_dbussend_msg, 
            sizeof(buffer)))
            fprintf(stdout, "Successfully to send msg!\n");
        else
            fprintf(stdout, "Failed to send msg!\n");
#ifdef COST_TIME_TEST
    }
    gettimeofday(&tm_end, NULL);
    interval = (tm_end.tv_sec - tm_start.tv_sec) * 1000 +
        (tm_end.tv_usec - tm_start.tv_usec)/1000;
    fprintf(stdout, "Send_mechodcall: interval=%d ms\n", interval);
#endif
    dbussend_uninit(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    fprintf(stdout, "Test-sendst exit!\n");
    
    return 0;
}

int dbussend_msg_to_pmd_test(void)
{
    dbussend_hdl_st *p_dbussend_hdl = NULL;
    char buffer[1024] = {0};
    dbussend_msg_st *p_dbussend_msg = (dbussend_msg_st *)buffer;

    fprintf(stdout, "Test-sendst entry!\n");    
    p_dbussend_hdl = dbussend_init();
    if (NULL == p_dbussend_hdl)
    {
        fprintf(stdout, "TEST: dbussend_init failed!\n");
        return -1;
    }

    p_dbussend_msg->data_length = sizeof(buffer) - sizeof(dbussend_msg_st);
    memcpy(p_dbussend_msg->buffer, "/bin/dhcp6c -c /etc/dhcp6c.conf -D -f eth1\0", strlen("/bin/dhcp6c -c /etc/dhcp6c.conf -D -f eth1\0"));
    if (0 == dbussend_sendmsg(p_dbussend_hdl, 
                                "ctl.msgCTL_MSG_START_APP",
                                NULL, 
                                p_dbussend_msg, 
                                sizeof(dbussend_msg_st)+p_dbussend_msg->data_length))
        fprintf(stdout, "Successfully to send msg!\n");
    else
        fprintf(stdout, "Failed to send msg!\n");
   
    dbussend_uninit(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    fprintf(stdout, "Test-sendst exit!\n");
    
    return 0;
}

int test_multiprocess_signal_startapp(void)
{
	char buffer[1024] = {0};
	dbussend_msg_st *p_dbussend_msg = (dbussend_msg_st *)buffer;	
	int i = 0;

	fprintf(stdout, "test_multiprocess_signal_startapp entry!\n");    

	p_dbussend_msg->data_length = strlen("/usr/sbin/udhcpd -f") + 1;
	strcpy(p_dbussend_msg->buffer, "/usr/sbin/udhcpd -f");

	for (i=0; i<5; i++)
	{
		if (0 == dbussend_sendmsg_noinit("ctl.msgCTL_MSG_START_APP",
									NULL, 
									p_dbussend_msg, 
									sizeof(dbussend_msg_st)+p_dbussend_msg->data_length))
			fprintf(stdout, "Successfully to send msg!\n");
		else
			fprintf(stdout, "Failed to send msg!\n");
	}
	fprintf(stdout, "test_multiprocess_signal_startapp exit!\n");
	
	return 0;
}

int test_multiprocess_signal_stopapp(void)
{
	char buffer[1024] = {0};
	dbussend_msg_st *p_dbussend_msg = (dbussend_msg_st *)buffer;
	CtlMsgAppinfo *p_msgappinfo = p_dbussend_msg->buffer;
	int i = 0;

	fprintf(stdout, "test_multiprocess_signal_stopapp entry!\n");
	
	p_dbussend_msg->data_length = strlen("/usr/sbin/udhcpd -f") + 1;
	strcpy(p_dbussend_msg->buffer, "/usr/sbin/udhcpd -f");
	for (i=0; i<5; i++)
	{
		if (0 == dbussend_sendmsg_noinit("ctl.msgCTL_MSG_STOP_APP",
									NULL, 
									p_dbussend_msg, 
									sizeof(dbussend_msg_st)+p_dbussend_msg->data_length))
			fprintf(stdout, "Successfully to send msg!\n");
		else
			fprintf(stdout, "Failed to send msg!\n");
	}
	fprintf(stdout, "test_multiprocess_signal_stopapp exit!\n");
	
	return 0;
}

int test_multiprocess_methodcall_startapp(CtlMsgAppinfo *p_msgappinfo, int *p_count)
{
	dbussend_hdl_st *p_dbussend_hdl = NULL;
	char buffer[1024] = {0};
	dbussend_msg_st *p_dbussend_msg = (dbussend_msg_st *)buffer;
	CtlMsgAppinfo *p_msgappinfotmp = (CtlMsgAppinfo *)p_dbussend_msg->buffer;
	int i = 0;
	int n = 0;
	int reply = 0;
	int reply_len = sizeof(int);

	fprintf(stdout, "test_multiprocess_methodcall_startapp entry!\n");    
	p_dbussend_hdl = dbussend_init();
	if (NULL == p_dbussend_hdl)
	{
		fprintf(stdout, "TEST: dbussend_init failed!\n");
		return -1;
	}

	p_dbussend_msg->data_length = sizeof(CtlMsgAppinfo);
	strcpy(p_msgappinfotmp->fullcmd, "/usr/sbin/udhcpd -f");
	for (i=0; i<5; i++)
	{
		fprintf(stdout, "sizeof(dbussend_msg_st)+p_dbussend_msg->data_length=%d\n", sizeof(dbussend_msg_st)+p_dbussend_msg->data_length);
		if (0 == dbussend_sendmsg_methodcall_reply(p_dbussend_hdl,
									NID_PMD,
									"ctl.msgCTL_MSG_START_APP",
									NULL, 
									p_dbussend_msg, 
									sizeof(dbussend_msg_st)+p_dbussend_msg->data_length,
									(void *)&reply, 
									&reply_len))
		{
			fprintf(stdout, "Successfully to send msg! - start\n");
			strcpy(p_msgappinfo[n].fullcmd, p_msgappinfotmp->fullcmd);
			p_msgappinfo[n].pid = reply;
			n++;
			fprintf(stdout, "pid=%d\n", p_msgappinfo[i].pid);
		}
		else
			fprintf(stdout, "Failed to send msg! - start\n");
	}
	*p_count = n;
   
	dbussend_uninit(p_dbussend_hdl);
	p_dbussend_hdl = NULL;
	
	fprintf(stdout, "test_multiprocess_methodcall_startapp exit!\n");
	
	return 0;
}


int test_multiprocess_methodcall_stopapp(CtlMsgAppinfo *p_msgappinfo, int count)
{
	dbussend_hdl_st *p_dbussend_hdl = NULL;
	char buffer[1024] = {0};
	dbussend_msg_st *p_dbussend_msg = (dbussend_msg_st *)buffer;
	CtlMsgAppinfo *p_msgappinfotmp = (CtlMsgAppinfo *)p_dbussend_msg->buffer;
	int i = 0;
	int n = 0;
	int reply = 0;
	int reply_len = sizeof(int);

	fprintf(stdout, "test_multiprocess_methodcall_stopapp entry!\n");	 
	p_dbussend_hdl = dbussend_init();
	if (NULL == p_dbussend_hdl)
	{
		fprintf(stdout, "TEST: dbussend_init failed!\n");
		return -1;
	}

	p_dbussend_msg->data_length = sizeof(CtlMsgAppinfo);
	strcpy(p_msgappinfotmp->fullcmd, "/usr/sbin/udhcpd -f");
	for (i=0; i<5; i++)
	{
		p_msgappinfotmp->pid = p_msgappinfo[i].pid;
		if (0 == dbussend_sendmsg_methodcall_reply(p_dbussend_hdl,
									NID_PMD,
									"ctl.msgCTL_MSG_STOP_APP",
									NULL, 
									p_dbussend_msg, 
									sizeof(dbussend_msg_st)+p_dbussend_msg->data_length,
									(void *)&reply, 
									&reply_len))
		{
			fprintf(stdout, "Successfully to send msg - stop!\n");
			fprintf(stdout, "reply=%d\n", reply);
		}
		else
			fprintf(stdout, "Failed to send msg! - stop\n");
	}
   
	dbussend_uninit(p_dbussend_hdl);
	p_dbussend_hdl = NULL;
	
	fprintf(stdout, "test_multiprocess_methodcall_stopapp exit!\n");
	
	return 0;
}


void *do_terminate_thread(void *param)
{
    dbusrecv_hdl_st *p_dbusrecv_hdl = (void *)param;
    if (NULL == p_dbusrecv_hdl)
    {
        fprintf(stdout, "do_terminate_thread: invalid parameter!\n");
        return NULL;
    }
    fprintf(stdout, "I will sleep 4 seconds!\n");
    sleep(4);
    fprintf(stdout, "Sleep 4 seconds, I will uninit dbusrecv!\n");
    dbusrecv_uninit(p_dbusrecv_hdl);
    fprintf(stdout, "do_terminate_thread Exit!\n");
    return NULL;
}

int dbusrecv_msg_test(void)
{
    pthread_t terminate_thread;
    int retval = 0;
    dbusrecv_hdl_st *p_dbusrecv_hdl = NULL;
    
    fprintf(stdout, "dbusrecv_msg Entry!\n");
    p_dbusrecv_hdl = dbusrecv_init("org.actiontec.DBus.PMD", filter_func);
    if (NULL == p_dbusrecv_hdl)
    {
        fprintf(stdout, "dbusrecv_init failed!\n");
        return -1;
    }
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.PMD", "DefaultMethod");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test1", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test2", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test3", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.SMD", "act");

    retval = pthread_create(&terminate_thread, NULL, (void *)do_terminate_thread, p_dbusrecv_hdl);
    if (retval != 0)
    {
        fprintf(stdout, "dbusrecv_msg_test: create thread failed!\n");
        dbusrecv_uninit(p_dbusrecv_hdl);
        fprintf(stdout, "dbusrecv_msg Exit, 1!\n");
        return -1;
    }    
    dbusrecv_listen(p_dbusrecv_hdl);
    //dbusrecv_uninit(p_dbusrecv_hdl);
    //pthread_join(terminate_thread
    fprintf(stdout, "dbusrecv_msg Exit, 2!\n");
    return 0;
}

//#define TEST_MULTIPROCESS_SIGNAL
#define TEST_MULTIPROCESS_METHODCALL
int main(int argc, char *argv[])
{
	int i = 0;
    //dbussend_msg_to_pmd_test();
    //dbussend_msg_test();
    //dbussend_msg_methodcall_test();
    //dbusrecv_msg_test();
#ifdef TEST_DBUS_DELAY_SEND_MODE
	dbussend_set_delaysendmode(1);	
	for (i=0; i<6; i++)
		dbussend_msg_test();
	sleep(2);
	fprintf(stdout, "sleep 2S\n");
	sleep(2);
	fprintf(stdout, "sleep 4S\n");
	sleep(1);
	fprintf(stdout, "sleep 5S\n");
   	dbussend_set_delaysendmode(0);
#endif

#ifdef TEST_MULTIPROCESS_SIGNAL	
	test_multiprocess_signal_startapp();
	i = 0;
	while(1)
	{
		sleep(1);
		i++;
		if (i == 3)
			test_multiprocess_signal_stopapp();			
	}
#endif

#ifdef TEST_MULTIPROCESS_METHODCALL
	CtlMsgAppinfo msg_appinfo[10] = {0};
	int count = 10;
	test_multiprocess_methodcall_startapp(&msg_appinfo, &count);
	i = 0;
	while(1)
	{
		sleep(1);
		i++;
		if (i == 3)
			test_multiprocess_methodcall_stopapp(&msg_appinfo, count);
	}
#endif	

	
	return 0;
}

