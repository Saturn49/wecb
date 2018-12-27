#ifndef TR69_ST_H
#define TR69_ST_H


tsl_char_t st_version[128] = "Internal";

typedef tsl_int_t (*tr69_func_get_obj_t)(tsl_char_t *p_oid_name, tsl_void_t *p_cur_data);
typedef tsl_int_t (*tr69_func_set_obj_t)(tsl_char_t *p_oid_name, tsl_void_t *p_cur_data, tsl_void_t *p_new_data);
struct tr69_register_func{
	 tsl_char_t oid_name[256];
	tr69_func_get_obj_t get_func;
	tr69_func_set_obj_t set_func;
};


struct tr69_register_func tr69_regfunc_tb[]={
	{
	"Device", 
	(tr69_func_get_obj_t )tr69_func_get_DeviceObject_value,
	(tr69_func_set_obj_t )tr69_func_set_DeviceObject_value
	},

	{
	"Device.DeviceInfo", 
	(tr69_func_get_obj_t )tr69_func_get_DeviceInfoObject_value,
	(tr69_func_set_obj_t )tr69_func_set_DeviceInfoObject_value
	},

	{
	"Device.DeviceInfo.VendorConfigFile", 
	(tr69_func_get_obj_t )tr69_func_get_VendorConfigFileObject_value,
	(tr69_func_set_obj_t )tr69_func_set_VendorConfigFileObject_value
	},

	{
	"Device.GatewayInfo", 
	(tr69_func_get_obj_t )tr69_func_get_GatewayInfoObject_value,
	(tr69_func_set_obj_t )tr69_func_set_GatewayInfoObject_value
	},

	{
	"Device.ManagementServer", 
	(tr69_func_get_obj_t )tr69_func_get_ManagementServerObject_value,
	(tr69_func_set_obj_t )tr69_func_set_ManagementServerObject_value
	},

	{
	"Device.ManagementServer.X_TWC_COM_RootCertificate", 
	(tr69_func_get_obj_t )tr69_func_get_X_TWC_COM_RootCertificateObject_value,
	(tr69_func_set_obj_t )tr69_func_set_X_TWC_COM_RootCertificateObject_value
	},

	{
	"Device.ManagementServer.ManageableDevice", 
	(tr69_func_get_obj_t )tr69_func_get_ManageableDeviceObject_value,
	(tr69_func_set_obj_t )tr69_func_set_ManageableDeviceObject_value
	},

	{
	"Device.Time", 
	(tr69_func_get_obj_t )tr69_func_get_TimeServerCfgObject_value,
	(tr69_func_set_obj_t )tr69_func_set_TimeServerCfgObject_value
	},

	{
	"Device.UserInterface", 
	(tr69_func_get_obj_t )tr69_func_get_UserInterfaceObject_value,
	(tr69_func_set_obj_t )tr69_func_set_UserInterfaceObject_value
	},

	{
	"Device.LAN", 
	(tr69_func_get_obj_t )tr69_func_get_LANObject_value,
	(tr69_func_set_obj_t )tr69_func_set_LANObject_value
	},

	{
	"Device.LAN.DHCPOption", 
	(tr69_func_get_obj_t )tr69_func_get_DHCPOptionObject_value,
	(tr69_func_set_obj_t )tr69_func_set_DHCPOptionObject_value
	},

	{
	"Device.LAN.Stats", 
	(tr69_func_get_obj_t )tr69_func_get_LANStatsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_LANStatsObject_value
	},

	{
	"Device.LAN.IPPingDiagnostics", 
	(tr69_func_get_obj_t )tr69_func_get_LANIPPingDiagnosticsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_LANIPPingDiagnosticsObject_value
	},

	{
	"Device.InterfaceStack", 
	(tr69_func_get_obj_t )tr69_func_get_InterfaceStackObject_value,
	(tr69_func_set_obj_t )tr69_func_set_InterfaceStackObject_value
	},

	{
	"Device.Ethernet", 
	(tr69_func_get_obj_t )tr69_func_get_EthernetObject_value,
	(tr69_func_set_obj_t )tr69_func_set_EthernetObject_value
	},

