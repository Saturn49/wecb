#ifndef TR69_INTERNAL_H
#define TR69_INTERNAL_H

#include "tsl_common.h"
#include "soapStub.h"

typedef enum{
        event_bootstrap = 1,
        event_boot,
        event_periodic,
        event_scheduled,
        event_value_change,
        event_kicked,
        event_connection_request,
        event_transfer_complete,
        event_diagnostics_complete,
        event_request_download,
        event_reboot,
        event_schedule_inform,
        event_download,
        event_upload
}event_id;

enum internal_inform_state{
        inform_boot_state = 1,
        inform_boottrap_state,
        inform_period_state,
        inform_value_change_state,
        inform_passive_value_change_state,
        inform_upload_transfer_complete_state,
        inform_download_transfer_complete_state,
        inform_schedule_state,
        inform_diagnostics_complete_state,
        inform_no_event_state,
        inform_reboot_state,
        inform_connection_request,
        inform_acs_update_state,

};

enum {
    ACS_NULL_CHANGE = 0,
    ACS_URL_CHANGE,
    ACS_PERIODIC_CHANGE,
} TYPE_ACS_CHANGE_VAL;

typedef struct inform_event_s{
        char event[32];
        event_id id;
}inform_event_t;

typedef struct fault_subcode_s{
        char para_name[128];
        int fault_code;
}fault_subcode_t;

typedef struct fault_detail_s{
        int fault_code;
        char fault_subcode[128];
        int sub_tb_count;
        fault_subcode_t sub_tb[128];
}fault_detail_t;


typedef struct SSL_Info_s
{
	tsl_int_t enable;
	tsl_char_t keyfile[128];
	tsl_char_t password[128];
	tsl_char_t cacertfile[128];
	tsl_char_t DHfile[128];
	tsl_char_t serveridentification[128];
}SSL_Info_t;

enum tr69_session_state{
        TR69_SESSION_STATE_IDLE = 101,
        TR69_SESSION_STATE_CONNECTION_UP,
        TR69_SESSION_STATE_CONNECTION_DOWN,
        TR69_SESSION_STATE_INITIATION,
        TR69_SESSION_STATE_ACTIVE_WAITING,
        TR69_SESSION_STATE_PASSIVE_WAITING,
        TR69_SESSION_STATE_TERMINATION
};

typedef struct tr69_info_s{
        sem_t tr69_data_mutex;
        
        tsl_int_t tr69_event_tb[16];
        tsl_int_t tr69_event_tb_count;

        tsl_uint_t tr69_retry_fail;
        
        tsl_int_t tr69_periodic_inform_enable;
        tsl_int_t tr69_periodic_inform_time;
#if defined(AEI_TR181) || defined(AEI_TR106)
        tsl_uint_t tr69_periodic_notification_throttle;
#endif

        tsl_char_t tr69_acs_server[512];
        tsl_char_t tr69_acs_username[264];
        tsl_char_t tr69_acs_password[264];
#ifdef AEI_TR69_TWC_URI_QUERY
        tsl_char_t tr69_acs_provcode[128];
        tsl_char_t tr69_acs_uriprefix[128];
#endif
        tsl_char_t tr69_connect_url[256];
        tsl_char_t tr69_connect_username[264];
        tsl_char_t tr69_connect_password[264];
        
        tsl_int_t tr69_active_SSL;
        tsl_int_t tr69_passive_SSL;
        tsl_int_t tr69_passive_digest;
}tr69_info_t;
#ifdef AEI_TR98
typedef enum {
    URL_FTP = 1,
    URL_HTTP = 2,
    URL_HTTPS = 3,
    URL_TFTP = 4,
} url_transport_t;

/* URL structure is transport://user:pass@host:port/path?query */
typedef struct {
    url_transport_t transport;
    char *user;
    char *pass;
    char *host;
    unsigned short port;
    int has_port; /* Does the URL contain a port number */
    char *path;
    char *query;
} parsed_url_t;

#define HTTP_PREFIX "http://"
#define HTTPS_PREFIX "https://"
#define FTP_PREFIX "ftp://"
#define TFTP_PREFIX "tftp://"
#endif
#define TIME_DM_FORMART_STR      "%Y-%m-%dT%H:%M:%S%z"
#define UNKNOWN_DATE            "0000-00-00T00:00:00+0000"
#define VERSION_NA     -1
#define VERSION_CHANGE     1
#define VERSION_NO_CHANGE 0

