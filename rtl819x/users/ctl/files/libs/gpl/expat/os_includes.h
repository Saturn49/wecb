/****************************************************************************
 *
 * rg/pkg/include/os_includes.h
 * 
 * Copyright (C) Jungo LTD 2004
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General 
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02111-1307, USA.
 *
 * Developed by Jungo LTD.
 * Residential Gateway Software Division
 * www.jungo.com
 * info@jungo.com
 */
/* SYNC: rg/pkg/include/os_includes.h <-> project/tools/util/os_includes.h */

/* IMPORTANT: Prior to including this file, define the needed sections. */

//#include <rg_os.h>

#ifdef OS_INCLUDE_IOCTL
#ifndef OS_INCLUDE_IOCTL_INCLUDED
#define OS_INCLUDE_IOCTL_INCLUDED
#ifdef __OS_VXWORKS__
#include <ioLib.h>
#define BLKFLSBUF  97 /* flush buffer cache */
#define FLASH_SYNC_IOCTL 98 /* flush internal buffers */
#endif
#include <sys/ioctl.h>
#endif
#endif

#ifdef OS_INCLUDE_STD
#ifndef OS_INCLUDE_STD_INCLUDED
#define OS_INCLUDE_STD_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#define OS_INCLUDE_STRING
#include <ctype.h>
#ifdef __OS_VXWORKS__
long int random(void);
void srandom(unsigned int seed);
int putenv(char *string);
#endif
#endif
#endif

#ifdef OS_INCLUDE_GETOPT
#include <unistd.h>
#ifdef __OS_VXWORKS__
#include <rg_getopt.h>
#elif defined(__OS_LINUX__)
#include <getopt.h>
#endif
#endif

#ifdef OS_INCLUDE_STDARG
#ifndef OS_INCLUDE_STDARG_INCLUDED
#define OS_INCLUDE_STDARG_INCLUDED
#include <stdarg.h>

/* Deal with pre-C99 compilers */
#ifndef va_copy
#ifdef __va_copy
#define va_copy(A, B) __va_copy(A, B)
#else
#warning "neither va_copy nor __va_copy is defined. Using simple copy."
#define va_copy(A, B) A = B
#endif
#endif

#endif
#endif

/* IO functions - open/read/close */
#ifdef OS_INCLUDE_IO
#ifndef OS_INCLUDE_IO_INCLUDED
#define OS_INCLUDE_IO_INCLUDED
#define OS_INCLUDE_TYPES
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif
#endif

#ifdef OS_INCLUDE_INET
#ifndef OS_INCLUDE_INET_INCLUDED
#define OS_INCLUDE_INET_INCLUDED
#ifdef __OS_VXWORKS__
#include <netinet/in_systm.h>
#include <inetLib.h>
#include <hostLib.h>
#define inet_aton(cp, inp) (inet_aton(cp, inp) ? 0 : -1)
/* We cannot use the VX implementation for inet_ntoa because it allocates a
 * buffer every time it is called, and no one frees it.
 */
char *sys_vx_inet_ntoa(struct in_addr in);
#define inet_ntoa(inp) (sys_vx_inet_ntoa(inp))
/* in_addr_t and in_port_t are missing in VxWorks' in.h */
#define in_addr_t u_int
#define in_port_t u_short
#include "vx_in6.h"
#elif defined(__OS_LINUX__)
#include <netinet/in.h>
#include <arpa/inet.h>
#define uh_sport source
#define uh_dport dest
#define uh_ulen len
#define uh_sum check
#endif
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#endif
#endif

#ifdef OS_INCLUDE_SOCKET
#ifndef OS_INCLUDE_SOCKET_INCLUDED
#define OS_INCLUDE_SOCKET_INCLUDED
#ifdef __OS_VXWORKS__
#include <sockLib.h>
#include <ioLib.h>
#elif defined(__OS_LINUX__)
#include <sys/socket.h>
#include <fcntl.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_IF_ETHER
#ifndef OS_INCLUDE_IF_ETHER_INCLUDED
#define OS_INCLUDE_IF_ETHER_INCLUDED
#ifdef __OS_VXWORKS__
#include <netinet/if_ether.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#define ETH_HLEN SIZEOF_ETHERHEADER
#elif defined(__OS_LINUX__)
#include <linux/if_ether.h>
#include <net/if_arp.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_IF_ATM
#ifndef OS_INCLUDE_IF_ATM_INCLUDED
#define OS_INCLUDE_IF_ATM_INCLUDED
#ifdef __OS_VXWORKS__
typedef struct {} atm_qos_t;
#elif defined(__OS_LINUX__)
#include <stdint.h>
#include <linux/atmdev.h>
typedef struct atm_qos atm_qos_t;
#endif
#endif
#endif

