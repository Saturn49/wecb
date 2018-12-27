/*
 *      Status Define 
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#ifndef ACT_STATUS_H
#define ACT_STATUS_H

#include "act_common.h"
#include "act_log.h"
#include "act_wcb.h"
#include "act_tr69.h"
#include "act_l3_local.h"

void getStatusValue(char *type, char *retvalue);
int getStatusList(request *wp, char *type);
int getMacByIntf(char *intf, char *macaddr);


#endif