#ifdef AEI_WECB_CUSTOMER_TELUS
#define PASSIVE_PORT 7547
#elif defined(AEI_WECB_CUSTOMER_TWC) || defined(AEI_WECB_CUSTOMER_BH)
#define PASSIVE_PORT 7547
#else
#define PASSIVE_PORT 4567
#endif

#if defined(AEI_WECB_CUSTOMER_TWC) || defined(AEI_WECB_CUSTOMER_BH)
/* Bright House ACS supports multiple RPC commands within one sessoin, 
   the interval between two commands can be up to 25 seconds, so we need to increase the gsoap receive timeout accordingly */  
#define RESPONSE_TIME_OUT 30 
#else
#define RESPONSE_TIME_OUT 15 
#endif
#define TR69_INFO                    "/mnt/rt_conf/tr69_info"
#define PERIODIC_INFORM_INTERVAL 86400
#ifdef AEI_TR98
#define CWMP_RETRY_DELAY 7200
#define UNKNOWN_DATE "0000-00-00T00:00:00+0000"
#define TR69_DOWNLOAD_COMPLETE 	"/mnt/rt_conf/tr69_download_complete"
#define TR69_UPDATE_COMPLETE 		"/mnt/rt_conf/tr69_fw_update_complete"
#endif
#define TR69_COMMAND_KEY         "/mnt/rt_conf/tr69_command_key"

#define INVALID_TRASFER_ERROR    12         /*receive motive send invalid trasfer complete (FaultCode 8803) */
extern sem_t global_tr69_inform_mutex;

extern tsl_char_t *global_acs_server;
extern tsl_char_t global_version[64];
extern tsl_int_t global_version_flag;

tsl_char_t global_tr69_ConnectionRequestUsername[128];
tsl_char_t global_tr69_ConnectionRequestPassword[128];


extern struct soap soap_passive_server;
extern struct soap soap_active_server;


extern tr69_info_t global_tr69_info;

tsl_rv tr69_build_send_inform(struct soap *p_soap, 
                              tsl_int_t *p_event_tb, 
                              tsl_int_t event_tb_size);

tsl_void_t *tr69_thread_event(tsl_void_t *argv);
tsl_rv tr69_set_connection_user_and_passwd();

tsl_void_t tr69_active_interface_inform_state(enum internal_inform_state inform_state);
tsl_void_t tr69_active_interface_session_state(enum tr69_session_state session_state);

tsl_int_t random_num(tsl_int_t begin, tsl_int_t end);
tsl_int_t get_retry_time();
#ifdef AEI_TR98
int internet_reachable(void);
time_t get_cwmp_first_inform_time(const time_t time);
#endif
tsl_rv tr69_set_ssl_info();
tsl_int_t set_ssl_context(struct soap *soap, int type);
tsl_int_t  check_ssl_accept(struct soap *soap);

time_t get_boot_time();

tsl_rv tr69_build_send_transferComplete(struct soap *p_soap, tsl_int_t type, tsl_int_t down);
tsl_void_t tr69_interface_terminate_soap();
tsl_void_t tr69_interface_terminate_msg();
void setDefaultActiveNotification(void);

#ifdef TR69_DEBUG_CWMP
tsl_int_t tr69debug_server_init(void);
tsl_int_t tr69debug_server_set_boot_inform(tsl_int_t boot_inform_sent);
tsl_int_t tr69debug_server_set_last_inform_time();
tsl_int_t tr69debug_server_set_inform_failures(tsl_int_t retry_fail);
tsl_int_t tr69debug_server_set_backoff_status(tsl_int_t backoff_status);
tsl_int_t tr69debug_server_cmd(tsl_int_t sock);
tsl_int_t tr69debug_client_cmd(int argc, char *argv[]);
#endif
#ifdef AEI_TR98
#define TR69_OID_ROOT_CONN "InternetGatewayDevice" 
#elif defined AEI_TR106 || defined AEI_TR181
#define TR69_OID_ROOT_CONN "Device"
#endif

#define TR69_PTR_FREE(ptr)    if (ptr)  { free(ptr); ptr=NULL; }

void free_cwmp_inform(struct _cwmp__Inform *p_inform);
void getMemStatus();
#define DEBUG_MEM_STRING   // printf("%s:%d\t", __FUNCTION__, __LINE__);  getMemStatus();

#ifdef AEI_TR69
void soap_Unknown_Method_fault(struct soap *soap);
#endif

tsl_int_t tr69_acs_change();

#endif