	{
	"Device.Ethernet.Interface", 
	(tr69_func_get_obj_t )tr69_func_get_EthernetInterfaceObject_value,
	(tr69_func_set_obj_t )tr69_func_set_EthernetInterfaceObject_value
	},

	{
	"Device.Ethernet.Interface.i.Stats", 
	(tr69_func_get_obj_t )tr69_func_get_EthernetInterfaceStatsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_EthernetInterfaceStatsObject_value
	},

	{
	"Device.Ethernet.Link", 
	(tr69_func_get_obj_t )tr69_func_get_EthernetLinkObject_value,
	(tr69_func_set_obj_t )tr69_func_set_EthernetLinkObject_value
	},

	{
	"Device.Ethernet.Link.i.Stats", 
	(tr69_func_get_obj_t )tr69_func_get_EthernetLinkStatsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_EthernetLinkStatsObject_value
	},

	{
	"Device.MoCA", 
	(tr69_func_get_obj_t )tr69_func_get_MOCAObject_value,
	(tr69_func_set_obj_t )tr69_func_set_MOCAObject_value
	},

	{
	"Device.MoCA.Interface", 
	(tr69_func_get_obj_t )tr69_func_get_MOCAInterfaceObject_value,
	(tr69_func_set_obj_t )tr69_func_set_MOCAInterfaceObject_value
	},

	{
	"Device.MoCA.Interface.i.Stats", 
	(tr69_func_get_obj_t )tr69_func_get_MOCAInterfaceStatObject_value,
	(tr69_func_set_obj_t )tr69_func_set_MOCAInterfaceStatObject_value
	},

	{
	"Device.MoCA.Interface.i.QoS", 
	(tr69_func_get_obj_t )tr69_func_get_MOCAQOSObject_value,
	(tr69_func_set_obj_t )tr69_func_set_MOCAQOSObject_value
	},

	{
	"Device.MoCA.Interface.i.QoS.FlowStats", 
	(tr69_func_get_obj_t )tr69_func_get_MOCAQOSFlowStatObject_value,
	(tr69_func_set_obj_t )tr69_func_set_MOCAQOSFlowStatObject_value
	},

	{
	"Device.MoCA.Interface.i.AssociatedDevice", 
	(tr69_func_get_obj_t )tr69_func_get_MOCAAssociatedDevObject_value,
	(tr69_func_set_obj_t )tr69_func_set_MOCAAssociatedDevObject_value
	},

	{
	"Device.WiFi", 
	(tr69_func_get_obj_t )tr69_func_get_WIFIObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFIObject_value
	},

	{
	"Device.WiFi.Radio", 
	(tr69_func_get_obj_t )tr69_func_get_WIFIRadioObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFIRadioObject_value
	},

	{
	"Device.WiFi.Radio.i.Stats", 
	(tr69_func_get_obj_t )tr69_func_get_WIFIRadioStatsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFIRadioStatsObject_value
	},

	{
	"Device.WiFi.SSID", 
	(tr69_func_get_obj_t )tr69_func_get_WIFISSIDObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFISSIDObject_value
	},

	{
	"Device.WiFi.SSID.i.Stats", 
	(tr69_func_get_obj_t )tr69_func_get_WIFISSIDStatsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFISSIDStatsObject_value
	},

	{
	"Device.WiFi.AccessPoint", 
	(tr69_func_get_obj_t )tr69_func_get_WIFIAccessPointObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFIAccessPointObject_value
	},

	{
	"Device.WiFi.AccessPoint.i.Security", 
	(tr69_func_get_obj_t )tr69_func_get_WIFIAccessPointSecurityObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFIAccessPointSecurityObject_value
	},

	{
	"Device.WiFi.AccessPoint.i.WPS", 
	(tr69_func_get_obj_t )tr69_func_get_WIFIAccessPointWpsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFIAccessPointWpsObject_value
	},

	{
	"Device.WiFi.AccessPoint.i.AssociatedDevice", 
	(tr69_func_get_obj_t )tr69_func_get_WIFIAccessPointAssociatedDeviceObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WIFIAccessPointAssociatedDeviceObject_value
	},

	{
	"Device.Bridging", 
	(tr69_func_get_obj_t )tr69_func_get_BridgingObject_value,
	(tr69_func_set_obj_t )tr69_func_set_BridgingObject_value
	},

