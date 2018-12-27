#ifndef _CTL_MSG_H
#define _CTL_MSG_H
#include "ctl.h"
#include "ctl_nid.h"
#include "tsl_common.h"
#include <sys/types.h>
#include <unistd.h>

#define _STR(s)		#s
#define STR(s)		_STR(s)

#define CTL_MSG_PREFIX	"ctl.msg"
#define CTL_MSG_TYPE(id)		(CTL_MSG_PREFIX STR(id))


// for CTL_MSG_ACCESS_CFG
#define CTL_METHOD_SAVE 	"SAVE"
#define CTL_METHOD_RESTORE 	"RESTORE"
#define CTL_METHOD_RESTORE_DEFAULT 	"RESTORE_DEFAULT"

typedef enum 
{
   CTL_MSG_ACCESS_CFG_METHODCALL  = 0x01, 	// Method call, Ask RTD to Save/Restore cfg.xml
   CTL_MSG_ACCESS_CFG_SIGNAL      = 0x02, 	// Signal msg,  Ask RTD to Save cfg.xml
   CTL_MSG_L2_STATE_CHANGED       = 0x03,       // DEBUG, simulator driver event

   //CTL_MSG_WAN_CONN_IPv4_ADDR = 0x10002408,
   CTL_MSG_WAN_CONN_IPV4_CHANGED = 0x04, 	// To support ip6ip4 tunnel, When IPv6 Changed in BRCM MDM, sync to CTL.
   CTL_MSG_WATCH_WAN_CONN_IPV4    = 0x05, 	// Request RTD to watch the ipv4 status
   CTL_MSG_WATCH_WAN_CONN_PPP	  = 0x06,	// Request RTD to watch the ppp status
   CTL_MSG_WAN_CONN_PHYSICAL_CHANGED = 0x07, //Physical Wan is Ethernet or xDSL
   CTL_MSG_WAN_CONN_INTERFACE_CHANGED = 0x08, //Wan Interface is ppp0 or ppp0.1 or others

   CTL_MSG_SYSTEM_BOOT    = 0x10000250, /**< system has booted, delivered to apps
                                         *   EIF_LAUNCH_ON_STARTUP set in their
                                         *   CmsEntityInfo.flags structure.
                                         */
   CTL_MSG_APP_LAUNCHED   = 0x10000251, /**< Used by apps to confirm that launch succeeded.
                                         *   Sent from app to smd in cmsMsg_init.
                                         */
   CTL_MSG_WAN_LINK_UP    = 0x10000252, /**< wan link is up (includes dsl, ethernet, etc) */
   CTL_MSG_WAN_LINK_DOWN  = 0x10000253, /**< wan link is down */
   CTL_MSG_WAN_CONNECTION_UP   = 0x10000254, /**< WAN connection is up (got IP address) */
   CTL_MSG_WAN_CONNECTION_DOWN = 0x10000255, /**< WAN connection is down (lost IP address) */
   CTL_MSG_ETH_LINK_UP    = 0x10000256, /**< eth link is up (only if eth is used as LAN interface) */
   CTL_MSG_ETH_LINK_DOWN  = 0x10000257, /**< eth link is down (only if eth is used as LAN interface) */
   CTL_MSG_USB_LINK_UP    = 0x10000258, /**< usb link is up (only if eth is used as LAN interface) */
   CTL_MSG_USB_LINK_DOWN  = 0x10000259, /**< usb link is down (only if eth is used as LAN interface) */
   CTL_MSG_ACS_CONFIG_CHANGED = 0x1000025A, /**< ACS configuration has changed. */
   CTL_MSG_DELAYED_MSG    = 0x1000025B, /**< This message is delivered to when delayed msg timer expires. */
   CTL_MSG_TR69_ACTIVE_NOTIFICATION = 0x1000025C,
						/**< This message is sent to tr69c when one or more
                                                   *   parameters with active notification attribute
                                                   *   has had their value changed.
                                                   */
   CTL_MSG_SHMID                     = 0x10000262, /**< Sent from ssk to smd when shmid is obtained. */
   CTL_MSG_MDM_INITIALIZED           = 0x10000263, /**< Sent from ssk to smd when MDM has been initialized. */
   CTL_MSG_DHCPC_STATE_CHANGED       = 0x10000264, /**< Sent from dhcp client when state changes, see also DhcpcStateChangeMsgBody */
   CTL_MSG_PPPOE_STATE_CHANGED       = 0x10000265, /**< Sent from pppoe when state changes, see also PppoeStateChangeMsgBody */
   CTL_MSG_DHCP6C_STATE_CHANGED      = 0x10000266, /**< Sent from dhcpv6 client when state changes, see also Dhcp6cStateChangeMsgBody */
   CTL_MSG_DHCPD_RELOAD		     = 0x10000268, /**< Sent to dhcpd to force it reload config file without restart */
   CTL_MSG_PING_STATE_CHANGED        = 0x10000267, /**< Ping state changed (completed, or stopped) */
   CTL_MSG_DHCPD_DENY_VENDOR_ID	    = 0x10000269, /**< Sent from dhcpd to notify a denied request with some vendor ID */
   CTL_MSG_DHCPD_HOST_INFO           = 0x1000026A, /**< Sent from dhcpd to inform of lan host add/delete */
   CTL_MSG_MYNETWORK_HOST_INFO 	= 0x1000026B,/**< Sent from mynetwork to notify a LAN host join or leave by static mode*/   
   CTL_MSG_DHCPD_HOST_ACTIVE         = 0x1000026C,
   CTL_MSG_DNSPROXY_RELOAD	     = 0x10000270, /**< Sent to dnsproxy to force it reload config file without restart */
   CTL_MSG_SNTP_STATE_CHANGED 	     = 0x10000271, /**< SNTP state changed */
   CTL_MSG_MCPD_RELOAD	             = 0x10000276, /**< Sent to mcpd to force it reload config file without restart */
   CTL_MSG_CONFIG_WRITTEN             = 0x10000280, /**< Event sent when a config file is written. */

   CTL_MSG_SET_PPP_UP                 = 0x10000290, /* Sent to ppp when set ppp up manually */
   CTL_MSG_SET_PPP_DOWN               = 0x10000291, /* Sent to ppp when set ppp down manually */  

   CTL_MSG_DNSPROXY_DUMP_STATUS       = 0x100002A1, /* Tell dnsproxy to dump its current status */
   CTL_MSG_DNSPROXY_DUMP_STATS        = 0x100002A2, /* Tell dnsproxy to dump its statistics */
   CTL_MSG_L3INTERFACE_START	 	  = 0x100002A3, /* rutWan_stopL3Interface */   
   CTL_MSG_L3INTERFACE_STOP			  = 0x100002A4, /* rutWan_startL3Interface */

   CTL_MSG_DATA_CENTER_READY		  = 0x100002A5, /* data_center has initialized completely */
   
#ifdef BRCM_WLAN
   CTL_MSG_WLAN_CHANGED          		     = 0x10000300,  /**< wlmngr jhc*/
#endif
   CTL_MSG_SNMPD_CONFIG_CHANGED = 0x1000301, /**< ACS configuration has changed. */
   CTL_MSG_MANAGEABLE_DEVICE_NOTIFICATION_LIMIT_CHANGED = 0x1000302, /**< Notification Limit of number of management device. */

   CTL_MSG_STORAGE_ADD_PHYSICAL_MEDIUM = 0x1000310,
   CTL_MSG_STORAGE_REMOVE_PHYSICAL_MEDIUM = 0x1000311,
   CTL_MSG_STORAGE_ADD_LOGICAL_VOLUME = 0x1000312,
   CTL_MSG_STORAGE_REMOVE_LOGICAL_VOLUME = 0x1000313,

   CTL_MSG_REGISTER_DELAYED_MSG      = 0x10000800, /**< request a message sometime in the future. */
   CTL_MSG_UNREGISTER_DELAYED_MSG    = 0x10000801, /**< cancel future message delivery. */
   CTL_MSG_REGISTER_EVENT_INTEREST   = 0x10000802, /**< request receipt of the specified event msg. */
   CTL_MSG_UNREGISTER_EVENT_INTEREST = 0x10000803, /**< cancel receipt of the specified event msg. */
   CTL_MSG_DIAG                    = 0x10000805, /**< request diagnostic to be run */
   CTL_MSG_TR69_GETRPCMETHODS_DIAG = 0x10000806, /**< request tr69c send out a GetRpcMethods */
   CTL_MSG_DSL_LOOP_DIAG_COMPLETE  = 0x10000807, /**< dsl loop diagnostic completes */

   CTL_MSG_START_APP         = 0x10000808, /**< request smd to start an app; pid is returned in the wordData */
   CTL_MSG_RESTART_APP       = 0x10000809, /**< request smd to stop and then start an app; pid is returned in the wordData */
   CTL_MSG_STOP_APP          = 0x1000080A, /**< request smd to stop an app */
   CTL_MSG_IS_APP_RUNNING    = 0x1000080B, /**< request to check if the the application is running or not */

   CTL_MSG_REBOOT_SYSTEM     = 0x10000850,  /**< request smd to reboot, a response means reboot sequence has started. */

   CTL_MSG_SET_LOG_LEVEL       = 0x10000860,  /**< request app to set its log level. */
   CTL_MSG_SET_LOG_DESTINATION = 0x10000861,  /**< request app to set its log destination. */

   CTL_MSG_SET_AUTO_WAN        = 0x10000862,  /**< request ssk to set wan auto detect setting. */
   CTL_MSG_MEM_DUMP_STATS      = 0x1000086A,  /**< request app to dump its memstats */
   CTL_MSG_MEM_DUMP_TRACEALL   = 0x1000086B,  /**< request app to dump all of its mem leak traces */
   CTL_MSG_MEM_DUMP_TRACE50    = 0x1000086C,  /**< request app to its last 50 mem leak traces */
   CTL_MSG_MEM_DUMP_TRACECLONES= 0x1000086D,  /**< request app to dump mem leak traces with clones */

   CTL_MSG_LOAD_IMAGE_STARTING = 0x10000870,  /**< notify smd that image network loading is starting. */
   CTL_MSG_LOAD_IMAGE_DONE     = 0x10000871,  /**< notify smd that image network loading is done. */
   CTL_MSG_GET_CONFIG_FILE     = 0x10000872,  /**< ask smd for a copy of the config file. */
   CTL_MSG_VALIDATE_CONFIG_FILE= 0x10000873,  /**< ask smd to validate the given config file. */
   CTL_MSG_WRITE_CONFIG_FILE   = 0x10000874,  /**< ask smd to write the config file. */
   CTL_MSG_VENDOR_CONFIG_UPDATE = 0x10000875,  /**<  the config file. */

   CTL_MSG_GET_WAN_LINK_STATUS = 0x10000880,  /**< request current WAN LINK status. */
   CTL_MSG_GET_WAN_CONN_STATUS = 0x10000881,  /**< request current WAN Connection status. */
   CTL_MSG_GET_LAN_LINK_STATUS = 0x10000882,  /**< request current LAN LINK status. */
#ifdef SUPPORT_DYNA_PHY_AUTO_SWITCH
   CTL_MSG_WATCH_WAN_PHY_TYPE= 0x10000895,
#endif

   CTL_MSG_WATCH_WAN_CONNECTION= 0x10000890,  /**< request ssk to watch the dsl link status and then change the connectionStatus for bridge, static MER and ipoa */
   CTL_MSG_WATCH_DSL_LOOP_DIAG = 0x10000891,  /**< request ssk to watch the dsl loop diag and then update the stats */

   CTL_MSG_GET_LEASE_TIME_REMAINING = 0x100008A0,  /**< ask dhcpd how much time remains on lease for particular LAN host */
   CTL_MSG_GET_DEVICE_INFO          = 0x100008A1,  /**< request system/device's info */
   CTL_MSG_REQUEST_FOR_PPP_CHANGE   = 0x100008A2,  /**< request for disconnect/connect ppp  */
#if 1 
   CTL_MSG_MOCA_WRITE_LOF           = 0x100008B1, /**< mocad reporting last operational frequency */
   CTL_MSG_MOCA_READ_LOF            = 0x100008B2, /**< mocad reporting last operational frequency */
   CTL_MSG_MOCA_WRITE_MRNONDEFSEQNUM= 0x100008B3, /**< mocad reporting moca reset non-def sequence number */
   CTL_MSG_MOCA_READ_MRNONDEFSEQNUM = 0x100008B4, /**< mocad reporting moca reset non-def sequence number */
   CTL_MSG_MOCA_NOTIFICATION        = 0x100008B5, /**< application reporting that it has updated moca parameters */

   CTL_MSG_QOS_DHCP_OPT60_COMMAND   = 0x100008C0, /**< QoS Vendor Class ID classification command */
   CTL_MSG_QOS_DHCP_OPT77_COMMAND   = 0x100008C1, /**< QoS User   Class ID classification command */
   
   CTL_MSG_VOICE_CONFIG_CHANGED= 0x10002000,  /**< Voice Configuration parameter changed private event msg. */
   CTL_MSG_VODSL_BOUNDIFNAME_CHANGED = 0x10002001, /**< vodsl BoundIfName param has changed. */
   CTL_MSG_SHUTDOWN_VODSL= 0x10002002,  /**< Voice shutdown request. */
   CTL_MSG_START_VODSL                 = 0x10002003, /**< Voice start request. */
   CTL_MSG_REBOOT_VODSL                = 0x10002004, /**< Voice reboot request. This is for the voice reboot command */
   CTL_MSG_RESTART_VODSL               = 0x10002005, /**< Voice re-start request. This is to restart the call manager when the IP address changes*/
   CTL_MSG_INIT_VODSL                  = 0x10002006, /**< Voice init request. */
   CTL_MSG_DEINIT_VODSL                = 0x10002007, /**< Voice init request. */
   CTL_MSG_RESTART_VODSL_CALLMGR       = 0x10002008, /**< Voice call manager re-start request. */
#ifdef DMP_X_ACTIONTEC_COM_NTR_1
   CTL_MSG_VOICE_NTR_CONFIG_CHANGED    = 0x10002009, /**< Voice NTR Configuration parameter changed private event msg. */
#endif /* DMP_X_ACTIONTEC_COM_NTR_1 */
	
   CTL_MSG_VOICE_DIAG          = 0x10002100,  /**< request voice diagnostic to be run */
   CTL_MSG_VOICE_STATISTICS_REQUEST    = 0x10002101, /**< request for Voice call statistics */
   CTL_MSG_VOICE_STATISTICS_RESPONSE   = 0x10002102, /**< response for Voice call statistics */
   CTL_MSG_VOICE_STATISTICS_RESET      = 0x10002103, /**< request to reset Voice call statistics */
   CTL_MSG_VOICE_CM_ENDPT_STATUS         = 0x10002104, /**< request for Voice Line obj */
   CTL_MSG_VODSL_IS_READY_FOR_DEINIT   = 0x10002105, /**< query if the voice app is ready to deinit */
#ifdef DMP_X_ACTIONTEC_COM_DECTENDPOINT_1
   CTL_MSG_VOICE_DECT_OPEN_REG_WND     = 0x100021F0, /**< request for opening DECT registration window */
   CTL_MSG_VOICE_DECT_CLOSE_REG_WND    = 0x100021F1, /**< request for closing DECT registration window */
   CTL_MSG_VOICE_DECT_INFO_REQ         = 0x100021F2, /**< request for Voice DECT status information */
   CTL_MSG_VOICE_DECT_INFO_RSP         = 0x100021F3, /**< response for Voice DECT status information */
   CTL_MSG_VOICE_DECT_AC_SET           = 0x100021F4, /**< request for Voice DECT Access Code Set */
   CTL_MSG_VOICE_DECT_HS_INFO_REQ      = 0x100021F5, /**< request for Voice DECT handset status information */
   CTL_MSG_VOICE_DECT_HS_INFO_RSP      = 0x100021F6, /**< response for Voice DECT handset status information */
   CTL_MSG_VOICE_DECT_HS_DELETE        = 0x100021F7, /**< request for deleting a handset from DECT module */
   CTL_MSG_VOICE_DECT_HS_PING          = 0x100021F8, /**< request for pinging a handset from DECT module */
   CTL_MSG_VOICE_DECT_NUM_ENDPT        = 0x100021F9, /**< request for number of DECT endpoints */
#endif /* DMP_X_ACTIONTEC_COM_DECTENDPOINT_1 */

   CTL_MSG_GET_GPON_OMCI_STATS = 0x10002200, /**< request GPON OMCI statistics */
   CTL_MSG_OMCI_COMMAND_REQUEST = 0x10002201, /**< GPON OMCI command message request */
   CTL_MSG_OMCI_COMMAND_RESPONSE = 0x10002202, /**< GPON OMCI command message response */
   CTL_MSG_OMCI_DEBUG_GET_REQUEST = 0x10002203, /**< GPON OMCI debug get message request */
   CTL_MSG_OMCI_DEBUG_GET_RESPONSE = 0x10002204, /**< GPON OMCI debug get message response */
   CTL_MSG_OMCI_DEBUG_SET_REQUEST = 0x10002205, /**< GPON OMCI debug set message request */
   CTL_MSG_OMCI_DEBUG_SET_RESPONSE = 0x10002206, /**< GPON OMCI debug set message response */

   CTL_MSG_CMF_SEND_REQUEST = 0x10002301, /**< CMF File Send message request */
   CTL_MSG_CMF_SEND_RESPONSE = 0x10002302, /**< CMF File Send message response */
#endif

#if 1 
#ifdef DMP_DOWNLOAD_1
   CTL_MSG_TR143_DLD_DIAG_COMPLETED = 0x10002400,
#endif
#ifdef DMP_UPLOAD_1
   CTL_MSG_TR143_UPLD_DIAG_COMPLETED = 0x10002401,
#endif
#ifdef DMP_UDPECHO_1
   CTL_MSG_TR143_ECHO_DIAG_RESULT = 0x10002402,
#endif

#if defined(DMP_UPNPDISCBASIC_1) && defined (DMP_UPNPDISCADV_1)
   CTL_MSG_UPNP_DISCOVERY_INFO = 0x10002403,   /*Send upnp device and service info*/
#endif
#ifdef DMP_PERIODICSTATSBASE_1
   CTL_MSG_PERIODIC_STATISTICS_TIMER = 0x10002404,
#endif
#endif

#ifdef AEI_SUPPORT_6RD
    CTL_MSG_WATCH_6RD_OPTION = 0x10002409,
#endif

#ifdef AEI_VDSL_CUSTOMER_NCS
   CTL_MSG_GET_LANHOSTS_RENEW = 0x10002407,  /**< ask mynetwork to renew the lanhost table */
   CTL_MSG_WAN_UPDATE_CONFIG = 0x10002408,
#endif

   CTL_MSG_OMCIPMD_MCAST_COUNTER_REQ = 0x10002410, /**< OMCIPMD command message request for multicast RX octets. */
   CTL_MSG_OMCIPMD_MCAST_COUNTER_RSP = 0x10002411, /**< OMCIPMD command message response for multicast RX octets. */

   CTL_MSG_OMCID_OMCIPMD_REQUEST = 0x10002420, /**< OMCIPMD command message request. */
   CTL_MSG_ENCAPSULATED_OMCI = 0x10002421, /**< OMCIPMD encapsulated OMCI raw frame. */

   CTL_MSG_MCPD_SHOW_CONFIG          = 0x10002430, /**< Sent to mcpd to show IGMP/MLD running config (ACTIONTEC_IGMP_MCF) */
   CTL_MSG_MCPD_SHOW_MCF_CONFIG      = 0x10002431, /**< Sent to mcpd to show IGMP/MLD Group Membership (ACTIONTEC_IGMP_MCF) */
   CTL_MSG_MCPD_SHOW_MEMBERSHIP      = 0x10002432, /**< Sent to mcpd to show IGMP/MLD Group Membership (ACTIONTEC_IGMP_MCF) */
    USER_REQUEST_PPP_RESET = 0x10002441,
    USER_REQUEST_IP_RESET = 0x10002442,
#ifdef AEI_SUPPORT_6RD
	CTL_MSG_6RD_OPTION_CHANGED = 0x10002450, /**< Sent from dhcp client when state changes, see also DhcpcStateChangeMsgBody */
	#endif
	CTL_MSG_CONFIG_LOAD = 0x10002451, /**<send from dal when user load config file. */
	CTL_MSG_REBOOT = 0x10002452,
   CTL_MSG_UPNPD_CONFIG_UPDATE	           = 0x10002453, /**< Sent from httpd or tr69 to miniupnpd to update config  */
   CTL_MSG_UPNPD_GET_PORTMAPPING_LIST	   = 0x10002454, /**< Sent from miniupnpd to ssk to get portmapping list  */
   CTL_MSG_UPNPD_ADD_PORTMAPPING           = 0x10002455, /**< Sent from miniupnpd to ssk to add port mapping info */
   CTL_MSG_UPNPD_DEL_PORTMAPPING	   = 0x10002456, /**< Sent from miniupnpd to ssk to del port mapping info */
} CtlMsgType;


