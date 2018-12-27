#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include "apmib.h"
#include "mibtbl.h"
#include "upmib.h"
#include "sysconf.h"
#include "sys_utility.h"
#include "syswan.h"
//extern int wlan_idx;	// interface index 
//extern int vwlan_idx;	// initially set interface index to root   
extern int set_QoS(int operation, int wan_type, int wisp_wan_id);
extern int setbridge(char *argv);
extern int setWlan_Applications(char *action, char *argv);
extern int SetWlan_idx(char *wlan_iface_name);
extern int setFirewallIptablesRules(int argc, char** argv);
extern void set_lan_dhcpd(char *interface, int mode);
#ifdef MULTI_PPPOE
extern void wan_disconnect(char *option , char *conncetOrder);
#else
extern void wan_disconnect(char *option);
#endif
extern void set_ipv6(int opmode);
void set_log(void);
#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
extern void Init_Domain_Query_settings(int operation_mode, int wlan_mode, int lan_dhcp_mode, char *lan_mac);
extern void domain_query_start_dnrd(int wlan_mode, int start_dnrd);
#endif
#ifdef CONFIG_SMART_REPEATER
int getWispRptIfaceName(char*pIface,int wlanId);
#endif

/*

//eth0 eth1 eth2 eth3 eth4 wlan0 wlan0-msh wlan0-va0 wlan0-va1 wlan0-va2 wlan0-va3 wlan0-vxd
//wlan0-wds0 wlan0-wds1 wlan0-wds2 wlan0-wds3 wlan0-wds4 wlan0-wds5 wlan0-wds6 wlan0-wds7

WLAN=>>> wlan0
WLANVXD=>>>wlan0-vxd
WLANVIRTUAL=>> wlan0-va0 wlan0-va1 wlan0-va2 wlan0-va3 wlan0-vxd
NUM_=>>>1
VIRTUALNUL=>>>4

*/

int gateway=0;
int enable_wan=0;
int enable_br=0;
char br_interface[16]={0};
char br_lan1_interface[16]={0};
char br_lan2_interface[16]={0};
char wan_interface[16]={0};
char vlan_interface[32]={0};
static char wlan_interface[16]={0};
char wlan_virtual_interface[80]={0};
static char wlan_vxd_interface[32]={0};
int num_wlan_interface=0;
int num_wlan_virtual_interface=0;
int num_wlan_vxd_interface=0;

#ifdef CONFIG_APP_TR069
char acsURLStr[CWMP_ACS_URL_LEN+1];
#endif //#ifdef CONFIG_APP_TR069

void set_br_interface(unsigned char *brif)
{

	int opmode=-1;
	unsigned char tmpBuff[512]={0};
  apmib_get(MIB_OP_MODE,(void *)&opmode);
	
	if(vlan_interface[0]){
		strcat(tmpBuff, vlan_interface);
		strcat(tmpBuff, " ");
	}
	if(wlan_interface[0]){
		strcat(tmpBuff, wlan_interface);
		strcat(tmpBuff, " ");
	}			
	if(wlan_virtual_interface[0]){
		strcat(tmpBuff, wlan_virtual_interface);
		strcat(tmpBuff, " ");
	}
#if defined(CONFIG_RTL_8198_AP_ROOT)
	if(br_lan2_interface[0]){
		strcat(tmpBuff, br_lan2_interface);
		strcat(tmpBuff, " ");
	}
#endif
	if(wlan_vxd_interface[0]){
		strcat(tmpBuff, wlan_vxd_interface);
		strcat(tmpBuff, " ");
	}

	strcat(tmpBuff, br_interface);
	strcat(tmpBuff, " ");
#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	if(opmode == GATEWAY_MODE)
	{
		strcat(tmpBuff, "eth0 eth2 eth3 eth4");
	}
	else
	{
		strcat(tmpBuff, "eth0 eth1 eth2 eth3 eth4");
	}	
#else
	
	strcat(tmpBuff, br_lan1_interface);

#if defined(CONFIG_RTL_IVL_SUPPORT)
	/*add eth1 to br0 when in bridge&wisp mode*/
	if(opmode == BRIDGE_MODE || opmode == WISP_MODE) 
	{
		strcat(tmpBuff, " ");
		strcat(tmpBuff, br_lan2_interface);
	}
#endif		
#endif

#if defined(CONFIG_RTL_ULINKER)
	strcat(tmpBuff, " ");
	strcat(tmpBuff, "usb0");
#endif
	memcpy(brif, tmpBuff, sizeof(tmpBuff));
	return;

}

