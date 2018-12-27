


#ifndef __RUT_IPTALBES_H__
#define __RUT_IPTALBES_H__


/*!\file tr69_rut_iptables.h
 * \brief System level interface functions for iptables functionality.
 *
 * The functions in this file should only be called by
 * RCL, STL, and other RUT functions.
 */


#include "ctl.h"
#include "tsl_common.h"
#include "tr69_func.h"

/************from orig file**************/


/*
 * directory prefix is: /lib/modules/"kernel_version"/kernel/net
 */
#define NF_DIR "netfilter"
#define IPV4_NF_DIR "ipv4/netfilter"

//#ifdef DMP_X_ACTIONTEC_COM_IPV6_1 /* aka SUPPORT_IPV6 */
#define IPV6_NF_DIR "ipv6/netfilter"
//#endif

#define MAX_NAME_LEN 32
#define MAX_DEP_NUM  8
#define MAX_DIR_LEN  32
#define MAX_PATH_LEN 128

typedef enum
{
    x_tables=1,
    nf_conntrack,
    nf_conntrack_ipv4,
    nfnetlink,
    xt_tcpudp,
    xt_multiport,
    xt_dscp,
    xt_DSCP,
    xt_esp,
    xt_limit,
    xt_mac,
    xt_mark,
    xt_MARK,
    xt_TCPMSS,
    xt_SKIPLOG,
    nfnetlink_queue,
    //   ip_queue,  /* ip_queue is obsoleted in kernel 2.6.30 */
    xt_state,
    ip_tables,
    nf_nat,
    iptable_filter,
    iptable_nat,
    iptable_mangle,
    iptable_raw,
    ipt_LOG,
    ipt_MASQUERADE,
    ipt_REDIRECT,
    nf_defrag_ipv4,
    nf_conntrack_ftp,
    nf_conntrack_h323,
    nf_conntrack_irc,
    nf_conntrack_tftp,
#ifdef SUPPORT_SIP 
    nf_conntrack_sip,
#endif
    nf_conntrack_rtsp,
    nf_nat_ftp,
    nf_nat_tftp,
    nf_nat_irc,
    nf_nat_h323,
#ifdef SUPPORT_SIP    
    nf_nat_sip,
#endif
    nf_nat_rtsp,
#ifdef SUPPORT_PPTP
    nf_conntrack_proto_gre,
    nf_nat_proto_gre,
    nf_conntrack_pptp,
    nf_nat_pptp,
#endif


    ip6_tables,
    ip6table_filter,
    ip6table_mangle,
    ip6t_LOG,
    ip6t_REJECT,
    nf_conntrack_ipv6,

#ifdef SUPPORT_CONNTRACK_TOOLS
    nf_conntrack_netlink,
#endif
}ctl_moduleEnum;

typedef struct
{
    ctl_moduleEnum moduleIdx;            /**< module index defined in moduleEnum above */
    tsl_char_t moduleName[MAX_NAME_LEN];   /**< module name */
    ctl_moduleEnum depends[MAX_DEP_NUM]; /**< dependency module index list */
    tsl_char_t directory[MAX_DIR_LEN];     /**< the directory the module resides */
    tsl_char_t *options;                   /**< optional parameters for the module */
    tsl_bool isStatic;                 /**< if TRUE, the module is static nad will not do an insmod */
} CTL_MODULE_INFO, *PCTL_MODULE_INFO;

 tsl_bool ctl_isModuleInserted(tsl_char_t *moduleName);

 /**  Remove all IP table rules attach with the given interface.
  * @param ifcName		 (IN) This is the wan interface name string for which the firewall policies applied to
  * @param isIPv4		   (IN) indicate if IPv4 or IPv6
  */
 void ctl_rutIpt_removeInterfaceIptableRules(tsl_char_t *ifcName, tsl_bool isIPv4 );

/**  Remove  remove ip table rules for the interface ifcName on a particular table and chain
 * @param domain        (IN) Communication domain, either PF_INET or PF_INET6.
 * @param ifcName       (IN) This is the wan interface name string 
 * @param table         (IN) the table name
 * @param chain         (IN) the chain name
 * @return        TRUE if more to be removed
 */
tsl_bool  ctl_rutIpt_removeIptableRules(tsl_32_t domain, const char *ifcName, const char *table, const char *chain);