typedef struct ctl_msg_header
{
    tsl_u32_t data_length; //this is the length of buffer
    tsl_u8_t buffer[0];     //the buffer head address. 
}CtlMsgHeader;

typedef struct ctl_msg_appinfo
{
	pid_t pid;
	tsl_u8_t fullcmd[MAX_NAME_SIZE];
}CtlMsgAppinfo;

#define EMPTY_MSG_HEADER   {0, {0} } /**< Initialize msg header to empty */
/** Data body for CTL_MSG_DHCP6C_STATE_CHANGED message type.
 *
 */
typedef struct
{
   tsl_bool prefixAssigned;  /**< Have we been assigned a site prefix ? */
   tsl_bool addrAssigned;    /**< Have we been assigned an IPv6 address ? */
   tsl_bool dnsAssigned;     /**< Have we been assigned dns server addresses ? */
   tsl_8_t sitePrefix[BUFLEN_48];   /**< New site prefix, if prefixAssigned==TSL_B_TRUE */
   tsl_u32_t prefixPltime;
   tsl_u32_t prefixVltime;
   tsl_u32_t prefixCmd;
   tsl_8_t ifname[BUFLEN_32];
   tsl_8_t address[BUFLEN_48];      /**< New IPv6 address, if addrAssigned==TSL_B_TRUE */
   tsl_8_t pdIfAddress[BUFLEN_48];      /**< New IPv6 address of PD interface */
   tsl_u32_t addrCmd;
   tsl_8_t nameserver[BUFLEN_128];  /**< New nameserver, if addressAssigned==TSL_B_TRUE */
#ifdef AEI_SUPPORT_DSLITE
   tsl_8_t aftrname[BUFLEN_64];        /* DSLite AFTR domain name */
#endif   
   tsl_bool defaultGWAssigned; /** if default gateway assigned */
   tsl_8_t defaultGW[BUFLEN_48]; /** default gateway if defaultGWAssigned it true */
   tsl_u32_t t1;					/*Timeout for renew*/
   tsl_u32_t t2;					/*Timeout for rebind*/
   tsl_8_t namesearchlist[BUFLEN_128];  /**dns search list */
} CtlDhcp6cStateChangedMsgBody;

