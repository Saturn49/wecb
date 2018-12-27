#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     //sleep
#include <sys/time.h>   //gettimeofday
#include "ctl_log.h"
#include <dbussend_msg.h>
#include <dbusrecv_msg.h>
#include <dbus_define.h>

#define RECVPM_DEBUG
#define TEST_MYNAME         "org.actiontec.Test"
#define TEST_INTERFACE      "org.actiontec.interface"

static DBusHandlerResult handle_defaultmethod (DBusConnection *connection, DBusMessage *message, void *user_data);

static DBusHandlerResult filter_func(DBusConnection *connection, DBusMessage *message, void *user_data)
{
    dbussend_msg_st *p_msgheader;
    char *p_buf = NULL;
    int length = 0;
    int ret_fun = 0;
    int retval = DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
#ifdef RECVPM_DEBUG
    int message_type = 0;
    const char *sender = NULL;    
    const char *destination = NULL;
    const char *interface = NULL;
    const char *member = NULL;
#endif    
        
#ifdef RECVPM_DEBUG
    message_type = dbus_message_get_type (message);
    sender = dbus_message_get_sender (message);
    destination = dbus_message_get_destination (message);
    interface = dbus_message_get_interface(message);
    member = dbus_message_get_member(message);
    printf("RECVPM: message_type=%d, sender=%s, destination=%s, interface=%s, method=%s\n", 
        message_type,
        sender,
        destination,
        interface,
        member);
#endif

    if (dbus_message_is_signal(message, TEST_INTERFACE, DBUS_DEFAULT_METHOD))
    {  
        static int count = 0;
        static struct timeval tm_start;
        static struct timeval tm_end;
        int interval = 0;

#ifdef RECVPM_DEBUG
        ret_fun = dbusrecv_getmsg(message, &p_buf, &length);
        if (ret_fun <= 0)
        {
            printf("RECVPM: couldn't get any msg!\n");
            return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
        }
        p_msgheader = (dbussend_msg_st *)p_buf;
        if (1)
        {
            int i=0;
            printf("-------------start(len=%d)-----------\n", length);
            for (i=0; i<length; i++)
                printf("%02x ", (unsigned char)p_buf[i]);
            printf("\n-------------end----------------\n");
        }
#endif
        if (count == 0)
        {
            gettimeofday(&tm_start, NULL);
        }
        count++;
        //printf("length=%d\n", length);
        if (count == 50)
        {
            count = 0;
            gettimeofday(&tm_end, NULL);
            interval = (tm_end.tv_sec - tm_start.tv_sec) * 1000 +
                (tm_end.tv_usec - tm_start.tv_usec)/1000;
            printf("Recv: interval=%d ms\n", interval);
        }
        retval = DBUS_HANDLER_RESULT_HANDLED;        
        goto BACK;
    }

    if (dbus_message_is_method_call(message, TEST_INTERFACE, DBUS_DEFAULT_METHOD))
    {
        return handle_defaultmethod(connection, message, user_data);
    }
BACK:
    
    return retval;
}

static DBusHandlerResult handle_defaultmethod (DBusConnection *connection, DBusMessage *message, void *user_data)
{
    DBusError error;
    DBusMessage *reply;
    char *p_array = NULL;
    int length = 0;
    char *reply_str = "SUCCESS";

    printf("%s: sending reply to handle_defaultmethod\n", __FILE__);    
    dbus_error_init (&error); 

#if 1  
    if (!dbus_message_get_args (message,
                           &error,
                           DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &p_array, &length,
                           DBUS_TYPE_INVALID))
    {
        reply = dbus_message_new_error (message,
                               error.name,
                               error.message);

        if (NULL == reply)
        {
            printf("%s: No memory, dbus_message_new_error\n", __FILE__);
            return DBUS_HANDLER_RESULT_NEED_MEMORY;
        }

        if (!dbus_connection_send (connection, reply, NULL))
        {
            printf("%s: No memory, dbus_connection_send\n", __FILE__);
            return DBUS_HANDLER_RESULT_NEED_MEMORY;
        }    
        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
#else
    dbusrecv_getmsg(message, &p_array, &length);
#endif
    if (length > 0)
    {
        int i=0;
        printf("-------------methodcall start(length=%d)-----------\n", length);
        for (i=0; i<length; i++)
            printf("%02x ", (unsigned char)p_array[i]);
        printf("\n-------------end----------------\n");
    }

    reply = dbus_message_new_method_return (message);
    if (NULL == reply)
    {
        printf("%s: No memory, dbus_message_new_method_return\n", __FILE__);
        return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }  
    if (!dbus_message_append_args (reply,
                      DBUS_TYPE_STRING, &reply_str,
                      DBUS_TYPE_INVALID))
    {
        printf("%s: No memory, dbus_message_append_args\n", __FILE__);
        return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }  
    if (!dbus_connection_send (connection, reply, NULL))
    {
        printf("%s: No memory, dbus_connection_send\n", __FILE__);
        return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }
    printf("%s: Echo service echoed string: \"%s\"\n", __FILE__, reply_str);
    dbus_message_unref (reply);

    return DBUS_HANDLER_RESULT_HANDLED;
}



int test_recvpm_init( void )
{
    dbusrecv_hdl_st *p_dbusrecv_hdl = NULL;

    p_dbusrecv_hdl = dbusrecv_init(TEST_MYNAME, filter_func);
    if (NULL == p_dbusrecv_hdl)
    {
        printf("RECVPM: dbusrecv_init failed!\n");
        return -1;
    }
    dbusrecv_addrules(p_dbusrecv_hdl, TEST_INTERFACE, DBUS_DEFAULT_METHOD);        
    
    dbusrecv_listen(p_dbusrecv_hdl);
    dbusrecv_uninit(p_dbusrecv_hdl);    
    
	return 0;
}


int main(int argc, char *argv[])
{
	int ret;

    printf("RECVPM: Module entry!\n");
	if( (ret = test_recvpm_init()) != 0 ) {
		printf( "init fail (%d). exit!", ret );
		return -1;
	}
    
    while (1)
        sleep(1);

    printf("RECVPM: Module exit!\n"); 

	return 0;
}

