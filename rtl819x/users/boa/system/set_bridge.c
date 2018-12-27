/*
 *      Utiltiy function for setting bridge
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "apmib.h"
#include "sysconf.h"
#include "sys_utility.h"
extern int wlan_idx;	// interface index
extern int vwlan_idx;	// initially set interface index to root
extern int apmib_initialized;
#define BR_IFACE_FILE "/var/system/br_iface"
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
#define BR_IFACE_FILE2 "/var/system/br_iface2"
#endif
#define MESH_PATHSEL "/bin/pathsel"
#define BR_INIT_FILE "/tmp/bridge_init"
#define ETH_VLAN_SWITCH "/proc/disable_l2_table"
#define DHCPD_CONF_FILE "/var/udhcpd.conf"
#define DHCPD_LEASE_FILE "/var/lib/misc/udhcpd.leases"

int SetWlan_idx(char * wlan_iface_name);

char wlan_wan_iface[20];
/*

//eth0 eth1 eth2 eth3 eth4 wlan0 wlan0-msh wlan0-va0 wlan0-va1 wlan0-va2 wlan0-va3 wlan0-vxd
//wlan0-wds0 wlan0-wds1 wlan0-wds2 wlan0-wds3 wlan0-wds4 wlan0-wds5 wlan0-wds6 wlan0-wds7
*/

void start_lan_dhcpd(char *interface)
{
	char tmpBuff1[32]={0}, tmpBuff2[32]={0};
	int intValue=0, dns_mode=0;
	char line_buffer[100]={0};
	char tmp1[64]={0};
	char tmp2[64]={0};
	char *strtmp=NULL, *strtmp1=NULL;
	DHCPRSVDIP_T entry;
	int i, entry_Num=0;
#ifdef   HOME_GATEWAY
	char tmpBuff3[32]={0};
#endif
	sprintf(line_buffer,"interface %s\n",interface);
	write_line_to_file(DHCPD_CONF_FILE, 1, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_START,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"start %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_END,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"end %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_SUBNET_MASK,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"opt subnet %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_LEASE_TIME, (void *)&intValue);
    if( (intValue==0) || (intValue==1) || (intValue<0) || (intValue>10080))
    {
	printf("===%s(%d), read MIB_DHCP_LEASE_TIME is %d\n", __FUNCTION__, __LINE__, intValue);
        //intValue = 480; //8 hours
        if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&intValue))
        {
        	printf("set MIB_DHCP_LEASE_TIME error\n");
        }

		apmib_update(CURRENT_SETTING);
    }
	intValue *= 60;
    sprintf(line_buffer,"opt lease %ld\n",intValue);
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

		apmib_get(MIB_IP_ADDR,  (void *)tmp1);
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
		sprintf(line_buffer,"opt router %s\n",strtmp);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

		sprintf(line_buffer,"opt dns %s\n",strtmp); /*now strtmp is ip address value */
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

		memset(tmp1, 0x00, 64);
		apmib_get( MIB_DOMAIN_NAME, (void *)&tmp1);
		if(tmp1[0]){
			sprintf(line_buffer,"opt domain %s\n",tmp1);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}

	/* may not need to set ip again*/
	apmib_get(MIB_IP_ADDR,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(tmpBuff1, "%s", strtmp);
	apmib_get(MIB_SUBNET_MASK,  (void *)tmp2);
	strtmp1= inet_ntoa(*((struct in_addr *)tmp2));
	sprintf(tmpBuff2, "%s", strtmp1);
	RunSystemCmd(NULL_FILE, "ifconfig", interface, tmpBuff1, "netmask", tmpBuff2,  NULL_STR);
	/*start dhcp server*/
	RunSystemCmd(NULL_FILE, "udhcpd", DHCPD_CONF_FILE, NULL_STR);


}


