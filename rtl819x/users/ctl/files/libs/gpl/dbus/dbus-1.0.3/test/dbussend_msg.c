#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbus/dbus.h>
#include "dbus_define.h"
#include "dbussend_msg.h"

#define MSG_ARRRY

#ifndef HAVE_STRTOLL
#undef strtoll
#define strtoll mystrtoll
#include "strtoll.c"
#endif

#ifndef HAVE_STRTOULL
#undef strtoull
#define strtoull mystrtoull
#include "strtoull.c"
#endif

static dbussend_cfg_st dbussend_cfg_default =
{     
    DBUS_BUS_SYSTEM,    //type     
    FALSE,              //print_reply
    FALSE,              //print_reply_literal 
    -1,                  //reply_timeout
    DBUS_MESSAGE_TYPE_SIGNAL,   //message_type    
    NULL,               //dest
    DBUS_DEFAULT_PATH,  //path
    NULL               //address        
};

dbussend_hdl_st *dbussend_init(void)
{
    int retval = -1;
    dbussend_hdl_st *p_dbussend_hdl = NULL;
#if 0
    if (NULL == interface)
    {
       printf("dbussend_init: invalid parameters, interface couldn't be NULL!\n"); 
       return NULL;
    }
#endif    
    p_dbussend_hdl = malloc(sizeof(dbussend_hdl_st));
    if (NULL == p_dbussend_hdl)
    {
        printf("dbussend_init: malloc failed, p_dbussend_hdl!\n");
        return NULL;
    }
    memset(p_dbussend_hdl, 0x0, sizeof(dbussend_hdl_st));
    memcpy(&p_dbussend_hdl->dbussend_cfg, &dbussend_cfg_default, sizeof(dbussend_cfg_st));     
        
    if (((DBUS_BUS_SYSTEM == p_dbussend_hdl->dbussend_cfg.type) || (DBUS_BUS_SESSION == p_dbussend_hdl->dbussend_cfg.type)) 
        && (p_dbussend_hdl->dbussend_cfg.address != NULL))
    {
        fprintf (stderr, "\"--address\" may not be used with \"--system\" or \"--session\"\n");
        goto OUT;
    }
    if (!(p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_METHOD_CALL ||
            p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_SIGNAL))
    {
        fprintf (stderr, "Message type \"%d\" is not supported\n", p_dbussend_hdl->dbussend_cfg.message_type);
        goto OUT;
    }

    dbus_error_init (&(p_dbussend_hdl->error));
    if (p_dbussend_hdl->dbussend_cfg.address != NULL)
    {
        p_dbussend_hdl->connection = dbus_connection_open (p_dbussend_hdl->dbussend_cfg.address, &(p_dbussend_hdl->error));
    }
    else
    {
        p_dbussend_hdl->connection = dbus_bus_get (p_dbussend_hdl->dbussend_cfg.type, &(p_dbussend_hdl->error));
    }
    if (p_dbussend_hdl->connection == NULL)
    {
      fprintf (stderr, "Failed to open connection to \"%s\" message bus: %s\n",
               (p_dbussend_hdl->dbussend_cfg.address != NULL) ? p_dbussend_hdl->dbussend_cfg.address :
                 ((p_dbussend_hdl->dbussend_cfg.type == DBUS_BUS_SYSTEM) ? "system" : "session"),
               p_dbussend_hdl->error.message);
      dbus_error_free (&(p_dbussend_hdl->error));
      goto OUT;
    }
#if 0
    if (p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_METHOD_CALL)
    {     
        p_dbussend_hdl->message = dbus_message_new_method_call (NULL,
                                              p_dbussend_hdl->dbussend_cfg.path,
                                              interface,
                                              method);
        dbus_message_set_auto_start (p_dbussend_hdl->message, TRUE);
    }    
    else if (p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_SIGNAL)
    {        
        printf("Frank: path=%s, name=%s, method=%s\n", p_dbussend_hdl->dbussend_cfg.path,
                                interface,
                                method);
        p_dbussend_hdl->message = dbus_message_new_signal (p_dbussend_hdl->dbussend_cfg.path, 
                                              interface,
                                              method);
    }
    else
    {
        fprintf (stderr, "Internal error, unknown message type\n");
        goto OUT;
    } 
    if (p_dbussend_hdl->message == NULL)
    {
        fprintf (stderr, "Couldn't allocate D-Bus message\n");
        goto OUT;
    }
   
    if (p_dbussend_hdl->dbussend_cfg.dest && !dbus_message_set_destination (p_dbussend_hdl->message, p_dbussend_hdl->dbussend_cfg.dest))
    {
        fprintf (stderr, "Not enough memory\n");
        goto OUT;
    }

    dbus_message_iter_init_append (p_dbussend_hdl->message, &(p_dbussend_hdl->iter));
#ifdef MSG_ARRRY
    {
        int type = DBUS_TYPE_BYTE;
        int container_type = DBUS_TYPE_ARRAY;
        DBusMessageIter *target_iter;
        DBusMessageIter container_iter;
        char sig[2];            

        sig[0] = type;
        sig[1] = '\0';
        dbus_message_iter_open_container (&p_dbussend_hdl->iter,
					    container_type,
					    sig,
					    &container_iter);
        target_iter = &container_iter;
        dbus_message_iter_close_container (&p_dbussend_hdl->iter, &container_iter);                
    }
#else
    {
        int type = DBUS_TYPE_BYTE;
        int container_type = DBUS_TYPE_ARRAY;
        DBusMessageIter *target_iter;
        DBusMessageIter container_iter;
        
        target_iter = &p_dbussend_hdl->iter;
    }
#endif
#endif
    retval = 0;
    
OUT:
    if (-1 == retval)
    {
        dbussend_uninit(p_dbussend_hdl);
        p_dbussend_hdl = NULL;
    }
    return p_dbussend_hdl;
}
    