char * strToValue(char *str,TYPE_T type)
{
	switch(type)
	{
		case BYTE_T:
		case WORD_T:
		case DWORD_T:
		{
			int i=atoi(str);
			*((int*)str)=i;
			return str;
		}
		case STRING_T:
			return str;
		case IA_T:
		{
			struct in_addr addr;
			inet_aton(str,&addr);
			*((struct in_addr*)str)=addr;
			return str;
		}
		default:
			return str;
	}
}
void upmib_setMib(UPMIB_T * upmib_array,unsigned short index)
{
	char def=0;//default setting
	char i=0,j=0;
	char defBuffer[32]={0};
	char buffer[16]={0};
	char hwBuffer[16]={0};
	unsigned char valueTmp[UPMIB_VALUE_MAX]={0};
	apmib_save_wlanIdx();
	if(!strncmp(upmib_array[index].name,"DEF_",strlen("DEF_")))
		def=1;		
	for(i=0;i<NUM_WLAN_INTERFACE;i++)
	{
		sprintf(buffer,"WLAN%d_",i);
		sprintf(defBuffer,"DEF_WLAN%d_",i);
		sprintf(hwBuffer,"HW_WLAN%d_",i);
		if(!strncmp(upmib_array[index].name,buffer,strlen(buffer))
		 ||!strncmp(upmib_array[index].name,defBuffer,strlen(defBuffer))
		 ||!strncmp(upmib_array[index].name,hwBuffer,strlen(hwBuffer)))
		{
			wlan_idx=i;
#ifdef AEI_WECB
            vwlan_idx = 0;//default is for root ap
#endif
			for(j=0;j<NUM_VWLAN;j++)
			{
				sprintf(buffer,"WLAN%d_VAP%d_",i,j);
				sprintf(defBuffer,"DEF_WLAN%d_VAP%d_",i,j);
				if(!strncmp(upmib_array[index].name,buffer,strlen(buffer))
				 ||!strncmp(upmib_array[index].name,defBuffer,strlen(defBuffer)))
				{
					vwlan_idx=j+1;
					break;
				}
			}
		}		
	}

	strcpy(valueTmp,upmib_array[index].value);
	if(def)
		apmib_setDef(upmib_array[index].id,strToValue(valueTmp,upmib_array[index].type));
	else
		apmib_set(upmib_array[index].id,strToValue(valueTmp,upmib_array[index].type));
	apmib_recov_wlanIdx();	
}
int up_mib_value()
{
        int old_ver=0;
        int new_ver=0;
        int i=0;
 
        apmib_get(MIB_MIB_VER, (void *)&old_ver);
        new_ver = atoi(update_mib[0].value);
        if(old_ver == new_ver)
        {
                return -1;
        }
        else
                printf("MIB Version update!\n");
 
        i=0;
        while(new_mib[i].id != 0)
        {
        		upmib_setMib(new_mib,i);
        		//apmib_set(new_mib[i].id,strToValue(new_mib[i].value,new_mib[i].type)); 
                //RunSystemCmd(NULL_FILE, "flash", "set", new_mib[i].name, new_mib[i].value, NULL_STR);
                i++;
        }
        i=0;
        while(update_mib[i].id != 0)
        {
        	upmib_setMib(update_mib,i);
        	//apmib_set(update_mib[i].id,strToValue(update_mib[i].value,update_mib[i].type));        	
              //  RunSystemCmd(NULL_FILE, "flash", "set", update_mib[i].name, update_mib[i].value, NULL_STR);
                i++;
        }
 		apmib_update(HW_SETTING);
 		apmib_update(DEFAULT_SETTING);
 		apmib_update(CURRENT_SETTING);
        return 0;
 
}


