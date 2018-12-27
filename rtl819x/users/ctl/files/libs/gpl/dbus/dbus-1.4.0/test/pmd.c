#include "dbus_define.h"
#include "test-utils.h"
//#include <util/rg_error.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

//#define rg_error(LEVEL, fmt, args...) fprintf(stderr, fmt, ##args);

static DBusHandlerResult filter_func(DBusConnection *connection, DBusMessage *message, void *user_data);
static void pmd_unregistered_func (DBusConnection  *connection, void *user_data);
static DBusHandlerResult pmd_message_func(DBusConnection  *connection, DBusMessage *message, void *user_data);
static DBusHandlerResult handle_echo(DBusConnection *connection, DBusMessage *message);
static DBusHandlerResult handle_system(DBusConnection *connection, DBusMessage *message);
static DBusHandlerResult handle_startdaemon(DBusConnection *connection, DBusMessage *message);
static DBusHandlerResult handle_stopdaemon(DBusConnection *connection, DBusMessage *message);
static DBusHandlerResult handle_restartdaemon(DBusConnection *connection, DBusMessage *message);

static DBusLoop *g_dbusloop = NULL;
static dbus_bool_t g_is_already_quit = FALSE;

int dbus_pmd_listen(void);

static DBusObjectPathVTable pmd_vtable = {
  pmd_unregistered_func,
  pmd_message_func,
  NULL,
};