/*!\PPPOE state defines
 * (was in syscall.h before)
 */

#define CTL_PPPOE_CLIENT_STATE_PADO          0   /* waiting for PADO */
#define CTL_PPPOE_CLIENT_STATE_PADS          1   /* got PADO, waiting for PADS */
#define CTL_PPPOE_CLIENT_STATE_CONFIRMED     2   /* got PADS, session ID confirmed */
#define CTL_PPPOE_CLIENT_STATE_DOWN          3   /* totally down */
#define CTL_PPPOE_CLIENT_STATE_UP            4   /* totally up */
#define CTL_PPPOE_SERVICE_AVAILABLE          5   /* ppp service is available on the remote */
#define CTL_PPPOE_AUTH_FAILED                7
#define CTL_PPPOE_RETRY_AUTH                 8
#define CTL_PPPOE_REPORT_LASTCONNECTERROR    9
#define CTL_PPPOE_CLIENT_STATE_UNCONFIGURED   10
#define CTL_PPPOE_CLIENT_IPV6_STATE_UP   11
#define CTL_PPPOE_CLIENT_IPV6_STATE_DOWN   12

/** Data body for CTL_MSG_DHCPC_STATE_CHANGED message type.
 *
 */
typedef struct
{
   tsl_bool addressAssigned; /**< Have we been assigned an IP address ? */
   tsl_char_t ip[BUFLEN_32];   /**< New IP address, if addressAssigned==TSL_B_TRUE */
   tsl_char_t mask[BUFLEN_32]; /**< New netmask, if addressAssigned==TSL_B_TRUE */
   tsl_char_t gateway[BUFLEN_32];    /**< New gateway, if addressAssigned==TSL_B_TRUE */
   tsl_char_t nameserver[BUFLEN_64]; /**< New nameserver, if addressAssigned==TSL_B_TRUE */
   tsl_char_t intfName[BUFLEN_128]; /**< New nameserver, if addressAssigned==TRUE */
#ifdef AEI_SUPPORT_6RD
   // 6rd
   tsl_bool   b6rdAssigned; // Have we been assigned 6rd prefix
   tsl_u32_t  ipv6rdipv4masklen;
   tsl_char_t ipv6rdbripv4addr[BUFLEN_32];
   tsl_u32_t  ipv6rdprefixlen;
   tsl_char_t ipv6rdprefix[BUFLEN_64];
#endif
} CtlDhcpcStateChangedMsgBody;