void set_log(void)
{
	int intValue=0,  intValue1=0;
	char tmpBuffer[32];
	char syslog_para[32];
	char localServer[32];
	
	apmib_get(MIB_SCRLOG_ENABLED, (void*)&intValue);
	if(intValue !=0 && intValue !=2 && intValue !=4 && intValue !=6 && intValue !=8 &&
		intValue !=10 && intValue !=12 && intValue !=14) {
			apmib_get(MIB_REMOTELOG_ENABLED, (void*)&intValue1);
			if(intValue1 != 0){
				apmib_get(MIB_REMOTELOG_SERVER,  (void *)tmpBuffer);
				if (memcmp(tmpBuffer, "\x0\x0\x0\x0", 4)){
#ifdef RINGLOG /* ring log */
					sprintf(localServer, "%s", inet_ntoa(*((struct in_addr *)tmpBuffer)));
					system("rm /var/log/log_split >/dev/null 2>&1");
					sprintf(tmpBuffer, "echo %d > /var/log/log_split", LOG_SPLIT);
					system(tmpBuffer);
					sprintf(tmpBuffer, "%d", MAX_LOG_SIZE);
					sprintf(syslog_para, "%d", LOG_SPLIT);
					fprintf(stderr, "syslog will use %dKB for log(%d rotate, 1 original, %dKB for each)\n",
						MAX_LOG_SIZE * (LOG_SPLIT+1), LOG_SPLIT, MAX_LOG_SIZE);
					RunSystemCmd(NULL_FILE, "syslogd", "-L","-R", localServer, "-s", tmpBuffer, "-b", syslog_para, NULL_STR);
#else
					sprintf(syslog_para, "%s", inet_ntoa(*((struct in_addr *)tmpBuffer)));
					RunSystemCmd(NULL_FILE, "syslogd", "-L", "-R", syslog_para, NULL_STR);
#endif					
				}
			}else{
			
//### add by sen_liu 2011.4.21 sync the system log update (enlarge from 1 pcs to 8 pcs) to	SDKv2.5 from kernel 2.4
#ifdef RINGLOG /* ring log */
				system("rm /var/log/log_split >/dev/null 2>&1");
				sprintf(tmpBuffer, "echo %d > /var/log/log_split", LOG_SPLIT);
				system(tmpBuffer);
				sprintf(tmpBuffer, "%d", MAX_LOG_SIZE);
				sprintf(syslog_para, "%d", LOG_SPLIT);
				fprintf(stderr, "syslog will use %dKB for log(%d rotate, 1 original, %dKB for each)\n",
					MAX_LOG_SIZE * (LOG_SPLIT+1), LOG_SPLIT, MAX_LOG_SIZE);
				RunSystemCmd(NULL_FILE, "syslogd", "-L", "-s", tmpBuffer, "-b", syslog_para, NULL_STR);
#else
//### end
				RunSystemCmd(NULL_FILE, "syslogd", "-L", NULL_STR);
#endif
			}
			RunSystemCmd(NULL_FILE, "klogd", NULL_STR);
		} 
		 
	
	return;
}

void start_wlanapp(int action)
{
	char tmpBuff[128];
	memset(tmpBuff, 0x00, sizeof(tmpBuff));

	sprintf(tmpBuff, "%s %s %s", wlan_interface, wlan_virtual_interface, br_interface); 	

//	printf("\r\n tmpBuff=[%s],__[%s-%u]\r\n",tmpBuff,__FILE__,__LINE__);

	if(tmpBuff[0])
		setWlan_Applications("start", tmpBuff);
	
}

void start_upnpd(int isgateway, int sys_op)
{
#ifdef   HOME_GATEWAY	
	int intValue=0,  intValue1=0;
	if(SetWlan_idx("wlan0")){
		apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&intValue);
	}
	if(isgateway==1 && sys_op !=1)
		apmib_get(MIB_UPNP_ENABLED, (void *)&intValue1);
	else 
		intValue1=0;
	if(intValue==0 && intValue1==0){//wps enabled and upnp igd is disabled
		RunSystemCmd(NULL_FILE, "mini_upnpd", "-wsc", "/tmp/wscd_config", "-daemon", NULL_STR);
		//system("mini_upnpd -wsc /tmp/wscd_config &");
	}else if(intValue==1 && intValue1==1){//wps is disabled, and upnp igd is enabled
		RunSystemCmd(NULL_FILE, "mini_upnpd", "-igd", "/tmp/igd_config", "-daemon", NULL_STR);
		//system("mini_upnpd -igd /tmp/igd_config &");
	}else if(intValue==0 && intValue1==1){//both wps and upnp igd are enabled
		RunSystemCmd(NULL_FILE, "mini_upnpd", "-wsc", "/tmp/wscd_config", "-igd", "/tmp/igd_config","-daemon",  NULL_STR);
		//system("mini_upnpd -wsc /tmp/wscd_config -igd /tmp/igd_config &");
	}else if(intValue==1 && intValue1==0){//both wps and upnp igd are disabled
		/*do nothing*/
	}
