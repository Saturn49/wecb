#ifndef _CTL_NID_H
#define _CTL_NID_H

//
// Ctrl Layer Node ID definitions
//

#define __CTL_NID_PREFIX 	"ctl."


#define NID_CORE 	__CTL_NID_PREFIX"core"


#define NID_TR64C	__CTL_NID_PREFIX"/usr/sbin/tr64"
#define NID_TR69C	__CTL_NID_PREFIX"tr69c"
#define NID_PMD		__CTL_NID_PREFIX"pmd"
#define NID_RTD		__CTL_NID_PREFIX"rtd"	// Router Daemon
#define NID_SSK		__CTL_NID_PREFIX"ssk"	// SSK ported from Rev.H, for IPv4
#define NID_PPP		__CTL_NID_PREFIX"/usr/sbin/pppd"
#define NID_DHCPC	__CTL_NID_PREFIX"/usr/sbin/udhcpc"
#define NID_DHCPD	__CTL_NID_PREFIX"/usr/sbin/udhcpd"
//   EID_FTPD=25,
//   EID_TFTPD=26,
//   EID_TFTP=27,
//   EID_DNSPROBE=28,
//   EID_SYSLOGD=30,
#define   NID_KLOGD  __CTL_NID_PREFIX"klog"
#define   NID_SYSLOGD  __CTL_NID_PREFIX"syslog"

//   EID_DDNSD=33,
#define   NID_ZEBRA       __CTL_NID_PREFIX"/usr/sbin/zebra"
#define   NID_RIPD        __CTL_NID_PREFIX"/usr/sbin/ripd"
#define   NID_SNTP        __CTL_NID_PREFIX"sntp"
#define   NID_URLFILTERD  __CTL_NID_PREFIX"urlfilterd"
#define   NID_IGMP        __CTL_NID_PREFIX"igmp"
#define   NID_PING        __CTL_NID_PREFIX"ping"
#define   NID_HTTPD        __CTL_NID_PREFIX"httpd"
#define   NID_SSHD        __CTL_NID_PREFIX"sshd"
#define   NID_UPNP        __CTL_NID_PREFIX"/usr/sbin/miniupnpd"
#define   NID_SNTP        __CTL_NID_PREFIX"sntp"
#define   NID_URLFILTER   __CTL_NID_PREFIX"urlfilter"
   //EID_URLFILTERD=37,
   //EID_IGMP=38,
   //EID_PING=40,
#define NID_DHCP6C	__CTL_NID_PREFIX"dhcp6c"
#define NID_DHCP6S	__CTL_NID_PREFIX"dhcp6s"
#define NID_RADVD	__CTL_NID_PREFIX"radvd"
#define NID_DNSPROXY	__CTL_NID_PREFIX"dnsproxy"
//   EID_IPPD=45,
//   EID_FTP=46,
#define NID_MLD		__CTL_NID_PREFIX"mld"
//   EID_DSLDIAGD=48,
//   EID_SOAPSERVER=49//   EID_PWRCTL=50,
//   EID_HOTPLUG=51,
//   EID_L2TPD=52,
//   EID_SAMBA=53,
//   EID_PPTPD=54,
//   EID_UNITTEST=90,
//   EID_MISC=91,
//   EID_WLWPS=93,
//   EID_CMFD=94,
#define NID_MCPD	__CTL_NID_PREFIX"mcpd"
//   EID_MOCAD=96,
//   EID_RNGD=97,
//   EID_DMSD=98,
//   EID_DECTDBGD=99,
//   EID_SWMDK=100,

//   EID_TRACEROUTE=101,
//   EID_TR143_DLD=110,
//   EID_TR143_UPLD=111,
//   EID_TR143_ECHO=112,
//   EID_MYNETWORK=113,
#define NID_MYNETWORK	__CTL_NID_PREFIX"mynetwork"
typedef char * CmsEntityId;

/** Mask to get the generic eid out of a UINT32 eid field. */
#define EID_EID_MASK  0x0000ffff 

/** Mask to get the pid out of a UINT32 eid field */
#define EID_PID_MASK  0xffff0000 

/** Get the generic eid out of a UINT32 eid field */
#define GENERIC_EID(e) (((UINT32) e) & EID_EID_MASK)

/** Make a specific UINT32 eid field out of a pid and a generic eid */
#define MAKE_SPECIFIC_EID(p, e) ((p << 16) | (((tsl_u32_t) e) & EID_EID_MASK))
#endif //_CTL_NID_H
