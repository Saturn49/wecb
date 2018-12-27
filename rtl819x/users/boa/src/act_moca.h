/*
 *      Moca Define 
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */
 
#ifndef ACT_MOCA_H
#define ACT_MOCA_H
#define CLNKSTAT_FILE "/tmp/clnkstat.txt"
#define MAC_NODE_NUM  16
#include "act_common.h"
#include "act_log.h"
#include "act_wcb.h"
#include "act_tr69.h"

void getMocaValue(char *type, char *retvalue);
int getMocaList(request *wp, char *type);

#endif