#endif		
}
/*method to start reload is co-operate to parse rule of reload.c*/
void start_wlan_by_schedule(int index)
{
	int intValue=0,  intValue1=0, i=0, entryNum=0, bak_idx=0, bak_vidx=0;
	char tmp1[64]={0};
	SCHEDULE_T wlan_sched;
	int newfile=1;

	bak_idx=wlan_idx;
	wlan_idx=index;
	bak_idx=vwlan_idx;
	vwlan_idx=0;
	apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&intValue);
	#define WLAN_SCHEDULE_FILE "/var/wlsch.conf"
	sprintf(tmp1,WLAN_SCHEDULE_FILE"%d",index);
	unlink(tmp1);

	if(intValue==0){
		apmib_get(MIB_WLAN_SCHEDULE_ENABLED, (void *)&intValue1);
		apmib_get(MIB_WLAN_SCHEDULE_TBL_NUM, (void *)&entryNum);

		if(intValue1==1 && entryNum > 0){
			
			for (i=1; i<=entryNum; i++) {
				*((char *)&wlan_sched) = (char)i;
				apmib_get(MIB_WLAN_SCHEDULE_TBL, (void *)&wlan_sched);

				char line_buffer[100]={0};
				if(wlan_sched.eco == 1 && !(wlan_sched.fTime == 0 && wlan_sched.tTime == 0))
				{
					sprintf(line_buffer,"%d,%d,%d,%d\n",i,wlan_sched.day,wlan_sched.fTime, wlan_sched.tTime);
					sprintf(tmp1,WLAN_SCHEDULE_FILE"%d",index);
					write_line_to_file(tmp1, (newfile==1?1:2), line_buffer);
					newfile = 2;
				}
			}
			if(index == (NUM_WLAN_INTERFACE-1)){
				sprintf(tmp1, "reload -k %s &", WLAN_SCHEDULE_FILE);
				system(tmp1);
			}
		}
		else{ /* do not care schedule*/

			if(index == (NUM_WLAN_INTERFACE-1)){
				sprintf(tmp1, "reload -k %s &", WLAN_SCHEDULE_FILE);
				system(tmp1);
			}
		}
	}
	else{
		/*wlan is disabled, we do not care wlan schedule*/
		if(index ==(NUM_WLAN_INTERFACE-1)){
			sprintf(tmp1, "reload -k %s &", WLAN_SCHEDULE_FILE);
			system(tmp1);
		}
	}

	vwlan_idx=bak_vidx;
	wlan_idx=bak_idx;
}
void clean_process(int sys_opmode,int wan_dhcp_mode,int gateway, int enable_wan, char *lanInterface, char *wlanInterface, char *wanInterface)
{
	char strPID[10], tmpBuff[200];
	int pid=-1;
	int i;
	RunSystemCmd(HW_NAT_FILE, "echo", "2", NULL_STR);	/*bridge mode*/
	

	RunSystemCmd(NULL_FILE, "killall", "-9", "reload", NULL_STR);
	
	
	
}
#if defined(CONFIG_APP_USBMOUNT)
#define	PARTITION_FILE "/proc/partitions"

int get_blockDevPartition(char *str, char *partition)
{

	unsigned char tk[50];
	unsigned int i,j;
	unsigned int curCnt,preCnt;
	
	if(str==NULL)
	{
		return -1;
	}
	
	memset(tk,0, sizeof(tk));

	/*	partition table format:
		major minor  #blocks  name
	*/
	
	preCnt=0;
	curCnt=0;
	for (i=0;i<strlen(str);i++)
	{          
		if( (str[i]!=' ') && (str[i]!='\n') && (str[i]!='\r'))
		{
			if(preCnt==curCnt)
			{
				tk[curCnt]=i;
				curCnt++;
			}
		}
		else if((str[i]==' ') || (str[i]=='\n') ||(str[i]=='\r') )
		{
			preCnt=curCnt;
		}
	}
	
	/*to check device major number is 8*/
	
	if(!isdigit(str[tk[0]]))
	{
		return -1;
	}

	if(tk[1]==0)
	{
		return -1;
	}

	if(tk[1]<=tk[0])
	{
		return -1;
	}

	if((str[tk[0]]!='8') ||(str[tk[0]+1]!=' '))
	{
		return -1;
	}
	
	if(tk[3]==0)
	{
		return -1;
	}

	/*to get partition name*/
	j=0;
	for(i=tk[3]; i<strlen(str); i++)
	{
		
		if((str[i]==' ') || (str[i]=='\n') ||(str[i]=='\n'))
		{
			partition[j]='\0';
			return 0;
		}
		else
		{
			partition[j]=str[i];
			j++;
		}
			
	}
	return 0;
}
int Check_shouldMount(char *partition_name)
{
	DIR *dir=NULL;
	struct dirent *next;
	int found=0;
	dir = opendir("/tmp/usb");
	if (!dir) {
		printf("Cannot open %s", "/tmp/usb");
		return -1;
	}
	while ((next = readdir(dir)) != NULL) {
		//printf("Check_shouldMount:next->d_reclen=%d, next->d_name=%s\n",next->d_reclen, next->d_name);
			/* Must skip ".." */
			if (strcmp(next->d_name, "..") == 0)
				continue;
			if (strcmp(next->d_name, ".") == 0)
				continue;
			if (strcmp(next->d_name, "mnt_map") == 0)
				continue;
			if(!strcmp(next->d_name, partition_name)){
				found=1;
				break;
			}
	}
	closedir(dir);
	return found;
}
void autoMountOnBootUp(void)
{
	FILE *fp;
	
	int line=0;
	char buf[512];
	char partition[32];
	char usbMntCmd[64];
	int ret=-1;
	if(isFileExist(PARTITION_FILE)){
		fp= fopen(PARTITION_FILE, "r");
		if (!fp) {
	        	printf("can not  open /proc/partitions\n");
			return; 
	   	}

		while (fgets(buf, sizeof(buf), fp)) 
		{
			ret=get_blockDevPartition(buf, &partition);
			if(ret==0)
			{
				if(Check_shouldMount(partition)==0){
				sprintf(usbMntCmd, "DEVPATH=/sys/block/sda/%s ACTION=add usbmount block", partition);
				RunSystemCmd(NULL_FILE,  "echo", usbMntCmd, NULL_STR);
				system(usbMntCmd);
			}
			}
			
		}
	
		fclose(fp);
	}
	

}

