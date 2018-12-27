/*
 *      Wireless Define
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#ifndef ACT_WIRELESS_H
#define ACT_WIRELESS_H

#include "act_common.h"
#include "act_log.h"
#include "act_wcb.h"

#define INIT_ALL    "all"
#define INIT_BRIDGE "bridge"
#define NON40DENY_FILE "/tmp/non40deny_file.txt"

#define BUF_LEN100         100
#define BUF_LEN8         8

extern void updateVapWscDisable(int wlan_root,int value);

#endif

