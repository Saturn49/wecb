#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbus/dbus.h>
#include "dbus_define.h"
#include "dbussend_msg.h"
#ifdef DBUS_DELAY_SEND_MODE
#include "tsl_list.h"
//#include "ctl_msg.h"
#endif
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
    //-1,                  //reply_timeout
    60*1000,                  //reply_timeout, unit in million seconds
    DBUS_MESSAGE_TYPE_SIGNAL,   //message_type    
    NULL,               //dest
    DBUS_DEFAULT_PATH,  //path
    NULL               //address        
};

#ifdef DBUS_DELAY_SEND_MODE
typedef struct _dbusmsg_list
{
	struct tsl_list_head list;
	char *p_interface;
	char *p_method;
	int buflen;
	char *p_buf;
}dbusmsg_list_t;

static int g_delaysendmode = 0;
static int g_list_cnt = 0;
static tsl_list_head_t g_list_head;

#ifdef __PTHREAD
#include <pthread.h>
static pthread_t pthread;

void *thread_dbusdelaysend_routine(void *data)
{
	fprintf(stderr, "dbussend: thread entry\n");
	while (g_delaysendmode)
	{
		sleep(1);
	}
	fprintf(stderr, "dbussend: thread exit\n");
	
	return NULL;
}
#endif

static dbusmsg_list_t *malloc_and_fill_dbusmsg(const char *interface, const char *method, void *buffer, int buflen)
{
	int retval = -1;
	dbusmsg_list_t *p_dbusmsg_list = NULL;

	do
	{
		p_dbusmsg_list = (dbusmsg_list_t *)malloc(sizeof(dbusmsg_list_t));
		if (NULL == p_dbusmsg_list)
		{
			fprintf(stderr, "dbussend: p_dbusmsg_list, malloc failed!\n");
			return NULL;
		}
		memset(p_dbusmsg_list, 0x0, sizeof(dbusmsg_list_t));
		
		TSL_INIT_LIST_HEAD(&p_dbusmsg_list->list);

		p_dbusmsg_list->p_interface = (char *)malloc(strlen(interface) + 1);
		if (NULL == p_dbusmsg_list->p_interface)
		{
			fprintf(stderr, "dbussend: p_interface, malloc failed!\n");		
			break;
		}
		strcpy(p_dbusmsg_list->p_interface, interface);

		p_dbusmsg_list->p_method = (char *)malloc(strlen(method) + 1);
		if (NULL == p_dbusmsg_list->p_method)
		{
			fprintf(stderr, "dbussend: p_method, malloc failed!\n");		
			break;
		}
		strcpy(p_dbusmsg_list->p_method, method);
		
		p_dbusmsg_list->p_buf = (char *)malloc(buflen);
		if (NULL == p_dbusmsg_list->p_buf)
		{
			fprintf(stderr, "dbussend: p_buf, malloc failed!\n");		
			break;
		}
		p_dbusmsg_list->buflen = buflen;
		memcpy(p_dbusmsg_list->p_buf, buffer, p_dbusmsg_list->buflen);

		retval = 0;
	}
	while (0);
	
	if (-1 == retval)
	{
		if (p_dbusmsg_list->p_interface)
		{
			free(p_dbusmsg_list->p_interface);
			p_dbusmsg_list->p_interface = NULL;
		}
		if (p_dbusmsg_list->p_method)
		{
			free(p_dbusmsg_list->p_method);
			p_dbusmsg_list->p_method = NULL;
		}
		if (p_dbusmsg_list->p_buf)
		{
			free(p_dbusmsg_list->p_buf);
			p_dbusmsg_list->p_buf = NULL;
		}
		if (p_dbusmsg_list)
		{
			free(p_dbusmsg_list);
			p_dbusmsg_list = NULL;
		}
		return NULL;
	}
	
	return p_dbusmsg_list;
}