void start_mount()
{
#if defined(HTTP_FILE_SERVER_SUPPORTED)
	RunSystemCmd("/proc/sys/vm/min_free_kbytes", "echo", "384", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_max", "echo", "1048576", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_max", "echo", "1048576", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_rmem", "echo", "4096 108544 4194304", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_wmem", "echo", "4096 108544 4194304", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_moderate_rcvbuf", "echo", "0", NULL_STR);
#else	
	/*config linux parameter for improving samba performance*/
	RunSystemCmd("/proc/sys/vm/min_free_kbytes", "echo", "1024", NULL_STR);
	
	RunSystemCmd("/proc/sys/net/core/netdev_max_backlog", "echo", "8192", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/optmem_max", "echo", "131072", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_default", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_max", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_default", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_max", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_rmem", "echo", "131072 262144 393216", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_wmem", "echo", "131072 262144 393216", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_mem", "echo", "768 1024 1380", NULL_STR);
#endif
	/*config hot plug and auto-mount*/
	RunSystemCmd("/proc/sys/kernel/hotplug", "echo", "/usr/hotplug", NULL_STR);
	RunSystemCmd(NULL_FILE, "mkdir", "-p /tmp/usb/", NULL_STR);

	/*force kernel to write data to disk, don't cache in memory for a long time*/
	RunSystemCmd("/proc/sys/vm/vfs_cache_pressure", "echo", "10000", NULL_STR);
	RunSystemCmd("/proc/sys/vm/dirty_background_ratio", "echo", "5", NULL_STR);
	RunSystemCmd("/proc/sys/vm/dirty_writeback_centisecs", "echo", "100", NULL_STR);
	/*automatically mount partions listed in /proc/partitions*/
	autoMountOnBootUp();

//	RunSystemCmd("/var/group",  "echo", " ",  NULL_STR);
//	RunSystemCmd(NULL_FILE,  "cp", "/etc/group", "/var/group",  NULL_STR);

}
#endif

#if defined(CONFIG_APP_SAMBA)
void start_samba()
{
	/*start samba*/
	RunSystemCmd(NULL_FILE,  "echo", "start samba", NULL_STR);
	RunSystemCmd(NULL_FILE,  "mkdir", "/var/samba", NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/samba/smb.conf", "/var/samba/smb.conf",  NULL_STR);
	RunSystemCmd("/var/group",  "echo", " ",  NULL_STR);
        RunSystemCmd(NULL_FILE,  "cp", "/etc/group", "/var/group",  NULL_STR);
	RunSystemCmd(NULL_FILE,  "smbd", "-D", NULL_STR);
	RunSystemCmd(NULL_FILE,  "nmbd", "-D", NULL_STR);
}
#endif

#if defined(CONFIG_APP_VSFTPD)
void start_vsftpd()
{
	RunSystemCmd(NULL_FILE, "echo","start vsftpd", NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/vsftpd.conf", "/var/config/vsftpd.conf",  NULL_STR);
	system("vsftpd /var/config/vsftpd.conf &");
}
#endif
#ifdef MULTI_PPPOE
void setInitMultiPPPoE()
{
	/*
		-----------------------Multi PPPoE--------------------------
		when something would lock global resource, should clear here,		
	*/
	
	system("rm /etc/ppp/firewall_lock >/dev/null 2>&1");
	system("echo 1 > /etc/ppp/firewall_lock ");
//	RunSystemCmd("tmp/firewall_lock", "echo", "1", NULL_STR);	
	//remove("/etc/ppp/hasPppoedevice");
	//remove("/etc/ppp/ppp_order_info");
	/*
	if((flushF = fopen("/etc/ppp/flushCmds","r+")) != NULL)
	{
		while(!feof(flushF))
		{
			// get the line and execute it , just for clear the ip policy rules
			fgets(cmd,sizeof(cmd),flushF);		
			system(cmd);
		}
		// delete flushCmds file
		unlink("/etc/ppp/flushCmds");
	}		
	*/
}
#endif

#ifdef AEI_CTL_BRIDGE
int init_mac()
{
    char tmpBuff[200], cmdBuffer[100];
    // set mac address for eth
    apmib_get(MIB_HW_NIC0_ADDR,  (void *)tmpBuff);
	sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x",
                (unsigned char)tmpBuff[0],
                (unsigned char)tmpBuff[1],
                (unsigned char)tmpBuff[2],
                (unsigned char)tmpBuff[3],
                (unsigned char)tmpBuff[4],
                (unsigned char)tmpBuff[5]);
	RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "hw", "ether",cmdBuffer, NULL_STR);
    //printf( "JBB AEI MAC: %s - %s\n", "eth0", cmdBuffer );

    // set mac address for wifi
    char wlan_name[16] = {0};
    int para_id;

    for( wlan_idx=0; wlan_idx<2; wlan_idx++ ) {
        for( vwlan_idx=0; vwlan_idx<=4; vwlan_idx++ ) {
            switch(vwlan_idx) {
                case 0:
                    para_id = MIB_HW_WLAN_ADDR;
                    break;
                case 1:
                    para_id = MIB_HW_WLAN_ADDR1;
                    break;
                case 2:
                    para_id = MIB_HW_WLAN_ADDR2;
                    break;
                case 3:
                    para_id = MIB_HW_WLAN_ADDR3;
                    break;
                case 4:
                    para_id = MIB_HW_WLAN_ADDR4;
                    break;
            }
            apmib_get(para_id, (void *)tmpBuff);
            sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x",
                        (unsigned char)tmpBuff[0],
                        (unsigned char)tmpBuff[1],
                        (unsigned char)tmpBuff[2],
                        (unsigned char)tmpBuff[3],
                        (unsigned char)tmpBuff[4],
                        (unsigned char)tmpBuff[5]);
        if(vwlan_idx)    
		snprintf(wlan_name, sizeof(wlan_name), "wlan%d-va%d", wlan_idx, vwlan_idx-1 );
	else
            snprintf(wlan_name, sizeof(wlan_name), "wlan%d", wlan_idx);
            //printf( "JBB AEI MAC: %s - %s\n", wlan_name, cmdBuffer );
            RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "down", NULL_STR);
            RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "hw", "ether", cmdBuffer, NULL_STR);
        }
    }
}
#endif
//#ifdef AEI_WECB
int get_interfaces_list(unsigned int name_len, char* key, char* key2, char *list_buf, unsigned int buf_len)
{
	int iface_num=0;
	FILE *fp = NULL;
	char buf[512];
	const char *delimiter = " :\t";
	char *s;
	const char *PATH_PROCNET_DEV = "/proc/net/dev";

	do{
		if(NULL==key||0==name_len||NULL==list_buf||0==buf_len){
			break;
		}

		memset(list_buf, 0, buf_len);
		
		fp = fopen(PATH_PROCNET_DEV, "r");
		if(NULL==fp){
			perror("fopen");
			break;
		}
		fgets(buf, sizeof buf, fp); /* eat line */
		fgets(buf, sizeof buf, fp);
		while (fgets(buf, sizeof buf, fp)) {
			s = strtok(buf, delimiter);
			if(strstr(s, key)==s
				&& (NULL==key2||strstr(s, key2))
				&&strlen(s)==name_len
				&&isdigit(s[name_len-1])){
				snprintf(list_buf+strlen(list_buf), buf_len-strlen(list_buf), "%s%s", iface_num?" ":"", s);
				iface_num++;
			}
		}
	}while(0);
	printf("list_buf=[%s]\n", list_buf);

	if(NULL!=fp){
		fclose(fp);
	}
	
	return iface_num;
}
//#endif