#ifdef OS_INCLUDE_TIME
#ifndef OS_INCLUDE_TIME_INCLUDED
#define OS_INCLUDE_TIME_INCLUDED
#ifdef __OS_VXWORKS__
#include <sysLib.h>
#include <tickLib.h>
#ifndef timercmp
#define	timercmp(tvp, uvp, cmp) \
    ((tvp)->tv_sec cmp (uvp)->tv_sec || \
    ((tvp)->tv_sec == (uvp)->tv_sec && \
    (tvp)->tv_usec cmp (uvp)->tv_usec))
#endif
#ifndef timeradd
#define timeradd(a, b, result)						      \
  do { \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec; \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec; \
    if ((result)->tv_usec >= 1000000) \
      { \
	++(result)->tv_sec; \
	(result)->tv_usec -= 1000000; \
      } \
  } while (0)
#endif
#ifndef timersub
#define timersub(a, b, result) \
  do { \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
    if ((result)->tv_usec < 0) { \
      --(result)->tv_sec; \
      (result)->tv_usec += 1000000; \
    } \
  } while (0)
#endif
#ifndef timerclear
#define	timerclear(tvp)	(tvp)->tv_sec = (tvp)->tv_usec = 0
#endif
#ifndef timerset
#define timerisset(tvp) ((tvp)->tv_sec || (tvp)->tv_usec)
#endif
#elif defined(__OS_LINUX__)
#include <sys/time.h>
#endif
#include <sys/times.h>
#include <time.h>
#endif
#endif

#ifdef OS_INCLUDE_SELECT
#ifndef OS_INCLUDE_SELECT_INCLUDED
#define OS_INCLUDE_SELECT_INCLUDED
#ifdef __OS_VXWORKS__
#include <selectLib.h>
#elif defined(__OS_LINUX__)
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_SIGNAL
#ifndef OS_INCLUDE_SIGNAL_INCLUDED
#define OS_INCLUDE_SIGNAL_INCLUDED
#include <sys/wait.h>
#include <signal.h>
#endif
#endif

#ifdef OS_INCLUDE_REBOOT
#ifndef OS_INCLUDE_REBOOT_INCLUDED
#define OS_INCLUDE_REBOOT_INCLUDED
#ifdef __OS_LINUX__
#include <sys/reboot.h>
#include <linux/reboot.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_MOUNT
#ifndef OS_INCLUDE_MOUNT_INCLUDED
#define OS_INCLUDE_MOUNT_INCLUDED
#ifdef __OS_LINUX__
#include <sys/mount.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_TERMIO
#ifndef OS_INCLUDE_TERMIO_INCLUDED
#define OS_INCLUDE_TERMIO_INCLUDED
#ifdef __OS_LINUX__
#include <termios.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_PROCESS
#ifndef OS_INCLUDE_PROCESS_INCLUDED
#define OS_INCLUDE_PROCESS_INCLUDED
#include <sys/types.h>
#ifdef __OS_LINUX__
#include <wait.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_BOOTP
#ifndef OS_INCLUDE_BOOTP_INCLUDED
#define OS_INCLUDE_BOOTP_INCLUDED
#ifdef __OS_VXWORKS__
#include <bootpLib.h>
#elif defined(__OS_LINUX__)
#include <netinet/ip.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_STRING
#ifndef OS_INCLUDE_STRING_INCLUDED
#define OS_INCLUDE_STRING_INCLUDED
#include <string.h>
#endif
#endif

#ifdef OS_INCLUDE_NAMESER
#ifndef OS_INCLUDE_NAMESER_INCLUDED
#define OS_INCLUDE_NAMESER_INCLUDED
#ifdef __OS_VXWORKS__
#include <resolv/nameser.h>
#define HFIXEDSZ 12
#define INT32SZ 4
#define INT16SZ 2
#define NS_NOTIFY_OP 0x4
#define T_NULL T_NULL_RR
#elif defined(__OS_LINUX__)
#include <arpa/nameser.h>
#define DNS_STATUS STATUS
#endif
#endif
#endif

