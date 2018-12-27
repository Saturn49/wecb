#ifndef __DBUSSEND_MSG_H__
#define __DBUSSEND_MSG_H__

#include <dbus/dbus.h>

/*this structure is only for test*/
typedef struct _dbussend_msg
{
    unsigned long data_length;    
    unsigned char buffer[0];
}dbussend_msg_st, *p_dbussend_msg_st;

typedef struct _dbussend_cfg
{    
    DBusBusType type;
    int print_reply;
    int print_reply_literal;
    int reply_timeout;    
    int message_type;
    char *dest;
    char *path;
    char *address;    
}dbussend_cfg_st, *pdbussend_cfg_st;

typedef struct _dbussend_hdl
{
    DBusConnection *connection;
    DBusError error;
    DBusMessage *message;
    DBusMessageIter iter;

    dbussend_cfg_st dbussend_cfg;
}dbussend_hdl_st, *pdbussend_hdl_st;

#define DBUS_DELAY_SEND_MODE
//#define __PTHREAD
#ifdef DBUS_DELAY_SEND_MODE
int dbussend_set_delaysendmode(int is_delaysendmode);
int dbussend_sendmsg_noinit(const char *interface, const char *method, void *p_dbussend_msg, int len);
#endif
dbussend_hdl_st *dbussend_init(void);
int dbussend_uninit(dbussend_hdl_st *p_dbussend_hdl);
int dbussend_sendmsg(dbussend_hdl_st *p_dbussend_hdl, const char *interface, const char *method, void *p_dbussend_msg, int len);
int dbussend_sendmsg_methodcall(dbussend_hdl_st *p_dbussend_hdl, const char *dest, const char *interface, const char *method, void *p_dbussend_msg, int len);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        dbussend_sendmsg_methodcall_reply
 *
 *	[DESCRIPTION]:
 *           Send message to destination and return reply message to caller.
 *           I will block until reply message coming.
 *
 *	[PARAMETER]:
 *		 p_dbussend_hdl  [in] : dbus handle  
 *		 dest				 [in] : destination
 *		 interface			 [in] : 
 *		 method 			 [in] :
 *		 p_dbussend_msg     [in] : the message you want to send
 *		 len				       [in] : send mesages length
 *		 p_reply			 [out]: the reply message 
 *								 (caller's space)
 *		 reply_len			 [out]: the reply message's length
 *
 *	[RETURN]
 *              0       : 		SUCCESS
 *              other :          	fail, error code
 *
 *   [HISTORY]
 *             @2011.7.5 uni.chen : create this function
 **************************************************************************/
int dbussend_sendmsg_methodcall_reply(dbussend_hdl_st *p_dbussend_hdl, \
	const char *dest, const char *interface, const char *method, \
	void *p_dbussend_msg, int len, void *p_reply, int *reply_len);

#endif

