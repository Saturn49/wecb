/*
 *      Session Function Define
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#ifndef ACT_SESSION_H
#define ACT_SESSION_H

#include "act_common.h"
#include "act_log.h"
#include "act_wcb.h"
#include <time.h>

#define SESSION_KEY         "aei_session_id_cookie_2012101001="
#define SESSION_TIME        1200


struct http_session {
	unsigned int id;
	unsigned int start_time;	
	char *username;
	int auth_level;
	int both_freq;
	int wlan_idx;
	int curr_ssid;
	int wifi;
	struct http_session *next;
};

struct http_session * er_http_session_lookup(unsigned int id);
void er_http_session_remove(unsigned int id);
void er_http_session_remove_all(void);
struct http_session * er_http_session_add(char *username, int auth_level);

#endif