static int free_dbusmsg_list(struct tsl_list_head *p_list_head)
{
	dbusmsg_list_t *p_dbusmsg_list = NULL;
	struct tsl_list_head *p_pos = NULL;
	struct tsl_list_head *p_n = NULL;

	if (NULL == p_list_head)
	{
		fprintf(stderr, "dbussend: p_list_head, invalid parameters!\n");
		return -1;
	}
	tsl_list_for_each_safe(p_pos, p_n, p_list_head)
	{
		p_dbusmsg_list = tsl_list_entry(p_pos, dbusmsg_list_t, list);
		if (NULL == p_dbusmsg_list)
		{
			fprintf(stderr, "dbussend: p_dbusmsg_list is NULL, why???\n");
			continue;
		}
		if (p_dbusmsg_list->p_interface)
		{
			free(p_dbusmsg_list->p_interface);
			p_dbusmsg_list->p_interface = NULL;
		}
		if (p_dbusmsg_list->p_method)
		{
			free(p_dbusmsg_list->p_method);
			p_dbusmsg_list->p_method = NULL;
		}
		if (p_dbusmsg_list->p_buf)
		{
			free(p_dbusmsg_list->p_buf);
			p_dbusmsg_list->p_buf = NULL;
		}
		free(p_dbusmsg_list);
		p_dbusmsg_list = NULL;
	}
	
	return 0;
}

int dbussend_set_delaysendmode(int is_delaysendmode)
{
	dbusmsg_list_t *p_dbusmsg_list = NULL;	
	struct tsl_list_head *p_pos = NULL;
	struct tsl_list_head *p_n = NULL;
	int list_cnt = 0;
	
	if ((g_delaysendmode == 0) && (is_delaysendmode != 0))
	{
		fprintf(stdout, "dbussend: Enter to delaysend mode!!\n");
		g_delaysendmode = is_delaysendmode;
		TSL_INIT_LIST_HEAD(&g_list_head);
		g_list_cnt = 0;
#ifdef __PTHREAD		
		if (pthread_create(&pthread, NULL, &thread_dbusdelaysend_routine, NULL) != 0)
		{
			fprintf(stderr, "dbussend: pthread_create failed!!!\n");
			return -1;
		}
#endif
	}
	else if ((g_delaysendmode != 0) && (is_delaysendmode == 0))
	{
		fprintf(stdout, "dbussend: Exit from delaysend mode!!\n");
		g_delaysendmode = is_delaysendmode;
#ifdef __PTHREAD		
		pthread_join(pthread, NULL);
#endif
		//send list
		//...
		tsl_list_for_each_safe(p_pos, p_n, &g_list_head)
		{
			p_dbusmsg_list = tsl_list_entry(p_pos, dbusmsg_list_t, list);			
			if (NULL == p_dbusmsg_list)
			{
				fprintf(stderr, "dbussend: p_dbusmsg_list is NULL, why???\n");
				continue;
			}
			dbussend_sendmsg_noinit(p_dbusmsg_list->p_interface, p_dbusmsg_list->p_method, p_dbusmsg_list->p_buf, p_dbusmsg_list->buflen);
			list_cnt++;
		}
		free_dbusmsg_list(&g_list_head);
		TSL_INIT_LIST_HEAD(&g_list_head);		
		fprintf(stdout, "g_list_cnt=%d, list_cnt=%d\n", g_list_cnt, list_cnt);
		g_list_cnt = 0;
	}
	else
	{
		fprintf(stderr, "same mode switch(%d->%d), why?\n", g_delaysendmode, is_delaysendmode);
	}
	fprintf(stdout, "g_delaysendmode=%d\n", g_delaysendmode);
	
	return 0;
}
#endif

