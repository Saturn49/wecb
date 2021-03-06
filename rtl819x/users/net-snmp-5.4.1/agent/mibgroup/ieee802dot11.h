/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.old-api.conf 17851 2009-11-30 16:46:06Z dts12 $
 */
#ifndef IEEE802DOT11_H
#define IEEE802DOT11_H

/*
 * function declarations 
 */
void            init_ieee802dot11(void);
FindVarMethod   var_ieee802dot11;
FindVarMethod   var_dot11StationConfigTable;
FindVarMethod   var_dot11AuthenticationAlgorithmsTable;
FindVarMethod   var_dot11WEPDefaultKeysTable;
FindVarMethod   var_dot11WEPKeyMappingsTable;
FindVarMethod   var_dot11PrivacyTable;
FindVarMethod   var_dot11MultiDomainCapabilityTable;
FindVarMethod   var_dot11WPAKeysTable;
FindVarMethod   var_dot11WPA2KeysTable;
FindVarMethod   var_dot11RadiusServerTable;
FindVarMethod   var_dot11OperationTable;
FindVarMethod   var_dot11CountersTable;
FindVarMethod   var_dot11GroupAddressesTable;
FindVarMethod   var_dot11ResourceInfoTable;
FindVarMethod   var_dot11PhyOperationTable;
FindVarMethod   var_dot11PhyAntennaTable;
FindVarMethod   var_dot11PhyTxPowerTable;
FindVarMethod   var_dot11PhyFHSSTable;
FindVarMethod   var_dot11PhyDSSSTable;
FindVarMethod   var_dot11PhyIRTable;
FindVarMethod   var_dot11RegDomainsSupportedTable;
FindVarMethod   var_dot11AntennasListTable;
FindVarMethod   var_dot11SupportedDataRatesTxTable;
FindVarMethod   var_dot11SupportedDataRatesRxTable;
FindVarMethod   var_dot11PhyOFDMTable;
FindVarMethod   var_dot11PhyHRDSSSTable;
FindVarMethod   var_dot11HoppingPatternTable;
WriteMethod     write_dot11StationID;
WriteMethod     write_dot11MediumOccupancyLimit;
WriteMethod     write_dot11CFPPeriod;
WriteMethod     write_dot11CFPMaxDuration;
WriteMethod     write_dot11AuthenticationResponseTimeOut;
WriteMethod     write_dot11PowerManagementMode;
WriteMethod     write_dot11DesiredSSID;
WriteMethod     write_dot11DesiredBSSType;
WriteMethod     write_dot11OperationalRateSet;
WriteMethod     write_dot11BeaconPeriod;
WriteMethod     write_dot11DTIMPeriod;
WriteMethod     write_dot11AssociationResponseTimeOut;
WriteMethod     write_dot11MultiDomainCapabilityImplemented;
WriteMethod     write_dot11MultiDomainCapabilityEnabled;
WriteMethod     write_dot11RegDomain;
WriteMethod     write_dot11DataRate;
WriteMethod     write_dot11ProtectionDisabled;
WriteMethod     write_dot11nSTBC;
WriteMethod     write_dot11nCoexist;
WriteMethod     write_dot11nUse40M;
WriteMethod     write_dot11n2ndChOffset;
WriteMethod     write_dot11nShortGIfor20M;
WriteMethod     write_dot11nShortGIfor40M;
WriteMethod     write_dot11nAMPDU;
WriteMethod     write_dot11nAMSDU;
WriteMethod     write_dot11AuthenticationAlgorithmsEnable;
WriteMethod     write_dot11WEPDefaultKeyValue;
WriteMethod     write_dot11WEPKeyMappingAddress;
WriteMethod     write_dot11WEPKeyMappingWEPOn;
WriteMethod     write_dot11WEPKeyMappingValue;
WriteMethod     write_dot11WEPKeyMappingStatus;
WriteMethod     write_dot11PrivacyInvoked;
WriteMethod     write_dot11WEPDefaultKeyID;
WriteMethod     write_dot11WEPKeyMappingLength;
WriteMethod     write_dot11ExcludeUnencrypted;
WriteMethod     write_dot11FirstChannelNumber;
WriteMethod     write_dot11NumberofChannels;
WriteMethod     write_dot11MaximumTransmitPowerLevel;
WriteMethod     write_dot11WPAKeysIndex;
WriteMethod     write_dot11WPAKeysFormat;
WriteMethod     write_dot11WPAKeysValue;
WriteMethod     write_dot11WPA2KeysIndex;
WriteMethod     write_dot11WPA2KeysFormat;
WriteMethod     write_dot11WPA2KeysValue;
WriteMethod     write_dot11RadiusServerIPAddress;
WriteMethod     write_dot11RadiusServerPort;
WriteMethod     write_dot11RadiusServerPassword;
WriteMethod     write_dot11RTSThreshold;
WriteMethod     write_dot11ShortRetryLimit;
WriteMethod     write_dot11LongRetryLimit;
WriteMethod     write_dot11FragmentationThreshold;
WriteMethod     write_dot11MaxTransmitMSDULifetime;
WriteMethod     write_dot11MaxReceiveLifetime;
WriteMethod     write_dot11IappEnable;
WriteMethod     write_dot11BlockRelay;
WriteMethod     write_dot11WIFISpecific;
WriteMethod     write_dot11QosEnable;
WriteMethod     write_dot11NetworkType;
WriteMethod     write_dot11LedType;
WriteMethod     write_dot11OperationMode;
WriteMethod     write_dot11BroadCastSSID;
WriteMethod     write_dot11GuestAccess;
WriteMethod     write_dot11Address;
WriteMethod     write_dot11GroupAddressesStatus;
WriteMethod     write_dot11CurrentRegDomain;
WriteMethod     write_dot11ShortPreamble;
WriteMethod     write_dot11PhyBandSelect;
WriteMethod     write_dot11TrSwitch;
WriteMethod     write_dot11PowerScale;
WriteMethod     write_dot11CurrentTxAntenna;
WriteMethod     write_dot11CurrentRxAntenna;
WriteMethod     write_dot11CurrentTxPowerLevel;
WriteMethod     write_dot11CurrentChannelNumber;
WriteMethod     write_dot11CurrentDwellTime;
WriteMethod     write_dot11CurrentSet;
WriteMethod     write_dot11CurrentPattern;
WriteMethod     write_dot11CurrentIndex;
WriteMethod     write_dot11EHCCPrimeRadix;
WriteMethod     write_dot11EHCCNumberofChannelsFamilyIndex;
WriteMethod     write_dot11EHCCCapabilityImplemented;
WriteMethod     write_dot11EHCCCapabilityEnabled;
WriteMethod     write_dot11HopAlgorithmAdopted;
WriteMethod     write_dot11RandomTableFlag;
WriteMethod     write_dot11HopOffset;
WriteMethod     write_dot11CurrentChannel;
WriteMethod     write_dot11CurrentCCAMode;
WriteMethod     write_dot11EDThreshold;
WriteMethod     write_dot11CCAWatchdogTimerMax;
WriteMethod     write_dot11CCAWatchdogCountMax;
WriteMethod     write_dot11CCAWatchdogTimerMin;
WriteMethod     write_dot11CCAWatchdogCountMin;
WriteMethod     write_dot11SupportedTxAntenna;
WriteMethod     write_dot11SupportedRxAntenna;
WriteMethod     write_dot11DiversitySelectionRx;
WriteMethod     write_dot11CurrentFrequency;
WriteMethod     write_dot11TIThreshold;
WriteMethod     write_dot11RandomTableFieldNumber;

#endif                          /* IEEE802DOT11_H */
