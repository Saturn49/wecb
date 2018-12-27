
#include <stdio.h>
#include "dbus_define.h"
#include "dbusrecv_msg.h"
#include "test-utils.h"
//#include "../../tr69fw/include/tsl_common.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#define DEFAULT_DBUSRECV_MSG_LEN       (256)
#define DBUS_MALLOC                     malloc  //_MALLOC
#define DBUS_REALLOC                    realloc  //_MALLOC
#define DBUS_FREE                       free

static void dbusrecv_unregistered_func (DBusConnection  *connection, void *user_data);
static DBusHandlerResult dbusrecv_message_func(DBusConnection  *connection, DBusMessage *message, void *user_data);
static DBusHandlerResult handle_echo(DBusConnection *connection, DBusMessage *message, void *user_data);
static DBusHandlerResult handle_system(DBusConnection *connection, DBusMessage *message, void *user_data);

static DBusObjectPathVTable dbusrecv_vtable = {
dbusrecv_unregistered_func,
dbusrecv_message_func,
NULL,
};

#if 0
static DBusHandlerResult filter_func(DBusConnection *connection, DBusMessage *message, void *user_data)
{
    printf("filter_func: path=%s, interface=%s, member=%s\n", 
                dbus_message_get_path(message),
                dbus_message_get_interface(message),
                dbus_message_get_member(message));
    if (dbus_message_is_signal (message,
                           DBUS_DEFAULT_INTERFACE,
                           DBUS_DEFAULT_METHOD))
    {
        _dbus_verbose("%s: dbus_message_is_signal disconnected!!!\n", __FILE__);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else
    {    
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
}
#endif


static void dbusrecv_unregistered_func (DBusConnection  *connection, void *user_data)
{
/* connection was finalized */
}

static DBusHandlerResult dbusrecv_message_func(DBusConnection  *connection, DBusMessage *message, void *user_data)
{
    dbusrecv_hdl_st *p_dbusrecv_hdl = (dbusrecv_hdl_st *)user_data;

    if (NULL == p_dbusrecv_hdl)
    {
        printf("dbusrecv_message_func: invalid handle!\n");
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
    
    if (dbus_message_is_method_call (message, 
                                     DBUS_DEFAULT_INTERFACE, 
                                     "Echo"))
    {
        return handle_echo(connection, message, user_data);
    }
    else if (dbus_message_is_method_call (message,
                                     DBUS_DEFAULT_INTERFACE,
                                     "Exit"))
    {
        _dbus_loop_quit (p_dbusrecv_hdl->dbusloop);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else if (dbus_message_is_method_call(message, 
                                     DBUS_DEFAULT_INTERFACE, 
                                     "system"))
    {
        return handle_system(connection, message, user_data);
    }    
    else
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

static DBusHandlerResult handle_echo (DBusConnection *connection, DBusMessage *message, void *user_data)
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

static DBusHandlerResult handle_system(DBusConnection *connection, DBusMessage *message, void *user_data)
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
 
    p_send_str = DBUS_REPLY_SUCC;
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

dbusrecv_hdl_st *dbusrecv_init(char *myname, callbk_filter_func filter_func)
{    
    dbusrecv_hdl_st *p_dbusrecv_hdl = NULL;
    int result = 0;
    int retval = 0;  

    if (NULL == myname)
    {
        printf("dbusrev_init: invalid parameters, myname couldn't be NULL!\n"); 
        return NULL;
    }    
    if (NULL == filter_func)
    {
        printf("dbusrev_init: invalid parameters, filter_func couldn't be NULL!\n"); 
        return NULL;
    }
    
    p_dbusrecv_hdl = DBUS_MALLOC(sizeof(dbusrecv_hdl_st));
    if (NULL == p_dbusrecv_hdl)
    {
        printf("dbusrev_init: malloc failed, p_dbusrecv_hdl!\n");
        return NULL;
    }    

    p_dbusrecv_hdl->dbusrecv_cfg.myname = (char *)malloc(strlen(myname) + 1);
    memset(p_dbusrecv_hdl->dbusrecv_cfg.myname, 0x0, strlen(myname) + 1);
    memcpy(p_dbusrecv_hdl->dbusrecv_cfg.myname, myname, strlen(myname));        
    p_dbusrecv_hdl->filter_func = filter_func;
    
    dbus_error_init (&p_dbusrecv_hdl->error);
    p_dbusrecv_hdl->connection = dbus_bus_get (DBUS_BUS_SYSTEM, &p_dbusrecv_hdl->error);
    if (NULL == p_dbusrecv_hdl->connection)
    {
        printf("%s: Failed to open connection to activating message bus: %s\n", __FILE__, p_dbusrecv_hdl->error.message);
        retval = -1;
        goto ERROR;
    }
    p_dbusrecv_hdl->dbusloop = _dbus_loop_new ();
     if (NULL == p_dbusrecv_hdl->dbusloop)
     {
         printf("%s: No memory, p_dbusrecv_hdl->dbusloop\n", __FILE__);
         retval = -1;
         goto ERROR;
     }
    if (!test_connection_setup (p_dbusrecv_hdl->dbusloop, p_dbusrecv_hdl->connection))
    {
        printf("%s: No memory, test_connection_setup\n", __FILE__);
        retval = -1;
        goto ERROR;
    }

    /*
        match options:
                type
                interface;
                member;
                sender;
                destination;
                path;
    */  
#if 0    
    memset(rules, 0x0, sizeof(rules));
    sprintf(rules, "type='signal', interface='%s'", p_dbusrecv_hdl->dbusrecv_cfg.interface);
    dbus_bus_add_match (p_dbusrecv_hdl->connection, rules, &p_dbusrecv_hdl->error);
    if (dbus_error_is_set (&p_dbusrecv_hdl->error))
    {
        printf("%s: add match(%s) failed\n", __FILE__, rules);
        retval = -1;
        goto ERROR;
    }
    memset(rules, 0x0, sizeof(rules));
    sprintf(rules, "type='signal', interface='org.actiontec.Test'");
    dbus_bus_add_match (p_dbusrecv_hdl->connection, rules, &p_dbusrecv_hdl->error);
    if (dbus_error_is_set (&p_dbusrecv_hdl->error))
    {
        printf("%s: add match(%s) failed\n", __FILE__, rules);
        retval = -1;
        goto ERROR;
    }
#endif    
    if (!dbus_connection_add_filter(p_dbusrecv_hdl->connection, p_dbusrecv_hdl->filter_func, (void *)p_dbusrecv_hdl, NULL))
    {
        printf("%s: No memory, dbus_connection_add_filter\n", __FILE__);
        retval = -1;
        goto ERROR;
    }
    p_dbusrecv_hdl->is_addfilter = 1;

    if (!dbus_connection_register_object_path (p_dbusrecv_hdl->connection,
                                          DBUS_DEFAULT_PATH,
                                          &dbusrecv_vtable,
                                          (void *)p_dbusrecv_hdl))
                                          //(void*) 0xdeadbeef))
    {
        printf("%s: No memory, dbus_connection_register_object_path\n", __FILE__);
        retval = -1;
        goto ERROR;
    }

    {
        void *d;
        if (!dbus_connection_get_object_path_data (p_dbusrecv_hdl->connection, DBUS_DEFAULT_PATH, &d))
        {
            printf("%s: No memory, dbus_connection_get_object_path_data\n", __FILE__);
            retval = -1;
            goto ERROR;
        }
        //if (d != (void*) 0xdeadbeef)
        if (d != (void*) p_dbusrecv_hdl)
        {
            printf("%s: dbus_connection_get_object_path_data() doesn't seem to work right\n", __FILE__);
            retval = -1;
            goto ERROR;
        }
    }

    result = dbus_bus_request_name(p_dbusrecv_hdl->connection, myname, 0, &p_dbusrecv_hdl->error);
    if (dbus_error_is_set (&p_dbusrecv_hdl->error))
    {
        printf("%s: Failed to acquire service: %s\n", __FILE__, p_dbusrecv_hdl->error.message);
        retval = -1;
        goto ERROR;
    } 
    //_dbus_verbose("%s: service(%s) entering main loop!\n", __FILE__, myname);
    //_dbus_loop_run(p_dbusrecv_hdl->dbusloop);    
    retval = 0;
    
ERROR:
    if (-1 == retval)
    {
        dbus_error_free (&p_dbusrecv_hdl->error);
        if (p_dbusrecv_hdl->connection)
        {
            test_connection_shutdown (p_dbusrecv_hdl->dbusloop, p_dbusrecv_hdl->connection);
            if (p_dbusrecv_hdl->is_addfilter)            
                dbus_connection_remove_filter (p_dbusrecv_hdl->connection, p_dbusrecv_hdl->filter_func, NULL);//
            dbus_connection_unref (p_dbusrecv_hdl->connection);
            p_dbusrecv_hdl->connection = NULL;
        }
        if (p_dbusrecv_hdl->dbusloop)
        {
            _dbus_loop_unref (p_dbusrecv_hdl->dbusloop);
            p_dbusrecv_hdl->dbusloop = NULL;
        }
        dbus_shutdown();
        _dbus_verbose("%s: service(%s) exiting!\n", __FILE__, myname);
        return NULL;
    }
     
    return p_dbusrecv_hdl;
}

int dbusrecv_addrules(dbusrecv_hdl_st *p_dbusrecv_hdl, char *interface, char *method)
{
    char rules[256] = {0};

    if (NULL == p_dbusrecv_hdl)
    {
        printf("dbusrecv_addrules: invalid parameter, p_dbusrecv_hdl!");
        return -1;
    }
    if (NULL == interface)
    {
        printf("dbusrecv_addrules: invalid parameters, interface couldn't be NULL!\n"); 
        return -1;
    }
    memset(rules, 0x0, sizeof(rules));
    sprintf(rules, "type='signal', interface='%s'", interface);
    /*
        match options:
                type
                interface;
                member;
                sender;
                destination;
                path;
    */
    dbus_bus_add_match (p_dbusrecv_hdl->connection, rules, &p_dbusrecv_hdl->error);
    if (dbus_error_is_set (&p_dbusrecv_hdl->error))
    {
        printf("%s: add match(%s) failed\n", __FILE__, rules);
        return -1;
    }
    return 0;
}

int dbusrecv_listen(dbusrecv_hdl_st *p_dbusrecv_hdl)
{
    if (NULL == p_dbusrecv_hdl)
    {
        printf("dbusrecv_listen: invalid parameter, p_dbusrecv_hdl!");
        return -1;
    }
    printf("%s: service(%s) entering main loop!\n", __FILE__, p_dbusrecv_hdl->dbusrecv_cfg.myname);
    _dbus_loop_run(p_dbusrecv_hdl->dbusloop);
    printf("_dbus_loop_run!!!!!!!!!!!!\n");
    if (p_dbusrecv_hdl->connection)
    {
        test_connection_shutdown (p_dbusrecv_hdl->dbusloop, p_dbusrecv_hdl->connection);
        if (p_dbusrecv_hdl->filter_func)            
            dbus_connection_remove_filter (p_dbusrecv_hdl->connection, p_dbusrecv_hdl->filter_func, (void *)p_dbusrecv_hdl);
        dbus_connection_unref (p_dbusrecv_hdl->connection);
        p_dbusrecv_hdl->connection = NULL;
    }
    if (p_dbusrecv_hdl->dbusloop)
    {
        _dbus_loop_unref (p_dbusrecv_hdl->dbusloop);
        p_dbusrecv_hdl->dbusloop = NULL;
    }

    if (p_dbusrecv_hdl->dbusrecv_cfg.myname)
    {
        free(p_dbusrecv_hdl->dbusrecv_cfg.myname);
        p_dbusrecv_hdl->dbusrecv_cfg.myname = NULL;
    }    
    free(p_dbusrecv_hdl);
    p_dbusrecv_hdl = NULL;   
    dbus_shutdown();   
    printf("%s: service exiting!\n", __FILE__);
    
    return 0;
}

#ifdef OLD_GETMSG_METHOD
int loop_iter (DBusMessageIter *iter, int depth, char **pp_buf, int *p_length)
{
    int retval = 0;
    int type = 0;
    int max_buf_len = 0;
    char *p_buf = NULL;    

    do
    {    
        type = dbus_message_iter_get_arg_type (iter);
        if (type == DBUS_TYPE_INVALID)
            break;
        switch (type)
        {
            case DBUS_TYPE_ARRAY:
            {
                int current_type;
                DBusMessageIter subiter;

                dbus_message_iter_recurse (iter, &subiter);
                current_type = dbus_message_iter_get_arg_type (&subiter);
                //msg_len = dbus_message_iter_get_array_len(&subiter)
                if (current_type == DBUS_TYPE_BYTE)
                {
                    unsigned int len = 0;                        
                    int current_type_tmp;

                    if (NULL == p_buf)
                    {
                        p_buf = DBUS_MALLOC(DEFAULT_DBUSRECV_MSG_LEN + 1);
                        max_buf_len = DEFAULT_DBUSRECV_MSG_LEN;
                    }
                    if (NULL == p_buf)
                    {
                        printf("loop_iter: malloc failed, p_buf!\n");
                        return -1;
                    }
                    while ((current_type_tmp = dbus_message_iter_get_arg_type (&subiter))
                      != DBUS_TYPE_INVALID)
                    {                        
                        unsigned char val;
                        dbus_message_iter_get_basic (&subiter, &val);
                        p_buf[len] = val;
                        len++;
                        if (len == max_buf_len)
                        {
                            max_buf_len <<= 1;
                            p_buf = DBUS_REALLOC(p_buf, max_buf_len+1);
                            if (NULL == p_buf)
                            {
                                printf("loop_iter: DBUS_REALLOC failed, p_buf!\n");
                                return -1;
                            }
                        }
                        dbus_message_iter_next (&subiter);
                    }
                    p_buf[len] = '\0';
                    *pp_buf = p_buf;
                    *p_length = len;
                    retval = len;
                    break;
                }
                while (current_type != DBUS_TYPE_INVALID)
                {
                    loop_iter (&subiter, depth+1, pp_buf, p_length);
                    dbus_message_iter_next (&subiter);
                }
                break;
            }

            default:
                break;
        }
    }while(dbus_message_iter_next (iter));

    return retval;
}


int dbusrecv_getmsg(DBusMessage *message, char **pp_buf, int *p_length)
{
    
    DBusMessageIter iter;
#ifdef DBUSRECV_MSG_DEBUG     
    int message_type = 0;
    const char *sender = NULL;    
    const char *destination = NULL;    
#endif    
    int bytecount = 0;

    if (NULL == message)
    {
        printf("dbusrecv_getmsg: invalid parameters!\n");
        return -1;
    }
    
#ifdef DBUSRECV_MSG_DEBUG    
    message_type = dbus_message_get_type (message);
    sender = dbus_message_get_sender (message);
    destination = dbus_message_get_destination (message);    
#endif    
    dbus_message_iter_init (message, &iter);
    bytecount = loop_iter(&iter, 1, pp_buf, p_length); 
#ifdef DBUSRECV_MSG_DEBUG
    if (bytecount > 0)
    {
        int i=0;
        char *p = *pp_buf;
        printf("-------------start(len=%d)-----------\n", bytecount);
        for (i=0; i<bytecount; i++)
            printf("%02x ", p[i]);
        printf("\n-------------end----------------\n");
    }
#endif    
    return bytecount;
}

#else
int dbusrecv_getmsg(DBusMessage *message, char **pp_buf, int *p_length)
{
    DBusError error;

    if (NULL == message)
    {
        printf("dbusrecv_getmsg: invalid parameters!\n");
        return -1;
    }
    
    dbus_error_init (&error);
    if (!dbus_message_get_args (message,
                           &error,
                           DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, pp_buf, p_length,
                           DBUS_TYPE_INVALID))
    {
        printf("dbusrecv_getmsg: get msg failed!\n");
        return -1;
    }
    return *p_length;
}
#endif

int dbusrecv_uninit(dbusrecv_hdl_st *p_dbusrecv_hdl)
{
    char cmd[256] = {0};
    
    if (NULL == p_dbusrecv_hdl)
    {
        printf("dbusrecv_uninit: invalid parameter, p_dbusrecv_hdl!");
        return -1;
    }
    if (p_dbusrecv_hdl->dbusloop)
    {
        //_dbus_loop_quit (p_dbusrecv_hdl->dbusloop);
        sprintf(cmd, "dbus-send --system --type=method_call --print-reply --dest=%s / %s.Exit",
        p_dbusrecv_hdl->dbusrecv_cfg.myname, DBUS_DEFAULT_INTERFACE);
        system(cmd);
    }
    printf("dbusrecv_uninit exit!\n");
    return 0;
}

#if 0
int main(int argc, char **argv)
{
    dbusrecv_hdl_st *p_dbusrecv_hdl = NULL;
    
    printf("dbusrecv_msg Entry!\n");
    p_dbusrecv_hdl = dbusrecv_init(DBUS_PMD_MYNAME, filter_func);
    if (NULL == p_dbusrecv_hdl)
    {
        printf("dbusrecv_init failed!\n");
        return -1;
    }
    dbusrecv_addrules(p_dbusrecv_hdl, DBUS_DEFAULT_INTERFACE, DBUS_DEFAULT_METHOD);
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test1", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test2", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.Test3", "act");
    dbusrecv_addrules(p_dbusrecv_hdl, "org.actiontec.SMD", "act");
    dbusrecv_listen(p_dbusrecv_hdl);
    dbusrecv_uninit(p_dbusrecv_hdl);
    printf("dbusrecv_msg Exit!\n");
    return 0;
}
#endif