typedef struct
{
   tsl_bool logState;      
   tsl_u32_t logLevel;  
   tsl_char_t moduleName[BUFLEN_32];
} CtlLogLevelChangeMsgBody;


typedef struct
{
   tsl_u8_t pppState;       /**< pppoe states */
   tsl_char_t ip[BUFLEN_32];   /**< New IP address, if pppState==PPPOE_CLIENT_STATE_UP */
   tsl_char_t mask[BUFLEN_32]; /**< New netmask, if pppState==PPPOE_CLIENT_STATE_UP */
   tsl_char_t gateway[BUFLEN_32];    /**< New gateway, if pppState==PPPOE_CLIENT_STATE_UP */
   tsl_char_t nameserver[BUFLEN_64]; /**< New nameserver, if pppState==PPPOE_CLIENT_STATE_UP */
   tsl_char_t servicename[BUFLEN_256]; /**< service name, if pppState==PPPOE_CLIENT_STATE_UP */
   tsl_char_t ppplastconnecterror[PPP_CONNECT_ERROR_REASON_LEN];
} CtlPppoeStateChangeMsgBody;

typedef struct
{
	tsl_char_t ConnectionStatus[BUFLEN_32];
	tsl_char_t ConnectionType[BUFLEN_32];
	tsl_char_t AddressingType[BUFLEN_32];
	tsl_char_t ExternalIPAddress[BUFLEN_32];
	//tsl_char_t SubnetMask[BUFLEN_32];
	//tsl_char_t DefaultGateway[BUFLEN_32];
	tsl_u32_t MaxMTUSize;
	//MACAddress;
	tsl_char_t X_ACTIONTEC_COM_IfName[BUFLEN_32];
} CtlWANConnIPv4StateChangedMsgBody;