	{
	"Device.Bridging.Bridge", 
	(tr69_func_get_obj_t )tr69_func_get_BridgingBridgeObject_value,
	(tr69_func_set_obj_t )tr69_func_set_BridgingBridgeObject_value
	},

	{
	"Device.Bridging.Bridge.i.Port", 
	(tr69_func_get_obj_t )tr69_func_get_BridgingBridgePortObject_value,
	(tr69_func_set_obj_t )tr69_func_set_BridgingBridgePortObject_value
	},

	{
	"Device.Bridging.Bridge.i.Port.i.Stats", 
	(tr69_func_get_obj_t )tr69_func_get_BridgingBridgePortStatsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_BridgingBridgePortStatsObject_value
	},

	{
	"Device.Bridging.Bridge.i.VLAN", 
	(tr69_func_get_obj_t )tr69_func_get_BridgingBridgeVlanObject_value,
	(tr69_func_set_obj_t )tr69_func_set_BridgingBridgeVlanObject_value
	},

	{
	"Device.Bridging.Bridge.i.VLANPort", 
	(tr69_func_get_obj_t )tr69_func_get_BridgingBridgeVlanPortObject_value,
	(tr69_func_set_obj_t )tr69_func_set_BridgingBridgeVlanPortObject_value
	},

	{
	"Device.IP", 
	(tr69_func_get_obj_t )tr69_func_get_IpObject_value,
	(tr69_func_set_obj_t )tr69_func_set_IpObject_value
	},

	{
	"Device.IP.Interface", 
	(tr69_func_get_obj_t )tr69_func_get_IpInterfaceObject_value,
	(tr69_func_set_obj_t )tr69_func_set_IpInterfaceObject_value
	},

	{
	"Device.IP.Interface.i.IPv4Address", 
	(tr69_func_get_obj_t )tr69_func_get_IpInterfaceIpv4AddressObject_value,
	(tr69_func_set_obj_t )tr69_func_set_IpInterfaceIpv4AddressObject_value
	},

	{
	"Device.IP.Interface.i.IPv6Address", 
	(tr69_func_get_obj_t )tr69_func_get_IpInterfaceIpv6AddressObject_value,
	(tr69_func_set_obj_t )tr69_func_set_IpInterfaceIpv6AddressObject_value
	},

	{
	"Device.Hosts", 
	(tr69_func_get_obj_t )tr69_func_get_HostsObject_value,
	(tr69_func_set_obj_t )tr69_func_set_HostsObject_value
	},

	{
	"Device.Hosts.Host", 
	(tr69_func_get_obj_t )tr69_func_get_HostsHostObject_value,
	(tr69_func_set_obj_t )tr69_func_set_HostsHostObject_value
	},

	{
	"Device.Hosts.Host.i.IPv4Address", 
	(tr69_func_get_obj_t )tr69_func_get_HostsHostIpv4AddressObject_value,
	(tr69_func_set_obj_t )tr69_func_set_HostsHostIpv4AddressObject_value
	},

	{
	"Device.Hosts.Host.i.IPv6Address", 
	(tr69_func_get_obj_t )tr69_func_get_HostsHostIpv6AddressObject_value,
	(tr69_func_set_obj_t )tr69_func_set_HostsHostIpv6AddressObject_value
	},

	{
	"Device.DHCPv4", 
	(tr69_func_get_obj_t )tr69_func_get_Dhcpv4Object_value,
	(tr69_func_set_obj_t )tr69_func_set_Dhcpv4Object_value
	},

	{
	"Device.DHCPv4.Client", 
	(tr69_func_get_obj_t )tr69_func_get_Dhcpv4ClientObject_value,
	(tr69_func_set_obj_t )tr69_func_set_Dhcpv4ClientObject_value
	},

	{
	"Device.DHCPv4.Server", 
	(tr69_func_get_obj_t )tr69_func_get_Dhcpv4ServerObject_value,
	(tr69_func_set_obj_t )tr69_func_set_Dhcpv4ServerObject_value
	},

	{
	"Device.DHCPv4.Server.Pool", 
	(tr69_func_get_obj_t )tr69_func_get_Dhcpv4ServerPoolObject_value,
	(tr69_func_set_obj_t )tr69_func_set_Dhcpv4ServerPoolObject_value
	},

