/*
 *      Access Function Define
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#ifndef ACT_ACCESS_H
#define ACT_ACCESS_H

#include "act_common.h"
#include "act_log.h"
#include "act_wcb.h"

#define NAME_HASH_PRIME 139

typedef struct {
	char *url;
	short perm;
} url_perm_t;

short get_perm(const char *req_url, int auth_level);
void load_perm_table(void);


#endif