dbussend_hdl_st *dbussend_init(void)
{
    int retval = -1;
    dbussend_hdl_st *p_dbussend_hdl = NULL;
#if 0
    if (NULL == interface)
    {
       fprintf(stdout, "dbussend_init: invalid parameters, interface couldn't be NULL!\n"); 
       return NULL;
    }
#endif    
    p_dbussend_hdl = malloc(sizeof(dbussend_hdl_st));
    if (NULL == p_dbussend_hdl)
    {
        fprintf(stdout, "dbussend_init: malloc failed, p_dbussend_hdl!\n");
        return NULL;
    }
    memset(p_dbussend_hdl, 0x0, sizeof(dbussend_hdl_st));
    memcpy(&p_dbussend_hdl->dbussend_cfg, &dbussend_cfg_default, sizeof(dbussend_cfg_st));     
        
    if (((DBUS_BUS_SYSTEM == p_dbussend_hdl->dbussend_cfg.type) || (DBUS_BUS_SESSION == p_dbussend_hdl->dbussend_cfg.type)) 
        && (p_dbussend_hdl->dbussend_cfg.address != NULL))
    {
        fprintf (stderr, "\"--address\" may not be used with \"--system\" or \"--session\"\n");
        goto _OUT;
    }
    if (!(p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_METHOD_CALL ||
            p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_SIGNAL))
    {
        fprintf (stderr, "Message type \"%d\" is not supported\n", p_dbussend_hdl->dbussend_cfg.message_type);
        goto _OUT;
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
      goto _OUT;
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
        fprintf(stdout, "Frank: path=%s, name=%s, method=%s\n", p_dbussend_hdl->dbussend_cfg.path,
                                interface,
                                method);
        p_dbussend_hdl->message = dbus_message_new_signal (p_dbussend_hdl->dbussend_cfg.path, 
                                              interface,
                                              method);
    }
    else
    {
        fprintf (stderr, "Internal error, unknown message type\n");
        goto _OUT;
    } 
    if (p_dbussend_hdl->message == NULL)
    {
        fprintf (stderr, "Couldn't allocate D-Bus message\n");
        goto _OUT;
    }
   
    if (p_dbussend_hdl->dbussend_cfg.dest && !dbus_message_set_destination (p_dbussend_hdl->message, p_dbussend_hdl->dbussend_cfg.dest))
    {
        fprintf (stderr, "Not enough memory\n");
        goto _OUT;
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
    
_OUT:
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
        fprintf(stdout, "dbussend_uninit: invalid handle!\n");
        return -1;
    }

    //dbus_message_unref(p_dbussend_hdl->message);
    dbus_connection_unref(p_dbussend_hdl->connection);
    free(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    return 0;
}

#ifdef DBUS_DELAY_SEND_MODE
int dbussend_sendmsg_noinit(const char *interface, const char *method, void *p_dbussend_msg, int len)
{
	dbussend_hdl_st *p_dbussend_hdl = NULL;
	char buffer[2048] = {0};
	//CtlMsgHeader *p_msg = (CtlMsgHeader *)buffer;

	if (len > sizeof(buffer))
	{
		fprintf(stdout, "dbussend: our buffer is too small!!!!!(%d, %d)\n", sizeof(buffer), len);
		return -1;	
	}
	  
	p_dbussend_hdl = dbussend_init();
	if (NULL == p_dbussend_hdl)
	{
		fprintf(stdout, "dbussend: dbussend_init failed!\n");
		return -1;
	}
	
	if (0 == dbussend_sendmsg(p_dbussend_hdl, 
								interface, 
								method, 
								p_dbussend_msg,
								len))
		fprintf(stdout, "dbussend: Successfully to send msg!(%s, %s)\n", interface, method);
	else
		fprintf(stdout, "dbussend: Failed to send msg!\n");
   
	dbussend_uninit(p_dbussend_hdl);
	p_dbussend_hdl = NULL;
	
	return 0;
}
#endif

int dbussend_sendmsg(dbussend_hdl_st *p_dbussend_hdl, const char *interface, const char *method, void *p_dbussend_msg, int len)
{
    int retval = -1;
    char *pmethod = (char *)method;
    char *parray = (char *)p_dbussend_msg;
#ifdef DBUS_DELAY_SEND_MODE	
	dbusmsg_list_t *p_dbusmsg_list = NULL;
#endif

    p_dbussend_hdl->dbussend_cfg.message_type = DBUS_MESSAGE_TYPE_SIGNAL;
    if (NULL == p_dbussend_hdl)
    {
        fprintf(stdout, "dbussend__sendmsg: invalid handle, p_dbussend_hdl!\n");
        return -1;
    }
    if (NULL == p_dbussend_msg)
    {
        fprintf(stdout, "dbussend__sendmsg: invalid parameter, p_dbussend_msg!\n");
        return -1;
    }
	if (NULL == interface)
	{
		fprintf(stdout, "dbussend_sendmsg: invalid parameter, interface!\n");
        return -1;
	}
	if (len < 1)
	{
		fprintf(stdout, "dbussend_sendmsg: invalid parameter, len!\n");
	}
    if (NULL == pmethod)
        pmethod = DBUS_DEFAULT_METHOD;    
	
#ifdef DBUS_DELAY_SEND_MODE	
	if (g_delaysendmode)
	{
		p_dbusmsg_list = (dbusmsg_list_t *)malloc_and_fill_dbusmsg(interface, pmethod, p_dbussend_msg, len);
		if (p_dbusmsg_list == NULL)
		{
			fprintf(stdout, "dbussend_sendmsg: malloc dbusmsg failed!!!\n");
			return -1;
		}			
		tsl_list_add(&p_dbusmsg_list->list, &g_list_head);
		g_list_cnt++;
		fprintf(stdout, "dbussend_sendmsg: dbus send delay msg count=%d\n", g_list_cnt);
		return 0;
	}
#endif

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
        //fprintf(stdout, "Dbussend: path=%s, name=%s, method=%s\n", p_dbussend_hdl->dbussend_cfg.path,
        //                        interface,
        //                        pmethod);
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
        goto _OUT;
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
        fprintf(stdout, "dbussend__sendmsg: append args failed!\n");
        goto _OUT;
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
            goto _OUT;
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
        fprintf(stdout, "dbussend__sendmsg: message_type error: %d\n", p_dbussend_hdl->dbussend_cfg.message_type);
        goto _OUT;
    }
    retval = 0;
    
_OUT:
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
        fprintf(stdout, "dbussend__sendmsg: invalid handle, p_dbussend_hdl!\n");
        return -1;
    }
    if (NULL == p_dbussend_msg)
    {
        fprintf(stdout, "dbussend__sendmsg: invalid parameter, p_dbussend_msg!\n");
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
        //fprintf(stdout, "Dbussend: path=%s, name=%s, method=%s\n", p_dbussend_hdl->dbussend_cfg.path,
        //                        interface,
        //                        pmethod);
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
        goto _OUT;
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
        fprintf(stdout, "dbussend__sendmsg: append args failed!\n");
        goto _OUT;
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
            goto _OUT;
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
        fprintf(stdout, "dbussend__sendmsg: message_type error: %d\n", p_dbussend_hdl->dbussend_cfg.message_type);
        goto _OUT;
    }
    retval = 0;
    
