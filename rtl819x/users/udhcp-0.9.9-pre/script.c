/* script.c
 *
 * Functions to call the DHCP client notification scripts 
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "script.h"
#include "options.h"
#include "dhcpd.h"
#include "dhcpc.h"
#include "packet.h"
#include "options.h"
#include "debug.h"

#ifdef AEI_WECB
#if defined(AEI_PROBE_SUBNET)
#include "arpping.h"
#endif
#include "libtr69_client.h"

static char local_ip[32]="";
static char router_ip[32]="";
static char dns_ip[128]="";
static char subnet_ip[32]="";
extern int g_lease;
#endif

/* get a rough idea of how long an option will be (rounding up...) */
static int max_option_length[] = {
	[OPTION_IP] =		sizeof("255.255.255.255 "),
	[OPTION_IP_PAIR] =	sizeof("255.255.255.255 ") * 2,
	[OPTION_STRING] =	1,
	[OPTION_BOOLEAN] =	sizeof("yes "),
	[OPTION_U8] =		sizeof("255 "),
	[OPTION_U16] =		sizeof("65535 "),
	[OPTION_S16] =		sizeof("-32768 "),
	[OPTION_U32] =		sizeof("4294967295 "),
	[OPTION_S32] =		sizeof("-2147483684 "),
};


static int upper_length(int length, struct dhcp_option *option)
{
	return max_option_length[option->flags & TYPE_MASK] *
	       (length / option_lengths[option->flags & TYPE_MASK]);
}


static int sprintip(char *dest, char *pre, unsigned char *ip)
{
	return sprintf(dest, "%s%d.%d.%d.%d", pre, ip[0], ip[1], ip[2], ip[3]);
}


/* really simple implementation, just count the bits */
static int mton(struct in_addr *mask)
{
	int i;
	unsigned long bits = ntohl(mask->s_addr);
	/* too bad one can't check the carry bit, etc in c bit
	 * shifting */
	for (i = 0; i < 32 && !((bits >> i) & 1); i++);
	return 32 - i;
}


/* Fill dest with the text of option 'option'. */
static void fill_options(char *dest, unsigned char *option, struct dhcp_option *type_p)
{
	int type, optlen;
	u_int16_t val_u16;
	int16_t val_s16;
	u_int32_t val_u32;
	int32_t val_s32;
	int len = option[OPT_LEN - 2];
#ifdef AEI_WECB
    char tmp[128]="";
#endif

	dest += sprintf(dest, "%s=", type_p->name);

	type = type_p->flags & TYPE_MASK;
	optlen = option_lengths[type];
	for(;;) {
		switch (type) {
		case OPTION_IP_PAIR:
			dest += sprintip(dest, "", option);
			*(dest++) = '/';
			option += 4;
			optlen = 4;
		case OPTION_IP:	/* Works regardless of host byte order. */
			dest += sprintip(dest, "", option);
#ifdef AEI_WECB
            sprintip(tmp, "",option);
            if (!strcmp(type_p->name, "dns")) {
                if (strlen(dns_ip) > 0) {
                    strcat(dns_ip, ",");
                }
                strcat(dns_ip, tmp);
            } else if (!strcmp(type_p->name, "router")) {
                strcpy(router_ip, tmp);
            } else if (!strcmp(type_p->name, "subnet")) {
                strcpy(subnet_ip, tmp);
            }
#endif
 			break;
		case OPTION_BOOLEAN:
			dest += sprintf(dest, *option ? "yes" : "no");
			break;
		case OPTION_U8:
			dest += sprintf(dest, "%u", *option);
			break;
		case OPTION_U16:
			memcpy(&val_u16, option, 2);
			dest += sprintf(dest, "%u", ntohs(val_u16));
			break;
		case OPTION_S16:
			memcpy(&val_s16, option, 2);
			dest += sprintf(dest, "%d", ntohs(val_s16));
			break;
		case OPTION_U32:
			memcpy(&val_u32, option, 4);
			dest += sprintf(dest, "%lu", (unsigned long) ntohl(val_u32));
			break;
		case OPTION_S32:
			memcpy(&val_s32, option, 4);
			dest += sprintf(dest, "%ld", (long) ntohl(val_s32));
			break;
		case OPTION_STRING:
			memcpy(dest, option, len);
			dest[len] = '\0';
			return;	 /* Short circuit this case */
		}
		option += optlen;
		len -= optlen;
		if (len <= 0) break;
		dest += sprintf(dest, " ");
	}
}