typedef struct
{
	tsl_char_t Result[BUFLEN_32];
	tsl_char_t RTT[BUFLEN_32];
} Ctl6RDNUDMsgBody;

typedef struct
{
	tsl_char_t PhysicalWANType[BUFLEN_32];
	tsl_char_t AddressingType[BUFLEN_32];
}CtlPhysicalWANChangedMsgBody;

typedef struct
{
	tsl_bool isIpObj;
	tsl_char_t IfName[BUFLEN_32];
}CtlWANIfNameChangedMsgBody;

typedef struct
{
	tsl_char_t ConnectionStatus[BUFLEN_32];
	tsl_char_t ConnectionType[BUFLEN_32];
	tsl_char_t AddressingType[BUFLEN_32];
	//tsl_char_t ExternalIPAddress[BUFLEN_32];
	//tsl_char_t SubnetMask[BUFLEN_32];
	//tsl_char_t DefaultGateway[BUFLEN_32];
	tsl_u32_t MaxMTUSize;
	//MACAddress;
	tsl_char_t X_ACTIONTEC_COM_IfName[BUFLEN_32];
} CtlWANConnPppStateChangedMsgBody;

/*!\PPPOE state defines 
 * (was in syscall.h before)
 */

#define PPPOE_CLIENT_STATE_PADO          0   /* waiting for PADO */
#define PPPOE_CLIENT_STATE_PADS          1   /* got PADO, waiting for PADS */
#define PPPOE_CLIENT_STATE_CONFIRMED     2   /* got PADS, session ID confirmed */
#define PPPOE_CLIENT_STATE_DOWN          3   /* totally down */
#define PPPOE_CLIENT_STATE_UP            4   /* totally up */
#define PPPOE_SERVICE_AVAILABLE          5   /* ppp service is available on the remote */
#define PPPOE_AUTH_FAILED                7
#define PPPOE_REPORT_LASTCONNECTERROR    9
#define BCM_PPPOE_CLIENT_IPV6_STATE_UP   11
#define BCM_PPPOE_CLIENT_IPV6_STATE_DOWN   12

