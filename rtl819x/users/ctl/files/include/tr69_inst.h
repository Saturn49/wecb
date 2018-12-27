#ifndef TR69_INST_H
#define TR69_INST_H

struct tr69_inst_numb_of_entries_s{
	char numb_of_entries_oid[256];
	char entries_oid[256];
};

struct tr69_inst_numb_of_entries_s tr69_inst_numb_tb[]={
	{
		"InterfaceStackNumberOfEntries",
		"Device.InterfaceStack.i"
	},

	{
		"VendorConfigFileNumberOfEntries",
		"Device.DeviceInfo.VendorConfigFile.i"
	},

	{
		"SupportedDataModelNumberOfEntries",
		""
	},

	{
		"ProcessorNumberOfEntries",
		""
	},

	{
		"VendorLogFileNumberOfEntries",
		""
	},

	{
		"LocationNumberOfEntries",
		""
	},

	{
		"ManageableDeviceNumberOfEntries",
		"Device.ManagementServer.ManageableDevice.i"
	},

	{
		"EmbeddedDeviceNumberOfEntries",
		""
	},

	{
		"VirtualDeviceNumberOfEntries",
		""
	},

	{
		"X_TWC_COM_RootCertificateNumberOfEntries",
		"Device.ManagementServer.X_TWC_COM_RootCertificate.i"
	},

	{
		"DHCPOptionNumberOfEntries",
		"Device.LAN.DHCPOption.i"
	},

	{
		"InterfaceNumberOfEntries",
		"Device.Ethernet.Interface.i"
	},

	{
		"LinkNumberOfEntries",
		"Device.Ethernet.Link.i"
	},

	{
		"InterfaceNumberOfEntries",
		"Device.MoCA.Interface.i"
	},

	{
		"AssociatedDeviceNumberOfEntries",
		"Device.MoCA.Interface.i.AssociatedDevice.i"
	},

	{
		"FlowStatsNumberOfEntries",
		"Device.MoCA.Interface.i.QoS.FlowStats.i"
	},

	{
		"RadioNumberOfEntries",
		"Device.WiFi.Radio.i"
	},

	{
		"SSIDNumberOfEntries",
		"Device.WiFi.SSID.i"
	},

	{
		"AccessPointNumberOfEntries",
		"Device.WiFi.AccessPoint.i"
	},

	{
		"AssociatedDeviceNumberOfEntries",
		"Device.WiFi.AccessPoint.i.AssociatedDevice.i"
	},

	{
		"BridgeNumberOfEntries",
		"Device.Bridging.Bridge.i"
	},

	{
		"FilterNumberOfEntries",
		""
	},

	{
		"PortNumberOfEntries",
		"Device.Bridging.Bridge.i.Port.i"
	},

	{
		"VLANNumberOfEntries",
		"Device.Bridging.Bridge.i.VLAN.i"
	},

	{
		"VLANPortNumberOfEntries",
		"Device.Bridging.Bridge.i.VLANPort.i"
	},

	{
		"InterfaceNumberOfEntries",
		"Device.IP.Interface.i"
	},

	{
		"IPv4AddressNumberOfEntries",
		"Device.IP.Interface.i.IPv4Address.i"
	},

	{
		"IPv6AddressNumberOfEntries",
		"Device.IP.Interface.i.IPv6Address.i"
	},

	{
		"HostNumberOfEntries",
		"Device.Hosts.Host.i"
	},

	{
		"IPv4AddressNumberOfEntries",
		"Device.Hosts.Host.i.IPv4Address.i"
	},

	{
		"IPv6AddressNumberOfEntries",
		"Device.Hosts.Host.i.IPv6Address.i"
	},

	{
		"ClientNumberOfEntries",
		"Device.DHCPv4.Client.i"
	},

	{
		"PoolNumberOfEntries",
		"Device.DHCPv4.Server.Pool.i"
	},

	{
		"UserNumberOfEntries",
		"Device.Users.User.i"
	},

	{
		"ConnectionNumberOfEntries",
		"Device.XMPP.Connection.i"
	},

	{
		"ServerNumberOfEntries",
		"Device.XMPP.Connection.i.Server.i"
	},

};

#endif
