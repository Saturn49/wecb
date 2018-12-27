#ifndef _CTL_H
#define _CTL_H

#ifndef BUFLEN_4 // if this macro was defined, measn already included BRCM's header file

#define IP_TBL_COL_NUM     0
#define IP_TBL_COL_PKTS    1
#define IP_TBL_COL_BYTES   2
#define IP_TBL_COL_TARGET  3
#define IP_TBL_COL_PROT    4
#define IP_TBL_COL_OPT     5
#define IP_TBL_COL_IN      6
#define IP_TBL_COL_OUT     7
#define IP_TBL_COL_SRC     8
#define IP_TBL_COL_DST     9
#define IP_TBL_COL_MAX     10


/** Maximum value for a UINT64 */
#define MAX_UINT64 18446744073709551615ULL

/** Maximum value for a SINT64 */
#define MAX_SINT64 9223372036854775807LL

/** Minimum value for a SINT64 */
#define MIN_SINT64 (-1 * MAX_SINT64 - 1)

/** Maximum value for a UINT32 */
#define MAX_UINT32 4294967295U   

/** Maximum value for a SINT32 */
#define MAX_SINT32 2147483647    

/** Minimum value for a SINT32 */
#define MIN_SINT32 (-2147483648) 

/** Maximum value for a UINT16 */
#define MAX_UINT16  65535        

/** Maximum value for a SINT16 */
#define MAX_SINT16  32767

/** Minimum value for a SINT16 */
#define MIN_SINT16  (-32768)
#define BUFLEN_4        4     //!< buffer length 4
#define BUFLEN_8        8     //!< buffer length 8
#define BUFLEN_16       16    //!< buffer length 16
#define BUFLEN_18       18    //!< buffer length 18 -- for ppp session id
#define BUFLEN_24       24    //!< buffer length 24 -- mostly for password
#define BUFLEN_32       32    //!< buffer length 32
#define BUFLEN_40       40    //!< buffer length 40
#define BUFLEN_48       48    //!< buffer length 48
#define BUFLEN_64       64    //!< buffer length 64
#define BUFLEN_80       80    //!< buffer length 80
#define BUFLEN_128      128   //!< buffer length 128
#define BUFLEN_256      256   //!< buffer length 256
#define BUFLEN_264      264   //!< buffer length 264
#define BUFLEN_512      512   //!< buffer length 512
#define BUFLEN_1024     1024  //!< buffer length 1024

#define IIDSTACK_BUF_LEN  40  //!< good length to use for mdm_dumpIidStack
#define MAC_ADDR_LEN    6     //!< Mac address len in an array of 6 bytes
#define MAC_STR_LEN     17    //!< Mac String len with ":". eg: xx:xx:xx:xx:xx:xx
#define VPI_MIN         0     //!< VPI min 
#define VPI_MAX         255   //!< VPI max 
#define VCI_MIN         32    //!< VCI min 
#define VCI_MAX         65535 //!< VCI max 

#define PPP_CONNECT_ERROR_REASON_LEN 48

#define CTL_IFNAME_LENGTH  BUFLEN_32   //!< interface name length

#define CMS_IFDESC_LENGTH  BUFLEN_128 //!< interface description length


/* ACTIONTEC */
#define CMS_DEFAULT_BRIDGE_IFNAME   "br0"
#define CMS_LAN_ETH_IFNAME_PREFIX   "eth"
#define CMS_LAN_ETH_IFNAME_DEFAULT   "eth0"
#define CMS_WAN_ETH_IFNAME          "eth10"
#define CMS_WAN_MOCA_IFNAME         "moca1"
#define CMS_LAN_MOCA_IFNAME         "moca0"
#define CMS_ALL_WAN_IFNAME          "all_wan"
//#ifdef DMP_X_ACTIONTEC_COM_IPV6_1    /* aka SUPPORT_IPV6 */
#define CTL_IPADDR_LENGTH  44          //!< IP address length to hold IPv6 in CIDR notation
//#else
//#endif /* DMP_X_ACTIONTEC_COM_IPV6_1 */
//#
#define CTL_MAX_DEFAULT_GATEWAY     8  //!< max default gateways allowed in L3 X_ACTIONTEC_COM_DefaultConnectionServices
#define CTL_MAX_DNSIFNAME           8  //!< max dns wan interface names in X_ACTIONTEC_Networking.DNSIfName
#define CTL_MAX_ACTIVE_DNS_IP       4  //!< max active dns ip (in resolv.conf)

#define OGF_NORMAL_UPDATE    0x0000

/** Get default values of the object instead of the current values.
 *  *
 *   * This flag is used in the getFlags param of tr69_get_obj_by_oid only.
 *    */
#define OGF_DEFAULT_VALUES    0x0001

/** Get the current values in the MDM; do not call STL handler function to update values.
 *  *
 *   * This flag can be used with tr69_get_obj_by_oid and all of the tr69_get_next_obj_by_oid and cmObj_getAncestor().
 *    */
