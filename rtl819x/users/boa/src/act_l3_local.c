
#include "act_l3_local.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/types.h>
#include <netinet/in.h>

//#define DEBUG_L3_LOCAL
//#define DEMO_L3_LOCAL

static int getIPv6AddrType(const struct in6_addr *addr)
{
	int type = IPV6_UNKNOWN_TYPE;
	__be32 st;

	do {
		st = addr->s6_addr32[0];

		if ((st & htonl(0xFFFF0000)) == htonl(0xFE800000)) {
			type = IPV6_LLA;
		} else if ((st & htonl(0xFE000000)) == htonl(0xFC000000)) {
			/* RFC 4193 */
			type = IPV6_ULA;
		} else {
			type = IPV6_GUA;
		}
	} while(0);

	return type;
}


static int dump_ip_addrs( L3Addr_t * ipList, const int maxcnt, const L3lo_filter *filter )
{
	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];
	int cnt = 0;
	char *token;
	char *saveptr;
	int type = IPV6_UNKNOWN_TYPE;

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return -1;
	}

	/* Walk through linked list, maintaining head pointer so we
	   can free list later */
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		family = ifa->ifa_addr->sa_family;

		//////////////////////////////////////////////////////
		// skip those unconcerned address by given filter
		if( (filter->family != AF_UNSPEC) && (filter->family != family)) {
			continue;
		}
		if( 0 != strcmp(filter->ifname, ifa->ifa_name)) {
			continue;
		}
		type = IPV6_UNKNOWN_TYPE;
		if( AF_INET6 == family ) {
			type = getIPv6AddrType(&((struct sockaddr_in6 *)(ifa->ifa_addr))->sin6_addr);
			//printf( "type = %d\n", type);
			if( 0 == (filter->type & type )) {
				continue;
			}
		}
		//////////////////////////////////////////////////////

		/* Display interface name and family (including symbolic
		   form of the latter for the common families) */
#ifdef DEBUG_L3_LOCAL
		printf("%s  address family: %d%s\n",
				ifa->ifa_name, family,
				(family == AF_PACKET) ? " (AF_PACKET)" :
				(family == AF_INET) ?   " (AF_INET)" :
				(family == AF_INET6) ?  " (AF_INET6)" : "");
#endif
		/* For an AF_INET* interface address, display the address */

		if (family == AF_INET || family == AF_INET6) {
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) :
					sizeof(struct sockaddr_in6),
					host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				return -1;
			}
#ifdef DEBUG_L3_LOCAL
			printf("\taddress: <%s>\n", host);
#endif
			//	
			// copied data into array entry
			//
			ipList[cnt].family = family;
			//ipList[cnt].ipAddr = 
			token = strtok_r(host, "%", &saveptr);
			if( token == NULL ) {
				snprintf( ipList[cnt].ipAddrStr,
					sizeof(ipList[cnt].ipAddrStr),
					"%s", host );
			} else {
				snprintf( ipList[cnt].ipAddrStr,
					sizeof(ipList[cnt].ipAddrStr),
					"%s", token );
			}
			ipList[cnt].type = type;

			cnt ++;
			if( cnt >= maxcnt ) {
				break;
			}
		}
	}

	freeifaddrs(ifaddr);
	return cnt;
}

L3lo_filter def_filter = { "br0", AF_INET6, IPV6_LLA | IPV6_ULA | IPV6_GUA };

int getL3LocalList( L3Addr_t * ipList, const int maxcnt, const L3lo_filter *filter )
{
	int ret = -1;

	do {
		if( (NULL == ipList) || (maxcnt<=0)) {
			perror( "Invalid input param\n" );
			break;
		}
		if( NULL == filter ) {
			filter = & def_filter;
		}
		ret = dump_ip_addrs( ipList, maxcnt, filter);
	} while(0);

	return ret;
}


#ifdef DEMO_L3_LOCAL
#include <stdio.h>

int main( int argc, char ** argv)
{
	L3Addr_t ips[10];
	L3lo_filter filter = { "eth0", AF_INET6, IPV6_ULA | IPV6_GUA };
	int cnt = 0;
	int i;

	//cnt = getL3LocalList( ips, 10, NULL );
	/* NULL means default filter: ("br0", AF_INET6, IPV6_LLA | IPV6_ULA | IPV6_GUA) */
	cnt = getL3LocalList( ips, 10, & filter );

	printf( "ipaddr (totally %d):\n", cnt );
	for(i=0; i<cnt; i++) {
		printf( "%s, type(%d)\n", ips[i].ipAddrStr, ips[i].type );
	}

	return 0;
}
#endif

