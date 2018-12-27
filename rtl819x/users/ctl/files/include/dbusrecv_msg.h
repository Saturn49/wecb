#ifndef __DBUSRECV_MSG_H__
#define __DBUSRECV_MSG_H__

#include <dbus/dbus.h>
#include <dbus/dbus-mainloop.h>

typedef DBusHandlerResult (*callbk_filter_func)(DBusConnection *connection, DBusMessage *message, void *user_data);

typedef struct _dbusrecv_cfg
{    
    char *myname;    
}dbusrecv_cfg_st, *pdbusrecv_cfg_st;

typedef struct _dbusrecv_hdl
{
    dbusrecv_cfg_st dbusrecv_cfg;
    DBusError error;
    DBusLoop *dbusloop;
    DBusConnection *connection;        
    callbk_filter_func filter_func;
    int is_addfilter;
}dbusrecv_hdl_st, *pdbusrecv_hdl_st;

dbusrecv_hdl_st *dbusrecv_init(char *myname, callbk_filter_func filter_func);
int dbusrecv_addrules(dbusrecv_hdl_st *p_dbusrecv_hdl, char *interface, char *method);
int dbusrecv_listen(dbusrecv_hdl_st *p_dbusrecv_hdl);
int dbusrecv_getmsg(DBusMessage *message, char **pp_buf, int *p_length);
int dbusrecv_uninit(dbusrecv_hdl_st *p_dbusrecv_hdl);
/*
* function 		: dbusrecv_reply
* create date	: 2011.7.5
* author		: uni.chen
* description 	: 
				 reply message to sender.
* para			:
					connection	[in] : the connection with sender  
					message	[in] : recieved message from sender
					reply_msg	[in] : the reply message you want to send
					reply_len	[in] : the reply message's length
* return		: 
					0 		: success
					other 	: fail, error code
* history		:
*/
int dbusrecv_reply(DBusConnection *connection, DBusMessage *message, void *reply_msg,int reply_len);
int dbusrecv_reply_simple(DBusConnection *connection, DBusMessage *message, int ret);

#endif