#define OGF_NO_VALUE_UPDATE   0x0002

#define BRIDGE_PROTO_STR      "Bridge"
#define IPOA_PROTO_STR        "IPoA"
#define IPOE_PROTO_STR        "IPoE"
#define PPPOE_PROTO_STR       "PPPoE"
#define PPPOA_PROTO_STR       "PPPoA"
#define IPOW_PROTO_STR        "IPoW"

#define ETH_IFC_STR           "eth"
#define USB_IFC_STR           "usb"
#define WLAN_IFC_STR          "wl"
#define MOCA_IFC_STR          "moca"
#define GPON_IFC_STR          "gpon"
#define ATM_IFC_STR           "atm"
#define PTM_IFC_STR           "ptm"
#define BRIDGE_IFC_STR        "br"
#define IPOA_IFC_STR          "ipoa"
#define IPOE_IFC_STR          "ipoe"
#define PPP_IFC_STR           "ppp"
#define PPPOE_IFC_STR         "pppoe"
#define PPPOA_IFC_STR         "pppoa"
#define IPA_IFC_STR           "ipa"
#define BRIDGE_SMUX_STR       "bridge"

#define L3_TYPE_IP_STR	"IP"
#define L3_TYPE_PPP_STR	"PPP"

#define IFC_WAN_MAX        8  //!< Max WAN Connection in the system.
#define IFC_VLAN_MAX       8  //!< Max VLAN on single PVC
#define ATM_QUEUE_MAX      8  //!< Max ATM queues

/* for interface group with routed pvc */
#define RT_TABLES_BASE  200
typedef enum
{
    ATM=0,          /**< WanDev is used for DSL ATM  */
    PTM=1,          /**< WanDev is used for DSL PTM  */
    Ethernet=2      /**< WanDev is used for Ethernet  */
}WanLinkType;

typedef enum
{
    CMS_CONNECTION_MODE_DEFAULT=0,      /**< Default connection mdoe - single wan service over 1 connection */

    CMS_CONNECTION_MODE_VLANMUX=1,      /**< Vlan mux connection mdoe - multiple vlan service over 1 connection */

    CMS_CONNECTION_MODE_MSC=2,          /**< MSC connection mdoe - multiple wan service over 1 connection */

} ConnectionModeType;


typedef enum
{
    ATM_EOA=0,        /**< WanDev is used for DSL ATM  */
    ATM_IPOA=1,       /**< WanDev is used for DSL ATM IPOA */
    ATM_PPPOA=2,      /**< WanDev is used for DSL ATM PPPoA */
    PTM_EOA=3         /**< WanDev is used for DSL PTM  */

}Layer2IfNameType;
/** Free a buffer and set the pointer to null.
 */

/* ACTIONTEC */
#define CTL_VALID_IPADDR_MASK       0
#define CTL_INVALID_IPADDR          -1
#define CTL_INVALID_IPMASK          -2
#define CTL_INVALID_IPADDR_MASK     -3

#define MAX_PERSISTENT_WAN_PORT     39       /**< Max Ethernet ports can be configured as WAN ports */
#define SIOCDEVPRIVATE  0x89F0  /* to 89FF */
enum {
    SIOCGLINKSTATE = SIOCDEVPRIVATE + 1,
    SIOCSCLEARMIBCNTR,
    SIOCMIBINFO,
    SIOCGENABLEVLAN,
    SIOCGDISABLEVLAN,
    SIOCGQUERYNUMVLANPORTS,
    SIOCGQUERYNUMPORTS,
    SIOCPORTMIRROR,
    SIOCSWANPORT,
    SIOCGWANPORT,
    SIOCETHCTLOPS,
    SIOCGPONIF,
    SIOCETHSWCTLOPS,
    SIOCGSWITCHPORT,
    SIOCLAST,
};  
 #define CTLMEM_FREE_BUF_AND_NULL_PTR(p) \
    do { \
	  if ((p) != NULL) {free((p)); (p) = NULL;}   \
	} while (0)

#define IS_EMPTY_STRING(s)    ((s == NULL) || (*s == '\0'))

#define SOFTWARE_VERSION "v0.16.4.3"
#define RG_SOFTWARE_VERSION "FTH-BHRK2-10-10-08U"
#define RG_HARDWARE_VERSION "I"
#define RG_PRODUCT_ID       "FBK2-C1"
#define CMS_RELEASE_VERSION "4.08L.02"