int setinit(int argc, char** argv)
{
	int i, cmdRet=-1;
	int intValue=0, intValue1=0;
	char cmdBuffer[100], tmpBuff[512];
	char *token=NULL, *savestr1=NULL;
	char tmp_args[16];
	char Ip[32], Mask[32], Gateway[32];
	char strPID[32];
	int pid = 0;
	int wlan_support = 0;
	int index; 
	int old_wlan_idx;
	int repeater_enable1=0;
	int repeater_enable2=0;
	int reinit=1;


#ifdef AEI_WECB
    if( (!strcmp(argv[2], "aei")) && (!strcmp(argv[3], "mac"))) {
#ifdef AEI_CTL_BRIDGE
        init_mac();
#endif
		up_mib_value();
		RunSystemCmd(REINIT_FILE, "echo", "1", NULL_STR);
		RunSystemCmd("/var/sys_op", "echo", "1", NULL_STR);
		RunSystemCmd("/proc/sys/net/ipv4/ip_forward", "echo", "0", NULL_STR);
		RunSystemCmd(PROC_BR_IGMPPROXY, "echo", "0", NULL_STR);
		/*cleanup hardware tables*/
		RunSystemCmd(HW_NAT_FILE, "echo", "2", NULL_STR);	/*bridge mode*/
		
		RunSystemCmd("/proc/br_wlanblock", "echo","0",NULL_STR);

		RunSystemCmd(NULL_FILE, "iptables", "-F", NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-F", "-t", "nat",  NULL_STR);
#if defined(AEI_FIREWALL)
		RunSystemCmd(NULL_FILE, "iptables", "-P", "INPUT", "ACCEPT", NULL_STR);
#else
		RunSystemCmd(NULL_FILE, "iptables", "-A", "INPUT", "-j", "ACCEPT", NULL_STR);
#endif
		RunSystemCmd(NULL_FILE, "rm", "-f", "/var/eth1_ip", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", "/var/ntp_run", NULL_STR);

		//set kthreadd high priority for performance
		RunSystemCmd(NULL_FILE, "renice -20 2", NULL_STR);
		//set ksoftirqd high priority for performance
		RunSystemCmd(NULL_FILE, "renice -20 3", NULL_STR);
		//set boa high priority
		if(isFileExist(WEBS_PID_FILE)){
			pid=getPid_fromFile(WEBS_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "renice", "-20", strPID, NULL_STR);
			}
		}

		RunSystemCmd("/proc/custom_Passthru", "echo", (intValue & 0x1)?"1":"0", NULL_STR);
		/*enable igmp snooping*/
		/*igmp snooping is independent with igmp proxy*/
#ifdef AEI_IGMPV3
		RunSystemCmd(PROC_BR_IGMPVERSION, "echo", "3", NULL_STR);
#else
		RunSystemCmd(PROC_BR_IGMPVERSION, "echo", "2", NULL_STR);
#endif
		RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "1", NULL_STR);
#ifdef AEI_DISABLE_IGMP_QUERY
		RunSystemCmd(PROC_BR_IGMPQUERY, "echo", "0", NULL_STR);
#else
		RunSystemCmd(PROC_BR_IGMPQUERY, "echo", "1", NULL_STR);
#endif

        /* QA-Bug #177702: IPTV cannot get video behind WCB3000N */
		RunSystemCmd(PROC_BR_MLDSNOOP, "echo", "1", NULL_STR);
#if !defined(AEI_DEF_DISABLE_IPV6)
		RunSystemCmd(PROC_BR_MLDQUERY, "echo", "1", NULL_STR);
#endif
		/*increase routing cache rebuild count from 4 to 2048*/
		RunSystemCmd(RT_CACHE_REBUILD_COUNT, "echo", "2048", NULL_STR);
		return 0;
    }
#endif

//step 1: 802.1x client support
#ifdef CONFIG_RTL_802_1X_CLIENT_SUPPORT
	if((isFileExist(RS_USER_CERT_5G)==0) && (isFileExist(RS_ROOT_CERT_5G)==0) &&  (isFileExist(RS_USER_CERT_2G)==0) && (isFileExist(RS_ROOT_CERT_2G)==0)){
		RunSystemCmd(NULL_FILE, "rsCert","-rd", NULL_STR);
	}
#endif
	printf("Init Start...\n");

//step 2		
	num_wlan_interface=get_interfaces_list(5, "wlan", NULL, wlan_interface, sizeof(wlan_interface));
	num_wlan_virtual_interface=get_interfaces_list(9, "wlan", "-va", wlan_virtual_interface, sizeof(wlan_virtual_interface));
	
/*currently, we just support init gw/ap all */	
	
//		clean_process(BRIDGE_MODE,0,1, 1, "br0", wlan_interface, "eth1");
	RunSystemCmd(HW_NAT_FILE, "echo", "2", NULL_STR);	/*bridge mode*/
	RunSystemCmd(NULL_FILE, "killall", "-9", "reload", NULL_STR);


	/*init wlan interface*/
//ifconfig rootap down && flash set_mib
		for(i=0;i<NUM_WLAN_INTERFACE;i++)
		{
			int wlan_disable = 1;
			unsigned char wlan_name[10];
			memset(wlan_name,0x00,sizeof(wlan_name));
			int j;
			for(j=0; j<NUM_VWLAN; j++){
				sprintf(wlan_name, "wlan%d-va%d",i, j);
				RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "down", NULL_STR);
			}
			sprintf(wlan_name, "wlan%d",i);
			apmib_save_wlanIdx();
			if(SetWlan_idx(wlan_name))
			{			
				apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disable);	  
				if(wlan_disable == 1)
				{
					RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "down", NULL_STR);
					RunSystemCmd(NULL_FILE, "iwpriv", wlan_name, "radio_off", NULL_STR);					
				}
				else
				{
						
					RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "down", NULL_STR);
					cmdRet=RunSystemCmd(NULL_FILE, "flash", "set_mib", wlan_name, NULL_STR);
			
					if(cmdRet != 0)
					{
						printf("init %s failed!\n", wlan_name);
						continue;
					}

				}
			}
			apmib_recov_wlanIdx();
		}
		

