
/***********************************************************************
 *
 *  Copyright (c) 2009  Actiontec Electronics, Inc
 *  All Rights Reserved
 *
 *  cms_firewall.h
 *
 *  Firewall Configuration/Statistics related DEFINES are here
 *  ACTIONTEC_FIREWALL
 *
************************************************************************/

#ifndef __CMS_FIREWALL_H
#define __CMS_FIREWALL_H

#define MAX_HOSTNAME_SIZE				64
#define MAX_PROTOCOL_SIZE				16
#define MAX_PORTS_BUF_SIZE				32

#define MAX_SERVICE_INFO_SIZE			64
#define MAX_MULTI_SERVICE_INFO_SIZE		1024

#define MAX_SERVICES_NAME_SIZE			64
#define MAX_SERVICES_DESC_SIZE			256

#define MAX_SCHEDULER_NAME_SIZE			64
#define MAX_MULTI_SCHEDULER_INFO_SIZE	1024

#define MAX_PM_DESC_SIZE				256

#define MAX_PT_APPNAME_SIZE				64
#define MAX_PT_RULE_SIZE				MAX_PT_APPNAME_SIZE+64


#define MAX_FW_LOG_LINE			128
#define FW_LOG_EVENT_STR		"Firewall Setup"
#define FW_LOG_EVENT_TYPE_STR	"Configuration Change"

/***********************************************************************
 * DEFINES
 **********************************************************************/
/*
 * To filter packets on Bridge ports either
 * 'FW_USE_EBTABLES_FOR_BRPORTS' or 'FW_USE_IPTABLES_FOR_BRPORTS' should
 * be defined.
 */

#undef FW_PER_INTF_ADV_FILTERING

#define MAX_IPTABLE_CMDLEN		512


#define MAX_CHAINNAME_LEN		((CTL_IFNAME_LENGTH)+32)
#define MAX_IPT_LEN				16
#define MAX_ACTION_LEN			16
#define MAX_IPSTR_LEN			((CTL_IPADDR_LENGTH)+8)
#define MAX_PROTOCOL_LEN		MAX_PROTOCOL_SIZE
#define MAX_PORTSTR_LEN			MAX_PORTS_BUF_SIZE


/***********************************************************************
***********************************************************************/
typedef enum pt_proto_enum
{
    TCP,
    UDP,
	TCP_UDP
} pt_proto_e;


/* Port Triggerring Service Port */
typedef struct pt_port_s
{
    pt_proto_e triggerProtocol;
    tsl_u16_t triggerPortStart;
    tsl_u16_t triggerPortEnd;
    pt_proto_e openProtocol;
    tsl_u16_t openPortStart;
    tsl_u16_t openPortEnd;
} pt_port_t;

/* Port Triggerring Service */
#define MAX_PT_SERVPORTS	16
typedef struct pt_service_s
{
    char *ptServName;
    char *ptServDesc;
    tsl_u32_t ptServPortEntryCount;
    struct pt_port_s ptServPorts[MAX_PT_SERVPORTS];

	/* This is updated at run time */
	tsl_bool_t inUse;
} pt_service_t;


typedef enum fw_proto_enum
{
    FW_TCP,
    FW_UDP,
    FW_ICMP,
    FW_OTHER
} fw_proto_e;

/* Firewall Service */
#define FW_ICMP_TYPE(fwPort)		(((fwPort->protoNum)&0xFF00)>> 8)
#define FW_ICMP_CODE(fwPort)		((fwPort->protoNum)&0xFF)
typedef struct fw_port_s
{
    fw_proto_e protocol;
    tsl_u16_t srcPortStart;
    tsl_u16_t srcPortEnd;
    tsl_u16_t dstPortStart;
    tsl_u16_t dstPortEnd;
	/* 
	 * if protocol is OTHER, protoNum specifies port number 
	 * if protocol is ICMP, protoNum specifies ICMP Type and ICMP Code
	 * as follows (protoNum = (ICMP_TYPE << 8)|ICMP_CODE
	 */
	tsl_u32_t protoNum;
} fw_port_t;

#define MAX_FW_SERVPORTS	24
typedef struct fw_service_s
{
    char *servName;
    char *servDesc;
    tsl_u32_t servPortEntryCount;
    struct fw_port_s servPorts[MAX_FW_SERVPORTS];
	tsl_bool_t isAdvanced;
	tsl_bool_t isInActive;

	tsl_u32_t mdmInstanceID;
	tsl_u32_t refcnt;
} fw_service_t;
typedef struct service_info_s
{
    fw_proto_e protocol;

    tsl_u16_t srcPortStart;
    tsl_u16_t srcPortEnd;

    tsl_u16_t dstPortStart;
    tsl_u16_t dstPortEnd;

	/* 
	 * if protocol is OTHER, protoNum specifies port number 
	 * if protocol is ICMP, protoNum specifies ICMP Type and ICMP Code
	 * as follows (protoNum = (ICMP_TYPE << 8)|ICMP_CODE
	 */
	tsl_u32_t protoNum;

	tsl_bool_t excludeProtocol;
	tsl_bool_t excludeSrcPort;
	tsl_bool_t excludeDstPort;
} service_info_t;


/***********************************************************************
 **********************************************************************/
tsl_u32_t firewallGetPtServiceCount(void);
pt_service_t *firewallGetPtServiceEntry(tsl_u32_t appIndex);
void firewallMarkPtService(tsl_u32_t appIndex, tsl_bool_t mark);
char *firewallGetPtProtoString(pt_proto_e proto);

char *firewallGetFwProtoString(fw_proto_e proto);
tsl_u32_t firewallGetFwServiceCount(void);
fw_service_t *firewallGetFwServiceEntry(tsl_u32_t appIndex);
void firewallUpdateFwService(tsl_u32_t appIndex, tsl_u32_t instanceID);
void firewallFwServiceAdjustRefcnt(tsl_u32_t appIndex, tsl_32_t delta);
tsl_bool_t firewallFwServiceFind(char *name);
#define rcl_firewallLogSecurity(s) (strlen(s))

#define VRTSRV_ENABLE_NEW_OR_ENABLE_EXISTING(n, c) \
   (((n) != NULL && (n)->portMappingEnabled && (c) == NULL) || \
   ((n) != NULL && (n)->portMappingEnabled && (c) != NULL && !((c)->portMappingEnabled)))

#define VRTSRV_DELETE_OR_DISABLE_EXISTING(n, c) \
   (((n) == NULL) ||                                                    \
    ((n) != NULL && !((n)->portMappingEnabled) && (c) != NULL && (c)->portMappingEnabled))

#define VRTSRV_POTENTIAL_CHANGE_OF_EXISTING(n, c) \
   (((n) != NULL && (n)->portMappingEnabled && (c) != NULL && (c)->portMappingEnabled))
#endif /* __CMS_FIREWALL_H */