static DBusHandlerResult filter_func(DBusConnection *connection, DBusMessage *message, void *user_data)
{
    if (dbus_message_is_signal (message,
                              DBUS_PMD_INTERFACE,
                              "Disconnected"))
    {
        _dbus_verbose("%s: dbus_message_is_signal disconnected!!!\n", __FILE__);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else
    {
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
}

static void pmd_unregistered_func (DBusConnection  *connection, void *user_data)
{
  /* connection was finalized */
}

static DBusHandlerResult pmd_message_func(DBusConnection  *connection, DBusMessage *message, void *user_data)
{
    if (dbus_message_is_method_call (message, 
                                        DBUS_PMD_INTERFACE, 
                                        DBUS_PMD_METHOD_ECHO))
    {
        return handle_echo(connection, message);
    }
    else if (dbus_message_is_method_call (message,
                                        DBUS_PMD_INTERFACE,
                                        DBUS_PMD_METHOD_EXIT))
    {
        if (!g_is_already_quit)
        {
            _dbus_loop_quit (g_dbusloop);
            g_is_already_quit = TRUE;
        }
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else if (dbus_message_is_method_call(message, 
                                        DBUS_PMD_INTERFACE, 
                                        DBUS_PMD_METHOD_SYSTEM))
    {
        return handle_system(connection, message);
    }
    else if (dbus_message_is_method_call(message, 
                                        DBUS_PMD_INTERFACE, 
                                        DBUS_PMD_METHOD_STARTDAEMON))
    {
        return handle_startdaemon(connection, message);
    }
    else if (dbus_message_is_method_call(message,
                                        DBUS_PMD_INTERFACE,
                                        DBUS_PMD_METHOD_STOPDAEMON))
    {
        return handle_stopdaemon(connection, message);
    }
    else if (dbus_message_is_method_call(message,
                                        DBUS_PMD_INTERFACE,
                                        DBUS_PMD_METHOD_RESTARTDAEMON))
    {
        return handle_restartdaemon(connection, message);
    }
    else
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

static DBusHandlerResult handle_echo (DBusConnection *connection, DBusMessage *message)
{
    DBusError error;
    DBusMessage *reply;
    char *s = NULL;

    _dbus_verbose("%s: sending reply to Echo method\n", __FILE__);
    dbus_error_init (&error);

    if (!dbus_message_get_args (message,
                              &error,
                              DBUS_TYPE_STRING, &s,
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
            _dbus_verbose("%s: No memory, dbus_connection_send\n", __FILE__);
            return DBUS_HANDLER_RESULT_NEED_MEMORY;
        }
        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    reply = dbus_message_new_method_return (message);
    if (NULL == reply)
    {
        printf("%s: No memory, dbus_message_new_method_return\n", __FILE__);
        return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }  
    if (!dbus_message_append_args (reply,
                                 DBUS_TYPE_STRING, &s,
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
    _dbus_verbose("%s: Echo service echoed string: \"%s\"\n", __FILE__, s);
    dbus_message_unref (reply);
    
    return DBUS_HANDLER_RESULT_HANDLED;
}

static DBusHandlerResult handle_system(DBusConnection *connection, DBusMessage *message)
{
    DBusError error;
    DBusMessage *reply;
    char *p_recv_str = NULL;
    char *p_send_str = NULL;

    _dbus_verbose("%s: sending reply to handle_system method\n", __FILE__);
    dbus_error_init (&error);

    if (!dbus_message_get_args (message,
                              &error,
                              DBUS_TYPE_STRING, &p_recv_str,
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

    reply = dbus_message_new_method_return (message);
    if (NULL == reply)
    {
        printf("%s: No memory, dbus_message_new_method_return\n", __FILE__);
        return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }    
    
    p_send_str = DBUS_SUC_STR;
    if (!dbus_message_append_args (reply,
                                 DBUS_TYPE_STRING, &p_send_str,
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
    _dbus_verbose("%s: system method echoed string: \"%s\"\n", __FILE__, p_send_str);
    dbus_message_unref (reply);
  
    system(p_recv_str);
    _dbus_verbose("Successful to system daemon: %s\n", p_recv_str);
    
    return DBUS_HANDLER_RESULT_HANDLED;
}



static DBusHandlerResult handle_startdaemon(DBusConnection *connection, DBusMessage *message)
{
    DBusError error;
    DBusMessage *reply;
    char *p_recv_str = NULL;
    char *p_send_str = NULL;

    _dbus_verbose("%s: sending reply to handle_startdaemon method\n", __FILE__);
    dbus_error_init (&error);

    if (!dbus_message_get_args (message,
                              &error,
                              DBUS_TYPE_STRING, &p_recv_str,
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

    reply = dbus_message_new_method_return (message);
    if (NULL == reply)
    {
        printf("%s: No memory, dbus_message_new_method_return\n", __FILE__);
        return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }    
    
    p_send_str = DBUS_SUC_STR;
    if (!dbus_message_append_args (reply,
                                 DBUS_TYPE_STRING, &p_send_str,
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
    _dbus_verbose("%s: Echo service echoed string: \"%s\"\n", __FILE__, p_send_str);
    dbus_message_unref (reply);
  
    system(p_recv_str);
    printf("Successful to start daemon: %s\n", p_recv_str);
    
    return DBUS_HANDLER_RESULT_HANDLED;
}


static DBusHandlerResult handle_stopdaemon(DBusConnection *connection, DBusMessage *message)
{
    DBusError error;
    DBusMessage *reply;
    char *p_recv_str = NULL;
    char *p_send_str = NULL;
    //char cmd[256] = {0};

    _dbus_verbose("%s: sending reply to handle_stopdaemon method\n", __FILE__);
    dbus_error_init (&error);

    if (!dbus_message_get_args (message,
                              &error,
                              DBUS_TYPE_STRING, &p_recv_str,
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

    reply = dbus_message_new_method_return (message);
    if (NULL == reply)
    {
        printf("%s: No memory, dbus_message_new_method_return\n", __FILE__);
        return DBUS_HANDLER_RESULT_NEED_MEMORY;
    }    
    
    p_send_str = DBUS_SUC_STR;
    if (!dbus_message_append_args (reply,
                                 DBUS_TYPE_STRING, &p_send_str,
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
    _dbus_verbose("%s: Echo service echoed string: \"%s\"\n", __FILE__, p_send_str);
    dbus_message_unref (reply);

    system(p_recv_str);
    printf("Successful to start daemon: %s\n", p_recv_str);
    
    return DBUS_HANDLER_RESULT_HANDLED;
}


static DBusHandlerResult handle_restartdaemon(DBusConnection *connection, DBusMessage *message)
{
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int dbus_pmd_listen(void)
{    
    DBusError error;
    DBusConnection *connection = NULL;
    dbus_bool_t do_fork = FALSE;
    char *name = DBUS_PMD_MYNAME;
    int result = 0;
    int retval = 0;
    int is_addfiltersuc = 0;

    //do_fork = strcmp ("pmd", "fork") == 0;
    if (do_fork)
    {
        pid_t pid = fork();
        if (pid != 0)
        {
            printf("%s: fork failed\n", __FILE__);
            return -1;
        }
        sleep (1);
    }

    dbus_error_init (&error);
    connection = dbus_bus_get (DBUS_BUS_SYSTEM, &error);
    if (NULL == connection)
    {
        printf("%s: Failed to open connection to activating message bus: %s\n", __FILE__, error.message);
        retval = -1;
        goto ERROR;
    }
    g_dbusloop = _dbus_loop_new ();
    if (NULL == g_dbusloop)
    {
        printf("%s: No memory, g_dbusloop\n", __FILE__);
        retval = -1;
        goto ERROR;
    }
    if (!test_connection_setup (g_dbusloop, connection))
    {
        printf("%s: No memory, test_connection_setup\n", __FILE__);
        retval = -1;
        goto ERROR;
    }

    if (!dbus_connection_add_filter(connection, filter_func, NULL, NULL))
    {
        printf("%s: No memory, dbus_connection_add_filter\n", __FILE__);
        retval = -1;
        goto ERROR;
    }
    is_addfiltersuc = 1;

  if (!dbus_connection_register_object_path (connection,
                                             DBUS_PMD_PATH,
                                             &pmd_vtable,
                                             (void*) 0xdeadbeef))
   {
        printf("%s: No memory, dbus_connection_register_object_path\n", __FILE__);
        retval = -1;
        goto ERROR;
    }

    {
        void *d;
        if (!dbus_connection_get_object_path_data (connection, DBUS_PMD_PATH, &d))
        {
            printf("%s: No memory, dbus_connection_get_object_path_data\n", __FILE__);
            retval = -1;
            goto ERROR;
        }
        if (d != (void*) 0xdeadbeef)
        {
            printf("%s: dbus_connection_get_object_path_data() doesn't seem to work right\n", __FILE__);
            retval = -1;
            goto ERROR;
        }
    }

    result = dbus_bus_request_name(connection, name, 0, &error);

    if (dbus_error_is_set (&error))
    {
        printf("%s: Failed to acquire service: %s\n", __FILE__, error.message);
        retval = -1;
        goto ERROR;
    }

    _dbus_verbose("%s: service(%s) entering main loop!\n", __FILE__, name);
    _dbus_loop_run(g_dbusloop);

ERROR:
    dbus_error_free (&error);
    if (connection)
    {
        test_connection_shutdown (g_dbusloop, connection);
        if (is_addfiltersuc)            
            dbus_connection_remove_filter (connection, filter_func, NULL);//
        dbus_connection_unref (connection);
        connection = NULL;
    }
    if (g_dbusloop)
    {
        _dbus_loop_unref (g_dbusloop);
        g_dbusloop = NULL;
    }
    dbus_shutdown();
    _dbus_verbose("%s: service(%s) exiting!\n", __FILE__, name);
    
    return -1;
}

int main(int argc, char **argv)
{
    printf("PMD Entry!\n");   
    dbus_pmd_listen();
    printf("PMD Exit!\n");
    return 0;
}

