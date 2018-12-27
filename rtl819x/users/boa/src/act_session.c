/*
 *      Session Function Implementation
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_session.h"

static struct http_session *er_http_sessions = NULL;

struct http_session * er_http_session_lookup(unsigned int id)
{
	struct http_session *cur = NULL;
	
	if(id == 0)
		return NULL;

	kdebug("Enter");	

	for(cur = er_http_sessions; cur; cur = cur->next)
		if(cur->id == id)
			return cur;
	return NULL;
}

void er_http_session_remove(unsigned int id)
{
	struct http_session *cur = NULL;
	struct http_session *prv = NULL;

	kdebug("Enter");

	if(id != 0)
	{
		for(cur = er_http_sessions; cur; prv = cur, cur = cur->next)
		{
			if(cur->id == id)
			{
				if(prv)
					prv->next = cur->next;
				else
					er_http_sessions = cur->next;

				CMSMEM_FREE_BUF_AND_NULL_PTR(cur->username);				
				CMSMEM_FREE_BUF_AND_NULL_PTR(cur);
				
				break;
			}
		}
	}
}

void er_http_session_remove_all(void)
{
	struct http_session *cur = NULL;
	struct http_session *prv = NULL;
	struct timeval tv;
	unsigned int cur_time = 0;

	kdebug("Enter");
	
	gettimeofday(&tv, NULL);
	cur_time = (unsigned int) tv.tv_sec;	

	if(er_http_sessions != NULL)
	{
		for(cur = er_http_sessions; cur; prv = cur, cur = cur->next)
		{
			kdebug("id: %u, start_time: %u, diff_time: %d, username: %s", cur->id, cur->start_time, (cur_time - cur->start_time), cur->username);
			
			if(prv)
				prv->next = cur->next;
			else
				er_http_sessions = cur->next;					

			CMSMEM_FREE_BUF_AND_NULL_PTR(cur->username);	
			CMSMEM_FREE_BUF_AND_NULL_PTR(cur);	

			if(er_http_sessions == NULL)
				break;
		}	
	}
}	

struct http_session * er_http_session_add(char *username, int auth_level)
{
	struct http_session *new = NULL;
	struct timeval tv;	
	unsigned int sec = 0;
	unsigned int usec = 0;	
	
	if((new = (struct http_session *) malloc(sizeof(struct http_session))) != NULL)
	{
		memset(new, 0, sizeof(struct http_session));
		
		gettimeofday(&tv, NULL);
		sec = (unsigned int) tv.tv_sec;
		usec = (unsigned int) tv.tv_usec;
		
		new->start_time = sec;
		kinfo("login added start_time: %u", new->start_time);

		sec = 10000000 + sec % 10000000;
		usec = usec / 10000;

		new->id = (sec * 100 + usec);
		
		if ((new->username = (char *) malloc(strlen(username) + 1)) != NULL)
		{
			memset(new->username, 0, strlen(username) + 1);
			strncpy(new->username, username, strlen(username)); 
		}

		new->auth_level = auth_level;
		new->both_freq = 1;
		new->curr_ssid = 1;
		new->wlan_idx = 0;		
#ifdef AEI_WIFI
		new->wifi = 1;
#else
		new->wifi = 0;
#endif

		new->next = er_http_sessions;
		er_http_sessions = new;
	}
	return new;
}