//ifconfig vap down && flash set_mib
	if(wlan_interface[0]){				
		if(wlan_virtual_interface[0]){
			token=NULL;
			savestr1=NULL;
			sprintf(tmpBuff, "%s", wlan_virtual_interface);
			token = strtok_r(tmpBuff," ", &savestr1);
			do{
				if (token == NULL){
					break;
				}else{
					RunSystemCmd(NULL_FILE, "ifconfig", token, "down", NULL_STR);
					RunSystemCmd(NULL_FILE, "flash", "set_mib", token, NULL_STR);/*set virtual wlan iface*/
				}
				token = strtok_r(NULL, " ", &savestr1);
			}while(token !=NULL);
		}
	}	
///////////////////////////////////////////////////////////	
			//set_br_interface(tmpBuff);
	sprintf(tmpBuff, "%s %s", wlan_interface, wlan_virtual_interface);
	setbridge(tmpBuff);//ifconfig wlanx up or not

	sprintf(br_interface, "%s", "br0");
	start_wlanapp(1);

	system("reload -k /var/wlsch.conf &");

#if defined(CONFIG_IPV6)
	set_ipv6(BRIDGE_MODE);
#endif
	return 0;
}

void Init_Internet(int argc, char** argv)
{
#ifdef   HOME_GATEWAY	
	int wisp_id=0, wan_mode=0, opmode=0;
	char br_interface[16]={0};
	char wan_interface[16]={0};
	char tmp_args[16]={0};
	
	
	if(argc < 4)
		return;
		
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	if(opmode == BRIDGE_MODE)
		return;
	
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_id);
	
	sprintf(tmp_args, "%s",argv[1]);
	if(!strcmp(tmp_args, "pppoe"))
		wan_mode=3;
	else if(!strcmp(tmp_args, "pptp"))
		wan_mode=4;	
	else if(!strcmp(tmp_args, "l2tp"))
		wan_mode=6;
	else{
		printf("Un-support wan type for init\n");
		return;
	}
	sprintf(br_interface, "%s", "br0");
	sprintf(wan_interface, "%s",argv[3]);
	start_wan(wan_mode, opmode, wan_interface, br_interface, wisp_id, 0);		
