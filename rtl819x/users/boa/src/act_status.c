/*
 *      Status Function Implementation  
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_status.h"
#include <stdio.h>
#include <ctype.h>         /* for isspace           */
#include <string.h>
#include <stdlib.h>        /* for malloc            */
#include <time.h>
#include <unistd.h>        /* for close             */
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>    /* for connect and socket*/
#include <netinet/in.h>    /* for sockaddr_in       */
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>         /* for open modes        */

#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <net/route.h>
#include <linux/if_bridge.h>
#include <asm/param.h>
#include <arpa/inet.h>

extern error_buf[];

int getMacByIntf(char *intf, char *macaddr)
{
	 int fd;
	 struct ifreq ifr;

	 kinfo("Enter, intf: %s", intf);

	 fd = socket(AF_INET, SOCK_DGRAM, 0);

	 ifr.ifr_addr.sa_family = AF_INET;
	 strncpy(ifr.ifr_name, intf, IFNAMSIZ - 1);

	 if(ioctl(fd, SIOCGIFHWADDR, &ifr))
	 {
		kerror("iotcl SIOCGIFHWADDR Error");
		return 1;
	 }

	 if(fd)
	 	close(fd);

	 if(ifr.ifr_hwaddr.sa_data != NULL)
	 {
		 /* display result */		
		 sprintf(macaddr,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
		         (unsigned char)ifr.ifr_hwaddr.sa_data[0],
		         (unsigned char)ifr.ifr_hwaddr.sa_data[1],
		         (unsigned char)ifr.ifr_hwaddr.sa_data[2],
		         (unsigned char)ifr.ifr_hwaddr.sa_data[3],
		         (unsigned char)ifr.ifr_hwaddr.sa_data[4],
		         (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
	 }
	 return 0;
}

int getIPv6Addr(char *intf, char *ipv6addr)
{
	L3Addr_t ips[10] = {0};
	int cnt = 0;
	int i = 0;
	char buf[100] = {0};

	kinfo("Enter");

	if(!safe_strcmp(intf, "br0"))
		cnt = getL3LocalList(ips, 10, NULL);

	kinfo("ipaddr (totally %d)", cnt);
	for(i = 0; i < cnt; i++) {
		kinfo("%s, type(%d)", ips[i].ipAddrStr, ips[i].type);

		changeString(ips[i].ipAddrStr);
		
		sprintf(buf, "%d|%s||", ips[i].type, ips[i].ipAddrStr);

		strcat(ipv6addr, buf);
	}
	
	return 0;		
}

void getStatusValue(char *type, char *retvalue)
{
  if (!safe_strcmp(type, "ethmac")) 
  {
    getMacByIntf("eth0", retvalue);    
  }
  else if (!safe_strcmp(type, "wlan0"))
  {
  	getMacByIntf("wlan0-va0", retvalue);
  } 
  else if (!safe_strcmp(type, "wlan1"))
  {
  	getMacByIntf("wlan1-va0", retvalue);
  }  
  else if (!safe_strcmp(type, "mocamac")) 
  {
  	getMacByIntf("eth0", retvalue);
  }
  else if (!safe_strcmp(type, "version"))
  {
  	strcpy(retvalue, "10.1.1");
  }
  else if (!safe_strcmp(type, "ipv6addr"))
  {
	 getIPv6Addr("br0", retvalue);
  }
  
  kinfo("%s -> %s", type, retvalue);
}

int getStatusList(request *wp, char *status_type)
{	
	char buf[256] = {0};
	char full_name[256] = {0};
	int host_entries = 0;
	int i = 0;
	int nBytesSent = 0;	
	char ip_address[20] = {0};	
	char mac_address[20] = {0};	
	char interface_type[20] = {0};
	int active = 0;
	
	kinfo("status_type: %s", status_type);	
	
	if(tr69GetUnfreshLeafData(buf, "Device.Hosts", "HostNumberOfEntries"))
    {
        strcpy(error_buf, "Get Device.Hosts.HostNumberOfEntries error");
	    return 0;
    }
	host_entries = atoi(buf);

	kinfo("host_entries: %d", host_entries);

	for(i = 1; i <= host_entries; i++)
	{
		sprintf(full_name, "Device.Hosts.Host.%d", i);

		if(tr69GetUnfreshLeafData(ip_address, full_name, "IPAddress"))
        {
            sprintf(error_buf, "Get %s.IPAddress error", full_name);
    	    break;
        }   
        
		if(tr69GetUnfreshLeafData(mac_address, full_name, "PhysAddress"))
        {
            sprintf(error_buf, "Get %s.PhysAddress error", full_name);
    	    break;
        }  
        
		if(tr69GetUnfreshLeafData(interface_type, full_name, "X_ACTIONTEC_COM_InterfaceType"))
        {
            sprintf(error_buf, "Get %s.X_ACTIONTEC_COM_InterfaceType error", full_name);
    	    break;
        }  
        
		if(tr69GetUnfreshLeafData(buf, full_name, "Active"))
        {
            sprintf(error_buf, "Get %s.Active error", full_name);
    	    break;
        }  
		active = atoi(buf);

		if(active)
		{
			nBytesSent += req_format_write(wp, "%s", "<table id=\"tbl_clients\" width=\"80%%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"status_style\" style=\"margin:0px auto 0 auto;\">\n<tr>\n");
			
			if(!safe_strcmp(interface_type, "Wireless"))
				nBytesSent += req_format_write(wp, "%s", "<td class=\"left_cell\" id=\"wrls_on\"></td>\n");
			else
				nBytesSent += req_format_write(wp, "%s", "<td class=\"left_cell\" id=\"eth_on\"></td>\n");

			nBytesSent += req_format_write(wp, "<td class=\"mid_cell\" id=\"client_name_on\">%s</td>\n", mac_address);
			nBytesSent += req_format_write(wp, "%s", "<td class=\"right_cell\" id=\"client_connected\">\n");
			nBytesSent += req_format_write(wp, "%s", "<p class=\"client_ip\">Connected</p>\n");
			nBytesSent += req_format_write(wp, "<p class=\"client_ip\">%s</p>\n", ip_address);		  
			nBytesSent += req_format_write(wp, "<p class=\"client_data\">Via %s</p>\n", interface_type);
			
			nBytesSent += req_format_write(wp, "%s", "</td>\n</tr>\n</table>\n");
		}
	}

	if(nBytesSent==0)
        {
        req_format_write(wp, "%s", "NULL");
        }

	return nBytesSent;
}