int SetWlan_idx(char * wlan_iface_name)
{
	int idx;

		idx = atoi(&wlan_iface_name[4]);
		if (idx >= NUM_WLAN_INTERFACE) {
				printf("invalid wlan interface index number!\n");
				return 0;
		}
		wlan_idx = idx;
		vwlan_idx = 0;

#ifdef MBSSID

		if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
				wlan_iface_name[6] == 'v' && wlan_iface_name[7] == 'a') {
				idx = atoi(&wlan_iface_name[8]);
				if (idx >= NUM_VWLAN_INTERFACE) {
					printf("invalid virtual wlan interface index number!\n");
					return 0;
				}

				vwlan_idx = idx+1;
				idx = atoi(&wlan_iface_name[4]);
				wlan_idx = idx;
		}
#endif

#ifdef UNIVERSAL_REPEATER
				if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
						!memcmp(&wlan_iface_name[6], "vxd", 3)) {
					vwlan_idx = NUM_VWLAN_INTERFACE;
					idx = atoi(&wlan_iface_name[4]);
					wlan_idx = idx;
				}
#endif

//printf("\r\n wlan_iface_name=[%s],wlan_idx=[%u],vwlan_idx=[%u],__[%s-%u]\r\n",wlan_iface_name,wlan_idx,vwlan_idx,__FILE__,__LINE__);

return 1;
}