#endif	
}
 
void Init_QoS(int argc, char** argv)
{
#ifdef   HOME_GATEWAY	
	int wisp_id=0, wan_mode=0, opmode=0;
	
	
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	if(opmode == 1)
		return;
		
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_id);
	apmib_get(MIB_WAN_DHCP,(void *)&wan_mode);		
	set_QoS(opmode, wan_mode, wisp_id);
#endif	
	
} 
 
#ifdef CONFIG_SMART_REPEATER
int getWispRptIfaceName(char*pIface,int wlanId)
{
	int rptEnabled=0,wlanMode=0,opMode=0;
	char wlan_wanIfName[16]={0};
	if(wlanId == 0)
		apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
	else if(1 == wlanId)
		apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);
	else return -1;
	apmib_get(MIB_OP_MODE,(void *)&opMode);
	if(opMode!=WISP_MODE)
		return -1;
	apmib_save_wlanIdx();

	sprintf(wlan_wanIfName,"wlan%d",wlanId);
	SetWlan_idx(wlan_wanIfName);
	//for wisp rpt mode,only care root ap
	apmib_get(MIB_WLAN_MODE, (void *)&wlanMode);
	if(AP_MODE==wlanMode && rptEnabled)
	{//root AP mode and rpt enabled, use -vxd as wanIf
		if(!strstr(pIface,"-vxd"))
		{
			strcat(pIface,"-vxd");
		}
	}else
	{
		char * ptmp = strstr(pIface,"-vxd");
		if(ptmp)
			memset(ptmp,0,sizeof(char)*strlen("-vxd"));
	}
	apmib_recov_wlanIdx();
	return 0;
}
#endif
 
 
 
 
 