/** Setup firewall for DHCPv6,  Construct ip6table rule to allow dhcpv6 traffic (TCP/UDP port 546).
 *
 * Setup firewall rules to block or unblock DHCPv6 packets.
 * @param unblock       (IN) unblock if TRUE, otherwise block.
 * @param ifcName       (IN) This is the wan interface name string for which the firewall policies applied to
 */
void ctl_rutIpt_setupFirewallForDHCPv6(tsl_bool unblock, const char *ifName);
void ctl_rutIpt_setupFirewallForDHCP6s(tsl_bool unblock, const char *ifName);

/** Initialize firewall exceptions
 *
 * Initailize firewall incoming and outgoing exceptions for the interface with iptables commands.
 * @param ifcName       (IN) This is the interface name string for which the firewall policies applied to
 */
void ctl_rutIpt_initFirewallExceptions(const char *ifName); 


/** Initialize the firewall
 *
 * Initailize the firewall for the interface with iptables commands.
 * @param domain        (IN) Communication domain, either PF_INET or PF_INET6.
 * @param ifcName       (IN) This is the interface name string for which the firewall policies applied to
 */
void ctl_rutIpt_initFirewall(tsl_32_t domain, const char *ifName);


/** issue iptables rules for TCP MSS option manipulation
 *
 * @param domain (IN)  Communication domain, either PF_INET or PF_INET6.
 * @param ifName (IN)  Wan interface name
 */
void ctl_rutIpt_TCPMSSRules(tsl_32_t domain, const char *ifName);
/************end orig file*****************/
#define CONNTRACK_MAX         1000

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

#define UPNP_IP_ADDRESS    "239.255.255.250"


void insertModuleByName(char *moduleName);

/** Insert the ip related modules
 *
 * @param (IN) void
 * @return void
 */
void rutIpt_insertIpModules(void);

      
/** Insert the LAN/WAN NAT Masquerade rules for the given ifname.
 *
 * @param ifName      (IN) ifname
 * @param localSubnet (IN) localsubnet in dotted form
 * @param localSubnetMask  (IN) localsubnetmask in dotted form
 * @param isFullCone (IN) is FullCone nat enabled on this interface.
 *
 */
void rutIpt_insertNatMasquerade(const char *ifName, const char *localSubnet, const char *localSubnetMask, tsl_bool_t isFullCone);



/** Delete the LAN/WAN NAT Masquerade rules for the given ifname.
 *
 * @param ifName      (IN) ifname
 * @param localSubnet (IN) localsubnet in dotted form
 * @param localSubnetMask  (IN) localsubnetmask in dotted form
 *
 */
void rutIpt_deleteNatMasquerade(const char *ifName, const char *localSubnet, const char *localSubnetMask);



/** Initialize the ipsec policy
 *
 * Initailize the ipsec for the wan interface with iptables commands.
 * @param ifcName       (IN) This is the wan interface name string for which the ipsec firewall policies applied to
 */
void rutIpt_initIpSecPolicy(const char *ifName);

/** Remove the ipsec policy
 *
 * Remove the ipsec for the wan interface with iptables commands.
 * @param ifcName       (IN) This is the wan interface name string for which the ipsec firewall policies applied to
 */
void rutIpt_removeIpSecPolicy(const char *ifName);





/** Initialize the firewall
 *
 * Initailize the firewall for the interface with iptables commands.
 * @param domain        (IN) Communication domain, either PF_INET or PF_INET6.
 * @param ifcName       (IN) This is the interface name string for which the firewall policies applied to
 */
void rutIpt_initFirewall(tsl_32_t domain, const char *ifName);



 

/**  Execute commands to setup NAT Masquerade for PPP on demand
 * @param ifcName       (IN) This is the wan interface name string 
 */
void rutIpt_initNatForPppOnDemand(char *ifcName, tsl_bool_t fullCone);


/**  execute nat commands to enable DNS forwarding
 *
 * Forwarding is from bridgeName (normally br0) to a single (primary) dns server.
 * dnsprobe will tell us if the primary dns server goes down and we
 * need to switch to a secondary one.
 * Not sure if the hard coded br0 will cause problems
 * when we have multiple LAN subnets.
 *
 * @param bridgeName  (IN)            bridgeName  - bridge name (br0, br1)
 * @param dns1 (IN)            dns1 dns ip address
 */