#define PROTYPE_XML_FILE 	"/etc/protype.xml"
#define CFG_XML_FILE 		"/etc/cfg.xml"
#define PRE_CFG_XML_FILE 	"/tmp/pre_cfg.xml"
#define CUR_CFG_XML_FILE 	"/tmp/cfg.xml"
#define TR69_CFG    			"tr69_cfg"
#define ZIP_CFG_FULLPATH 	"/mnt/rt_conf/pre_cfg.xml.gz"
#define TMPZIP_CFG_FULLPATH 	"/tmp/pre_cfg.xml.gz"
#define ZIP_COMMAND 		"/bin/zip"
#define CFG_VER_FOR_RD		"Internal"
#define DEF_INST_LIST_FILE 	"/etc/instoid.lst"
#define PRE_INST_LIST_FILE	"/mnt/rt_conf/instoid.lst" // bugfix: #7512 @agile
#define CFG_DOWNLOAD_LIST	"/mnt/rt_conf/cfg_download.lst"

#define ZIP_CACHE_CFG_FULLPATH 	"/mnt/rt_conf/cache_cfg.xml.gz"
#define TMPZIP_CACHE_CFG_FULLPATH 	"/tmp/run/pre_cfg.xml.gz"
#define CACHE_CFG_XML_FILE			"/tmp/run/pre_cfg.xml"
#define TR69_CONFIG_FILE 			"/tmp/tr69_cfg.xml"
#define TR69_DOWNLOAD_COMPLETE 	"/mnt/rt_conf/tr69_download_complete"
#define TR69_UPDATE_COMPLETE 		"/mnt/rt_conf/tr69_fw_update_complete"
#define TR69_FIRMWARE_FILE 		"/tmp/tr69_firmware.bin"
#define WAN_UPGRADE_FIRMWARE_VER "/tmp/wan_upgrade_firmware.ver"

#ifdef AEI_PARAM_FORCEUPDATE
#define PREV_VERSION_CFG_XML_FILE "/mnt/rt_conf/cfg.xml.prev_ver.gz"
#define PREV_VERSION_TMP_XML_FILE "/tmp/cfg.xml.prev_ver"
#endif

#define CFG_SRC_RESTORE	"restore default"
#define CFG_SRC_LOCAL	"from local"
#define CFG_SRC_ACS		"from ACS"
#define CFG_VER_KEY		 "framework_xml_version"

#define APP_DHCPD		"/usr/sbin/udhcpd"
#define APP_PPP			"/usr/sbin/pppd"
#define APP_PING		"/usr/local/sbin/ping"
#define APP_TR64C		"/usr/sbin/tr64"
#define APP_DHCPC		"/usr/sbin/udhcpc"
#define APP_ZEBRA       	"/usr/sbin/zebra"
#define APP_RIPD        	"/usr/sbin/ripd"
#define APP_DHCP6S		"/bin/dhcp6s"
#define APP_RADVD		"/bin/radvd"
#define APP_DHCP6C		"/bin/dhcp6c"
#define APP_IGMP			"/bin/igmp"


// for RTL platform, WECB project
// With dbus, data_center will send out a message to notify its ready
// Without dbus, check pid file to wait data_center ready
#include <unistd.h> // for access() & sleep()

#define DATACENTER_PID_FILE_NAME                "/var/data_center.pid"
#define DEFAULT_WAIT_DATA_CENTER_READY_TIMEOUT  (90) // in second
#define MIN_WAIT_DATA_CENTER_READY_TIMEOUT      (15) // in second

/*
 * Description:
 *     This macro wait until data_center become ready to recv request or timeout.
 * Input:
 *     in seconds. <0 means infinity, at least wait 10 seconds.
 * Return Value:
 *     Return 1 if data_center becomes ready before timeout; otherwise return 0.
 */
#define waitDataCenterReady( t1 ) \
({ \
    int ready = 0; \
    int timeout = t1; \
    int left_time; /* in second*/ \
    if( timeout >= 0) { \
        if(timeout < MIN_WAIT_DATA_CENTER_READY_TIMEOUT) \
            timeout = MIN_WAIT_DATA_CENTER_READY_TIMEOUT; \
        left_time = timeout; \
    } \
    while(1) { \
        /* Wait here until data_center ready */ \
        if( 0 == access(DATACENTER_PID_FILE_NAME, F_OK ) ) { \
            ready = 1; \
            break; \
        } \
        sleep(2); \
        /* infinity */ \
        if( timeout < 0 ) \
            continue; \
        left_time -= 2; /* 2 seconds interval */ \
        if( left_time < 0 ) { \
            /* wait too long time, data_center still not ready */ \
            break; \
        } \
    } \
    /* return */ ready;\
})

#ifdef AEI_WECB
/* Realtek's 'flash reset' will call /etc/ctl_restore.sh to rm /mnt/rt_conf/tr69_* files */
#define DO_RESTORE_DEFAULT() \
do { \
        system("flash reset"); \
        system("killall watchdog_datacenter"); \
        system("killall data_center"); \
        system("rm -f "ZIP_CFG_FULLPATH); \
        system("sync"); \
} while(0)
#else
#error pls define restore_default here
#define DO_RESTORE_DEFAULT()
#endif

#endif // BUFLEN_4
#endif //_CTL_H

