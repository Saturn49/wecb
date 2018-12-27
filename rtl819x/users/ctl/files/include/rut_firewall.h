/***********************************************************************
 *
 *  Copyright (c) 2009  Actiontec Electronics, Inc
 *  All Rights Reserved
 *
 *  rut_firewall.h
 *
 *  Firewall/NAT Configuration using iptables
 *  ACTIONTEC_FIREWALL
 *
************************************************************************/

#ifndef __RUT_FIREWALL_H__
#define __RUT_FIREWALL_H__

#ifdef DMP_X_ACTIONTEC_COM_FIREWALL_1

#include "ctl.h"

#define LOOPBACK_IFNAME						"lo"

#define IPTABLE_FILTER						"filter"
#define IPTABLE_NAT							"nat"
#define IPTABLE_MANGLE						"mangle"

#define EBTABLE_FILTER						"filter"
#define EBTABLE_NAT							"nat"
#define EBTABLE_BROUTE						"broute"

#define INBOUND_FILTER_CHAIN				"fw_inbound"
#define OUTBOUND_FILTER_CHAIN				"fw_outbound"
#define INPUT_FILTER_CHAIN					"fw_input"
#define OUTPUT_FILTER_CHAIN					"fw_output"

#define INBOUND_FILTER_CHAIN_PREFIX			"fw_inbound_"
#define OUTBOUND_FILTER_CHAIN_PREFIX		"fw_outbound_"
#define INPUT_FILTER_CHAIN_PREFIX			"fw_input_"
#define OUTPUT_FILTER_CHAIN_PREFIX			"fw_output_"

#define BRPORTS_INBOUND_FILTER_CHAIN		"fw_inbound_brports"
#define BRPORTS_OUTBOUND_FILTER_CHAIN		"fw_outbound_brports"
#define BRPORTS_INPUT_FILTER_CHAIN			"fw_input_brports"

#define TCP_SYN_FLOOD_CHAIN					"tcp_syn_flood"
#define UDP_FLOOD_CHAIN						"udp_flood"
#define ICMP_FLOOD_CHAIN					"icmp_flood"
#define FW_ATTACK_CHAIN						"fw_attacks"

#define REMOTE_MGMT_CHAIN					"remote_mgmt"
#define LOCAL_MGMT_CHAIN					"local_mgmt"

#define ACCESS_CONTROL_ALLOW_CHAIN			"access_control_allow"
#define ACCESS_CONTROL_BLOCK_CHAIN			"access_control_block"

#define ADV_FILTER_INBOUND_CHAIN_PREFIX		"adv_filter_inbound_"
#define ADV_FILTER_OUTBOUND_CHAIN_PREFIX	"adv_filter_outbound_"
#define ADV_FILTER_INPUT_CHAIN_PREFIX		"adv_filter_input_"
#define ADV_FILTER_OUTPUT_CHAIN_PREFIX		"adv_filter_output_"

#define ADV_FILTER_INBOUND_WAN_CHAIN		"adv_filter_inbound_wan"
#define ADV_FILTER_OUTBOUND_WAN_CHAIN		"adv_filter_outbound_wan"
#define ADV_FILTER_INPUT_WAN_CHAIN			"adv_filter_input_wan"
#define ADV_FILTER_OUTPUT_WAN_CHAIN			"adv_filter_output_wan"
#define ADV_FILTER_INBOUND_LAN_CHAIN		"adv_filter_inbound_lan"
#define ADV_FILTER_OUTBOUND_LAN_CHAIN		"adv_filter_outbound_lan"
#define ADV_FILTER_INPUT_LAN_CHAIN			"adv_filter_input_lan"
#define ADV_FILTER_OUTPUT_LAN_CHAIN			"adv_filter_output_lan"

#define APP_INBOUND_CHAIN_PREFIX			"app_inbound_"
#define APP_OUTBOUND_CHAIN_PREFIX			"app_outbound_"
#define APP_INPUT_CHAIN_PREFIX				"app_input_"
#define APP_OUTPUT_CHAIN_PREFIX				"app_output_"

#define DNAT_INBOUND_FILTER_CHAIN			"dnat_inbound_filter_all"
#define DNAT_INBOUND_FILTER_CHAIN_PREFIX	"dnat_inbound_filter_"
#define DNAT_OUTBOUND_FILTER_CHAIN			"dnat_outbound_filter_all"
#define DNAT_OUTBOUND_FILTER_CHAIN_PREFIX	"dnat_outbound_filter_"

#define DNAT_NAT_CHAIN						"dnat_nat_all"
#define DNAT_NAT_CHAIN_PREFIX				"dnat_nat_"

#define SNAT_NAT_CHAIN						"snat_nat_all"
#define SNAT_NAT_CHAIN_PREFIX				"snat_nat_"

#define SNAT_PORT_MAPPING_CHAIN			    "dnat_snat_pm_all"
#define SNAT_PORT_MAPPING_CHAIN_PREFIX		"dnat_snat_pm_"

#define LOCAL_MGMT_REDIRECT_CHAIN			"local_mgmt_redirect"
#define LOCAL_MGMT_MARK_CHAIN				"local_mgmt_mark"
#define REMOTE_MGMT_REDIRECT_CHAIN			"remote_mgmt_redirect"
#define REMOTE_MGMT_MARK_CHAIN				"remote_mgmt_mark"