_OUT:
    dbus_message_unref(p_dbussend_hdl->message);    
    return retval;
}


int dbussend_sendmsg_methodcall_reply(dbussend_hdl_st *p_dbussend_hdl, \
	const char *dest, const char *interface, const char *method, \
	void *p_dbussend_msg, int len, void *p_reply, int *reply_len)
{
    int retval = -1;
    char *pmethod = (char *)method;
    char *parray = (char *)p_dbussend_msg;

	if((NULL==p_reply) || (NULL==reply_len)){
        fprintf(stdout, "Please use your own mem!\n");
		return retval;
	}

    p_dbussend_hdl->dbussend_cfg.message_type = DBUS_MESSAGE_TYPE_METHOD_CALL;
    p_dbussend_hdl->dbussend_cfg.dest = dest;
    if (NULL == p_dbussend_hdl)
    {
        fprintf(stdout, "dbussend__sendmsg: invalid handle, p_dbussend_hdl!\n");
        return -1;
    }
    if (NULL == p_dbussend_msg)
    {
        fprintf(stdout, "dbussend__sendmsg: invalid parameter, p_dbussend_msg!\n");
        return -1;
    }
    if (NULL == pmethod)
	{
        pmethod = DBUS_DEFAULT_METHOD;    
    }

//  if (p_dbussend_hdl->dbussend_cfg.message_type == DBUS_MESSAGE_TYPE_METHOD_CALL)
    {     
        p_dbussend_hdl->message = dbus_message_new_method_call (NULL,
                                              p_dbussend_hdl->dbussend_cfg.path,
                                              interface,
                                              pmethod);
        dbus_message_set_auto_start (p_dbussend_hdl->message, TRUE);
    }    
    if (p_dbussend_hdl->message == NULL)
    {
        fprintf (stderr, "Couldn't allocate D-Bus message\n");
        return -1;
    }
   
    if (p_dbussend_hdl->dbussend_cfg.dest && !dbus_message_set_destination (p_dbussend_hdl->message, p_dbussend_hdl->dbussend_cfg.dest))
    {
        fprintf (stderr, "Not enough memory\n");
        goto _OUT;
    }

    dbus_message_iter_init_append (p_dbussend_hdl->message, &(p_dbussend_hdl->iter));
 
    if (!dbus_message_append_args (p_dbussend_hdl->message,
            DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &parray, len,
            DBUS_TYPE_INVALID))
    {
        fprintf(stdout, "dbussend__sendmsg: append args failed!\n");
        goto _OUT;
    }
//    if (DBUS_MESSAGE_TYPE_METHOD_CALL == p_dbussend_hdl->dbussend_cfg.message_type)
    {
        DBusMessage *reply;

        dbus_error_init (&(p_dbussend_hdl->error));
        reply = dbus_connection_send_with_reply_and_block (p_dbussend_hdl->connection,
                                                         p_dbussend_hdl->message, p_dbussend_hdl->dbussend_cfg.reply_timeout,
                                                         &(p_dbussend_hdl->error));
        if (dbus_error_is_set (&(p_dbussend_hdl->error)))
        {
            fprintf (stderr, "Error %s: %s\n", p_dbussend_hdl->error.name, p_dbussend_hdl->error.message);
            goto _OUT;
        }

        if (reply)
        {
			DBusError error;
			void *pdata;
			int data_len;
			dbus_error_init (&error); 
			if (!dbus_message_get_args (reply,
								   &error,
								   DBUS_TYPE_ARRAY,DBUS_TYPE_BYTE, 
								   &pdata,&data_len,
								   DBUS_TYPE_INVALID))
			{
				fprintf(stderr,"error.name=%s error.message=%s\n",\
					error.name, error.message);
				dbus_message_unref (reply);
				goto _OUT;
			}
			if(NULL == pdata){
				fprintf(stderr,"NO DATA!\n");
				dbus_message_unref (reply);
				goto _OUT;
			}
			memcpy(p_reply,pdata,data_len);
			*reply_len = data_len;
            dbus_message_unref (reply);
        }
    }
    retval = 0;
    
_OUT:
    dbus_message_unref(p_dbussend_hdl->message);    
    return retval;
}