void set_lan_dhcpd(char *interface, int mode)
{
	char tmpBuff1[32]={0}, tmpBuff2[32]={0};
	int intValue=0, dns_mode=0;
	char line_buffer[100]={0};
	char tmp1[64]={0};
	char tmp2[64]={0};
	char *strtmp=NULL, *strtmp1=NULL;
	DHCPRSVDIP_T entry;
	int i, entry_Num=0;
#ifdef   HOME_GATEWAY
	char tmpBuff3[32]={0};
#endif
#if defined(CONFIG_RTL_ULINKER)
{//// must be gateway mode!
	int opmode;
	int auto_wan;
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	if (opmode==GATEWAY_MODE) {
		apmib_get(MIB_ULINKER_AUTO,(void *)&auto_wan);
		system("brctl addif br0 usb0 > /dev/null 2>&1");
	#if 0
		if (auto_wan == 0)
			system("ifconfig usb0 0.0.0.0 > /dev/null 2>&1");
	#endif
	}
}
#endif
	sprintf(line_buffer,"interface %s\n",interface);
	write_line_to_file(DHCPD_CONF_FILE, 1, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_START,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"start %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_END,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"end %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_SUBNET_MASK,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"opt subnet %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_LEASE_TIME, (void *)&intValue);
    if( (intValue==0) || (intValue<0) || (intValue>10080))
    {
		intValue = 480; //8 hours
		if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&intValue))
		{
			printf("set MIB_DHCP_LEASE_TIME error\n");
		}

		apmib_update(CURRENT_SETTING);
    }
	intValue *= 60;
    sprintf(line_buffer,"opt lease %ld\n",intValue);
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	if(mode==1){//ap
		apmib_get( MIB_DEFAULT_GATEWAY,  (void *)tmp2);
		if (memcmp(tmp2, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmp2));
			sprintf(line_buffer,"opt router %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}


	}else{
		apmib_get(MIB_IP_ADDR,  (void *)tmp1);
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
		sprintf(line_buffer,"opt router %s\n",strtmp);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
#ifdef   HOME_GATEWAY
		apmib_get( MIB_DNS_MODE, (void *)&dns_mode);
		if(dns_mode==0){
			sprintf(line_buffer,"opt dns %s\n",strtmp); /*now strtmp is ip address value */
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
#endif
	}
	if((mode==1) 
#if 1
	||(mode==2 && dns_mode==1)
#endif
	){
#if defined(HOME_GATEWAY) && !defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
		apmib_get( MIB_DNS1,  (void *)tmpBuff1);
		apmib_get( MIB_DNS2,  (void *)tmpBuff2);
		apmib_get( MIB_DNS3,  (void *)tmpBuff3);

		if (memcmp(tmpBuff1, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff1));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
		if (memcmp(tmpBuff2, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff2));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
		if (memcmp(tmpBuff3, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff3));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
#endif

#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT
		apmib_get(MIB_IP_ADDR,  (void *)tmp1);
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
		sprintf(line_buffer,"opt dns %s\n",strtmp); /*now strtmp is ip address value */
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
#endif //#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT

		if(intValue==0){ /*no dns option for dhcp server, use default gatewayfor dns opt*/

			if(mode==1){
				apmib_get( MIB_DEFAULT_GATEWAY,  (void *)tmp2);
				if (memcmp(tmp2, "\x0\x0\x0\x0", 4)){
					strtmp= inet_ntoa(*((struct in_addr *)tmp2));
					sprintf(line_buffer,"opt dns %s\n",strtmp);
					write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}
			}else {
				apmib_get( MIB_IP_ADDR,  (void *)tmp2);
				if (memcmp(tmp2, "\x0\x0\x0\x0", 4)){
					strtmp= inet_ntoa(*((struct in_addr *)tmp2));
					sprintf(line_buffer,"opt dns %s\n",strtmp);
					write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}
			}
		}
	}
	memset(tmp1, 0x00, 64);
	apmib_get( MIB_DOMAIN_NAME, (void *)&tmp1);
	if(tmp1[0]){
		sprintf(line_buffer,"opt domain %s\n",tmp1);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}
/*static dhcp entry static_lease 000102030405 192.168.1.199*/
	intValue=0;
	apmib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&intValue);
	if(intValue==1){
		apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
		if(entry_Num>0){
			for (i=1; i<=entry_Num; i++) {
				*((char *)&entry) = (char)i;
				apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry);
				sprintf(line_buffer, "static_lease %02x%02x%02x%02x%02x%02x %s\n", entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
				entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], inet_ntoa(*((struct in_addr*)entry.ipAddr)));
				write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			}
		}
	}
	/* may not need to set ip again*/
	apmib_get(MIB_IP_ADDR,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(tmpBuff1, "%s", strtmp);
	apmib_get(MIB_SUBNET_MASK,  (void *)tmp2);
	strtmp1= inet_ntoa(*((struct in_addr *)tmp2));
	sprintf(tmpBuff2, "%s", strtmp1);
	RunSystemCmd(NULL_FILE, "ifconfig", interface, tmpBuff1, "netmask", tmpBuff2,  NULL_STR);
	/*start dhcp server*/
	char tmpBuff4[100];
	sprintf(tmpBuff4,"udhcpd %s\n",DHCPD_CONF_FILE);
	system(tmpBuff4);
	//RunSystemCmd(stdout, "udhcpd", DHCPD_CONF_FILE, NULL_STR);


}
void set_lan_dhcpc(char *iface)
{
	char script_file[100], deconfig_script[100], pid_file[100];
	char *strtmp=NULL;
	char tmp[32], Ip[32], Mask[32], Gateway[32];
	char cmdBuff[200];
#ifdef  HOME_GATEWAY
	int intValue=0;
#endif
	sprintf(script_file, "/usr/share/udhcpc/%s.sh", iface); /*script path*/
	sprintf(deconfig_script, "/usr/share/udhcpc/%s.deconfig", iface);/*deconfig script path*/
	sprintf(pid_file, "/etc/udhcpc/udhcpc-%s.pid", iface); /*pid path*/
	apmib_get( MIB_IP_ADDR,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Ip, "%s",strtmp);

	apmib_get( MIB_SUBNET_MASK,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Mask, "%s",strtmp);

	apmib_get( MIB_DEFAULT_GATEWAY,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Gateway, "%s",strtmp);

	Create_script(deconfig_script, iface, LAN_NETWORK, Ip, Mask, Gateway);

	//RunSystemCmd(NULL_FILE, "udhcpc", "-i", iface, "-p", pid_file, "-s", script_file,  "-n", "-x", NULL_STR);
	//sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s -n &", iface, pid_file, script_file);
	sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s &", iface, pid_file, script_file);
	system(cmdBuff);
}

