#ifndef _ACT_L3_LOCAL_H_
#define _ACT_L3_LOCAL_H_

#include <netinet/in.h>

typedef struct {
	char * ifname;
        int family;
	int type;
#if 0
        int index;
        int state;
        int unused_only;
#ifdef _P_WCB_
        inet_prefix pfx;
#endif
        int flushed;
        char *flushb;
        int flushp;
        int flushe;
#endif
} L3lo_filter;


#define IPV6_UNKNOWN_TYPE 	(0x0)
#define IPV6_UNSPEC 		(~0x0)
#define IPV6_HLA 		(0x01) 		// Host Local Address
#define IPV6_LLA 		(0x01<<1) 	// Link Local Address
#define IPV6_ULA 		(0x01<<2) 	// Unique Local Address
#define IPV6_GUA 		(0x01<<3) 	// Global Unique Address

typedef struct {
	int family;
	struct in6_addr ipAddr;
	char ipAddrStr[INET6_ADDRSTRLEN +1];
	int type;
} L3Addr_t;

int getL3LocalList( L3Addr_t * ipList, const int maxcnt, const L3lo_filter *filter );

#endif 
