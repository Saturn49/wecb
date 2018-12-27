#ifndef __CTL_NET_H__
#define __CTL_NET_H__


/** Check if two IPv6 addresses are equal.
 *
 * @param ip6Addr1 (IN) IPv6 address 1.
 * @param ip6Addr2 (IN) IPv6 address 2.
 *
 * @return TRUE if the specified two IPv6 addresses are equal.
 */
tsl_bool_t tr69_util_ip6_addr_equal(const char *ip6Addr1, const char *ip6Addr2);

#endif