int dbussend_uninit(dbussend_hdl_st *p_dbussend_hdl)
{
    if (NULL == p_dbussend_hdl)
    {
        printf("dbussend_uninit: invalid handle!\n");
        return -1;
    }

    //dbus_message_unref(p_dbussend_hdl->message);
    dbus_connection_unref(p_dbussend_hdl->connection);
    free(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    return 0;
}

int dbussend_sendmsg(dbussend_hdl_st *p_dbussend_hdl, const char *interface, const char *method, void *p_dbussend_msg, int len)
{
    int retval = -1;
    char *pmethod = (char *)method;
    char *parray = (char *)p_dbussend_msg;

    p_dbussend_hdl->dbussend_cfg.message_type = DBUS_MESSAGE_TYPE_SIGNAL;
    if (NULL == p_dbussend_hdl)
    {
        printf("dbussend__sendmsg: invalid handle, p_dbussend_hdl!\n");
        return -1;
    }
    if (NULL == p_dbussend_msg)
    {
        printf("dbussend__sendmsg: invalid parameter, p_dbussend_msg!\n");
        return -1;
    }
    if (NULL == pmethod)
        pmethod = DBUS_DEFAULT_METHOD;    

    if (p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_METHOD_CALL)
    {     
        p_dbussend_hdl->message = dbus_message_new_method_call (NULL,
                                              p_dbussend_hdl->dbussend_cfg.path,
                                              interface,
                                              pmethod);
        dbus_message_set_auto_start (p_dbussend_hdl->message, TRUE);
    }    
    else if (p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_SIGNAL)
    {        
//        printf("Frank: path=%s, name=%s, method=%s\n", p_dbussend_hdl->dbussend_cfg.path,
//                                interface,
//                                pmethod);
        p_dbussend_hdl->message = dbus_message_new_signal (p_dbussend_hdl->dbussend_cfg.path, 
                                              interface,
                                              pmethod);
    }
    else
    {
        fprintf (stderr, "Internal error, unknown message type\n");
        return -1;
    } 
    if (p_dbussend_hdl->message == NULL)
    {
        fprintf (stderr, "Couldn't allocate D-Bus message\n");
        return -1;
    }
   
    if (p_dbussend_hdl->dbussend_cfg.dest && !dbus_message_set_destination (p_dbussend_hdl->message, p_dbussend_hdl->dbussend_cfg.dest))
    {
        fprintf (stderr, "Not enough memory\n");
        goto OUT;
    }

    dbus_message_iter_init_append (p_dbussend_hdl->message, &(p_dbussend_hdl->iter));
#if 0    
    {
        int type = DBUS_TYPE_BYTE;
        int container_type = DBUS_TYPE_ARRAY;
        DBusMessageIter *target_iter;
        DBusMessageIter container_iter;
        char sig[2];            

        sig[0] = type;
        sig[1] = '\0';
        dbus_message_iter_open_container (&p_dbussend_hdl->iter,
					    container_type,
					    sig,
					    &container_iter);
        target_iter = &container_iter;
        dbus_message_iter_close_container (&p_dbussend_hdl->iter, &container_iter);                
    }
#endif    
    if (! dbus_message_append_args (p_dbussend_hdl->message,
            DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &parray, len,
            DBUS_TYPE_INVALID))
    {
        printf("dbussend__sendmsg: append args failed!\n");
        goto OUT;
    }
    if (DBUS_MESSAGE_TYPE_METHOD_CALL == p_dbussend_hdl->dbussend_cfg.message_type)
    {
        DBusMessage *reply;

        dbus_error_init (&(p_dbussend_hdl->error));
        reply = dbus_connection_send_with_reply_and_block (p_dbussend_hdl->connection,
                                                         p_dbussend_hdl->message, p_dbussend_hdl->dbussend_cfg.reply_timeout,
                                                         &(p_dbussend_hdl->error));
        if (dbus_error_is_set (&(p_dbussend_hdl->error)))
        {
            fprintf (stderr, "Error %s: %s\n", p_dbussend_hdl->error.name, p_dbussend_hdl->error.message);
            goto OUT;
        }

        if (reply)
        {
            //print_message (reply, p_dbussend_hdl->dbussend_cfg.print_reply_literal);
            dbus_message_unref (reply);
        }
    }
    else if (DBUS_MESSAGE_TYPE_SIGNAL == p_dbussend_hdl->dbussend_cfg.message_type)
    {
        dbus_connection_send (p_dbussend_hdl->connection, p_dbussend_hdl->message, NULL);
        dbus_connection_flush (p_dbussend_hdl->connection);
    }
    else
    {
        printf("dbussend__sendmsg: message_type error: %d\n", p_dbussend_hdl->dbussend_cfg.message_type);
        goto OUT;
    }
    retval = 0;
    
OUT:
    dbus_message_unref(p_dbussend_hdl->message);    
    return retval;
}

int dbussend_sendmsg_methodcall(dbussend_hdl_st *p_dbussend_hdl, const char *dest, const char *interface, const char *method, void *p_dbussend_msg, int len)
{
    int retval = -1;
    char *pmethod = (char *)method;
    char *parray = (char *)p_dbussend_msg;

    p_dbussend_hdl->dbussend_cfg.message_type = DBUS_MESSAGE_TYPE_METHOD_CALL;
    p_dbussend_hdl->dbussend_cfg.dest = dest;
    if (NULL == p_dbussend_hdl)
    {
        printf("dbussend__sendmsg: invalid handle, p_dbussend_hdl!\n");
        return -1;
    }
    if (NULL == p_dbussend_msg)
    {
        printf("dbussend__sendmsg: invalid parameter, p_dbussend_msg!\n");
        return -1;
    }
    if (NULL == pmethod)
        pmethod = DBUS_DEFAULT_METHOD;    

    if (p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_METHOD_CALL)
    {     
        p_dbussend_hdl->message = dbus_message_new_method_call (NULL,
                                              p_dbussend_hdl->dbussend_cfg.path,
                                              interface,
                                              pmethod);
        dbus_message_set_auto_start (p_dbussend_hdl->message, TRUE);
    }    
    else if (p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_SIGNAL)
    {        
//        printf("Frank: path=%s, name=%s, method=%s\n", p_dbussend_hdl->dbussend_cfg.path,
//                                interface,
//                                pmethod);
        p_dbussend_hdl->message = dbus_message_new_signal (p_dbussend_hdl->dbussend_cfg.path, 
                                              interface,
                                              pmethod);
    }
    else
    {
        fprintf (stderr, "Internal error, unknown message type\n");
        return -1;
    } 
    if (p_dbussend_hdl->message == NULL)
    {
        fprintf (stderr, "Couldn't allocate D-Bus message\n");
        return -1;
    }
   
    if (p_dbussend_hdl->dbussend_cfg.dest && !dbus_message_set_destination (p_dbussend_hdl->message, p_dbussend_hdl->dbussend_cfg.dest))
    {
        fprintf (stderr, "Not enough memory\n");
        goto OUT;
    }

    dbus_message_iter_init_append (p_dbussend_hdl->message, &(p_dbussend_hdl->iter));
#if 0    
    {
        int type = DBUS_TYPE_BYTE;
        int container_type = DBUS_TYPE_ARRAY;
        DBusMessageIter *target_iter;
        DBusMessageIter container_iter;
        char sig[2];            

        sig[0] = type;
        sig[1] = '\0';
        dbus_message_iter_open_container (&p_dbussend_hdl->iter,
                        container_type,
                        sig,
                        &container_iter);
        target_iter = &container_iter;
        dbus_message_iter_close_container (&p_dbussend_hdl->iter, &container_iter);                
    }
#endif    
    if (!dbus_message_append_args (p_dbussend_hdl->message,
            DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &parray, len,
            DBUS_TYPE_INVALID))
    {
        printf("dbussend__sendmsg: append args failed!\n");
        goto OUT;
    }
    if (DBUS_MESSAGE_TYPE_METHOD_CALL == p_dbussend_hdl->dbussend_cfg.message_type)
    {
        DBusMessage *reply;

        dbus_error_init (&(p_dbussend_hdl->error));
        reply = dbus_connection_send_with_reply_and_block (p_dbussend_hdl->connection,
                                                         p_dbussend_hdl->message, p_dbussend_hdl->dbussend_cfg.reply_timeout,
                                                         &(p_dbussend_hdl->error));
        if (dbus_error_is_set (&(p_dbussend_hdl->error)))
        {
            fprintf (stderr, "Error %s: %s\n", p_dbussend_hdl->error.name, p_dbussend_hdl->error.message);
            goto OUT;
        }

        if (reply)
        {
            //print_message (reply, p_dbussend_hdl->dbussend_cfg.print_reply_literal);
            dbus_message_unref (reply);
        }
    }
    else if (DBUS_MESSAGE_TYPE_SIGNAL == p_dbussend_hdl->dbussend_cfg.message_type)
    {
        dbus_connection_send (p_dbussend_hdl->connection, p_dbussend_hdl->message, NULL);
        dbus_connection_flush (p_dbussend_hdl->connection);
    }
    else
    {
        printf("dbussend__sendmsg: message_type error: %d\n", p_dbussend_hdl->dbussend_cfg.message_type);
        goto OUT;
    }
    retval = 0;
    
OUT:
    dbus_message_unref(p_dbussend_hdl->message);    
    return retval;
}


#if 0
int main (int argc, char *argv[])
{
    dbussend_hdl_st *p_dbussend_hdl = NULL;
    dbussend_msg_st dbussend_msg;

    printf("Test-sendst entry!\n");    
    p_dbussend_hdl = dbussend_init();
    if (NULL == p_dbussend_hdl)
    {
        printf("TEST: dbussend_init failed!\n");
        return -1;
    }

    memset(&dbussend_msg, 0x0, sizeof(dbussend_msg_st));
    dbussend_msg.a = 1;
    dbussend_msg.b = 2;
    dbussend_msg.p = NULL;    
    memcpy(dbussend_msg.sz, "It is a test case of dbussend_!\n", strlen("It is a test case of dbussend_!\n"));
    if (0 == dbussend_sendmsg(p_dbussend_hdl, DBUS_PMD_INTERFACE, NULL, &dbussend_msg, sizeof(dbussend_msg)))
        printf("Successfully to send msg!\n");
    else
        printf("Failed to send msg!\n");
    
    if (0 == dbussend_sendmsg(p_dbussend_hdl, "org.actiontec.Test1", NULL, &dbussend_msg, sizeof(dbussend_msg)))
        printf("Successfully to send msg!\n");
    else
        printf("Failed to send msg!\n");

    if (0 == dbussend_sendmsg(p_dbussend_hdl, "org.actiontec.Test2", NULL, &dbussend_msg, sizeof(dbussend_msg)))
        printf("Successfully to send msg!\n");
    else
        printf("Failed to send msg!\n");

    if (0 == dbussend_sendmsg(p_dbussend_hdl, "org.actiontec.Test3", NULL, &dbussend_msg, sizeof(dbussend_msg)))
        printf("Successfully to send msg!\n");
    else
        printf("Failed to send msg!\n");

    
    dbussend_uninit(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    printf("Test-sendst exit!\n");
    
    return 0;
}
#endif