/** These values are returned in the wordData field of the response msg to
 *  CTL_MSG_GET_WAN_LINK_STATUS.
 *  See dslIntfStatusValues in ctl-data-model.xml
 * There is a bit of confusion here.  These values are associated with the
 * WANDSLInterfaceConfig object, but usually, a caller is asking about
 * a WANDSLLinkConfig object. For now, the best thing to do is just check
 * for WAN_LINK_UP.  All other values basically mean the requested link is
 * not up.
 */
#define WAN_LINK_UP                   0
#define WAN_LINK_INITIALIZING         1
#define WAN_LINK_ESTABLISHINGLINK     2
#define WAN_LINK_NOSIGNAL             3
#define WAN_LINK_ERROR                4
#define WAN_LINK_DISABLED             5

#define LAN_LINK_UP                   0
#define LAN_LINK_DISABLED             1


/** Data body for CTL_MSG_PPPOE_STATE_CHANGED message type.
 *
 */
typedef struct
{
   tsl_u8_t pppState;       /**< pppoe states */
   char ip[BUFLEN_32];   /**< New IP address, if pppState==PPPOE_CLIENT_STATE_UP */
   char mask[BUFLEN_32]; /**< New netmask, if pppState==PPPOE_CLIENT_STATE_UP */
   char gateway[BUFLEN_32];    /**< New gateway, if pppState==PPPOE_CLIENT_STATE_UP */
   char nameserver[BUFLEN_64]; /**< New nameserver, if pppState==PPPOE_CLIENT_STATE_UP */
   char servicename[BUFLEN_256]; /**< service name, if pppState==PPPOE_CLIENT_STATE_UP */
   char ppplastconnecterror[PPP_CONNECT_ERROR_REASON_LEN];
} PppoeStateChangeMsgBody;


/*!\enum NetworkAccessMode
 * \brief Different types of network access modes, returned by cmsDal_getNetworkAccessMode
 *        and also in the wordData field of CTL_MSG_GET_NETWORK_ACCESS_MODE
 */
typedef enum {
   NETWORK_ACCESS_DISABLED   = 0,  /**< access denied */
   NETWORK_ACCESS_LAN_SIDE   = 1,  /**< access from LAN */
   NETWORK_ACCESS_WAN_SIDE   = 2,  /**< access from WAN */
   NETWORK_ACCESS_CONSOLE    = 3   /**< access from serial console */
} NetworkAccessMode;

/**DHCP event for LAN host
*/
typedef enum {
   DHCP_EVENT_NO_ACTION = 0,
   DHCP_EVENT_ADD,
   DHCP_EVENT_UPDATE,
   DHCP_EVENT_DELETE
} DhcpEventType;

/** Data body for CTL_MSG_DHCPD_HOST_INFO message type.
 *
 */
typedef struct
{
   tsl_bool_t deleteHost;  /**< TRUE if we are deleting a LAN host, otherwise we are adding or editing LAN host */
   tsl_32_t leaseTimeRemaining;      /** Number of seconds left in the lease, -1 means no expiration */
   char ifName[CTL_IFNAME_LENGTH]; /**< brx which this host is on */
   char ipAddr[BUFLEN_48];         /**< IP Address of the host */
   char macAddr[MAC_STR_LEN+1];    /**< mac address of the host */
   char addressSource[BUFLEN_32];  /** source of IP address assignment, same value as
                                     * LANDevice.{i}.Hosts.Host.{i}.addressSource */
   char interfaceType[BUFLEN_32];  /** type of interface used by LAN host, same values as 
                                     * LANDevice.{i}.Hosts.Host.{i}.InterfaceType */
   tsl_bool_t active;                  /** the host is active or not , same value as */
   int icon;                       /** Device Identifer according to DHCP option 12/60 */
   char reservstatus[2];          /** DHCP Reservation Status */
   DhcpEventType eventType;
   char hostName[BUFLEN_64];       /** Both dhcpd and data model specify hostname as 64 bytes */
   char oui[BUFLEN_8];             /** Host's manufacturing OUI */
   char serialNum[BUFLEN_64];      /** Host's serial number */
   char productClass[BUFLEN_64];   /** Host's product class */
   char vendorID[BUFLEN_256];      /* vendor class id, option 60, rfc 2132, max length is 255 */
   int  leaseType;                /* type of lease dhcp,static dhcp or detected device */
   char clsname[BUFLEN_16];      /** Option 60 string for qos classification*/
   char macs[BUFLEN_128]; 
   char userClassID[BUFLEN_256];
   char venderClassID[BUFLEN_256];
   char clientID[BUFLEN_256];
   tsl_bool_t isStb;                  /** The lan host pc is stb or not */
   char subnet[BUFLEN_32];
   char gateway[BUFLEN_32];
   char dns[BUFLEN_64];
#ifdef MYNTK_IPV6
   char ipv6LLA[BUFLEN_48]; 	/** IPv6 addresses*/
   char ipv6ULA[BUFLEN_48];
   char ipv6GUA[BUFLEN_48];
#endif
} DhcpdHostInfoMsgBody;


