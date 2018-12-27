/* xmppc.h
** XMPP client
** Copyright (C) 2014 Actiontec, Inc.
** fkong@actiontec.com
***/

#ifndef _XMPPC_H_
#define _XMPPC_H_

#define CUSTOM_PING_INTERVAL  300
#define C2S_PING_TIMEOUT 6000                // 6 secs


#define  XMPP_STATUS_ENABLED        "Enabled"
#define  XMPP_STATUS_DISABLED       "Disabled"
#define  XMPP_STATUS_UNKNOWN        "Unknown"
#define  XMPP_STATUS_DORMANT        "Dormant"
#define  XMPP_STATUS_CONNECTING     "Connecting"
#define  XMPP_STATUS_SVRNOT         "ServerNotPresent"
#define  XMPP_STATUS_MISCONF        "Error_Misconfigured"
#define  XMPP_STATUS_AUTHFAIL       "Error_AuthenticationFailure"


#define MAX(a, b)   (((a)>(b))? (a):(b))
#define MIN(a, b)   (((a)<(b))? (a):(b))

typedef struct _xmpp_info_t {

    char username[256];
    char password[256];
    char domain[64];
    char resource[64];
    char server[256];
    int port;
    int n_fail_retry;
    int KeepAliveInterval;
    int ServerConnectAttempts;
    int ServerRetryInitialInterval;
    int ServerRetryIntervalMultiplier;
    int ServerRetryMaxInterval;
    int UseTLS;
    int cur_svrip_id;
    int svr_ip_cnt;

} xmpp_info_t;

void xmpp_info_update();
void xmpp_retry_update();

int check_xmpp_retry();
int get_retry_time(xmpp_info_t * p_xmpp_info);
int inc_xmpp_retry();
int need_try_nextsvr();

int set_svrip_cnt(int cnt);
int get_svrip_id();

#endif