#if 0
int main (int argc, char *argv[])
{
    dbussend_hdl_st *p_dbussend_hdl = NULL;
    dbussend_msg_st dbussend_msg;

    fprintf(stdout, "Test-sendst entry!\n");    
    p_dbussend_hdl = dbussend_init();
    if (NULL == p_dbussend_hdl)
    {
        fprintf(stdout, "TEST: dbussend_init failed!\n");
        return -1;
    }

    memset(&dbussend_msg, 0x0, sizeof(dbussend_msg_st));
    dbussend_msg.a = 1;
    dbussend_msg.b = 2;
    dbussend_msg.p = NULL;    
    memcpy(dbussend_msg.sz, "It is a test case of dbussend_!\n", strlen("It is a test case of dbussend_!\n"));
    if (0 == dbussend_sendmsg(p_dbussend_hdl, DBUS_PMD_INTERFACE, NULL, &dbussend_msg, sizeof(dbussend_msg)))
        fprintf(stdout, "Successfully to send msg!\n");
    else
        fprintf(stdout, "Failed to send msg!\n");
    
    if (0 == dbussend_sendmsg(p_dbussend_hdl, "org.actiontec.Test1", NULL, &dbussend_msg, sizeof(dbussend_msg)))
        fprintf(stdout, "Successfully to send msg!\n");
    else
        fprintf(stdout, "Failed to send msg!\n");

    if (0 == dbussend_sendmsg(p_dbussend_hdl, "org.actiontec.Test2", NULL, &dbussend_msg, sizeof(dbussend_msg)))
        fprintf(stdout, "Successfully to send msg!\n");
    else
        fprintf(stdout, "Failed to send msg!\n");

    if (0 == dbussend_sendmsg(p_dbussend_hdl, "org.actiontec.Test3", NULL, &dbussend_msg, sizeof(dbussend_msg)))
        fprintf(stdout, "Successfully to send msg!\n");
    else
        fprintf(stdout, "Failed to send msg!\n");

    
    dbussend_uninit(p_dbussend_hdl);
    p_dbussend_hdl = NULL;

    fprintf(stdout, "Test-sendst exit!\n");
    
    return 0;
}
#endif


