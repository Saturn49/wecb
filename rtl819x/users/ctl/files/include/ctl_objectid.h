#ifndef _CTL_OBJECTID_H
#define _CTL_OBJECTID_H

// This file was referred to:
//      ctl_layer/tr69fw/include/tr69_st.h
//      brcm462/userspace/public/include/mdm_objectid.h

#define CTLOID_IGD 					"InternetGatewayDevice"
#define CTLOID_IGD_Q_MGMT			"InternetGatewayDevice.ACTIPv6_QueueManagement"
#define CTLOID_IGD_Q_MGMT_CLASS		"InternetGatewayDevice.ACTIPv6_QueueManagement.Classification"
#define CTLOID_IGD_AEI_FIREWALL		"InternetGatewayDevice.ACTIPv6_X_AEI_COM_Firewall"
#define CTLOID_IGD_DEVICE_INFO 		"InternetGatewayDevice.ACTIPv6_DeviceInfo" 
#define CTLOID_VENDOR_CONFIG_FILE	"InternetGatewayDevice.ACTIPv6_DeviceInfo.VendorConfigFile" 
#define CTLOID_DEVICE_CONFIG 		"InternetGatewayDevice.ACTIPv6_DeviceConfig"
#define CTLOID_MANAGEMENT_SERVER 	"InternetGatewayDevice.ACTIPv6_ManagementServer"
#define CTLOID_TIME_SERVER_CFG 		"InternetGatewayDevice.ACTIPv6_Time"
#define CTLOID_IP_PING_DIAG 		"InternetGatewayDevice.ACTIPv6_IPPingDiagnostics"
#define CTLOID_LAN_DEV 				"InternetGatewayDevice.ACTIPv6_LANDevice"

#ifndef SUPPORT_IPV6_MULTI_LAN
#define CTLOID_LAN_HOST_CFG 		"InternetGatewayDevice.ACTIPv6_LANDevice.1.LANHostConfigManagement"
#define CTLOID_LAN_IP_INTF 			"InternetGatewayDevice.ACTIPv6_LANDevice.1.LANHostConfigManagement.IPInterface.1"
#define CTLOID_IPV6_LAN_HOST_CFG 	"InternetGatewayDevice.ACTIPv6_LANDevice.1.X_ACTIONTEC_COM_IPv6LANHostConfigManagement"
#define CTLOID_MLD_SNOOPING_CFG 	"InternetGatewayDevice.ACTIPv6_LANDevice.1.X_ACTIONTEC_COM_IPv6LANHostConfigManagement.X_ACTIONTEC_COM_MldSnoopingConfig"
#define CTLOID_LAN_ETH_INTF 		"InternetGatewayDevice.ACTIPv6_LANDevice.1.LANEthernetInterfaceConfig"
#define CTLOID_LAN_ETH_INTF_STATS 	"InternetGatewayDevice.ACTIPv6_LANDevice.1.LANEthernetInterfaceConfig.1.Stats"
#define CTLOID_LAN_HOSTS 			"InternetGatewayDevice.ACTIPv6_LANDevice.1.Hosts"
#define CTLOID_LAN_HOST_ENTRY 		"InternetGatewayDevice.ACTIPv6_LANDevice.1.Hosts.Host"
#define CTLOID_IPV6_LAN_PD_CFG		"InternetGatewayDevice.ACTIPv6_LANDevice.1.LANPDConfigManagement"
#define CTLOID_IPV6_LAN_PD_INST		"InternetGatewayDevice.ACTIPv6_LANDevice.1.LANPDConfigManagement.LanPDInstance"

