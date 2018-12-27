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


#define CHUNK 128	
#define IDLE_TIMEOUT 60

struct fdb_entry
{
	__u8 mac_addr[6];
	short port_no;
	unsigned char is_local;
	unsigned long ageing_timer_value;
};

int actbr_socket_fd = -1;


int getMacByIntf(char *intf, char *macaddr)
{
	 int fd;
	 struct ifreq ifr;

	 fd = socket(AF_INET, SOCK_DGRAM, 0);

	 ifr.ifr_addr.sa_family = AF_INET;
	 strncpy(ifr.ifr_name, intf, IFNAMSIZ - 1);

	 ioctl(fd, SIOCGIFHWADDR, &ifr);

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

int actbr_init(void)
{
	if ((actbr_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return errno;
	return 0;
}

static inline void __actcopy_fdb(struct fdb_entry *ent, const struct __fdb_entry *f)
{
	memcpy(ent->mac_addr, f->mac_addr, 6);
	ent->port_no = f->port_no;
	ent->is_local = f->is_local;
	ent->ageing_timer_value= f->ageing_timer_value;
}

int actbr_read_fdb(const char *bridge, struct fdb_entry *fdbs, 	unsigned long offset, int num)
{
	struct __fdb_entry fe[num];
	int n,i;
	unsigned long args[4] = { BRCTL_GET_FDB_ENTRIES,
				  (unsigned long) fe,
				  num, offset };
	struct ifreq ifr;
	int retries = 0;

	strncpy(ifr.ifr_name, bridge, IFNAMSIZ);
	ifr.ifr_data = (char *) args;
	
retry:
	n = ioctl(actbr_socket_fd, SIOCDEVPRIVATE, &ifr);
	if (n < 0 && errno == EAGAIN && ++retries < 10) {
		sleep(0);
		goto retry;
	}
	
	for (i = 0; i < n; i++) 
		__actcopy_fdb(fdbs+i, fe+i);

	return n;
}

/*
   if mac in arp table, return 1
   if not, return 0
*/
static int read_arp(char* dev_mac,char *device_ip)
{
	int fd;
	int res;
	char buf[1024];
	char mac_buf[20];
	char tmp_ip[16];

    memset (mac_buf, 0, sizeof (mac_buf));
    memset (tmp_ip, 0, sizeof (tmp_ip));
    strncpy (mac_buf, dev_mac, sizeof (mac_buf) - 1);
    tolower_string (mac_buf);

	fd = open("/proc/net/arp", O_RDONLY);
	if (fd<0)   return 0;
	res = read(fd, buf, sizeof(buf)-1);
	close(fd);

	if (res <= 0) return 0;	

	char* pos=NULL;
	char* ptr=buf;
	while ( ( (pos = strchr(ptr, '\n')) || (pos = strchr(ptr, '\r')) )
		&& (ptr-buf<res) ) 
	{
		char *ip,*hw_type, *hw_flags, *mac, *mask, *device;
		*pos = 0;
		ip  = strtok(ptr,  " \t\n");
		if (!safe_strcmp (ip, "IP")) { //skip first  line
			ptr = pos + 1;
			continue;
		}
		
		hw_type = strtok(NULL,  " \t\n");
		hw_flags  = strtok(NULL,  " \t\n");
		mac = strtok(NULL,  " \t\n");
		mask = strtok(NULL,  " \t\n");
		device = strtok(NULL, " \t\n");

		if (strcmp(device, "br0") )
		    continue;
		
		tolower_string (mac);
		if (!safe_strcmp (mac,mac_buf))
		{
			strcpy(device_ip,ip);
			return 1;
		}
		ptr = pos+1;
	}
	return 0;
}

int getStatusList_bak(request *wp, char *type)
{
	int nBytesSent = 0;	
	const char *brname = "br0";
	struct fdb_entry *fdb = NULL;
	int offset = 0;
	int n,i;
	int fd;
	int count =0;
	int j;	

	kinfo("type: %s", type);
		
	i = actbr_init();

	if (i){
		kerror("br_init failed.");
		return ;
	}
	
	fdb = realloc(fdb, (offset + CHUNK) * sizeof(struct fdb_entry));
	if (!fdb) {
		kerror("realloc fdb failed.");
	}
	
	n = actbr_read_fdb(brname, fdb+offset, offset, CHUNK);
	  
	if (n <= 0) // it is impossible to get zero items in bridge
	{
	   free(fdb);
	   kerror("read fdb failed,result:%d.", n);
	   return;
	}
	  
	offset += n;
	int first_flag = 1;
	  
	for (i = 0; i < offset; i++) 
	{
		const struct fdb_entry *f = fdb + i;
		char mac_buf[20];
		char device_name[60];
		char temp[1024];
		
		memset(device_name,0,60);
		char device_ip[16];
		int idle=0;
		memset(device_ip,0, sizeof(device_ip));
		memset(mac_buf,0, sizeof(mac_buf));
		memset(temp,0, sizeof(temp));
		
		if (!f->is_local)
		{ // only process LAN nodes
		
		  sprintf(mac_buf, "%02x:%02x:%02x:%02x:%02x:%02x",
			  f->mac_addr[0], f->mac_addr[1], f->mac_addr[2],
			  f->mac_addr[3], f->mac_addr[4], f->mac_addr[5]);
		   
		  if (f->ageing_timer_value/HZ<=IDLE_TIMEOUT)
			idle=1;
		  else
			idle=0;
		  
		  int port_no = f->port_no;
		  
		  if(!read_arp(mac_buf, device_ip))
		  {		
			strcpy(device_ip,"N/A");
		  }
			
		  nBytesSent += req_format_write(wp, "%s", "<tr>\n");
		  if (port_no==3)
			nBytesSent += req_format_write(wp, "%s", "<td class=\"left_cell\" id=\"wrls_on\"></td>\n");
		  else
			nBytesSent += req_format_write(wp, "%s", "<td class=\"left_cell\" id=\"eth_on\"></td>\n");
	
		  nBytesSent += req_format_write(wp, "<td class=\"mid_cell\" id=\"client_name_on\">%s</td>\n", mac_buf);
		  nBytesSent += req_format_write(wp, "%s", "<td class=\"right_cell\" id=\"client_connected\">\n");
		  nBytesSent += req_format_write(wp, "%s", "<p class=\"client_ip\">Connected</p>\n");
		  nBytesSent += req_format_write(wp, "<p class=\"client_ip\">%s</p>\n", device_ip);
		  
		  if (port_no==3)
			nBytesSent += req_format_write(wp, "%s", "<p class=\"client_data\">Via Wireless</p>\n");
		  else if (port_no==1)
			nBytesSent += req_format_write(wp, "%s", "<p class=\"client_data\">Via Coax</p>\n");
		  else 
			nBytesSent += req_format_write(wp, "%s", "<p class=\"client_data\">Via Ethernet</p>\n");
	
		  nBytesSent += req_format_write(wp, "%s", "</td>\n");
		  nBytesSent += req_format_write(wp, "%s", "</tr>\n");		  
		}
	  }
	
	  free(fdb);
	  close(actbr_socket_fd);
	  actbr_socket_fd = -1;
	  return nBytesSent;	
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
	
	tr69GetUnfreshLeafData(buf, "Device.Hosts", "HostNumberOfEntries");
	cutGetTR69(buf);
	host_entries = atoi(buf);

	kinfo("host_entries: %d", host_entries);

	for(i = 1; i <= host_entries; i++)
	{
		sprintf(full_name, "Device.Hosts.Host.%d", i);

		tr69GetUnfreshLeafData(ip_address, full_name, "IPAddress");
		cutGetTR69(ip_address);
		
		tr69GetUnfreshLeafData(mac_address, full_name, "PhysAddress");
		cutGetTR69(mac_address);
		
		tr69GetUnfreshLeafData(interface_type, full_name, "X_ACTIONTEC_COM_InterfaceType");
		cutGetTR69(interface_type);

		tr69GetUnfreshLeafData(buf, full_name, "Active");
		cutGetTR69(buf);
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
				
	return nBytesSent;
}


