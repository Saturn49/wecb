#ifndef _SCRIPT_H
#define _SCRIPT_H

#include "packet.h"

#ifdef AEI_WECB
#define INTF_ID_POOL_SIZE    (AEI_MAX_DUT_CNT)
#define MIN_INTF_ID_POOL_SIZE    (1)
#define MAX_INTF_ID_POOL_SIZE    (20)
#define DEF_INTF_ID_POOL_SIZE    (2)

#endif

void run_script(struct dhcpMessage *packet, const char *name);

#endif
