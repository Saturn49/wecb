/*TR69_FUNC.h*/

#ifndef TR69_FUNC_H
#define TR69_FUNC_H

#include "tsl_common.h"

#define TR69_RT_SUCCESS_VALUE_UNCHANGED 0x101
#define TR69_RT_SUCCESS_VALUE_CHANGED 0x102



/*Device*/
typedef struct st_DeviceObject_obj {
	 tsl_char_t *rootDataModelVersion;
	 tsl_uint_t interfaceStackNumberOfEntries;
	 tsl_char_t *deviceSummary;
}DeviceObject;
typedef DeviceObject _DeviceObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_DeviceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _DeviceObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_DeviceObject_value(tsl_char_t *p_oid_name, _DeviceObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_DeviceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _DeviceObject *p_cur_data
 *	        _DeviceObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_DeviceObject_value(tsl_char_t *p_oid_name, _DeviceObject *p_cur_data, _DeviceObject *p_new_data);


/*Device.DeviceInfo*/
typedef struct st_DeviceInfoObject_obj {
	 tsl_char_t *manufacturer;
	 tsl_char_t *manufacturerOUI;
	 tsl_char_t *modelName;
	 tsl_char_t *description;
	 tsl_char_t *productClass;
	 tsl_char_t *serialNumber;
	 tsl_char_t *X_ACTIONTEC_COM_RealSerialNumber;
	 tsl_char_t *X_ACTIONTEC_COM_HostName;
	 tsl_char_t *hardwareVersion;
	 tsl_char_t *softwareVersion;
	 tsl_char_t *X_ACTIONTEC_COM_LastSoftwareVersion;
	 tsl_char_t *additionalHardwareVersion;
	 tsl_char_t *additionalSoftwareVersion;
	 tsl_char_t *specVersion;
	 tsl_char_t *provisioningCode;
	 tsl_uint_t upTime;
	 tsl_char_t *firstUseDate;
	 tsl_int_t X_ACTIONTEC_COM_Autoconfig;
	 tsl_uint_t vendorConfigFileNumberOfEntries;
	 tsl_uint_t supportedDataModelNumberOfEntries;
	 tsl_uint_t processorNumberOfEntries;
	 tsl_uint_t vendorLogFileNumberOfEntries;
	 tsl_uint_t locationNumberOfEntries;
}DeviceInfoObject;
typedef DeviceInfoObject _DeviceInfoObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_DeviceInfoObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DeviceInfo
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _DeviceInfoObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_DeviceInfoObject_value(tsl_char_t *p_oid_name, _DeviceInfoObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_DeviceInfoObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DeviceInfo
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _DeviceInfoObject *p_cur_data
 *	        _DeviceInfoObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_DeviceInfoObject_value(tsl_char_t *p_oid_name, _DeviceInfoObject *p_cur_data, _DeviceInfoObject *p_new_data);


/*Device.DeviceInfo.VendorConfigFile*/
typedef struct st_VendorConfigFileObject_obj {
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_char_t *version;
	 tsl_char_t *date;
	 tsl_char_t *description;
}VendorConfigFileObject;
typedef VendorConfigFileObject _VendorConfigFileObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_VendorConfigFileObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DeviceInfo.VendorConfigFile
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _VendorConfigFileObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_VendorConfigFileObject_value(tsl_char_t *p_oid_name, _VendorConfigFileObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_VendorConfigFileObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DeviceInfo.VendorConfigFile
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _VendorConfigFileObject *p_cur_data
 *	        _VendorConfigFileObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_VendorConfigFileObject_value(tsl_char_t *p_oid_name, _VendorConfigFileObject *p_cur_data, _VendorConfigFileObject *p_new_data);


/*Device.GatewayInfo*/
typedef struct st_GatewayInfoObject_obj {
	 tsl_char_t *manufacturerOUI;
	 tsl_char_t *serialNumber;
	 tsl_char_t *productClass;
	 tsl_char_t *X_ACTIONTEC_COM_IPAddress;
	 tsl_char_t *X_ACTIONTEC_COM_MACAddress;
	 tsl_char_t *X_ACTIONTEC_COM_ArpEntry;
}GatewayInfoObject;
typedef GatewayInfoObject _GatewayInfoObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_GatewayInfoObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.GatewayInfo
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _GatewayInfoObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_GatewayInfoObject_value(tsl_char_t *p_oid_name, _GatewayInfoObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_GatewayInfoObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.GatewayInfo
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _GatewayInfoObject *p_cur_data
 *	        _GatewayInfoObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_GatewayInfoObject_value(tsl_char_t *p_oid_name, _GatewayInfoObject *p_cur_data, _GatewayInfoObject *p_new_data);


/*Device.ManagementServer*/
typedef struct st_ManagementServerObject_obj {
	 tsl_int_t enableCWMP;
	 tsl_char_t *URL;
	 tsl_char_t *X_ACTIONTEC_COM_LastConnectedURL;
	 tsl_char_t *username;
	 tsl_int_t X_ACTIONTEC_COM_Username_Flag;
	 tsl_char_t *password;
	 tsl_int_t X_ACTIONTEC_COM_CPE_DIGEST;
	 tsl_int_t periodicInformEnable;
	 tsl_uint_t periodicInformInterval;
	 tsl_char_t *periodicInformTime;
	 tsl_char_t *X_ACTIONTEC_COM_PeriodicInformTime;
	 tsl_uint_t X_ACTIONTEC_COM_BackoffInterval;
	 tsl_char_t *parameterKey;
	 tsl_char_t *commandKey;
	 tsl_char_t *X_ACTIONTEC_COM_UUID;
	 tsl_char_t *connectionRequestURL;
	 tsl_int_t X_ACTIONTEC_COM_SetConnReqURL;
	 tsl_char_t *connectionRequestUsername;
	 tsl_char_t *connectionRequestPassword;
	 tsl_int_t upgradesManaged;
	 tsl_char_t *kickURL;
	 tsl_char_t *downloadProgressURL;
	 tsl_uint_t defaultActiveNotificationThrottle;
	 tsl_uint_t CWMPRetryMinimumWaitInterval;
	 tsl_uint_t CWMPRetryIntervalMultiplier;
	 tsl_uint_t X_Verizon_MaxRetryInterval;
	 tsl_uint_t X_Verizon_RetryInterval;
	 tsl_char_t *UDPConnectionRequestAddress;
	 tsl_int_t STUNEnable;
	 tsl_char_t *STUNServerAddress;
	 tsl_uint_t STUNServerPort;
	 tsl_char_t *STUNUsername;
	 tsl_char_t *STUNPassword;
	 tsl_int_t STUNMaximumKeepAlivePeriod;
	 tsl_uint_t STUNMinimumKeepAlivePeriod;
	 tsl_int_t NATDetected;
	 tsl_int_t aliasBasedAddressing;
	 tsl_char_t *instanceMode;
	 tsl_int_t autoCreateInstances;
	 tsl_char_t *supportedConnReqMethods;
	 tsl_char_t *connReqXMPPConnection;
	 tsl_char_t *connReqAllowedJabberIDs;
	 tsl_char_t *connReqJabberID;
	 tsl_uint_t manageableDeviceNumberOfEntries;
	 tsl_uint_t embeddedDeviceNumberOfEntries;
	 tsl_uint_t virtualDeviceNumberOfEntries;
	 tsl_int_t X_TWC_COM_ValidateManagementServerCertificate;
	 tsl_int_t X_TWC_COM_ValidateDownloadServerCertificate;
	 tsl_uint_t X_TWC_COM_RootCertificateNumberOfEntries;
}ManagementServerObject;
typedef ManagementServerObject _ManagementServerObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_ManagementServerObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.ManagementServer
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _ManagementServerObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_ManagementServerObject_value(tsl_char_t *p_oid_name, _ManagementServerObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_ManagementServerObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.ManagementServer
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _ManagementServerObject *p_cur_data
 *	        _ManagementServerObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_ManagementServerObject_value(tsl_char_t *p_oid_name, _ManagementServerObject *p_cur_data, _ManagementServerObject *p_new_data);


/*Device.ManagementServer.X_TWC_COM_RootCertificate*/
typedef struct st_X_TWC_COM_RootCertificateObject_obj {
	 tsl_int_t enabled;
	 tsl_char_t *certificate;
	 tsl_char_t *lastModif;
	 tsl_char_t *serialNumber;
	 tsl_char_t *issuer;
	 tsl_char_t *notBefore;
	 tsl_char_t *notAfter;
	 tsl_char_t *subject;
	 tsl_char_t *subjectAlt;
	 tsl_char_t *signatureAlgorithm;
	 tsl_char_t *X_ACTIONTEC_COM_Hash;
}X_TWC_COM_RootCertificateObject;
typedef X_TWC_COM_RootCertificateObject _X_TWC_COM_RootCertificateObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_X_TWC_COM_RootCertificateObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.ManagementServer.X_TWC_COM_RootCertificate
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _X_TWC_COM_RootCertificateObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_X_TWC_COM_RootCertificateObject_value(tsl_char_t *p_oid_name, _X_TWC_COM_RootCertificateObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_X_TWC_COM_RootCertificateObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.ManagementServer.X_TWC_COM_RootCertificate
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _X_TWC_COM_RootCertificateObject *p_cur_data
 *	        _X_TWC_COM_RootCertificateObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_X_TWC_COM_RootCertificateObject_value(tsl_char_t *p_oid_name, _X_TWC_COM_RootCertificateObject *p_cur_data, _X_TWC_COM_RootCertificateObject *p_new_data);


/*Device.ManagementServer.ManageableDevice*/
typedef struct st_ManageableDeviceObject_obj {
	 tsl_char_t *alias;
	 tsl_char_t *manufacturerOUI;
	 tsl_char_t *serialNumber;
	 tsl_char_t *productClass;
	 tsl_char_t *host;
}ManageableDeviceObject;
typedef ManageableDeviceObject _ManageableDeviceObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_ManageableDeviceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.ManagementServer.ManageableDevice
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _ManageableDeviceObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_ManageableDeviceObject_value(tsl_char_t *p_oid_name, _ManageableDeviceObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_ManageableDeviceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.ManagementServer.ManageableDevice
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _ManageableDeviceObject *p_cur_data
 *	        _ManageableDeviceObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_ManageableDeviceObject_value(tsl_char_t *p_oid_name, _ManageableDeviceObject *p_cur_data, _ManageableDeviceObject *p_new_data);


/*Device.Time*/
typedef struct st_TimeServerCfgObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *NTPServer1;
	 tsl_char_t *NTPServer2;
	 tsl_char_t *NTPServer3;
	 tsl_char_t *NTPServer4;
	 tsl_char_t *NTPServer5;
	 tsl_char_t *currentLocalTime;
	 tsl_char_t *localTimeZone;
	 tsl_uint_t X_ACTIONTEC_COM_ForceUpdateFrequency;
	 tsl_uint_t X_ACTIONTEC_COM_MinUpdateFrequency;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
}TimeServerCfgObject;
typedef TimeServerCfgObject _TimeServerCfgObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_TimeServerCfgObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Time
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _TimeServerCfgObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_TimeServerCfgObject_value(tsl_char_t *p_oid_name, _TimeServerCfgObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_TimeServerCfgObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Time
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _TimeServerCfgObject *p_cur_data
 *	        _TimeServerCfgObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_TimeServerCfgObject_value(tsl_char_t *p_oid_name, _TimeServerCfgObject *p_cur_data, _TimeServerCfgObject *p_new_data);


/*Device.UserInterface*/
typedef struct st_UserInterfaceObject_obj {
	 tsl_int_t X_TWC_COM_InhibitRadioControl;
}UserInterfaceObject;
typedef UserInterfaceObject _UserInterfaceObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_UserInterfaceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.UserInterface
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _UserInterfaceObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_UserInterfaceObject_value(tsl_char_t *p_oid_name, _UserInterfaceObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_UserInterfaceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.UserInterface
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _UserInterfaceObject *p_cur_data
 *	        _UserInterfaceObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_UserInterfaceObject_value(tsl_char_t *p_oid_name, _UserInterfaceObject *p_cur_data, _UserInterfaceObject *p_new_data);


/*Device.LAN*/
typedef struct st_LANObject_obj {
	 tsl_char_t *addressingType;
	 tsl_char_t *IPAddress;
	 tsl_char_t *X_ACTIONTEC_COM_IPAddress;
	 tsl_char_t *subnetMask;
	 tsl_char_t *defaultGateway;
	 tsl_char_t *DNSServers;
	 tsl_char_t *MACAddress;
	 tsl_int_t MACAddressOverride;
	 tsl_uint_t DHCPOptionNumberOfEntries;
}LANObject;
typedef LANObject _LANObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_LANObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.LAN
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _LANObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_LANObject_value(tsl_char_t *p_oid_name, _LANObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_LANObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.LAN
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _LANObject *p_cur_data
 *	        _LANObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_LANObject_value(tsl_char_t *p_oid_name, _LANObject *p_cur_data, _LANObject *p_new_data);


/*Device.LAN.DHCPOption*/
typedef struct st_DHCPOptionObject_obj {
	 tsl_int_t request;
	 tsl_uint_t tag;
	 tsl_char_t *value;
}DHCPOptionObject;
typedef DHCPOptionObject _DHCPOptionObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_DHCPOptionObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.LAN.DHCPOption
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _DHCPOptionObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_DHCPOptionObject_value(tsl_char_t *p_oid_name, _DHCPOptionObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_DHCPOptionObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.LAN.DHCPOption
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _DHCPOptionObject *p_cur_data
 *	        _DHCPOptionObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_DHCPOptionObject_value(tsl_char_t *p_oid_name, _DHCPOptionObject *p_cur_data, _DHCPOptionObject *p_new_data);


/*Device.LAN.Stats*/
typedef struct st_LANStatsObject_obj {
	 tsl_uint_t totalBytesSent;
	 tsl_uint_t totalBytesReceived;
	 tsl_uint_t totalPacketsSent;
	 tsl_uint_t totalPacketsReceived;
}LANStatsObject;
typedef LANStatsObject _LANStatsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_LANStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.LAN.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _LANStatsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_LANStatsObject_value(tsl_char_t *p_oid_name, _LANStatsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_LANStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.LAN.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _LANStatsObject *p_cur_data
 *	        _LANStatsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_LANStatsObject_value(tsl_char_t *p_oid_name, _LANStatsObject *p_cur_data, _LANStatsObject *p_new_data);


/*Device.LAN.IPPingDiagnostics*/
typedef struct st_LANIPPingDiagnosticsObject_obj {
	 tsl_char_t *diagnosticsState;
	 tsl_char_t *host;
	 tsl_uint_t numberOfRepetitions;
	 tsl_uint_t timeOut;
	 tsl_uint_t dataBlockSize;
	 tsl_uint_t DSCP;
	 tsl_uint_t successCount;
	 tsl_uint_t failureCount;
	 tsl_uint_t averageResponseTime;
	 tsl_uint_t minimumResponseTime;
	 tsl_uint_t maximumResponseTime;
}LANIPPingDiagnosticsObject;
typedef LANIPPingDiagnosticsObject _LANIPPingDiagnosticsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_LANIPPingDiagnosticsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.LAN.IPPingDiagnostics
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _LANIPPingDiagnosticsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_LANIPPingDiagnosticsObject_value(tsl_char_t *p_oid_name, _LANIPPingDiagnosticsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_LANIPPingDiagnosticsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.LAN.IPPingDiagnostics
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _LANIPPingDiagnosticsObject *p_cur_data
 *	        _LANIPPingDiagnosticsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_LANIPPingDiagnosticsObject_value(tsl_char_t *p_oid_name, _LANIPPingDiagnosticsObject *p_cur_data, _LANIPPingDiagnosticsObject *p_new_data);


/*Device.InterfaceStack*/
typedef struct st_InterfaceStackObject_obj {
	 tsl_char_t *higherLayer;
	 tsl_char_t *lowerLayer;
}InterfaceStackObject;
typedef InterfaceStackObject _InterfaceStackObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_InterfaceStackObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.InterfaceStack
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _InterfaceStackObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_InterfaceStackObject_value(tsl_char_t *p_oid_name, _InterfaceStackObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_InterfaceStackObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.InterfaceStack
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _InterfaceStackObject *p_cur_data
 *	        _InterfaceStackObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_InterfaceStackObject_value(tsl_char_t *p_oid_name, _InterfaceStackObject *p_cur_data, _InterfaceStackObject *p_new_data);


/*Device.Ethernet*/
typedef struct st_EthernetObject_obj {
	 tsl_uint_t interfaceNumberOfEntries;
	 tsl_uint_t linkNumberOfEntries;
}EthernetObject;
typedef EthernetObject _EthernetObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_EthernetObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_EthernetObject_value(tsl_char_t *p_oid_name, _EthernetObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_EthernetObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetObject *p_cur_data
 *	        _EthernetObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_EthernetObject_value(tsl_char_t *p_oid_name, _EthernetObject *p_cur_data, _EthernetObject *p_new_data);


/*Device.Ethernet.Interface*/
typedef struct st_EthernetInterfaceObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_uint_t lastChange;
	 tsl_uint_t X_ACTIONTEC_COM_LastChangeTime;
	 tsl_char_t *lowerLayers;
	 tsl_int_t upstream;
	 tsl_char_t *MACAddress;
	 tsl_uint_t maxBitRate;
	 tsl_char_t *duplexMode;
}EthernetInterfaceObject;
typedef EthernetInterfaceObject _EthernetInterfaceObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_EthernetInterfaceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet.Interface
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetInterfaceObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_EthernetInterfaceObject_value(tsl_char_t *p_oid_name, _EthernetInterfaceObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_EthernetInterfaceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet.Interface
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetInterfaceObject *p_cur_data
 *	        _EthernetInterfaceObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_EthernetInterfaceObject_value(tsl_char_t *p_oid_name, _EthernetInterfaceObject *p_cur_data, _EthernetInterfaceObject *p_new_data);


/*Device.Ethernet.Interface.0.Stats*/
typedef struct st_EthernetInterfaceStatsObject_obj {
	 tsl_uint_t bytesSent;
	 tsl_uint_t bytesReceived;
	 tsl_uint_t packetsSent;
	 tsl_uint_t packetsReceived;
	 tsl_uint_t errorsSent;
	 tsl_uint_t errorsReceived;
	 tsl_uint_t unicastPacketsSent;
	 tsl_uint_t unicastPacketsReceived;
	 tsl_uint_t discardPacketsSent;
	 tsl_uint_t discardPacketsReceived;
	 tsl_uint_t multicastPacketsSent;
	 tsl_uint_t multicastPacketsReceived;
	 tsl_uint_t broadcastPacketsSent;
	 tsl_uint_t broadcastPacketsReceived;
	 tsl_uint_t unknownProtoPacketsReceived;
}EthernetInterfaceStatsObject;
typedef EthernetInterfaceStatsObject _EthernetInterfaceStatsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_EthernetInterfaceStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet.Interface.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetInterfaceStatsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_EthernetInterfaceStatsObject_value(tsl_char_t *p_oid_name, _EthernetInterfaceStatsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_EthernetInterfaceStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet.Interface.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetInterfaceStatsObject *p_cur_data
 *	        _EthernetInterfaceStatsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_EthernetInterfaceStatsObject_value(tsl_char_t *p_oid_name, _EthernetInterfaceStatsObject *p_cur_data, _EthernetInterfaceStatsObject *p_new_data);


/*Device.Ethernet.Link*/
typedef struct st_EthernetLinkObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_uint_t lastChange;
	 tsl_uint_t X_ACTIONTEC_COM_LastChangeTime;
	 tsl_char_t *lowerLayers;
	 tsl_char_t *MACAddress;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
}EthernetLinkObject;
typedef EthernetLinkObject _EthernetLinkObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_EthernetLinkObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet.Link
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetLinkObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_EthernetLinkObject_value(tsl_char_t *p_oid_name, _EthernetLinkObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_EthernetLinkObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet.Link
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetLinkObject *p_cur_data
 *	        _EthernetLinkObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_EthernetLinkObject_value(tsl_char_t *p_oid_name, _EthernetLinkObject *p_cur_data, _EthernetLinkObject *p_new_data);


/*Device.Ethernet.Link.0.Stats*/
typedef struct st_EthernetLinkStatsObject_obj {
	 tsl_uint_t bytesSent;
	 tsl_uint_t bytesReceived;
	 tsl_uint_t packetsSent;
	 tsl_uint_t packetsReceived;
	 tsl_uint_t errorsSent;
	 tsl_uint_t errorsReceived;
	 tsl_uint_t unicastPacketsSent;
	 tsl_uint_t unicastPacketsReceived;
	 tsl_uint_t discardPacketsSent;
	 tsl_uint_t discardPacketsReceived;
	 tsl_uint_t multicastPacketsSent;
	 tsl_uint_t multicastPacketsReceived;
	 tsl_uint_t broadcastPacketsSent;
	 tsl_uint_t broadcastPacketsReceived;
	 tsl_uint_t unknownProtoPacketsReceived;
}EthernetLinkStatsObject;
typedef EthernetLinkStatsObject _EthernetLinkStatsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_EthernetLinkStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet.Link.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetLinkStatsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_EthernetLinkStatsObject_value(tsl_char_t *p_oid_name, _EthernetLinkStatsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_EthernetLinkStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Ethernet.Link.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _EthernetLinkStatsObject *p_cur_data
 *	        _EthernetLinkStatsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_EthernetLinkStatsObject_value(tsl_char_t *p_oid_name, _EthernetLinkStatsObject *p_cur_data, _EthernetLinkStatsObject *p_new_data);


/*Device.MoCA*/
typedef struct st_MOCAObject_obj {
	 tsl_uint_t interfaceNumberOfEntries;
}MOCAObject;
typedef MOCAObject _MOCAObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_MOCAObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_MOCAObject_value(tsl_char_t *p_oid_name, _MOCAObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_MOCAObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAObject *p_cur_data
 *	        _MOCAObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_MOCAObject_value(tsl_char_t *p_oid_name, _MOCAObject *p_cur_data, _MOCAObject *p_new_data);


/*Device.MoCA.Interface*/
typedef struct st_MOCAInterfaceObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *X_ACTIONTEC_COM_Status;
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_uint_t lastChange;
	 tsl_uint_t X_ACTIONTEC_COM_LastChangeTime;
	 tsl_char_t *lowerLayers;
	 tsl_int_t upstream;
	 tsl_char_t *MACAddress;
	 tsl_char_t *firmwareVersion;
	 tsl_uint_t maxBitRate;
	 tsl_uint_t maxIngressBW;
	 tsl_uint_t maxEgressBW;
	 tsl_char_t *highestVersion;
	 tsl_char_t *currentVersion;
	 tsl_uint_t networkCoordinator;
	 tsl_uint_t nodeID;
	 tsl_int_t maxNodes;
	 tsl_int_t preferredNC;
	 tsl_uint_t backupNC;
	 tsl_int_t privacyEnabledSetting;
	 tsl_int_t privacyEnabled;
	 tsl_char_t *freqCapabilityMask;
	 tsl_char_t *freqCurrentMaskSetting;
	 tsl_char_t *freqCurrentMask;
	 tsl_uint_t currentOperFreq;
	 tsl_uint_t lastOperFreq;
	 tsl_char_t *keyPassphrase;
	 tsl_uint_t txPowerLimit;
	 tsl_uint_t powerCntlPhyTarget;
	 tsl_uint_t beaconPowerLimit;
	 tsl_uint_t networkTabooMask;
	 tsl_uint_t nodeTabooMask;
	 tsl_uint_t txBcastRate;
	 tsl_uint_t txBcastPowerReduction;
	 tsl_int_t QAM256Capable;
	 tsl_uint_t packetAggregationCapability;
	 tsl_uint_t associatedDeviceNumberOfEntries;
}MOCAInterfaceObject;
typedef MOCAInterfaceObject _MOCAInterfaceObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_MOCAInterfaceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAInterfaceObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_MOCAInterfaceObject_value(tsl_char_t *p_oid_name, _MOCAInterfaceObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_MOCAInterfaceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAInterfaceObject *p_cur_data
 *	        _MOCAInterfaceObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_MOCAInterfaceObject_value(tsl_char_t *p_oid_name, _MOCAInterfaceObject *p_cur_data, _MOCAInterfaceObject *p_new_data);


/*Device.MoCA.Interface.0.Stats*/
typedef struct st_MOCAInterfaceStatObject_obj {
	 tsl_uint_t bytesSent;
	 tsl_uint_t bytesReceived;
	 tsl_uint_t packetsSent;
	 tsl_uint_t packetsReceived;
	 tsl_uint_t errorsSent;
	 tsl_uint_t errorsReceived;
	 tsl_uint_t unicastPacketsSent;
	 tsl_uint_t unicastPacketsReceived;
	 tsl_uint_t discardPacketsSent;
	 tsl_uint_t discardPacketsReceived;
	 tsl_uint_t multicastPacketsSent;
	 tsl_uint_t multicastPacketsReceived;
	 tsl_uint_t broadcastPacketsSent;
	 tsl_uint_t broadcastPacketsReceived;
	 tsl_uint_t unknownProtoPacketsReceived;
}MOCAInterfaceStatObject;
typedef MOCAInterfaceStatObject _MOCAInterfaceStatObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_MOCAInterfaceStatObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAInterfaceStatObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_MOCAInterfaceStatObject_value(tsl_char_t *p_oid_name, _MOCAInterfaceStatObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_MOCAInterfaceStatObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAInterfaceStatObject *p_cur_data
 *	        _MOCAInterfaceStatObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_MOCAInterfaceStatObject_value(tsl_char_t *p_oid_name, _MOCAInterfaceStatObject *p_cur_data, _MOCAInterfaceStatObject *p_new_data);


/*Device.MoCA.Interface.0.QoS*/
typedef struct st_MOCAQOSObject_obj {
	 tsl_uint_t egressNumFlows;
	 tsl_uint_t ingressNumFlows;
	 tsl_uint_t flowStatsNumberOfEntries;
}MOCAQOSObject;
typedef MOCAQOSObject _MOCAQOSObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_MOCAQOSObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface.i.QoS
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAQOSObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_MOCAQOSObject_value(tsl_char_t *p_oid_name, _MOCAQOSObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_MOCAQOSObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface.i.QoS
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAQOSObject *p_cur_data
 *	        _MOCAQOSObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_MOCAQOSObject_value(tsl_char_t *p_oid_name, _MOCAQOSObject *p_cur_data, _MOCAQOSObject *p_new_data);


/*Device.MoCA.Interface.0.QoS.FlowStats*/
typedef struct st_MOCAQOSFlowStatObject_obj {
	 tsl_uint_t flowID;
	 tsl_char_t *packetDA;
	 tsl_uint_t maxRate;
	 tsl_uint_t maxBurstSize;
	 tsl_uint_t leaseTime;
	 tsl_uint_t leaseTimeLeft;
	 tsl_uint_t flowPackets;
}MOCAQOSFlowStatObject;
typedef MOCAQOSFlowStatObject _MOCAQOSFlowStatObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_MOCAQOSFlowStatObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface.i.QoS.FlowStats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAQOSFlowStatObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_MOCAQOSFlowStatObject_value(tsl_char_t *p_oid_name, _MOCAQOSFlowStatObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_MOCAQOSFlowStatObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface.i.QoS.FlowStats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAQOSFlowStatObject *p_cur_data
 *	        _MOCAQOSFlowStatObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_MOCAQOSFlowStatObject_value(tsl_char_t *p_oid_name, _MOCAQOSFlowStatObject *p_cur_data, _MOCAQOSFlowStatObject *p_new_data);


/*Device.MoCA.Interface.0.AssociatedDevice*/
typedef struct st_MOCAAssociatedDevObject_obj {
	 tsl_char_t *MACAddress;
	 tsl_uint_t nodeID;
	 tsl_int_t preferredNC;
	 tsl_char_t *highestVersion;
	 tsl_uint_t PHYTxRate;
	 tsl_uint_t PHYRxRate;
	 tsl_uint_t txPowerControlReduction;
	 tsl_uint_t rxPowerLevel;
	 tsl_uint_t txBcastRate;
	 tsl_uint_t rxBcastPowerLevel;
	 tsl_uint_t txPackets;
	 tsl_uint_t rxPackets;
	 tsl_uint_t rxErroredAndMissedPackets;
	 tsl_int_t QAM256Capable;
	 tsl_uint_t packetAggregationCapability;
	 tsl_uint_t rxSNR;
	 tsl_int_t active;
}MOCAAssociatedDevObject;
typedef MOCAAssociatedDevObject _MOCAAssociatedDevObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_MOCAAssociatedDevObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface.i.AssociatedDevice
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAAssociatedDevObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_MOCAAssociatedDevObject_value(tsl_char_t *p_oid_name, _MOCAAssociatedDevObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_MOCAAssociatedDevObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.MoCA.Interface.i.AssociatedDevice
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MOCAAssociatedDevObject *p_cur_data
 *	        _MOCAAssociatedDevObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_MOCAAssociatedDevObject_value(tsl_char_t *p_oid_name, _MOCAAssociatedDevObject *p_cur_data, _MOCAAssociatedDevObject *p_new_data);


/*Device.WiFi*/
typedef struct st_WIFIObject_obj {
	 tsl_int_t X_ACTIONTEC_COM_WiFiRestoreDefault;
	 tsl_uint_t radioNumberOfEntries;
	 tsl_uint_t SSIDNumberOfEntries;
	 tsl_uint_t accessPointNumberOfEntries;
}WIFIObject;
typedef WIFIObject _WIFIObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFIObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFIObject_value(tsl_char_t *p_oid_name, _WIFIObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFIObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIObject *p_cur_data
 *	        _WIFIObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFIObject_value(tsl_char_t *p_oid_name, _WIFIObject *p_cur_data, _WIFIObject *p_new_data);


/*Device.WiFi.Radio*/
typedef struct st_WIFIRadioObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_uint_t lastChange;
	 tsl_char_t *lowerLayers;
	 tsl_int_t upstream;
	 tsl_char_t *supportedFrequencyBands;
	 tsl_char_t *operatingFrequencyBand;
	 tsl_char_t *supportedStandards;
	 tsl_char_t *operatingStandards;
	 tsl_char_t *possibleChannels;
	 tsl_char_t *channelsInUse;
	 tsl_uint_t channel;
	 tsl_int_t autoChannelSupported;
	 tsl_int_t autoChannelEnable;
	 tsl_int_t X_ACTIONTEC_COM_AutoChannelRefresh;
	 tsl_char_t *operatingChannelBandwidth;
	 tsl_char_t *extensionChannel;
	 tsl_char_t *guardInterval;
	 tsl_char_t *transmitPowerSupported;
	 tsl_int_t transmitPower;
	 tsl_int_t IEEE80211hSupported;
	 tsl_int_t IEEE80211hEnabled;
	 tsl_char_t *regulatoryDomain;
}WIFIRadioObject;
typedef WIFIRadioObject _WIFIRadioObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFIRadioObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.Radio
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIRadioObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFIRadioObject_value(tsl_char_t *p_oid_name, _WIFIRadioObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFIRadioObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.Radio
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIRadioObject *p_cur_data
 *	        _WIFIRadioObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFIRadioObject_value(tsl_char_t *p_oid_name, _WIFIRadioObject *p_cur_data, _WIFIRadioObject *p_new_data);


/*Device.WiFi.Radio.0.Stats*/
typedef struct st_WIFIRadioStatsObject_obj {
	 tsl_uint_t bytesSent;
	 tsl_uint_t bytesReceived;
	 tsl_uint_t packetsSent;
	 tsl_uint_t packetsReceived;
	 tsl_uint_t errorsSent;
	 tsl_uint_t errorsReceived;
	 tsl_uint_t discardPacketsSent;
	 tsl_uint_t discardPacketsReceived;
	 tsl_uint_t PLCPErrorCount;
	 tsl_uint_t FCSErrorCount;
	 tsl_uint_t invalidMACCount;
	 tsl_uint_t packetsOtherReceived;
}WIFIRadioStatsObject;
typedef WIFIRadioStatsObject _WIFIRadioStatsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFIRadioStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.Radio.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIRadioStatsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFIRadioStatsObject_value(tsl_char_t *p_oid_name, _WIFIRadioStatsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFIRadioStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.Radio.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIRadioStatsObject *p_cur_data
 *	        _WIFIRadioStatsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFIRadioStatsObject_value(tsl_char_t *p_oid_name, _WIFIRadioStatsObject *p_cur_data, _WIFIRadioStatsObject *p_new_data);


/*Device.WiFi.SSID*/
typedef struct st_WIFISSIDObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_uint_t lastChange;
	 tsl_char_t *lowerLayers;
	 tsl_char_t *BSSID;
	 tsl_char_t *MACAddress;
	 tsl_char_t *SSID;
	 tsl_int_t X_ACTIONTEC_COM_InhibitSSIDControl;
	 tsl_int_t X_ACTIONTEC_COM_GuiSSIDConfig;
}WIFISSIDObject;
typedef WIFISSIDObject _WIFISSIDObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFISSIDObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.SSID
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFISSIDObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFISSIDObject_value(tsl_char_t *p_oid_name, _WIFISSIDObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFISSIDObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.SSID
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFISSIDObject *p_cur_data
 *	        _WIFISSIDObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFISSIDObject_value(tsl_char_t *p_oid_name, _WIFISSIDObject *p_cur_data, _WIFISSIDObject *p_new_data);


/*Device.WiFi.SSID.0.Stats*/
typedef struct st_WIFISSIDStatsObject_obj {
	 tsl_uint_t bytesSent;
	 tsl_uint_t bytesReceived;
	 tsl_uint_t packetsSent;
	 tsl_uint_t packetsReceived;
	 tsl_uint_t errorsSent;
	 tsl_uint_t retransCount;
	 tsl_uint_t failedRetransCount;
	 tsl_uint_t retryCount;
	 tsl_uint_t multipleRetryCount;
	 tsl_uint_t ACKFailureCount;
	 tsl_uint_t aggregatedPacketCount;
	 tsl_uint_t errorsReceived;
	 tsl_uint_t unicastPacketsSent;
	 tsl_uint_t unicastPacketsReceived;
	 tsl_uint_t discardPacketsSent;
	 tsl_uint_t discardPacketsReceived;
	 tsl_uint_t multicastPacketsSent;
	 tsl_uint_t multicastPacketsReceived;
	 tsl_uint_t broadcastPacketsSent;
	 tsl_uint_t broadcastPacketsReceived;
	 tsl_uint_t unknownProtoPacketsReceived;
}WIFISSIDStatsObject;
typedef WIFISSIDStatsObject _WIFISSIDStatsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFISSIDStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.SSID.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFISSIDStatsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFISSIDStatsObject_value(tsl_char_t *p_oid_name, _WIFISSIDStatsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFISSIDStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.SSID.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFISSIDStatsObject *p_cur_data
 *	        _WIFISSIDStatsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFISSIDStatsObject_value(tsl_char_t *p_oid_name, _WIFISSIDStatsObject *p_cur_data, _WIFISSIDStatsObject *p_new_data);


/*Device.WiFi.AccessPoint*/
typedef struct st_WIFIAccessPointObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *SSIDReference;
	 tsl_int_t SSIDAdvertisementEnabled;
	 tsl_int_t WMMCapability;
	 tsl_int_t UAPSDCapability;
	 tsl_int_t WMMEnable;
	 tsl_int_t UAPSDEnable;
	 tsl_uint_t associatedDeviceNumberOfEntries;
}WIFIAccessPointObject;
typedef WIFIAccessPointObject _WIFIAccessPointObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFIAccessPointObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.AccessPoint
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIAccessPointObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFIAccessPointObject_value(tsl_char_t *p_oid_name, _WIFIAccessPointObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFIAccessPointObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.AccessPoint
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIAccessPointObject *p_cur_data
 *	        _WIFIAccessPointObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFIAccessPointObject_value(tsl_char_t *p_oid_name, _WIFIAccessPointObject *p_cur_data, _WIFIAccessPointObject *p_new_data);


/*Device.WiFi.AccessPoint.0.Security*/
typedef struct st_WIFIAccessPointSecurityObject_obj {
	 tsl_char_t *modesSupported;
	 tsl_char_t *modeEnabled;
	 tsl_char_t *WEPKey;
	 tsl_char_t *preSharedKey;
	 tsl_char_t *keyPassphrase;
	 tsl_uint_t rekeyingInterval;
	 tsl_char_t *X_ACTIONTEC_COM_WPAKey;
	 tsl_char_t *X_TWC_COM_PreSharedKey;
	 tsl_char_t *X_TWC_COM_KeyPassPhrase;
	 tsl_char_t *X_TWC_COM_PreSharedKeySHA1;
	 tsl_char_t *X_TWC_COM_KeyPassPhraseSHA1;
	 tsl_char_t *radiusServerIPAddr;
	 tsl_uint_t radiusServerPort;
	 tsl_char_t *radiusSecret;
	 tsl_int_t X_ACTIONTEC_COM_RadiusEnabled;
	 tsl_char_t *X_ACTIONTEC_COM_WEPAuthenticationMode;
	 tsl_char_t *X_ACTIONTEC_COM_WPAEncryptionMode;
	 tsl_char_t *X_ACTIONTEC_COM_WPA2EncryptionMode;
}WIFIAccessPointSecurityObject;
typedef WIFIAccessPointSecurityObject _WIFIAccessPointSecurityObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFIAccessPointSecurityObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.AccessPoint.i.Security
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIAccessPointSecurityObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFIAccessPointSecurityObject_value(tsl_char_t *p_oid_name, _WIFIAccessPointSecurityObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFIAccessPointSecurityObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.AccessPoint.i.Security
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIAccessPointSecurityObject *p_cur_data
 *	        _WIFIAccessPointSecurityObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFIAccessPointSecurityObject_value(tsl_char_t *p_oid_name, _WIFIAccessPointSecurityObject *p_cur_data, _WIFIAccessPointSecurityObject *p_new_data);


/*Device.WiFi.AccessPoint.0.WPS*/
typedef struct st_WIFIAccessPointWpsObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *configMethodsSupported;
	 tsl_char_t *configMethodsEnabled;
}WIFIAccessPointWpsObject;
typedef WIFIAccessPointWpsObject _WIFIAccessPointWpsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFIAccessPointWpsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.AccessPoint.i.WPS
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIAccessPointWpsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFIAccessPointWpsObject_value(tsl_char_t *p_oid_name, _WIFIAccessPointWpsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFIAccessPointWpsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.AccessPoint.i.WPS
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIAccessPointWpsObject *p_cur_data
 *	        _WIFIAccessPointWpsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFIAccessPointWpsObject_value(tsl_char_t *p_oid_name, _WIFIAccessPointWpsObject *p_cur_data, _WIFIAccessPointWpsObject *p_new_data);


/*Device.WiFi.AccessPoint.0.AssociatedDevice*/
typedef struct st_WIFIAccessPointAssociatedDeviceObject_obj {
	 tsl_char_t *MACAddress;
	 tsl_int_t authenticationState;
	 tsl_uint_t lastDataDownlinkRate;
	 tsl_uint_t lastDataUplinkRate;
	 tsl_int_t signalStrength;
	 tsl_int_t active;
	 tsl_uint_t X_COMCAST_COM_LinkQuality;
}WIFIAccessPointAssociatedDeviceObject;
typedef WIFIAccessPointAssociatedDeviceObject _WIFIAccessPointAssociatedDeviceObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WIFIAccessPointAssociatedDeviceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.AccessPoint.i.AssociatedDevice
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIAccessPointAssociatedDeviceObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WIFIAccessPointAssociatedDeviceObject_value(tsl_char_t *p_oid_name, _WIFIAccessPointAssociatedDeviceObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WIFIAccessPointAssociatedDeviceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.WiFi.AccessPoint.i.AssociatedDevice
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WIFIAccessPointAssociatedDeviceObject *p_cur_data
 *	        _WIFIAccessPointAssociatedDeviceObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WIFIAccessPointAssociatedDeviceObject_value(tsl_char_t *p_oid_name, _WIFIAccessPointAssociatedDeviceObject *p_cur_data, _WIFIAccessPointAssociatedDeviceObject *p_new_data);


/*Device.Bridging*/
typedef struct st_BridgingObject_obj {
	 tsl_uint_t maxBridgeEntries;
	 tsl_uint_t maxDBridgeEntries;
	 tsl_uint_t maxQBridgeEntries;
	 tsl_uint_t maxVLANEntries;
	 tsl_uint_t maxFilterEntries;
	 tsl_uint_t bridgeNumberOfEntries;
	 tsl_uint_t filterNumberOfEntries;
}BridgingObject;
typedef BridgingObject _BridgingObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_BridgingObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_BridgingObject_value(tsl_char_t *p_oid_name, _BridgingObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_BridgingObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingObject *p_cur_data
 *	        _BridgingObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_BridgingObject_value(tsl_char_t *p_oid_name, _BridgingObject *p_cur_data, _BridgingObject *p_new_data);


/*Device.Bridging.Bridge*/
typedef struct st_BridgingBridgeObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *standard;
	 tsl_uint_t portNumberOfEntries;
	 tsl_uint_t VLANNumberOfEntries;
	 tsl_uint_t VLANPortNumberOfEntries;
}BridgingBridgeObject;
typedef BridgingBridgeObject _BridgingBridgeObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_BridgingBridgeObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgeObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_BridgingBridgeObject_value(tsl_char_t *p_oid_name, _BridgingBridgeObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_BridgingBridgeObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgeObject *p_cur_data
 *	        _BridgingBridgeObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_BridgingBridgeObject_value(tsl_char_t *p_oid_name, _BridgingBridgeObject *p_cur_data, _BridgingBridgeObject *p_new_data);


/*Device.Bridging.Bridge.0.Port*/
typedef struct st_BridgingBridgePortObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_uint_t lastChange;
	 tsl_uint_t X_ACTIONTEC_COM_LastChangeTime;
	 tsl_char_t *lowerLayers;
	 tsl_int_t managementPort;
	 tsl_uint_t defaultUserPriority;
	 tsl_char_t *priorityRegeneration;
	 tsl_char_t *portState;
	 tsl_uint_t PVID;
	 tsl_char_t *acceptableFrameTypes;
	 tsl_int_t ingressFiltering;
	 tsl_int_t priorityTagging;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
	 tsl_char_t *X_ACTIONTEC_COM_OldName;
}BridgingBridgePortObject;
typedef BridgingBridgePortObject _BridgingBridgePortObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_BridgingBridgePortObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge.i.Port
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgePortObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_BridgingBridgePortObject_value(tsl_char_t *p_oid_name, _BridgingBridgePortObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_BridgingBridgePortObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge.i.Port
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgePortObject *p_cur_data
 *	        _BridgingBridgePortObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_BridgingBridgePortObject_value(tsl_char_t *p_oid_name, _BridgingBridgePortObject *p_cur_data, _BridgingBridgePortObject *p_new_data);


/*Device.Bridging.Bridge.0.Port.0.Stats*/
typedef struct st_BridgingBridgePortStatsObject_obj {
	 tsl_uint_t bytesSent;
	 tsl_uint_t bytesReceived;
	 tsl_uint_t packetsSent;
	 tsl_uint_t packetsReceived;
	 tsl_uint_t errorsSent;
	 tsl_uint_t errorsReceived;
	 tsl_uint_t unicastPacketsSent;
	 tsl_uint_t unicastPacketsReceived;
	 tsl_uint_t discardPacketsSent;
	 tsl_uint_t discardPacketsReceived;
	 tsl_uint_t multicastPacketsSent;
	 tsl_uint_t multicastPacketsReceived;
	 tsl_uint_t broadcastPacketsSent;
	 tsl_uint_t broadcastPacketsReceived;
	 tsl_uint_t unknownProtoPacketsReceived;
}BridgingBridgePortStatsObject;
typedef BridgingBridgePortStatsObject _BridgingBridgePortStatsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_BridgingBridgePortStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge.i.Port.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgePortStatsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_BridgingBridgePortStatsObject_value(tsl_char_t *p_oid_name, _BridgingBridgePortStatsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_BridgingBridgePortStatsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge.i.Port.i.Stats
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgePortStatsObject *p_cur_data
 *	        _BridgingBridgePortStatsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_BridgingBridgePortStatsObject_value(tsl_char_t *p_oid_name, _BridgingBridgePortStatsObject *p_cur_data, _BridgingBridgePortStatsObject *p_new_data);


/*Device.Bridging.Bridge.0.VLAN*/
typedef struct st_BridgingBridgeVlanObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_uint_t VLANID;
}BridgingBridgeVlanObject;
typedef BridgingBridgeVlanObject _BridgingBridgeVlanObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_BridgingBridgeVlanObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge.i.VLAN
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgeVlanObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_BridgingBridgeVlanObject_value(tsl_char_t *p_oid_name, _BridgingBridgeVlanObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_BridgingBridgeVlanObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge.i.VLAN
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgeVlanObject *p_cur_data
 *	        _BridgingBridgeVlanObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_BridgingBridgeVlanObject_value(tsl_char_t *p_oid_name, _BridgingBridgeVlanObject *p_cur_data, _BridgingBridgeVlanObject *p_new_data);


/*Device.Bridging.Bridge.0.VLANPort*/
typedef struct st_BridgingBridgeVlanPortObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *alias;
	 tsl_char_t *VLAN;
	 tsl_char_t *port;
	 tsl_int_t untagged;
}BridgingBridgeVlanPortObject;
typedef BridgingBridgeVlanPortObject _BridgingBridgeVlanPortObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_BridgingBridgeVlanPortObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge.i.VLANPort
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgeVlanPortObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_BridgingBridgeVlanPortObject_value(tsl_char_t *p_oid_name, _BridgingBridgeVlanPortObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_BridgingBridgeVlanPortObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Bridging.Bridge.i.VLANPort
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _BridgingBridgeVlanPortObject *p_cur_data
 *	        _BridgingBridgeVlanPortObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_BridgingBridgeVlanPortObject_value(tsl_char_t *p_oid_name, _BridgingBridgeVlanPortObject *p_cur_data, _BridgingBridgeVlanPortObject *p_new_data);


/*Device.IP*/
typedef struct st_IpObject_obj {
	 tsl_int_t IPv4Enable;
	 tsl_char_t *IPv4Status;
	 tsl_uint_t interfaceNumberOfEntries;
}IpObject;
typedef IpObject _IpObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_IpObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.IP
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _IpObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_IpObject_value(tsl_char_t *p_oid_name, _IpObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_IpObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.IP
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _IpObject *p_cur_data
 *	        _IpObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_IpObject_value(tsl_char_t *p_oid_name, _IpObject *p_cur_data, _IpObject *p_new_data);


/*Device.IP.Interface*/
typedef struct st_IpInterfaceObject_obj {
	 tsl_int_t enable;
	 tsl_int_t IPv4Enable;
	 tsl_int_t IPv6Enable;
	 tsl_int_t ULAEnable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *name;
	 tsl_uint_t lastChange;
	 tsl_uint_t X_ACTIONTEC_COM_LastChangeTime;
	 tsl_char_t *lowerLayers;
	 tsl_int_t reset;
	 tsl_char_t *type;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
	 tsl_char_t *X_ACTIONTEC_COM_InternetStatus;
	 tsl_uint_t IPv4AddressNumberOfEntries;
	 tsl_uint_t IPv6AddressNumberOfEntries;
}IpInterfaceObject;
typedef IpInterfaceObject _IpInterfaceObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_IpInterfaceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.IP.Interface
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _IpInterfaceObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_IpInterfaceObject_value(tsl_char_t *p_oid_name, _IpInterfaceObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_IpInterfaceObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.IP.Interface
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _IpInterfaceObject *p_cur_data
 *	        _IpInterfaceObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_IpInterfaceObject_value(tsl_char_t *p_oid_name, _IpInterfaceObject *p_cur_data, _IpInterfaceObject *p_new_data);


/*Device.IP.Interface.0.IPv4Address*/
typedef struct st_IpInterfaceIpv4AddressObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *IPAddress;
	 tsl_char_t *subnetMask;
	 tsl_char_t *addressingType;
}IpInterfaceIpv4AddressObject;
typedef IpInterfaceIpv4AddressObject _IpInterfaceIpv4AddressObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_IpInterfaceIpv4AddressObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.IP.Interface.i.IPv4Address
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _IpInterfaceIpv4AddressObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_IpInterfaceIpv4AddressObject_value(tsl_char_t *p_oid_name, _IpInterfaceIpv4AddressObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_IpInterfaceIpv4AddressObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.IP.Interface.i.IPv4Address
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _IpInterfaceIpv4AddressObject *p_cur_data
 *	        _IpInterfaceIpv4AddressObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_IpInterfaceIpv4AddressObject_value(tsl_char_t *p_oid_name, _IpInterfaceIpv4AddressObject *p_cur_data, _IpInterfaceIpv4AddressObject *p_new_data);


/*Device.IP.Interface.0.IPv6Address*/
typedef struct st_IpInterfaceIpv6AddressObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *alias;
	 tsl_char_t *IPAddressStatus;
	 tsl_char_t *IPAddress;
	 tsl_char_t *origin;
}IpInterfaceIpv6AddressObject;
typedef IpInterfaceIpv6AddressObject _IpInterfaceIpv6AddressObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_IpInterfaceIpv6AddressObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.IP.Interface.i.IPv6Address
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _IpInterfaceIpv6AddressObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_IpInterfaceIpv6AddressObject_value(tsl_char_t *p_oid_name, _IpInterfaceIpv6AddressObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_IpInterfaceIpv6AddressObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.IP.Interface.i.IPv6Address
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _IpInterfaceIpv6AddressObject *p_cur_data
 *	        _IpInterfaceIpv6AddressObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_IpInterfaceIpv6AddressObject_value(tsl_char_t *p_oid_name, _IpInterfaceIpv6AddressObject *p_cur_data, _IpInterfaceIpv6AddressObject *p_new_data);


/*Device.Hosts*/
typedef struct st_HostsObject_obj {
	 tsl_uint_t hostNumberOfEntries;
}HostsObject;
typedef HostsObject _HostsObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_HostsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Hosts
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _HostsObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_HostsObject_value(tsl_char_t *p_oid_name, _HostsObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_HostsObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Hosts
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _HostsObject *p_cur_data
 *	        _HostsObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_HostsObject_value(tsl_char_t *p_oid_name, _HostsObject *p_cur_data, _HostsObject *p_new_data);


/*Device.Hosts.Host*/
typedef struct st_HostsHostObject_obj {
	 tsl_char_t *alias;
	 tsl_char_t *physAddress;
	 tsl_char_t *IPAddress;
	 tsl_char_t *DHCPClient;
	 tsl_char_t *associatedDevice;
	 tsl_char_t *layer1Interface;
	 tsl_char_t *layer3Interface;
	 tsl_char_t *hostName;
	 tsl_int_t active;
	 tsl_char_t *X_ACTIONTEC_COM_InterfaceType;
	 tsl_uint_t IPv4AddressNumberOfEntries;
	 tsl_uint_t IPv6AddressNumberOfEntries;
}HostsHostObject;
typedef HostsHostObject _HostsHostObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_HostsHostObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Hosts.Host
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _HostsHostObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_HostsHostObject_value(tsl_char_t *p_oid_name, _HostsHostObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_HostsHostObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Hosts.Host
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _HostsHostObject *p_cur_data
 *	        _HostsHostObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_HostsHostObject_value(tsl_char_t *p_oid_name, _HostsHostObject *p_cur_data, _HostsHostObject *p_new_data);


/*Device.Hosts.Host.0.IPv4Address*/
typedef struct st_HostsHostIpv4AddressObject_obj {
	 tsl_char_t *IPAddress;
}HostsHostIpv4AddressObject;
typedef HostsHostIpv4AddressObject _HostsHostIpv4AddressObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_HostsHostIpv4AddressObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Hosts.Host.i.IPv4Address
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _HostsHostIpv4AddressObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_HostsHostIpv4AddressObject_value(tsl_char_t *p_oid_name, _HostsHostIpv4AddressObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_HostsHostIpv4AddressObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Hosts.Host.i.IPv4Address
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _HostsHostIpv4AddressObject *p_cur_data
 *	        _HostsHostIpv4AddressObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_HostsHostIpv4AddressObject_value(tsl_char_t *p_oid_name, _HostsHostIpv4AddressObject *p_cur_data, _HostsHostIpv4AddressObject *p_new_data);


/*Device.Hosts.Host.0.IPv6Address*/
typedef struct st_HostsHostIpv6AddressObject_obj {
	 tsl_char_t *IPAddress;
}HostsHostIpv6AddressObject;
typedef HostsHostIpv6AddressObject _HostsHostIpv6AddressObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_HostsHostIpv6AddressObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Hosts.Host.i.IPv6Address
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _HostsHostIpv6AddressObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_HostsHostIpv6AddressObject_value(tsl_char_t *p_oid_name, _HostsHostIpv6AddressObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_HostsHostIpv6AddressObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Hosts.Host.i.IPv6Address
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _HostsHostIpv6AddressObject *p_cur_data
 *	        _HostsHostIpv6AddressObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_HostsHostIpv6AddressObject_value(tsl_char_t *p_oid_name, _HostsHostIpv6AddressObject *p_cur_data, _HostsHostIpv6AddressObject *p_new_data);


/*Device.DHCPv4*/
typedef struct st_Dhcpv4Object_obj {
	 tsl_uint_t clientNumberOfEntries;
}Dhcpv4Object;
typedef Dhcpv4Object _Dhcpv4Object;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_Dhcpv4Object_value
 *
 *	[DESCRIPTION]:
 *	        Device.DHCPv4
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _Dhcpv4Object *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_Dhcpv4Object_value(tsl_char_t *p_oid_name, _Dhcpv4Object *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_Dhcpv4Object_value
 *
 *	[DESCRIPTION]:
 *	        Device.DHCPv4
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _Dhcpv4Object *p_cur_data
 *	        _Dhcpv4Object *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_Dhcpv4Object_value(tsl_char_t *p_oid_name, _Dhcpv4Object *p_cur_data, _Dhcpv4Object *p_new_data);


/*Device.DHCPv4.Client*/
typedef struct st_Dhcpv4ClientObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *alias;
	 tsl_char_t *interface;
	 tsl_char_t *status;
	 tsl_char_t *DHCPStatus;
	 tsl_int_t renew;
	 tsl_char_t *IPAddress;
	 tsl_char_t *subnetMask;
	 tsl_char_t *IPRouters;
	 tsl_char_t *DNSServers;
	 tsl_int_t leaseTimeRemaining;
	 tsl_char_t *DHCPServer;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
	 tsl_char_t *X_ACTIONTEC_COM_DefaultIPAddress;
	 tsl_char_t *X_ACTIONTEC_COM_DefaultSubnetMask;
	 tsl_char_t *X_ACTIONTEC_COM_DefaultIPRouters;
	 tsl_int_t X_ACTIONTEC_COM_LeaseTime;
	 tsl_uint_t X_ACTIONTEC_COM_LastChangeTime;
}Dhcpv4ClientObject;
typedef Dhcpv4ClientObject _Dhcpv4ClientObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_Dhcpv4ClientObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DHCPv4.Client
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _Dhcpv4ClientObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_Dhcpv4ClientObject_value(tsl_char_t *p_oid_name, _Dhcpv4ClientObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_Dhcpv4ClientObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DHCPv4.Client
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _Dhcpv4ClientObject *p_cur_data
 *	        _Dhcpv4ClientObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_Dhcpv4ClientObject_value(tsl_char_t *p_oid_name, _Dhcpv4ClientObject *p_cur_data, _Dhcpv4ClientObject *p_new_data);


/*Device.DHCPv4.Server*/
typedef struct st_Dhcpv4ServerObject_obj {
	 tsl_uint_t poolNumberOfEntries;
	 tsl_int_t enable;
}Dhcpv4ServerObject;
typedef Dhcpv4ServerObject _Dhcpv4ServerObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_Dhcpv4ServerObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DHCPv4.Server
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _Dhcpv4ServerObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_Dhcpv4ServerObject_value(tsl_char_t *p_oid_name, _Dhcpv4ServerObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_Dhcpv4ServerObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DHCPv4.Server
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _Dhcpv4ServerObject *p_cur_data
 *	        _Dhcpv4ServerObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_Dhcpv4ServerObject_value(tsl_char_t *p_oid_name, _Dhcpv4ServerObject *p_cur_data, _Dhcpv4ServerObject *p_new_data);


/*Device.DHCPv4.Server.Pool*/
typedef struct st_Dhcpv4ServerPoolObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *status;
	 tsl_char_t *interface;
	 tsl_char_t *minAddress;
	 tsl_char_t *maxAddress;
	 tsl_char_t *subnetMask;
	 tsl_char_t *DNSServers;
	 tsl_int_t leaseTime;
}Dhcpv4ServerPoolObject;
typedef Dhcpv4ServerPoolObject _Dhcpv4ServerPoolObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_Dhcpv4ServerPoolObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DHCPv4.Server.Pool
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _Dhcpv4ServerPoolObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_Dhcpv4ServerPoolObject_value(tsl_char_t *p_oid_name, _Dhcpv4ServerPoolObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_Dhcpv4ServerPoolObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.DHCPv4.Server.Pool
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _Dhcpv4ServerPoolObject *p_cur_data
 *	        _Dhcpv4ServerPoolObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_Dhcpv4ServerPoolObject_value(tsl_char_t *p_oid_name, _Dhcpv4ServerPoolObject *p_cur_data, _Dhcpv4ServerPoolObject *p_new_data);


/*Device.Users*/
typedef struct st_UsersObject_obj {
	 tsl_uint_t userNumberOfEntries;
}UsersObject;
typedef UsersObject _UsersObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_UsersObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Users
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _UsersObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_UsersObject_value(tsl_char_t *p_oid_name, _UsersObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_UsersObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Users
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _UsersObject *p_cur_data
 *	        _UsersObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_UsersObject_value(tsl_char_t *p_oid_name, _UsersObject *p_cur_data, _UsersObject *p_new_data);


/*Device.Users.User*/
typedef struct st_UsersUserObject_obj {
	 tsl_char_t *alias;
	 tsl_int_t enable;
	 tsl_int_t remoteAccessCapable;
	 tsl_char_t *username;
	 tsl_char_t *password;
	 tsl_char_t *language;
	 tsl_uint_t X_ACTIONTEC_COM_RandomRootPasswordEnable;
	 tsl_char_t *X_ACTIONTEC_COM_RandomRootPassword;
	 tsl_uint_t X_ACTIONTEC_COM_AuthLevel;
}UsersUserObject;
typedef UsersUserObject _UsersUserObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_UsersUserObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Users.User
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _UsersUserObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_UsersUserObject_value(tsl_char_t *p_oid_name, _UsersUserObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_UsersUserObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Users.User
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _UsersUserObject *p_cur_data
 *	        _UsersUserObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_UsersUserObject_value(tsl_char_t *p_oid_name, _UsersUserObject *p_cur_data, _UsersUserObject *p_new_data);


/*Device.Firewall*/
typedef struct st_FirewallObject_obj {
	 tsl_int_t enable;
}FirewallObject;
typedef FirewallObject _FirewallObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_FirewallObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Firewall
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _FirewallObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_FirewallObject_value(tsl_char_t *p_oid_name, _FirewallObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_FirewallObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.Firewall
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _FirewallObject *p_cur_data
 *	        _FirewallObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_FirewallObject_value(tsl_char_t *p_oid_name, _FirewallObject *p_cur_data, _FirewallObject *p_new_data);


/*Device.GRETunnel*/
typedef struct st_GreTunnelObject_obj {
	 tsl_char_t *X_ACTIONTEC_COM_GRETunnelRemoteIPv4Address;
	 tsl_char_t *X_ACTIONTEC_COM_GRETunnelLocalIPv4Address;
	 tsl_char_t *X_ACTIONTEC_COM_TunnelInterface;
	 tsl_int_t enable;
}GreTunnelObject;
typedef GreTunnelObject _GreTunnelObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_GreTunnelObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.GRETunnel
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _GreTunnelObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_GreTunnelObject_value(tsl_char_t *p_oid_name, _GreTunnelObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_GreTunnelObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.GRETunnel
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _GreTunnelObject *p_cur_data
 *	        _GreTunnelObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_GreTunnelObject_value(tsl_char_t *p_oid_name, _GreTunnelObject *p_cur_data, _GreTunnelObject *p_new_data);


/*Device.X_ACTIONTEC_COM_ZeroConf*/
typedef struct st_ZeroConfObject_obj {
}ZeroConfObject;
typedef ZeroConfObject _ZeroConfObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_ZeroConfObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_ZeroConf
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _ZeroConfObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_ZeroConfObject_value(tsl_char_t *p_oid_name, _ZeroConfObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_ZeroConfObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_ZeroConf
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _ZeroConfObject *p_cur_data
 *	        _ZeroConfObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_ZeroConfObject_value(tsl_char_t *p_oid_name, _ZeroConfObject *p_cur_data, _ZeroConfObject *p_new_data);


/*Device.X_ACTIONTEC_COM_ZeroConf.Extender*/
typedef struct st_ZeroConfExtenderObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *X_ACTIONTEC_COM_RouterType;
	 tsl_char_t *interface;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
	 tsl_char_t *status;
	 tsl_char_t *specVersion;
	 tsl_int_t periodicPullingEnable;
	 tsl_uint_t periodicPullingInterval;
	 tsl_uint_t lastChange;
	 tsl_uint_t X_ACTIONTEC_COM_LastChangeTime;
	 tsl_char_t *logLevel;
}ZeroConfExtenderObject;
typedef ZeroConfExtenderObject _ZeroConfExtenderObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_ZeroConfExtenderObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_ZeroConf.Extender
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _ZeroConfExtenderObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_ZeroConfExtenderObject_value(tsl_char_t *p_oid_name, _ZeroConfExtenderObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_ZeroConfExtenderObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_ZeroConf.Extender
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _ZeroConfExtenderObject *p_cur_data
 *	        _ZeroConfExtenderObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_ZeroConfExtenderObject_value(tsl_char_t *p_oid_name, _ZeroConfExtenderObject *p_cur_data, _ZeroConfExtenderObject *p_new_data);


/*Device.X_ACTIONTEC_COM_LED*/
typedef struct st_LedObject_obj {
	 tsl_char_t *LED_MoCA_Status;
	 tsl_char_t *LED_WiFi2G_Status;
	 tsl_char_t *LED_WiFi5G_Status;
}LedObject;
typedef LedObject _LedObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_LedObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_LED
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _LedObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_LedObject_value(tsl_char_t *p_oid_name, _LedObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_LedObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_LED
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _LedObject *p_cur_data
 *	        _LedObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_LedObject_value(tsl_char_t *p_oid_name, _LedObject *p_cur_data, _LedObject *p_new_data);


/*Device.X_ACTIONTEC_COM_RemoteLogin*/
typedef struct st_RemoteLoginObject_obj {
	 tsl_int_t enable;
	 tsl_uint_t port;
	 tsl_char_t *supportedProtocols;
	 tsl_char_t *protocol;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
	 tsl_uint_t idleTimeout;
	 tsl_char_t *username;
	 tsl_char_t *password;
}RemoteLoginObject;
typedef RemoteLoginObject _RemoteLoginObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_RemoteLoginObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_RemoteLogin
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _RemoteLoginObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_RemoteLoginObject_value(tsl_char_t *p_oid_name, _RemoteLoginObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_RemoteLoginObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_RemoteLogin
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _RemoteLoginObject *p_cur_data
 *	        _RemoteLoginObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_RemoteLoginObject_value(tsl_char_t *p_oid_name, _RemoteLoginObject *p_cur_data, _RemoteLoginObject *p_new_data);


/*Device.X_ACTIONTEC_COM_DNSServer*/
typedef struct st_DNSServerObject_obj {
	 tsl_int_t enable;
}DNSServerObject;
typedef DNSServerObject _DNSServerObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_DNSServerObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_DNSServer
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _DNSServerObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_DNSServerObject_value(tsl_char_t *p_oid_name, _DNSServerObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_DNSServerObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_DNSServer
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _DNSServerObject *p_cur_data
 *	        _DNSServerObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_DNSServerObject_value(tsl_char_t *p_oid_name, _DNSServerObject *p_cur_data, _DNSServerObject *p_new_data);


/*Device.X_ACTIONTEC_COM_MDNS*/
typedef struct st_MDNSObject_obj {
	 tsl_int_t enable;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
}MDNSObject;
typedef MDNSObject _MDNSObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_MDNSObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_MDNS
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MDNSObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_MDNSObject_value(tsl_char_t *p_oid_name, _MDNSObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_MDNSObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_MDNS
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _MDNSObject *p_cur_data
 *	        _MDNSObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_MDNSObject_value(tsl_char_t *p_oid_name, _MDNSObject *p_cur_data, _MDNSObject *p_new_data);


/*Device.X_ACTIONTEC_COM_NETBIOS*/
typedef struct st_NETBIOSObject_obj {
	 tsl_int_t enable;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
}NETBIOSObject;
typedef NETBIOSObject _NETBIOSObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_NETBIOSObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_NETBIOS
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _NETBIOSObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_NETBIOSObject_value(tsl_char_t *p_oid_name, _NETBIOSObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_NETBIOSObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_COM_NETBIOS
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _NETBIOSObject *p_cur_data
 *	        _NETBIOSObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_NETBIOSObject_value(tsl_char_t *p_oid_name, _NETBIOSObject *p_cur_data, _NETBIOSObject *p_new_data);


/*Device.X_ACTIONTEC_Upgrade*/
typedef struct st_FirwareUpgradeObject_obj {
}FirwareUpgradeObject;
typedef FirwareUpgradeObject _FirwareUpgradeObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_FirwareUpgradeObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_Upgrade
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _FirwareUpgradeObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_FirwareUpgradeObject_value(tsl_char_t *p_oid_name, _FirwareUpgradeObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_FirwareUpgradeObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_Upgrade
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _FirwareUpgradeObject *p_cur_data
 *	        _FirwareUpgradeObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_FirwareUpgradeObject_value(tsl_char_t *p_oid_name, _FirwareUpgradeObject *p_cur_data, _FirwareUpgradeObject *p_new_data);


/*Device.X_ACTIONTEC_Upgrade.WanUpgradeStatus*/
typedef struct st_WanUpgradeStatusObject_obj {
	 tsl_uint_t nextCheckTime;
	 tsl_char_t *checkStatus;
	 tsl_char_t *internetVersion;
}WanUpgradeStatusObject;
typedef WanUpgradeStatusObject _WanUpgradeStatusObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WanUpgradeStatusObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_Upgrade.WanUpgradeStatus
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WanUpgradeStatusObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WanUpgradeStatusObject_value(tsl_char_t *p_oid_name, _WanUpgradeStatusObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WanUpgradeStatusObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_Upgrade.WanUpgradeStatus
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WanUpgradeStatusObject *p_cur_data
 *	        _WanUpgradeStatusObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WanUpgradeStatusObject_value(tsl_char_t *p_oid_name, _WanUpgradeStatusObject *p_cur_data, _WanUpgradeStatusObject *p_new_data);


/*Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig*/
typedef struct st_WanUpgradeObject_obj {
	 tsl_int_t enable;
	 tsl_int_t forceUpgrade;
	 tsl_uint_t checkInterval;
	 tsl_char_t *urlAddress;
	 tsl_char_t *verUrlAddress;
}WanUpgradeObject;
typedef WanUpgradeObject _WanUpgradeObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_WanUpgradeObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WanUpgradeObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_WanUpgradeObject_value(tsl_char_t *p_oid_name, _WanUpgradeObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_WanUpgradeObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _WanUpgradeObject *p_cur_data
 *	        _WanUpgradeObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_WanUpgradeObject_value(tsl_char_t *p_oid_name, _WanUpgradeObject *p_cur_data, _WanUpgradeObject *p_new_data);


/*Device.XMPP*/
typedef struct st_XMPPObject_obj {
	 tsl_uint_t connectionNumberOfEntries;
}XMPPObject;
typedef XMPPObject _XMPPObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_XMPPObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.XMPP
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _XMPPObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_XMPPObject_value(tsl_char_t *p_oid_name, _XMPPObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_XMPPObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.XMPP
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _XMPPObject *p_cur_data
 *	        _XMPPObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_XMPPObject_value(tsl_char_t *p_oid_name, _XMPPObject *p_cur_data, _XMPPObject *p_new_data);


/*Device.XMPP.Connection*/
typedef struct st_XMPPConnectionObject_obj {
	 tsl_int_t enable;
	 tsl_int_t X_ACTIONTEC_COM_Trigger;
	 tsl_char_t *alias;
	 tsl_char_t *username;
	 tsl_char_t *password;
	 tsl_char_t *domain;
	 tsl_char_t *resource;
	 tsl_char_t *jabberID;
	 tsl_char_t *status;
	 tsl_char_t *lastChangeDate;
	 tsl_char_t *serverConnectAlgorithm;
	 tsl_int_t keepAliveInterval;
	 tsl_uint_t serverConnectAttempts;
	 tsl_uint_t serverRetryInitialInterval;
	 tsl_uint_t serverRetryIntervalMultiplier;
	 tsl_uint_t serverRetryMaxInterval;
	 tsl_int_t useTLS;
	 tsl_int_t TLSEstablished;
	 tsl_uint_t serverNumberOfEntries;
}XMPPConnectionObject;
typedef XMPPConnectionObject _XMPPConnectionObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_XMPPConnectionObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.XMPP.Connection
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _XMPPConnectionObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_XMPPConnectionObject_value(tsl_char_t *p_oid_name, _XMPPConnectionObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_XMPPConnectionObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.XMPP.Connection
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _XMPPConnectionObject *p_cur_data
 *	        _XMPPConnectionObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_XMPPConnectionObject_value(tsl_char_t *p_oid_name, _XMPPConnectionObject *p_cur_data, _XMPPConnectionObject *p_new_data);


/*Device.XMPP.Connection.0.Server*/
typedef struct st_XMPPConnServerObject_obj {
	 tsl_int_t enable;
	 tsl_char_t *alias;
	 tsl_uint_t priority;
	 tsl_int_t weight;
	 tsl_char_t *serverAddress;
	 tsl_uint_t port;
}_XMPPConnServerObject;


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_get_XMPPConnServerObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.XMPP.Connection.i.Server
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _XMPPConnServerObject *p_cur_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_get_XMPPConnServerObject_value(tsl_char_t *p_oid_name, _XMPPConnServerObject *p_cur_data);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_func_set_XMPPConnServerObject_value
 *
 *	[DESCRIPTION]:
 *	        Device.XMPP.Connection.i.Server
 *
 *	[PARAMETER]:
 *	        tsl_char_t *p_oid_name
 *	        _XMPPConnServerObject *p_cur_data
 *	        _XMPPConnServerObject *p_new_data
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/

tsl_int_t tr69_func_set_XMPPConnServerObject_value(tsl_char_t *p_oid_name, _XMPPConnServerObject *p_cur_data, _XMPPConnServerObject *p_new_data);

#endif