#ifdef OS_INCLUDE_RESOLV
#ifndef OS_INCLUDE_RESOLV_INCLUDED
#define OS_INCLUDE_RESOLV_INCLUDED
#include <resolv.h>
#endif
#endif

#ifdef OS_INCLUDE_CDEFS
#ifndef OS_INCLUDE_CDEFS_INCLUDED
#define OS_INCLUDE_CDEFS_INCLUDED
#ifdef __OS_VXWORKS__
#define __P(xxx) xxx
#define __BEGIN_DECLS
#define __END_DECLS
#elif defined(__OS_LINUX__)
#include <sys/cdefs.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_PARAM
#ifndef OS_INCLUDE_PARAM_INCLUDED
#define OS_INCLUDE_PARAM_INCLUDED
#ifdef __OS_LINUX__
#include <sys/param.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_NETDB
#ifndef OS_INCLUDE_NETDB_INCLUDED
#define OS_INCLUDE_NETDB_INCLUDED
#ifdef __OS_VXWORKS__
#define NETDB_INTERNAL -1
#define NETDB_SUCCESS 0
#endif
#include <netdb.h>
#endif
#endif

#ifdef OS_INCLUDE_UIO
#ifndef OS_INCLUDE_UIO_INCLUDED
#define OS_INCLUDE_UIO_INCLUDED
#ifdef __OS_LINUX__
#include <sys/uio.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_TYPES
#ifndef OS_INCLUDE_TYPES_INCLUDED
#define OS_INCLUDE_TYPES_INCLUDED
#ifdef __OS_VXWORKS__
#include <rg_types.h>
#endif
#include <sys/types.h>
#endif
#endif

#ifdef OS_INCLUDE_NETIF
#ifndef OS_INCLUDE_NETIF_INCLUDED
#define OS_INCLUDE_NETIF_INCLUDED
#include <net/if.h>
#endif
#endif

#ifdef OS_INCLUDE_SOCKIOS
#ifndef OS_INCLUDE_SOCKIOS_INCLUDED
#define OS_INCLUDE_SOCKIOS_INCLUDED
#ifdef __OS_LINUX__
#include <linux/sockios.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_TFTP
#ifndef OS_INCLUDE_TFTP_INCLUDED
#define OS_INCLUDE_TFTP_INCLUDED
#ifndef TFTP_0ACK
#define TFTP_0ACK 6
#endif
#ifdef __OS_VXWORKS__
#include <tftpLib.h>
/* For struct-alignment problem architectures (e.g. IXP225), we redefine the
 * packed version of TFTP header (without alignment)
 */
struct tftp_format {
    u_short blockOrError;
    char data[TFTP_SEGSIZE];
} __attribute__ ((__packed__));
 
struct tftphdr {
    u_short th_opcode; /* packet op code */
    union {
	char request[TFTP_SEGSIZE + 2];	/* request string */
	struct tftp_format misc;
    } __attribute__ ((__packed__)) th;
} __attribute__ ((__packed__));
#define	th_stuff th_request
#define th_msg th_data
#define th_code th_error
#elif defined(__OS_LINUX__)
#include <arpa/tftp.h>
#define TFTP_SEGSIZE SEGSIZE
#define	TFTP_RRQ RRQ
#define	TFTP_WRQ WRQ
#define	TFTP_DATA DATA
#define	TFTP_ACK ACK
#define	TFTP_ERROR ERROR
#endif
#endif
#endif

#ifdef OS_INCLUDE_TELNET
#ifndef OS_INCLUDE_TELNET_INCLUDED
#define OS_INCLUDE_TELNET_INCLUDED
#ifdef __OS_VXWORKS__
#include <telnetLib.h>
#define NTELOPTS 100
#elif defined(__OS_LINUX__)
#include <arpa/telnet.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_MIB_LIB
#ifndef OS_INCLUDE_MIB_LIB_INCLUDE
#define OS_INCLUDE_MIB_LIB_INCLUDE
#ifdef __OS_VXWORKS__
#include <m2Lib.h>
#endif
#endif
#endif

#ifdef OS_INCLUDE_ROUTE
#ifndef OS_INCLUDE_ROUTE_INCLUDE
#define OS_INCLUDE_ROUTE_INCLUDE
#ifdef __OS_VXWORKS__
#include <sys/socket.h>
#include <routeLib.h>
#endif
#include <net/route.h>
#endif
#endif