/** Data body for CTL_MSG_GET_LEASE_TIME_REMAINING message type.
 *
 * The lease time remaing is returned in the wordData field of the
 * response message.  A -1 means the lease does not expire.
 * A 0 could mean the lease is expired, or that dhcpd has not record
 * of the mac address that was given.
 *
 */
typedef struct
{
   char ifName[CTL_IFNAME_LENGTH]; /**< brx which this host is on */
   char macAddr[MAC_STR_LEN+1];    /**< mac address of the host */
} GetLeaseTimeRemainingMsgBody;



typedef enum 
{
   VOICE_DIAG_CFG_SHOW           = 0,
   VOICE_DIAG_EPTCMD             = 1,
   VOICE_DIAG_STUNLKUP           = 2,
   VOICE_DIAG_STATS_SHOW         = 3,
   VOICE_DIAG_PROFILE            = 4,
   VOICE_DIAG_EPTAPP_HELP        = 5,
   VOICE_DIAG_EPTAPP_SHOW        = 6,
   VOICE_DIAG_EPTAPP_CREATECNX   = 7,
   VOICE_DIAG_EPTAPP_DELETECNX   = 8,
   VOICE_DIAG_EPTAPP_MODIFYCNX   = 9,
   VOICE_DIAG_EPTAPP_EPTSIG      = 10,
   VOICE_DIAG_EPTAPP_SET         = 11,
   VOICE_DIAG_EPTPROV            = 12,
   VOICE_DIAG_EPTAPP_DECTTEST    = 13,
} VoiceDiagType;

/** Data body for Voice diagonistic message */
typedef struct
{
  VoiceDiagType type;
  char cmdLine[BUFLEN_128];  
} VoiceDiagMsgBody;

/** Data body for Voice Line Obj */
typedef struct
{
   char regStatus[BUFLEN_128];
   char callStatus[BUFLEN_128];
} VoiceLineObjStatus;


/** Data body for CTL_MSG_PING_DATA message type.
 *
 */
typedef struct
{
   char diagnosticsState[BUFLEN_32];  /**< Ping state: requested, none, completed... */
   char interface[BUFLEN_32];   /**< interface on which ICMP request is sent */
   char host[BUFLEN_32]; /**< Host -- either IP address form or hostName to send ICMP request to */
   tsl_u32_t    numberOfRepetitions; /**< Number of ICMP requests to send */
   tsl_u32_t    timeout;	/**< Timeout in seconds */
   tsl_u32_t    dataBlockSize;	/**< DataBlockSize  */
   tsl_u32_t    DSCP;	/**< DSCP */
   tsl_u32_t    successCount;	/**< SuccessCount */
   tsl_u32_t    failureCount;	/**< FailureCount */
   tsl_u32_t    averageResponseTime;	/**< AverageResponseTime */
   tsl_u32_t    minimumResponseTime;	/**< MinimumResponseTime */
   tsl_u32_t    maximumResponseTime;	/**< MaximumResponseTime */
   CmsEntityId requesterId;
}PingDataMsgBody;



/** Data body for the CTL_MSG_WATCH_WAN_CONNECTION message type.
 *
 */
 typedef struct
{
   //tr69_oid oid;              /**< Object Identifier */
   tsl_char_t oid[256];
   tr69_oid_stack_id iidStack;     /**< Instance Id Stack. for the ip/ppp Conn Object */
   tsl_bool_t isAdd;                 /**< add  wan connection to ssk list if TSL_B_TRUE.  */
   tsl_bool_t isStatic;              /**< If TSL_B_TRUE, it is is bridge, static IPoE and IPoA, TSL_B_FALSE: pppox or dynamic IPoE */   
   tsl_bool_t isDeleted;             /**< Used for auto detect feature only.  If TSL_B_TRUE, the wan interface is removed.*/  
   tsl_bool_t isAutoDetectChange;    /**< Used for auto detect feature only.  If TSL_B_TRUE, there is a change on the layer 2 auto detect flag */  
} WatchedWanConnection;
/*
 * Data body for the CTL_MSG_DHCPD_DENY_VENDOR_ID message type
 */
typedef struct
{
   long deny_time; /* Return from time(), in host order */
   unsigned char chaddr[16]; /* Usually the MAC address */
   char vendor_id[BUFLEN_256]; /**< max length in RFC 2132 is 255 bytes, add 1 for null terminator */
   char ifName[CTL_IFNAME_LENGTH];  /**< The interface that dhcpd got this msg on */
}DHCPDenyVendorID;

/*
 * Data body for CTL_MSG_GET_DEVICE_INFO message type.
 */
typedef struct
{
   char oui[BUFLEN_8];              /**< manufacturer OUI of device */
   char serialNum[BUFLEN_64];       /**< serial number of device */
   char productClass[BUFLEN_64];    /**< product class of device */
} GetDeviceInfoMsgBody;


typedef enum 
{
   USER_REQUEST_CONNECTION_DOWN  = 0,
   USER_REQUEST_CONNECTION_UP    = 1,

#ifdef ACTION_TEC//rayofox
//   USER_REQUEST_CONNECTION_RESET = 2,
#endif//rayofox

} UserConnectionRequstType;