void rutIpt_setDnsForwarding(const char *bridgeName, const char *dns1);


/**  execute nat commands to add iptable rule to stop multicast on the wan interface (for upnp)
 * @wanIfcName  (IN)          WAN interface name
 * @addRules  (IN)            TSL_B_TRUE = add rule, TSL_B_FALSE = delete rule
 */
void rutIpt_upnpConfigStopMulticast(char *wanIfcName, tsl_bool_t addRules);


/** add rip iptable rule for the WAN interface.
 *
 * @ifcName  (IN)     WAN interface name
 */
void rutIpt_ripAddIptableRule(const char *ifcName);


/** remove rip IP table rules.
 *
 *@param ripRule (IN) Name of the rule to remove.
 */
void rutIpt_ripRemoveIptableRule(const char *ripRule);


/** Activate Url Filter: insmod necessary modules and add an iptables chain. 
 *
 */
void rutIpt_activeUrlFilter(void);


/** Deactivate Url Filter: Delete iptables chain and references. 
 */
void rutIpt_deactiveUrlFilter(void);


/** Configure Url filter list mode. 
 *
 * @param  type (IN)  whitelist /blacklist mode
 */
void rutIpt_configUrlFilterMode(char *type) ;


/** Configure Url filter list entry. 
 *
 * @param obj (IN)
 * @param add  (IN)
 */
void rutIpt_urlFilterConfig(void *Obj, tsl_bool_t add) ;

/** Add nat iptable rules for advanced DMZ
 *
 * @param localLanIp (IN) LAN Ip address
 * @param localIp    (IN) WNA IP address served as the destination for postrout
 * @param nonDmzIp   (IN) non DMZ ip address
 * @param nonDmzMask (IN) non DMZ mask
 */
void rutIpt_addAdvancedDmzIpRules(const char *localLanIp, 
                               const char *localIp, 
                               const char *nonDmzIp, 
                               const char *nonDmzMask);



/** iptables rules for IGMP
 *
 * @param (IN) add
 * @param (IN) ifname
 * @return void
 */
void rutIpt_igmpRules(tsl_bool_t add, const char *ifname);

/** iptables rules for NAT rules for interface group
 *
 * @param ifName (IN) wan interface name 
 * @param bridgeIfName (IN) bridge interface name (br1...)

 * @return void
 */
void rutIpt_initNatForIntfGroup(const char *ifName, const char *bridgeIfName,
				tsl_bool_t fullCone);

/** remove the DNAT for DNS on non default bridge (br1 up)
 *
 * @param bridgeIfName (IN)  bridge interface name
 */
void rutIpt_removeBridgeIfNameIptableRules(const char *bridgeIfName);

/** issue iptables rules redirect http and telnet ports
 *
 * @param ifName (IN)  Wan interface name
 * @param ifName (IN)  the lan (br0) ip address
 */
void rutIpt_redirectHttpTelnetPorts(const char *ifName, const char *ipAddress);


/** load all the necessary modules to support qos.
 */
void rutIpt_qosLoadModule(void);

#ifdef DMP_X_ACTIONTEC_COM_IPV6_1 /* aka SUPPORT_IPV6 */
/**  Remove all ip66able rules attach with the given interface.
 * @param ifcName       (IN) This is the wan interface name string for which the firewall policies applied to
 */
void rutIpt_removeInterfaceIp6tableRules(char *ifcName);

/**  Construct ip6table rule to allow dhcpv6 traffic (TCP/UDP port 546).
 * @param unbloc       (IN) 
 * @param ifName       (IN) This is the wan interface for which the firewall policies applied to
 */
void rutIpt_setupFirewallForDHCPv6(tsl_bool_t unblock, const char *ifName);

/** Insert the IPv6 related modules
 *
 * @param (IN) void
 * @return void
 */
void rutIpt_insertIp6Modules(void);

/** iptables rules for MLD
 *
 * @param (IN) add
 * @param (IN) ifname
 * @return void
 */
void rutIpt_mldRules(tsl_bool_t add, const char *ifname);
#endif

#ifdef ACTION_TEC_VERIZON
tsl_bool_t rutIpt_geturlFilterConfigStatus(void);
#endif

#endif