	{
	"Device.Users", 
	(tr69_func_get_obj_t )tr69_func_get_UsersObject_value,
	(tr69_func_set_obj_t )tr69_func_set_UsersObject_value
	},

	{
	"Device.Users.User", 
	(tr69_func_get_obj_t )tr69_func_get_UsersUserObject_value,
	(tr69_func_set_obj_t )tr69_func_set_UsersUserObject_value
	},

	{
	"Device.Firewall", 
	(tr69_func_get_obj_t )tr69_func_get_FirewallObject_value,
	(tr69_func_set_obj_t )tr69_func_set_FirewallObject_value
	},

	{
	"Device.GRETunnel", 
	(tr69_func_get_obj_t )tr69_func_get_GreTunnelObject_value,
	(tr69_func_set_obj_t )tr69_func_set_GreTunnelObject_value
	},

	{
	"Device.X_ACTIONTEC_COM_ZeroConf", 
	(tr69_func_get_obj_t )tr69_func_get_ZeroConfObject_value,
	(tr69_func_set_obj_t )tr69_func_set_ZeroConfObject_value
	},

	{
	"Device.X_ACTIONTEC_COM_ZeroConf.Extender", 
	(tr69_func_get_obj_t )tr69_func_get_ZeroConfExtenderObject_value,
	(tr69_func_set_obj_t )tr69_func_set_ZeroConfExtenderObject_value
	},

	{
	"Device.X_ACTIONTEC_COM_LED", 
	(tr69_func_get_obj_t )tr69_func_get_LedObject_value,
	(tr69_func_set_obj_t )tr69_func_set_LedObject_value
	},

	{
	"Device.X_ACTIONTEC_COM_RemoteLogin", 
	(tr69_func_get_obj_t )tr69_func_get_RemoteLoginObject_value,
	(tr69_func_set_obj_t )tr69_func_set_RemoteLoginObject_value
	},

	{
	"Device.X_ACTIONTEC_COM_DNSServer", 
	(tr69_func_get_obj_t )tr69_func_get_DNSServerObject_value,
	(tr69_func_set_obj_t )tr69_func_set_DNSServerObject_value
	},

	{
	"Device.X_ACTIONTEC_COM_MDNS", 
	(tr69_func_get_obj_t )tr69_func_get_MDNSObject_value,
	(tr69_func_set_obj_t )tr69_func_set_MDNSObject_value
	},

	{
	"Device.X_ACTIONTEC_COM_NETBIOS", 
	(tr69_func_get_obj_t )tr69_func_get_NETBIOSObject_value,
	(tr69_func_set_obj_t )tr69_func_set_NETBIOSObject_value
	},

	{
	"Device.X_ACTIONTEC_Upgrade", 
	(tr69_func_get_obj_t )tr69_func_get_FirwareUpgradeObject_value,
	(tr69_func_set_obj_t )tr69_func_set_FirwareUpgradeObject_value
	},

	{
	"Device.X_ACTIONTEC_Upgrade.WanUpgradeStatus", 
	(tr69_func_get_obj_t )tr69_func_get_WanUpgradeStatusObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WanUpgradeStatusObject_value
	},

	{
	"Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig", 
	(tr69_func_get_obj_t )tr69_func_get_WanUpgradeObject_value,
	(tr69_func_set_obj_t )tr69_func_set_WanUpgradeObject_value
	},

	{
	"Device.XMPP", 
	(tr69_func_get_obj_t )tr69_func_get_XMPPObject_value,
	(tr69_func_set_obj_t )tr69_func_set_XMPPObject_value
	},

	{
	"Device.XMPP.Connection", 
	(tr69_func_get_obj_t )tr69_func_get_XMPPConnectionObject_value,
	(tr69_func_set_obj_t )tr69_func_set_XMPPConnectionObject_value
	},

	{
	"Device.XMPP.Connection.i.Server", 
	(tr69_func_get_obj_t )tr69_func_get_XMPPConnServerObject_value,
	(tr69_func_set_obj_t )tr69_func_set_XMPPConnServerObject_value
	},

};



#endif

