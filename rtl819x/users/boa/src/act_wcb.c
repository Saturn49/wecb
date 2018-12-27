/*
 *      Web server handler routines for includePage 
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_wcb.h"
#include "act_advanced.h"
#include "act_moca.h"
#include "act_status.h"
#include "act_wireless.h"

extern char currentPage[64];

struct tag_set {
  char *tagname;
  int (*output)(request *req, int argc, char **argv);
};

struct tag_set tag_sets[] = {
#ifdef ACTIONTEC_WCB
	  {"actInclude", actInclude},
	  {"actValue", actValue},
	  {"actTable", actTable}, 
	  {"getInfo", getInfo},
	  {"getIndex", getIndex},
#endif
	  {NULL, NULL}
};

static char * get_arg(char *args, char **next)
{
	char *arg, *end;

	if(!(end = strchr(args, ',')))
	{
		end = args + strlen(args);
		*next = NULL;
	}
	else
		*next = end + 1;

	for(arg = args; isspace(*arg) || *arg == '"'; arg++)
		for(*end-- = '\0'; isspace(*end) || (*end == '"'); end--)
			*end = '\0';

	return arg;
}

int parseFunc(char *func, request *wp)
{
	int nBytesSent = 0;
	char *args = NULL, *end = NULL, *next = NULL;
	struct tag_set *set;
	int argc = 0;
	char *argv[20];

	kdebug("Enter");

	gui_trim(func);
	
	if(!(args = strchr(func, '(')))
		return -1;
	if(!(end = strchr(func, ')')))
		return -1;
	*args++ = *end = '\0';
	
	for(argc = 0; (argc < 20) && args; argc++, args = next)
	{
		if(!(argv[argc] = get_arg(args, &next)))
			break;
	}

	for(set = &tag_sets[0]; set->tagname; set++)
	{
		if(!strncmp(set->tagname, func, strlen(set->tagname)))
		{
			kdebug("argc: %d, argv: %s", argc, argv[0]);
			nBytesSent += set->output(wp, argc, argv);
			break;
		}
	}	
	return nBytesSent;
}

int parse_sub(request *wp, const char *value, int *idx, char **remain)
{
	int nBytesSent = 0;
	char *head = NULL;
	char *func = NULL;	
	char *p = NULL;
	char *q = NULL;		

	kdebug("Enter");
	
	p = strstr(value, "<%");
	while(p)
	{					
		if ((head = (char *) malloc(p - &value[*idx] + 1)) != NULL)
		{
			memset(head, 0, p - &value[*idx] + 1);
			strncpy(head, &value[*idx], p - &value[*idx]); 
			
			*idx += (int)(p - &value[*idx]) + 2;	
			nBytesSent += req_format_write(wp, "%s", head);
	
			q = strstr(p, "%>");
			if(q)
			{								
				if ((func = (char *) malloc(q - &value[*idx] + 1)) != NULL)
				{
					memset(func, 0, q - &value[*idx] + 1);
					strncpy(func, &value[*idx], q - &value[*idx]); 
					
					*idx += (int)(q - &value[*idx]) + 2;	
					
					nBytesSent += parseFunc(func, wp);							
	
					p = strstr(q, "<%");
					CMSMEM_FREE_BUF_AND_NULL_PTR(func);	
				}					
			}
			else
			{			
				if ((*remain = (char *) malloc(strlen(value) - *idx + 1)) != NULL)
				{
					memset(*remain, 0, strlen(value) - *idx + 1);
					strncpy(*remain, &value[*idx], strlen(value) - *idx);	
					*idx -= 2;							
					break;		
				}								
			}
			CMSMEM_FREE_BUF_AND_NULL_PTR(head);
		}
	}	

	return nBytesSent;
}

int parse_tag(request *wp, FILE *fp)
{
	int nBytesSent = 0;
	char *remain = NULL;
 	char value[LIMIT_PARSEHTML] = {0};	
	char value_bakup[LIMIT_PARSEHTML] = {0};
	int rlen = 0;
	int idx = 0;	
	char *t = NULL;
	int special = 0;
	char begin[2] = {0};
	char end[2] = {0};	

	kdebug("Enter");
	
	while((rlen = fread(value, 1, sizeof(value) - 1, fp)) > 0)
	{
		value[sizeof(value) - 1] = '\0';
		idx = 0;			
		kdebug("value: %s, strlen(value): %d, rlen: %d, special: %d", 
			value, strlen(value), rlen, special);

		if(special)
		{
			special = 0;
			strncpy(begin, &value[0], 1); 

			kdebug("begin1: %s", begin);
			if(!safe_strcmp(begin, "%"))
			{
				kdebug("line end: <, next line begin: %%");

				strcpy(value_bakup, value);
				sprintf(value, "<%s", value_bakup);
				
				kdebug("value2: %s", value);
			}
			else
			{
				nBytesSent += req_format_write(wp, "%s", "<");
			}
		}

		strncpy(end, &value[rlen-1], 1);
		kdebug("end1: %s", end);
		
		if(!safe_strcmp(end, "<"))
			special = 1;		

		if(remain == NULL)
		{
			nBytesSent += parse_sub(wp, value, &idx, &remain);	
		}
		else 
		{				
			strncpy(end, &remain[strlen(remain)-1], 1);
			kdebug("end2: %s", end);

			if(!safe_strcmp(end, "%"))
			{
				strncpy(begin, &value[0], 1); 
				kdebug("begin2: %s", begin);
				
				if(!safe_strcmp(begin, ">"))
				{
					kdebug("line end: %%, next line begin: >");
					
					strcpy(value_bakup, value);
					sprintf(value, "%%%s", value_bakup);

					kdebug("value2: %s", value);
				}
			}

			t = strstr(value, "%>");
			if(t)
			{			
				if ((remain = (char *) realloc(remain, strlen(remain) + (t - &value[idx] + 1))) != NULL)
				{
					strncat(remain, &value[idx], (t - &value[idx]));

					idx += (int)(t - &value[idx]) + 2;
					kdebug("remain: %s", remain);

					nBytesSent += parseFunc(remain, wp);						
					
					CMSMEM_FREE_BUF_AND_NULL_PTR(remain);
				}				
			}	

			nBytesSent += parse_sub(wp, value, &idx, &remain);				
		}		

		kdebug("idx: %d", idx);
		if(remain == NULL)
		{		    
			if(special)	
			{			    
				value[strlen(value)-1] = '\0';
				nBytesSent += req_format_write(wp, "%s", &value[idx]);
			}
			else
				nBytesSent += req_format_write(wp, "%s", &value[idx]);
		}
		memset(value, 0, sizeof(value));       
	}   

	CMSMEM_FREE_BUF_AND_NULL_PTR(remain);

    kdebug("Leave");
	return nBytesSent;
}

int includePage(request *wp, char *path)
{
	FILE *fp = NULL;
	int nBytesSent = 0;
	char value[LIMIT_MSGHEAD] = {0};
	int rlen = 0;

	kdebug("path: %s", path);	
	
	if((fp = fopen(path, "r")) != NULL)
	{
		if(strstr(wp->request_uri, ".html") || strstr(wp->request_uri, ".htm"))
		{
			nBytesSent += parse_tag(wp, fp);
		}
		else
		{
			while((rlen = fread(value, 1, sizeof(value), fp)) > 0)
				nBytesSent += req_format_write(wp, "%s", value);
		}

		fclose(fp);		
	}	
	
	return nBytesSent;
}

void thankyouPage(request *wp)
{
	char *strVal = NULL;
	strVal = req_get_cstream_var(wp, ("mypage"), "");
	strncpy(currentPage, strVal, 63);
	send_redirect_perm(wp, THANKYOU_PAGE);
}

void nothankyouPage(request *wp)
{
	char *strVal = NULL;
	strVal = req_get_cstream_var(wp, ("mypage"), "");
	strncpy(currentPage, strVal, 63);
	send_redirect_perm(wp, currentPage);
}

void backPage(request *wp)
{
	char *strVal = NULL;
	char buf[100] = {0};

	strVal = req_get_cstream_var(wp, ("mypage"), "");	
	if(strVal[0])	
	{
		sprintf(buf, "%s?e=1", strVal);
		send_redirect_perm(wp, buf);			
	}
	else
		send_redirect_perm(wp, MAIN_PAGE);	
}


#ifdef ACTIONTEC_WCB
int actInclude(request *wp, int argc, char **argv)
{
	int nBytesSent = 0;
	char path[100] = {0};	
	char *p = NULL;
	kdebug("Enter");	

	if(argc != 1)
		return -1;

	kdebug("pathname: %s", wp->pathname);
	strncpy(path, wp->pathname, 100);
	
	p = strrchr(path, '/');
	if(p != NULL)
		*(p+1) = '\0';	

	if((strlen(path) + strlen(argv[0])) < 99)
		strcat(path, argv[0]);		
	else
		kerror("path[100] is too small or argv[0] is too long");
	
	nBytesSent += includePage(wp, path);
	
	return nBytesSent;
}

int actValue(request *wp, int argc, char **argv)
{
	int nBytesSent = 0;	
	int i = 0;
	char value[200] = {0}; 

	kdebug("Enter");

	if (argc < 2)
    	return -1;
	
	for(i = 0; i < argc; i++)
	{
		kdebug("argv[%d]: %s", i, argv[i]);
	}   
	
	if (!safe_strcmp(argv[0], "status"))
	{
	    getStatusValue(argv[1], value);
	}
	else if (!safe_strcmp(argv[0], "moca"))
	{
	  	getMocaValue(argv[1], value);
	}	
	else if (!safe_strcmp(argv[0], "adv"))
	{
	  	getAdvValue(argv[1], value);
	}	
	else
	{
	  	strcpy(value, " ");
	}

	nBytesSent += req_format_write(wp, "%s", value);
		
	return nBytesSent;
}


int actTable(request *wp, int argc, char **argv)
{
	int nBytesSent = 0;		
	kinfo("Enter");

	if (argc < 2)
    	return -1;
   
  	if (!safe_strcmp(argv[0], "wl"))
  	{
    	//None
  	}
	else if (!safe_strcmp(argv[0], "moca"))
   	 	nBytesSent += getMocaList(wp, argv[1]);
 	else if (!safe_strcmp(argv[0], "status"))
    	nBytesSent += getStatusList(wp, argv[1]);
	
	return nBytesSent;
}
#endif

