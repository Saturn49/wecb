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
#ifdef SUPPORT_PTHREAD
#include <pthread.h>
#endif

#define COST_TIME_TEST
#ifdef COST_TIME_TEST
#include <time.h>
#endif

#define TEST_DESTNAME       "org.actiontec.Test"
#define TEST_INTERFACE      "org.actiontec.interface"

static DBusHandlerResult filter_func(DBusConnection *connection, DBusMessage *message, void *user_data)
{
    char *p_buf = NULL;
    int length = 0;
    
    dbusrecv_getmsg(message, &p_buf, &length);
    if (length > 0)
    {
        printf("length=%d\n", length);
    }
    if (dbus_message_is_signal (message,
                              "org.actiontec.PMD",
                              "Disconnected"))
    {
        printf("%s: dbus_message_is_signal disconnected!!!\n", __FILE__);
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

    printf("Test-sendst entry!\n");    
    p_dbussend_hdl = dbussend_init();
    if (NULL == p_dbussend_hdl)
    {
        printf("TEST: dbussend_init failed!\n");
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
            ;//printf("Successfully to send msg!\n");
        else
            printf("Failed to send msg!\n");
#ifdef COST_TIME_TEST        
    }
    gettimeofday(&tm_end, NULL);
    interval = (tm_end.tv_sec - tm_start.tv_sec) * 1000 +
        (tm_end.tv_usec - tm_start.tv_usec)/1000;
    printf("Send_signal: interval=%d ms\n", interval);
#endif
   
    dbussend_uninit(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    printf("Test-sendst exit!\n");
    
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

    printf("Test-sendst entry!\n");    
    p_dbussend_hdl = dbussend_init();
    if (NULL == p_dbussend_hdl)
    {
        printf("TEST: dbussend_init failed!\n");
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
            printf("Successfully to send msg!\n");
        else
            printf("Failed to send msg!\n");
#ifdef COST_TIME_TEST
    }
    gettimeofday(&tm_end, NULL);
    interval = (tm_end.tv_sec - tm_start.tv_sec) * 1000 +
        (tm_end.tv_usec - tm_start.tv_usec)/1000;
    printf("Send_mechodcall: interval=%d ms\n", interval);
#endif
    dbussend_uninit(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    printf("Test-sendst exit!\n");
    
    return 0;
}

int dbussend_msg_to_pmd_test(void)
{
    dbussend_hdl_st *p_dbussend_hdl = NULL;
    char buffer[1024] = {0};
    dbussend_msg_st *p_dbussend_msg = (dbussend_msg_st *)buffer;

    printf("Test-sendst entry!\n");    
    p_dbussend_hdl = dbussend_init();
    if (NULL == p_dbussend_hdl)
    {
        printf("TEST: dbussend_init failed!\n");
        return -1;
    }

    p_dbussend_msg->data_length = sizeof(buffer) - sizeof(dbussend_msg_st);
    memcpy(p_dbussend_msg->buffer, "/bin/dhcp6c -D -f eth1", strlen("/bin/dhcp6c -D -f eth1"));
    if (0 == dbussend_sendmsg(p_dbussend_hdl, 
                                "ctl.msgCTL_MSG_START_APP",
                                NULL, 
                                p_dbussend_msg, 
                                sizeof(dbussend_msg_st)+p_dbussend_msg->data_length))
        printf("Successfully to send msg!\n");
    else
        printf("Failed to send msg!\n");
   
    dbussend_uninit(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    printf("Test-sendst exit!\n");
    
    return 0;
}

#ifdef SUPPORT_PTHREAD
void *do_terminate_thread(void *param)
{
    dbusrecv_hdl_st *p_dbusrecv_hdl = (void *)param;
    if (NULL == p_dbusrecv_hdl)
    {
        printf("do_terminate_thread: invalid parameter!\n");
        return NULL;
    }
    printf("I will sleep 4 seconds!\n");
    sleep(4);
    printf("Sleep 4 seconds, I will uninit dbusrecv!\n");
    dbusrecv_uninit(p_dbusrecv_hdl);
    printf("do_terminate_thread Exit!\n");
    return NULL;
}
#endif

int dbusrecv_msg_test(void)
{
#ifdef SUPPORT_PTHREAD
    pthread_t terminate_thread;
#endif
    int retval = 0;
    dbusrecv_hdl_st *p_dbusrecv_hdl = NULL;
    
    printf("dbusrecv_msg Entry!\n");
    p_dbusrecv_hdl = dbusrecv_init("org.actiontec.DBus.PMD", filter_func);
    if (NULL == p_dbusrecv_hdl)
    {
        printf("dbusrecv_init failed!\n");
        return -1;
    }
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.PMD", "DefaultMethod");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test1", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test2", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test3", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.SMD", "act");

#ifdef SUPPORT_PTHREAD
    retval = pthread_create(&terminate_thread, NULL, (void *)do_terminate_thread, p_dbusrecv_hdl);
    if (retval != 0)
    {
        printf("dbusrecv_msg_test: create thread failed!\n");
        dbusrecv_uninit(p_dbusrecv_hdl);
        printf("dbusrecv_msg Exit, 1!\n");
        return -1;
    }
#endif
    dbusrecv_listen(p_dbusrecv_hdl);
    //dbusrecv_uninit(p_dbusrecv_hdl);
    //pthread_join(terminate_thread
    printf("dbusrecv_msg Exit, 2!\n");
    return 0;
}

int main(int argc, char *argv[])
{
    dbussend_msg_to_pmd_test();
    //dbussend_msg_test();
    //dbussend_msg_methodcall_test();
    //dbusrecv_msg_test();
   
	return 0;
}