static char *find_env(const char *prefix, char *defaultstr)
{
	extern char **environ;
	char **ptr;
	const int len = strlen(prefix);

	for (ptr = environ; *ptr != NULL; ptr++) {
		if (strncmp(prefix, *ptr, len) == 0)
			return *ptr;
	}
	return defaultstr;
}


/* put all the paramaters into an environment */
static char **fill_envp(struct dhcpMessage *packet)
{
	int num_options = 0;
	int i, j;
	char **envp;
	unsigned char *temp;
	struct in_addr subnet;
	char over = 0;

	if (packet == NULL)
		num_options = 0;
	else {
		for (i = 0; options[i].code; i++)
			if (get_option(packet, options[i].code))
				num_options++;
		if (packet->siaddr) num_options++;
		if ((temp = get_option(packet, DHCP_OPTION_OVER)))
			over = *temp;
		if (!(over & FILE_FIELD) && packet->file[0]) num_options++;
		if (!(over & SNAME_FIELD) && packet->sname[0]) num_options++;		
	}
	
	envp = xmalloc((num_options + 5) * sizeof(char *));
	j = 0;
	envp[j++] = xmalloc(sizeof("interface=") + strlen(client_config.interface));
	sprintf(envp[0], "interface=%s", client_config.interface);
	envp[j++] = find_env("PATH", "PATH=/bin:/usr/bin:/sbin:/usr/sbin");
	envp[j++] = find_env("HOME", "HOME=/");

	if (packet == NULL) {
		envp[j++] = NULL;
		return envp;
	}

	envp[j] = xmalloc(sizeof("ip=255.255.255.255"));
#if !defined(AEI_WECB)
	sprintip(envp[j++], "ip=", (unsigned char *) &packet->yiaddr);
#else
    u_int32_t free_addr = packet->yiaddr;
#if defined(AEI_PROBE_SUBNET)
    unsigned char * free_ip = (unsigned char *) &free_addr;
    u_int32_t try_addr = packet->yiaddr;
    unsigned char * try_ip = (unsigned char *) &try_addr;

    u_int8_t intf_id_pool_base = 254;
    u_int8_t intf_id_size = INTF_ID_POOL_SIZE;
    u_int8_t try_intf_id = 0;

    u_int8_t free_intf_id = free_ip[3];
    char ret_hwaddr[6] = {0};
    char * def_ip = NULL;
    int type;

    do {
#ifdef AEI_WECB
        // Using default ip as the base of pool
        if( tr69_get_unfresh_leaf_data( "Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultIPAddress",
                                    (void *) &def_ip, &type) >= 0) {
            struct in_addr addr;

            if( def_ip ) {
                if (inet_aton(def_ip, &addr)) {
                    unsigned char * ip = (unsigned char *) &addr.s_addr;
                    intf_id_pool_base = ip[3];
                    LOG(LOG_DEBUG, "intf_id_pool_base = %d", intf_id_pool_base );
                }
                free(def_ip);
            }
        }
#endif
        // pool size cannot be too small or too large
        if( (intf_id_size < MIN_INTF_ID_POOL_SIZE) || (intf_id_size > MAX_INTF_ID_POOL_SIZE)) {
            intf_id_size = DEF_INTF_ID_POOL_SIZE;
        }
/*
        // If the addr assigned by DHCP server is in pool, just use it.
        // Unfortunately, BHR2 DHCPd has bug. When detect whether address free,
        // BHR should not send 'ICMP.Ping.Request' but ARP as what BRCM do.
        // To support BHR2, we must detect by arping ourself here.
        if( (free_intf_id <= intf_id_pool_base) && (free_intf_id > (intf_id_pool_base-intf_id_size))) {
            break;
        }
*/
        try_intf_id = intf_id_pool_base;
        while( (try_intf_id >= 1) && (try_intf_id > (intf_id_pool_base - intf_id_size))) {
            // detect addr by arping
            try_ip[3] = try_intf_id;
            int rv = arpping(try_addr, 0, client_config.arp, client_config.interface, &ret_hwaddr[0]);
            LOG(LOG_INFO, "arpping(%d) return %d", try_intf_id, rv );
            if( 1 == rv ) {
                // addr free
                free_intf_id = try_intf_id;
                break;
            }
            // addr used, try next one
            try_intf_id --;
        }
    } while(0);
    // Using the free intf id to generate ip addr
    if( free_intf_id != free_ip[3] ) {
        free_ip[3] = free_intf_id;
    }
#endif
    sprintip(envp[j++], "ip=", (unsigned char *) &free_addr);
    sprintip(local_ip, "", (unsigned char *) &free_addr);
    strcpy(dns_ip, "");
#endif

	for (i = 0; options[i].code; i++) {
		if (!(temp = get_option(packet, options[i].code)))
			continue;

		envp[j] = xmalloc(upper_length(temp[OPT_LEN - 2], &options[i]) + strlen(options[i].name) + 2);
		fill_options(envp[j++], temp, &options[i]);

		/* Fill in a subnet bits option for things like /24 */
		if (options[i].code == DHCP_SUBNET) {
			envp[j] = xmalloc(sizeof("mask=32"));
			memcpy(&subnet, temp, 4);
			sprintf(envp[j++], "mask=%d", mton(&subnet));
		}
	}
	if (packet->siaddr) {
		envp[j] = xmalloc(sizeof("siaddr=255.255.255.255"));
		sprintip(envp[j++], "siaddr=", (unsigned char *) &packet->siaddr);
	}
	if (!(over & FILE_FIELD) && packet->file[0]) {
		/* watch out for invalid packets */
		packet->file[sizeof(packet->file) - 1] = '\0';
		envp[j] = xmalloc(sizeof("boot_file=") + strlen(packet->file));
		sprintf(envp[j++], "boot_file=%s", packet->file);
	}
	if (!(over & SNAME_FIELD) && packet->sname[0]) {
		/* watch out for invalid packets */
		packet->sname[sizeof(packet->sname) - 1] = '\0';
		envp[j] = xmalloc(sizeof("sname=") + strlen(packet->sname));
		sprintf(envp[j++], "sname=%s", packet->sname);
	}	
	envp[j] = NULL;
#ifdef AEI_WECB
//	  sendEventMessage(TRUE, local_ip, subnet_ip, router_ip, dns_ip, g_lease );
#endif
	return envp;
}


