/*
 *      TR69 Function Implementation  
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */


extern "C"
{
	#include "libtr69_client.h"
}

#include "act_tr69.h"
#include "act_common.h"
#include "act_log.h"



int tr69SetUnfreshLeafDataInt(char *mdmoid, int num, char *parameter, int value)
{
	char strPara[FULL_NAME_SIZE]={0};

	if(num != 0)
		sprintf(strPara, "%s.%d.%s", mdmoid, num, parameter);
	else
		sprintf(strPara, "%s.%s", mdmoid, parameter);
	
	kinfo("%s -> %d", strPara, value);		
	return tr69_set_unfresh_leaf_data(strPara, &value, TR69_NODE_LEAF_TYPE_INT);
}

int tr69SetLeafDataInt(char *mdmoid, int num, char *parameter, int value)
{
	char strPara[FULL_NAME_SIZE]={0};

	if(num != 0)
		sprintf(strPara, "%s.%d.%s", mdmoid, num, parameter);
	else
		sprintf(strPara, "%s.%s", mdmoid, parameter);
	
	kinfo("%s -> %d", strPara, value);		
	return tr69_set_leaf_data(strPara, &value, TR69_NODE_LEAF_TYPE_INT);
}

int tr69CommitSetLeafDataInt(char *mdmoid, int num, char *parameter, int value)
{
	char strPara[FULL_NAME_SIZE]={0};

	if(num != 0)
		sprintf(strPara, "%s.%d.%s", mdmoid, num, parameter);
	else
		sprintf(strPara, "%s.%s", mdmoid, parameter);
	
	kinfo("%s -> %d", strPara, value);		
	return tr69_commit_set_leaf_data(strPara, &value, TR69_NODE_LEAF_TYPE_INT);
}

int tr69SetUnfreshLeafDataString(char *mdmoid, int num, char *parameter, char *value)
{
	char strPara[FULL_NAME_SIZE]={0};

	if(safe_strcmp(value, "true") == 0)
	{
		return tr69SetUnfreshLeafDataInt(mdmoid, num, parameter, 1);
	} 
	else if(safe_strcmp(value, "false") == 0)
	{
		return tr69SetUnfreshLeafDataInt(mdmoid, num, parameter, 0);
	} 
	else 
	{
		if(num != 0)
			sprintf(strPara, "%s.%d.%s", mdmoid, num, parameter);
		else
			sprintf(strPara, "%s.%s", mdmoid, parameter);
	
		kinfo("%s -> %s", strPara, value);		
		return tr69_set_unfresh_leaf_data(strPara, value, TR69_NODE_LEAF_TYPE_STRING);
	}	
}

int tr69SetLeafDataString(char *mdmoid, int num, char *parameter, char *value)
{
	char strPara[FULL_NAME_SIZE]={0};

	if(safe_strcmp(value, "true") == 0)
	{
		return tr69SetLeafDataInt(mdmoid, num, parameter, 1);
	} 
	else if(safe_strcmp(value, "false") == 0)
	{
		return tr69SetLeafDataInt(mdmoid, num, parameter, 0);
	} 
	else 
	{
		if(num != 0)
			sprintf(strPara, "%s.%d.%s", mdmoid, num, parameter);
		else
			sprintf(strPara, "%s.%s", mdmoid, parameter);
	
		kinfo("%s -> %s", strPara, value);		
		return tr69_set_leaf_data(strPara, value, TR69_NODE_LEAF_TYPE_STRING);
	}	
}

int tr69CommitSetLeafDataString(char *mdmoid, int num, char *parameter, char *value)
{
	char strPara[FULL_NAME_SIZE]={0};

	if(safe_strcmp(value, "true") == 0)
	{
		return tr69CommitSetLeafDataInt(mdmoid, num, parameter, 1);
	} 
	else if(safe_strcmp(value, "false") == 0)
	{
		return tr69CommitSetLeafDataInt(mdmoid, num, parameter, 0);
	} 
	else 
	{
		if(num != 0)
			sprintf(strPara, "%s.%d.%s", mdmoid, num, parameter);
		else
			sprintf(strPara, "%s.%s", mdmoid, parameter);
	
		kinfo("%s -> %s", strPara, value);		
		return tr69_commit_set_leaf_data(strPara, value, TR69_NODE_LEAF_TYPE_STRING);
	}	
}

int tr69CommitSetLeafDataEnd(char *mdmoid, int num)
{
	char strPara[FULL_NAME_SIZE]={0};
	
	if(num != 0)
		sprintf(strPara, "%s.%d", mdmoid, num);
	else
		sprintf(strPara, "%s", mdmoid);

	kinfo("%s", strPara);		
	return tr69_commit_set_leaf_data_end(strPara);	
}

int tr69GetUnfreshLeafData(char *buf, char *full_name, char *parameter)
{	
	char strPara[FULL_NAME_SIZE] = {0};
	char *value = NULL;
	int type = 0;
	int ret = 0;
	
	sprintf(strPara, "%s.%s", full_name, parameter);
		
	ret = tr69_get_unfresh_leaf_data(strPara, (void **)&value, &type);

	if(ret == 0)
	{
		if((value == NULL) && (type == TR69_NODE_LEAF_TYPE_STRING)) {		
			strcpy(buf, "NULL");		
		} else {
			if(type == TR69_NODE_LEAF_TYPE_STRING) {

				if(safe_strcmp(value, "") == 0)
					strcpy(buf, "NULL");
				else
					sprintf(buf, "%s", value);

				CMSMEM_FREE_BUF_AND_NULL_PTR(value);
			} else { 			
				sprintf(buf, "%d", (int)value);
			}	
		}
		
		kinfo("%s -> %s", strPara, buf);
	}
	return ret;
}

int tr69GetFreshLeafData(char *buf, char *full_name, char *parameter)
{	
	char strPara[FULL_NAME_SIZE] = {0};
	char *value = NULL;
	int type = 0;
	int ret = 0;
	
	sprintf(strPara, "%s.%s", full_name, parameter);
		
	ret = tr69_get_leaf_data(strPara, (void **)&value, &type);

	if(ret == 0)
	{
		if((value == NULL) && (type == TR69_NODE_LEAF_TYPE_STRING)) {		
			strcpy(buf, "NULL");		
		} else {
			if(type == TR69_NODE_LEAF_TYPE_STRING) {

				if(safe_strcmp(value, "") == 0)
					strcpy(buf, "NULL");
				else
					sprintf(buf, "%s", value);

				CMSMEM_FREE_BUF_AND_NULL_PTR(value);
			} else { 			
				sprintf(buf, "%d", (int)value);
			}	
		}
		
		kinfo("%s -> %s", strPara, buf);
	}
	return ret;
}


void turnGetTR69(char *buf)
{
	if(safe_strcmp(buf, "false|") == 0)
	{
		sprintf(buf, "0|");
	} 
	else if(safe_strcmp(buf, "true|") == 0)
	{
		sprintf(buf, "1|");
	}
}

void cutGetTR69(char *buf)
{
	char *p = NULL;
	
	if(safe_strcmp(buf, "false|") == 0)
	{
		sprintf(buf, "0");
	} 
	else if(safe_strcmp(buf, "true|") == 0)
	{
		sprintf(buf, "1");
	} else {
		p = strstr(buf, "|");
		if(p)
			*p = '\0';
	}
}