#else
#define CTLOID_LAN_HOST_CFG 		"InternetGatewayDevice.ACTIPv6_LANDevice.%d.LANHostConfigManagement"
#define CTLOID_LAN_IP_INTF 			"InternetGatewayDevice.ACTIPv6_LANDevice.%d.LANHostConfigManagement.IPInterface.%d"
#define CTLOID_IPV6_LAN_HOST_CFG 	"InternetGatewayDevice.ACTIPv6_LANDevice.%d.X_ACTIONTEC_COM_IPv6LANHostConfigManagement"
#define CTLOID_MLD_SNOOPING_CFG 	"InternetGatewayDevice.ACTIPv6_LANDevice.%d.X_ACTIONTEC_COM_IPv6LANHostConfigManagement.X_ACTIONTEC_COM_MldSnoopingConfig"
#define CTLOID_LAN_ETH_INTF 		"InternetGatewayDevice.ACTIPv6_LANDevice.%d.LANEthernetInterfaceConfig"
#define CTLOID_LAN_ETH_INTF_STATS 	"InternetGatewayDevice.ACTIPv6_LANDevice.%d.LANEthernetInterfaceConfig.%d.Stats"
#define CTLOID_LAN_HOSTS 			"InternetGatewayDevice.ACTIPv6_LANDevice.%d.Hosts"
#define CTLOID_LAN_HOST_ENTRY 		"InternetGatewayDevice.ACTIPv6_LANDevice.%d.Hosts.Host"
#define CTLOID_IPV6_LAN_PD_CFG		"InternetGatewayDevice.ACTIPv6_LANDevice.%d.LANPDConfigManagement"
#define CTLOID_IPV6_LAN_PD_INST		"InternetGatewayDevice.ACTIPv6_LANDevice.%d.LANPDConfigManagement.LanPDInstance"
#endif

#define CTLOID_WAN_DEV 				"InternetGatewayDevice.ACTIPv6_WANDevice.1"
#define CTLOID_WAN_COMMON_INTF_CFG 	"InternetGatewayDevice.ACTIPv6_WANDevice.1.WANCommonInterfaceConfig"
#define CTLOID_WAN_CONN_DEVICE 		"InternetGatewayDevice.ACTIPv6_WANDevice.1.WANConnectionDevice.1"
#define CTLOID_WAN_IP_CONN 			"InternetGatewayDevice.ACTIPv6_WANDevice.1.WANConnectionDevice.1.WANIPConnection.1"
#define CTLOID_WAN_MOCA_CONN 		"InternetGatewayDevice.ACTIPv6_WANDevice.2.WANConnectionDevice.1.WANIPConnection.1"
#define CTLOID_WAN_IP_CONN_PORTMAPPING 	"InternetGatewayDevice.ACTIPv6_WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping"
#define CTLOID_WAN_PPP_CONN 		"InternetGatewayDevice.ACTIPv6_WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1"
#define CTLOID_WAN_PPP_CONN_PORTMAPPING "InternetGatewayDevice.ACTIPv6_WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PortMapping"
#define CTLOID_IPV6_L3_FORWARDING 	"InternetGatewayDevice.ACTIPv6_X_ACTIONTEC_COM_IPv6Layer3Forwarding"
#define CTLOID_IPV6_L3_FORWARDING_ENTRY "InternetGatewayDevice.ACTIPv6_X_ACTIONTEC_COM_IPv6Layer3Forwarding.IPv6Forwarding"


#define CTLOID_6RD_NUD              "InternetGatewayDevice.X_AEI_COM_IPv6NUD"

/*******************TR181 OID***********************/
#define CTLOID_TR181_DHCPV6_SERVER		"InternetGatewayDevice.Device.DHCPv6.Server"
#define CTLOID_TR181_DHCPV6_SERVER_POOL	"InternetGatewayDevice.Device.DHCPv6.Server.Pool.1"
#define CTLOID_TR181_WAN_ETHERNET_IP	"InternetGatewayDevice.Device.IP.Interface.3"
#define CTLOID_TR181_WAN_ETHERNET_PPP	"InternetGatewayDevice.Device.IP.Interface.4"
/*******************TR181 OID end********************/

#endif //_CTL_OBJECTID_H