/* Call a script with a par file and env vars */
void run_script(struct dhcpMessage *packet, const char *name)
{
	int pid;
	char **envp;

	if (client_config.script == NULL)
		return;

	/* call script */
	pid = fork();

#ifdef AEI_WECB
	envp = fill_envp(packet);
#endif

	if (pid) {
		waitpid(pid, NULL, 0);

#ifdef AEI_WECB
    DEBUG(LOG_INFO, "client_config.script=%s, name=%s", client_config.script, name);
    DEBUG(LOG_INFO, "udhcp get local_ip=%s, router_ip=%s ", local_ip, router_ip);
    if (strlen(local_ip) > 0 ) {
         char cmd[256]="";
        //snprintf(cmd,sizeof(cmd),"arping -f -I br0 -s %s %s",local_ip, router_ip);
        snprintf(cmd,sizeof(cmd),"ping %s -c 2", router_ip);
        LOG(LOG_ERR,"%s",cmd);
        system(cmd);
        sleep(3);
        sendEventMessage(TRUE, local_ip, subnet_ip, router_ip, dns_ip, g_lease );
        }
#endif

		return;
	} else if (pid == 0) {
#ifndef AEI_WECB
		envp = fill_envp(packet);
#endif
		
		/* close fd's? */
		
		/* exec script */
		DEBUG(LOG_INFO, "execle'ing %s", client_config.script);
		execle(client_config.script, client_config.script,
		       name, NULL, envp);
		LOG(LOG_ERR, "script %s failed: %s",
		    client_config.script, strerror(errno));
		exit(1);
	}			
}