#define URL_FILTER_CHAIN					"url_filter"
#define IGMP_FILTER_CHAIN					"igmp_filter"
#define STB_FILTER_CHAIN					"stb_filter"


#define FTP_SERVER_PORT_21       21
#define FTP_SERVER_PORT_2121     2121
#define WEB_SERVER_PORT_80       80
#define WEB_SERVER_PORT_8080     8080
#define SNMP_AGENT_PORT_161      161
#define SNMP_AGENT_PORT_16116    16116
#define SSH_SERVER_PORT_22       22
#define SSH_SERVER_PORT_2222     2222
#define TELNET_SERVER_PORT_23    23
#define TELNET_SERVER_PORT_2323  2323
#define TFTP_SERVER_PORT_69      69
#define TFTP_SERVER_PORT_6969    6969


/***********************************************************************
 **********************************************************************/
typedef enum
{
	FW_SECURITY_LEVEL_LOW=1,
	FW_SECURITY_LEVEL_MEDIUM,
	FW_SECURITY_LEVEL_HIGH
} fw_security_level;


/***********************************************************************
 **********************************************************************/

void rutFirewall_createIntfChains(tsl_32_t domain, const char *ifName, tsl_bool_t isBridgePort);
void rutFirewall_destroyIntfChains(tsl_32_t domain, const char *ifName, tsl_bool_t isBridgePort);

void tr69_util_firewall_reconf_nat_and_firewall_for_intf(tsl_32_t domain, const char *ifName, 
		char *ipaddr, char *subnet, tsl_bool_t isWanIntf, tsl_bool_t isBridgePort, 
		tsl_bool_t isNatEnabled, tsl_bool_t isFwEnabled);
void tr69_util_firewall_delete_nat_and_firewall_intfchains(tsl_32_t domain, const char *ifName, 
		char *ipaddr, char *subnet, tsl_bool_t isWanIntf, tsl_bool_t isBridgePort,
		tsl_bool_t isNatEnabled, tsl_bool_t isFwEnabled);

void tr69_util_firewall_update_tcp_syn_floodchain(tsl_u32_t rate, tsl_u32_t logLevel);
void tr69_util_firewall_update_udp_floodchain(tsl_u32_t rate, tsl_u32_t logLevel);
void tr69_util_firewall_update_udp_icmp_floodchain(tsl_u32_t rate, tsl_u32_t logLevel);
void tr69_util_firewall_update_fw_attackchain(tsl_bool_t enableProtection, tsl_bool_t tcpSynProt, 
		tsl_bool_t udpFloodProt, tsl_bool_t icmpFloodProt);

void tr69_util_firewall_update_system_firewall_cfg(void *newObj, void *currObj);
void tr69_util_firewall_update_security_level(tsl_32_t fwLevel, tsl_bool_t cleanup, tsl_bool_t doIntfReconf);
void tr69_util_firewall_update_blockip_fragments(tsl_bool_t blockIpFragments);

tsl_rv_t tr69_util_firewall_get_service_object_by_name(const char *servName, tsl_32_t *fwServIndex, 
		tr69_oid_stack_id *parentIidStack, tsl_32_t delta);

tsl_rv_t rutFirewall_getSchedulerObjectByName(const char *schName, tr69_oid_stack_id *parentIidStack, tsl_32_t delta);

void rutFirewall_updateAccessControl(const char *policy, tsl_bool_t add, const _AccessCtrlObject *obj, 
		tsl_32_t fwServIndex, tr69_oid_stack_id *servIidStack, tr69_oid_stack_id *schRuleIidStack);

void rutFirewall_updateAdvanceFilter(const char *ifType, const char *dir, tsl_bool_t add, 
		const _AdvFilterObject *obj, tsl_32_t fwServIndex, 
		tr69_oid_stack_id *servIidStack, tr69_oid_stack_id *schRuleIidStack);

void rutFirewall_updatePortMapping(const _WanIpConnObject *wan_ip_con, const _WanPppConnObject *wan_ppp_con, 
		const _WanIpConnPortmappingObject *ipObj, const _WanPppConnPortmappingObject *pppObj, 
		const _WanConnPortmappingObject *allWanObj, const tsl_bool_t add, tsl_32_t fwServIndex, 
		tr69_oid_stack_id *servIidStack, tr69_oid_stack_id *schRuleIidStack);

tsl_rv_t rutFirewall_updatePortTriggering(const char *ifName);

void rutFirewall_updateStaticNAT(const _StaticNatObject *natObj, const tsl_bool_t add);
void rutFirewall_updateStaticNATProxyArps(char *ifName);

void rutFirewall_updateDmzHost(const char *ifName, const char *ipAddress, tsl_bool_t add);

void rutFirewall_updateRemoteMgmt(const _RemoteMgmtObject *remoteMgmtObj);

void rutFirewall_updateLocalMgmt(const _LocalMgmtObject *localMgmtObj);

void rutFirewall_updateIgmpRules(tsl_bool_t add, const char *ifName, tsl_bool_t updateRules2AllowIgmp);

void rutFirewall_updateStbRules(tsl_bool_t add, char *stbIpAddr);

void rutFirewall_init(void);
void rutFirewall_uninit(void);

void tr69_util_firewall_init_at_system_bootup(void);
void rutFirewall_uninitAtSystemBootup(void);

#endif /* DMP_X_ACTIONTEC_COM_FIREWALL_1 */

#endif /* __RUT_FIREWALL_H__ */