int setbridge(char *argv)
{
	FILE *fp=NULL;
	int j;
	int opmode=-1;
	char bridge_iface[300], tmpBuff[200], cmdBuffer[100];
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	char bridge_iface2[300];
#endif
	char iface_name[16], tmp_iface[16];
	char *token=NULL, *savestr1=NULL;
	int intVal=0;
	int iface_index=0;
	int vlan_enabled=0, wlan_disabled=0;
	int wlan_mode=0, wisp_wan_id=0;
	int iswlan_va=0, wlan_wds_enabled=0;
	int wlan_wds_num=0;
	int br_stp_enabled=0, dhcp_mode=0;
	char lanIp[30], lanMask[30], lanGateway[30];
	int lan_addr,lan_mask;


	token=NULL;
	savestr1=NULL;
	sprintf(tmpBuff, "%s", argv);
	
//printf("\r\n tmpBuff=[%s],__[%s-%u]\r\n",tmpBuff,__FILE__,__LINE__);
	
	token = strtok_r(tmpBuff," ", &savestr1);
	do{
		if(token == NULL){/*check if the first arg is NULL*/
			break;
		}else
		{
	    sprintf(iface_name,"%s", token);
				if (strlen(iface_name) >= 9 && iface_name[5] == '-' &&
						iface_name[6] == 'v' && iface_name[7] == 'a')
				{
					char wlanRootName[16];
					memset(wlanRootName, 0x00, sizeof(wlanRootName));
					strncpy(wlanRootName,iface_name, 5);

					if(SetWlan_idx(wlanRootName)){
						apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);//get root if enable/disable
						apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode); //get root if mode
						if(intVal==0)
						{
								if(SetWlan_idx( iface_name))
								{
									apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);//get va if enable/disable
									if(intVal==0)
										wlan_disabled=0;
									else
										wlan_disabled=1;
								}else
										wlan_disabled=1;
						}else
							wlan_disabled=1;
					}else
						wlan_disabled=1;//root if is disabled
				}
				else
				{
					if(SetWlan_idx( iface_name)){
						apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);
						wlan_disabled=intVal;
					}else
						wlan_disabled=1;
				}
				if(wlan_disabled==0){ //wlan if is enabled					
						RunSystemCmd(NULL_FILE, "ifconfig", iface_name, "0.0.0.0", NULL_STR);
					}

				//printf("[%s] [%s] [%s]\n",iface_name,bridge_iface,bridge_iface2);
		}
		token = strtok_r(NULL, " ", &savestr1);
	}while(token !=NULL);

#ifdef AEI_WECB
    // When wifi interface being ready after restart,
    // trigger Higher Layer Object of Device.SSID.{i}.
    system( "cli -s Device.Bridging.Bridge.1.Port.3.X_ACTIONTEC_COM_Trigger int 1" );
    system( "cli -s Device.Bridging.Bridge.1.Port.4.X_ACTIONTEC_COM_Trigger int 1" );
#if defined(AEI_WECB_CUSTOMER_TWC)
    system( "cli -s Device.Bridging.Bridge.1.Port.5.X_ACTIONTEC_COM_Trigger int 1" );
    system( "cli -s Device.Bridging.Bridge.1.Port.6.X_ACTIONTEC_COM_Trigger int 1" );
    system( "cli -s Device.Bridging.Bridge.1.Port.7.X_ACTIONTEC_COM_Trigger int 1" );
    system( "cli -s Device.Bridging.Bridge.1.Port.8.X_ACTIONTEC_COM_Trigger int 1" );
    system( "cli -s Device.Bridging.Bridge.1.Port.9.X_ACTIONTEC_COM_Trigger int 1" );
    system( "cli -s Device.Bridging.Bridge.1.Port.10.X_ACTIONTEC_COM_Trigger int 1" );
#elif defined(AEI_WECB_CUSTOMER_TELUS) || defined(AEI_WECB_CUSTOMER_NCS)
    system( "cli -s Device.Bridging.Bridge.1.Port.5.X_ACTIONTEC_COM_Trigger int 1" );
    system( "cli -s Device.Bridging.Bridge.1.Port.6.X_ACTIONTEC_COM_Trigger int 1" );
#endif
#if defined(AEI_WECB_CUSTOMER_COMCAST)
    system( "cli -s Device.Bridging.Bridge.2.Port.3.X_ACTIONTEC_COM_Trigger int 1" );
    system( "cli -s Device.Bridging.Bridge.2.Port.4.X_ACTIONTEC_COM_Trigger int 1" );
#endif

#endif

	return 0;
}