/*
 * Data body for CTL_MSG_WATCH_DSL_LOOP_DIAG message type.
 */
typedef struct
{
   tr69_oid_stack_id iidStack;
} dslDiagMsgBody;


/** Data body for CTL_MSG_VENDOR_CONFIG_UPDATE message type.
 *
 */
typedef struct
{
   char name[BUFLEN_64];              /**< name of configuration file */
   char version[BUFLEN_16];           /**< version of configuration file */
   char date[BUFLEN_64];              /**< date when config is updated */
   char description[BUFLEN_256];      /**< description of config file */
} vendorConfigUpdateMsgBody;

typedef enum
{
    MCAST_INTERFACE_ADD = 0,
    MCAST_INTERFACE_DELETE
} McastInterfaceAction;

typedef enum
{
    OMCI_IGMP_PHY_NONE = 0,
    OMCI_IGMP_PHY_ETHERNET,
    OMCI_IGMP_PHY_MOCA,
    OMCI_IGMP_PHY_WIRELESS,
    OMCI_IGMP_PHY_POTS,
    OMCI_IGMP_PHY_GPON
} OmciIgmpPhyType;

typedef enum
{
    OMCI_IGMP_MSG_JOIN = 0,
    OMCI_IGMP_MSG_RE_JOIN,
    OMCI_IGMP_MSG_LEAVE
} OmciIgmpMsgType;

/** Data body for CTL_MSG_OMCI_MCPD_SEND_REPLY message type.
 *
 */
typedef struct
{
    tsl_u16_t  tci;
    tsl_u32_t  sourceIpAddress;
    tsl_u32_t  groupIpAddress;
    tsl_u32_t  clientIpAddress;
    tsl_u16_t  phyPort;
    OmciIgmpPhyType phyType;
    OmciIgmpMsgType msgType;
} OmciIgmpMsgBody;


/** Data body for CTL_MSG_APP_TERMINATED message type.
 *
 */
typedef struct
{
   CmsEntityId eid;      /**< Entity id of the exit process */
   tsl_32_t sigNum;        /**< signal number */   
   tsl_32_t exitCode;      /**< process exit code */   
} appTermiatedMsgBody;


/** Data body for the CTL_MSG_GPON_WAN_LINK_INFO
 *
 */
 typedef struct
{
   tsl_bool_t isLinkUp;           /**< If TSL_B_TRUE, gpon link is up, and layer 2 interface is created already */
   tsl_u16_t gemPortIndex;       /**< GEM port index */   
   tsl_u16_t portID;             /**< It is a logical port ID which maps o the gem port index. eg. 3000 maps to gem port 0 */  
   tsl_u32_t interfaceCounter;   /**< This is the interface counter, if 0, gpon interface will be gpon0, etc... */
   tsl_u32_t direction; 	      /**< Direction of the gpon interface. Upstream, Downstream (For multicast)  and both direction  */
} GponWanLinkInfo;


typedef enum {
   WAN_CHECK_DISABLED  	= 0,  /**< wan firmware check disabled */
   WAN_CHECK_EMAIL   	= 1,  /**< wan firmware check by Email noticification */
   WAN_CHECK_UPDATE   	= 2,  /**< wan firmware check and update */
} WanUpdateCheckType;

/** Data body for WAN Update config message */
typedef struct
{
	WanUpdateCheckType checkType;
	tsl_32_t checkInterval;      /** the update server check interval, days  **/
	char urlAddress[BUFLEN_256];
} WanUpdateMsgBody;

/*!\SNTP state defines 
 */
#define SNTP_STATE_DISABLED                0   
#define SNTP_STATE_UNSYNCHRONIZED          1   
#define SNTP_STATE_SYNCHRONIZED            2
#define SNTP_STATE_FAIL_TO_SYNCHRONIZE     3
#define SNTP_STATE_ERROR                   4

//#define rut_sendMsgToSmd(m,w,d,l) (tr69_util_ctl_send_msg_to_smd_ex(CTL_MSG_TYPE(m),w,d,l))
//#define rut_sendMsgToSmdwithPid(m,w,d,p,l) (tr69_util_ctl_send_msg_to_smd_with_pidex(CTL_MSG_TYPE(m),w,d,p,l))


//UPNPD Message Define
#define UPNPD_MAX_IP_LEN               128
#define UPNPD_MAX_PROTOCOL_LEN         10
#define UPNPD_MAX_DESC_LEN             256
#define UPNPD_NAME		       "miniupnpd"

typedef enum UPNPD_DBUS_MSG_TYPE {
    DMT_UPNPD_ADD_PORTMAPPING,
    DMT_UPNPD_DEL_PORTMAPPING,
    DMT_UPNPD_CONFIG_UPDATE,
}EM_UPNPD_DBUS_MSG_TYPE;

typedef struct PortMapping {
    char inIP[UPNPD_MAX_IP_LEN];
    char extIP[UPNPD_MAX_IP_LEN];
    char desc[UPNPD_MAX_DESC_LEN];
    char appName[UPNPD_MAX_DESC_LEN];

    unsigned int inPort;
    unsigned int protocol;
    unsigned int extPort;
    unsigned int duration;
    int isGUI;
    int enable;
} ST_PORT_MAPPING, *P_ST_PORT_MAPPING;

typedef struct MiniUpnpd_CONFIG_INFO {
    int isAutoClear;
} ST_UPNPD_CONFIG_INFO,P_ST_UPNPD_CONFIG_INFO;


typedef struct CFG_DOWNLOAD_INFO
{
	char src[BUFLEN_32];
	char filename[BUFLEN_256];
	char version[BUFLEN_64];
	char datetime[BUFLEN_128];
}CFGDownloadInfo;

#endif //_CTL_MSG_H
