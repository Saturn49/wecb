#ifndef __CTL_OAL_H__
#define __CTL_OAL_H__

#include "tsl_common.h"
#include "ctl_tms.h"

/* in oal_timestamp.c */
extern tsl_rv_t tr69_util_get_ip6_addr_info(const char *ifname, tsl_u32_t addrIdx,
                      char *ipAddr, tsl_u32_t *ifIndex, tsl_u32_t *prefixLen, tsl_u32_t *scope, tsl_u32_t *ifaFlags);
tsl_rv_t oal_strtoul(const char *str, char **endptr, tsl_32_t base, tsl_u32_t *val);

/* in oal_network.c */
extern tsl_rv_t oalNet_getIfNameList(char **ifNameList);
#ifdef DMP_ETHERNETWAN_1
tsl_rv_t oal_Net_getPersistentWanIfNameList(char **PersistentWanifNameList);
#endif

#endif

