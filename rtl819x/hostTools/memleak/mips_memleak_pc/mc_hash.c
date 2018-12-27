/**************************************************************************
 *	版权:	
 *		 
 *
 *	文件名:	
 *		 mc_hash.c
 *
 *	作者:	
 *		 祝红亮 
 *
 *	功能描述:	
 *		 哈希散列表实现
 *			
 *              
 *      历史:
 *	         $Id: mc_hash.c,v 1.2 2009/07/03 02:07:19 hzhu Exp $
 *		
 **************************************************************************/

#include "mc_hash_imp.h"
#include "mc_port.h"

static  inline mc_ulong_t  mc_partial_name_hash(mc_int_t c, mc_int_t prevhash)
{
	return (prevhash +(c << 4) + (c >> 4))*11;
	
}
static  inline mc_int_t  mc_full_name_hash(mc_char_t *name, mc_size_t len)
{
	mc_ulong_t hash = 0;
	while(len--){
		hash = mc_partial_name_hash(*name++, hash);
	}
	return hash%MC_HASH_MASK;
}

/***********************************************************
 *	[函数名]:
 *		mc_hash_create
 *
 *	[函数功能]:
 *		初始化Hash映射表
 *
 *	[函数参数]: 
 *              无
 *
 *	[函数返回值]
 *		返回hash表的句柄
 *
 ************************************************************/
mc_hash_t * mc_hash_create()
{
	mc_int_t i;
	mc_hash_t *p_hash;

	
	//分配全局HASH散列区，即HASH结构数组;分配MC_HASH_MASK+1个大小为list_head的内存，且初始化为0
	MC_MALLOC_RV(p_hash, mc_hash_t, NULL);
	
	//初始化HASH全局表 
	for(i = 0; i < MC_HASH_MASK; i++){
		MC_INIT_LIST_HEAD(&p_hash->list_head[i]);
	}
	mc_mutex_init(&p_hash->list_mutex);
	return p_hash;	    
}


/***********************************************************
 *	[函数名]:
 *		mc_hash_add
 *
 *	[函数功能]:
 *		在Hash表中增加一项键值对
 *
 *	[函数参数]
 *		IN mc_char_t *key        键
 *		IN mc_void_t *value 	 值
 *		IN mc_hash_t *p_hash     hash表的句柄

 *
 *	[函数返回值]
 *		MC_RV_SUC           	 成功
 *		MC_RV_ERR                失败
 *
 ************************************************************/
mc_rv mc_hash_add(IN mc_char_t *p_key, 
		  IN mc_void_t *p_value,
		  IN mc_hash_t *p_hash)
{
	int hash;
	mc_list_head_t *head_hash, *pos;
	mc_hash_node_t *p_node; 
	
	
	VASSERT(p_key != NULL);
	VASSERT(p_value != NULL);
	VASSERT(p_hash != NULL);

	mc_mutex_lock(&p_hash->list_mutex);

	hash = mc_full_name_hash(p_key, strlen(p_key));
	head_hash = p_hash->list_head + hash;
	
	/*判断表头是否为空*/
	if (mc_is_list_empty(head_hash)){
		/*将链表加入到hash全局数组中去*/
		goto new;
	}
			
	for (pos = head_hash->prev; pos != head_hash; pos = pos->prev){
		p_node = mc_list_entry(pos, mc_hash_node_t, link_hash);
		if (strncmp(p_key, p_node->key,strlen(p_key)) == 0){
			/*表明已经查找成功,修改参数值然后返回*/
			p_node->value = p_value;
			mc_mutex_unlock(&p_hash->list_mutex);
			return MC_RV_SUC;
		}
	}
	
 new:
	//没有找到，新建一个节点并增加
	MC_MALLOC_RV(p_node, mc_hash_node_t, MC_RV_FAIL_MEM);
	
	//strlen不包括字符串结束符,而snprintf则包含\0
	sprintf(p_node->key, "%s", p_key);
	p_node->value = p_value;

	/*将链表加入到hash全局数组中去*/
	mc_list_add(&p_node->link_hash, head_hash);
	
	mc_mutex_unlock(&p_hash->list_mutex);
	
	return MC_RV_SUC;
}



/***********************************************************
 *	[函数名]:
 *		mc_hash_find
 *
 *	[函数功能]:
 *		在Hash表中查找键所对应的值
 *
 *	[函数参数]
 *		IN mc_char_t *key        键
 *		IN mc_hash_t *p_hash     Hash表句柄
 *
 *	[函数返回值]
 *		返回键所对应的值
 *
 ************************************************************/
mc_void_t *mc_hash_find(IN char *p_key, 
			IN mc_hash_t *p_hash)
{
	mc_int_t hash;
	mc_list_head_t *head_hash, *pos;
	mc_hash_node_t *p_node; 
		
	
	VASSERT_RV(p_key != NULL, NULL);
	VASSERT_RV(p_hash != NULL, NULL);
	
	mc_mutex_lock(&p_hash->list_mutex);
	
	hash = mc_full_name_hash(p_key, strlen(p_key));
	head_hash = p_hash->list_head + hash;
	
	/*判断表头是否为空*/
	if (mc_is_list_empty(head_hash)){
		mc_mutex_unlock(&p_hash->list_mutex);
		return NULL;	
	}
			
	for (pos = head_hash->prev; pos != head_hash; pos = pos->prev){
		p_node = mc_list_entry(pos, mc_hash_node_t, link_hash);
		if (!strncmp(p_key, p_node->key,strlen(p_key))){
			/*表明已经查找成功*/
			mc_mutex_unlock(&p_hash->list_mutex);
			return p_node->value;
		}
	}

	mc_mutex_unlock(&p_hash->list_mutex);

	return NULL;    
}



/***********************************************************
 *	[函数名]:
 *		mc_hash_del
 *
 *	[函数功能]:
 *		在Hash表中删除一项键值对
 *
 *	[函数参数]
 *		IN char *key_buffer      键
 *		IN mc_hash_t *p_hash     Hash表句柄
 *
 *	[函数返回值]
 *		MC_RV_SUC           	 成功
 *		MC_RV_ERR                失败
 *
 ************************************************************/
mc_rv mc_hash_del(IN mc_char_t *p_key, 
		  IN mc_hash_t *p_hash)
{
	mc_int_t hash;
	mc_list_head_t *head_hash, *pos;
	mc_hash_node_t *p_node; 
	

	VASSERT(p_key != NULL);
	VASSERT(p_hash != NULL);

	mc_mutex_lock(&p_hash->list_mutex);

	hash = mc_full_name_hash(p_key, strlen(p_key));
	head_hash = p_hash->list_head + hash;
	
	/*判断表头是否为空*/
	if (mc_is_list_empty(head_hash)){
		mc_mutex_unlock(&p_hash->list_mutex);
		return MC_RV_ERR;	
	}
			
	for (pos = head_hash->prev; pos != head_hash; )	{
		p_node = mc_list_entry(pos, mc_hash_node_t, link_hash);
		pos = pos->prev;
		if (!strncmp(p_key, p_node->key,strlen(p_key))){
			/*找到该节点，删除之*/
          		mc_list_del(&(p_node->link_hash));
			MC_FREE(p_node);		
       			mc_mutex_unlock(&p_hash->list_mutex);
			return MC_RV_SUC;
		}
	}

	mc_mutex_unlock(&p_hash->list_mutex);
	return MC_RV_ERR;       
}





/***********************************************************
 *	[函数名]:
 *		mc_hash_destroy
 *
 *	[函数功能]:
 *		释放Hash表,及vlaue值
 *
 *	[函数参数]
 *		IN mc_hash_t *p_hash  Hash表的句柄
 *
 *	[函数返回值]: 
 *              无
 *
 ************************************************************/
mc_void_t mc_hash_destroy(IN mc_hash_t *p_hash)
{
	mc_list_head_t *head_hash, *pos;
	mc_hash_node_t *p_node; 
	mc_int_t i;
	

	VASSERT_VRV(p_hash != NULL);
	mc_mutex_lock(&p_hash->list_mutex);

	/*循环释放空间*/
	for(i = 0; i<MC_HASH_MASK; i++){   
		head_hash = p_hash->list_head + i;
		if (mc_is_list_empty(head_hash)){
			continue;
		}	    
		for (pos = head_hash->prev; pos != head_hash;){
			p_node = mc_list_entry(pos, mc_hash_node_t, link_hash);     
			pos = pos->prev;
			mc_list_del(&(p_node->link_hash));
			MC_FREE(p_node);	
		}
	}
	mc_mutex_unlock(&p_hash->list_mutex);
	
 	MC_FREE(p_hash);
}


/***********************************************************
 *	[函数名]:
 *		mc_hash_value_destroy
 *
 *	[函数功能]:
 *		释放Hash表,及vlaue值
 *
 *	[函数参数]
 *		IN mc_hash_t *p_hash  Hash表的句柄
 *
 *	[函数返回值]: 
 *              无
 *
 ************************************************************/
mc_void_t mc_hash_value_destroy(IN mc_hash_t *p_hash)
{
	mc_list_head_t *head_hash, *pos;
	mc_hash_node_t *p_node; 
	mc_int_t i;
	

	VASSERT_VRV(p_hash != NULL);
	mc_mutex_lock(&p_hash->list_mutex);

	/*循环释放空间*/
	for(i = 0; i<MC_HASH_MASK; i++){   
		head_hash = p_hash->list_head + i;
		if (mc_is_list_empty(head_hash)){
			continue;
		}	    
		for (pos = head_hash->prev; pos != head_hash;){
			p_node = mc_list_entry(pos, mc_hash_node_t, link_hash);     
			pos = pos->prev;
			mc_list_del(&(p_node->link_hash));
			MC_FREE(p_node->value);
			MC_FREE(p_node);	
		}
	}
	mc_mutex_unlock(&p_hash->list_mutex);
	
 	MC_FREE(p_hash);
}

/***********************************************************
 *	[函数名]:
 *		mc_hash_value_destroy
 *
 *	[函数功能]:
 *		释放Hash表,及vlaue值
 *
 *	[函数参数]
 *		IN mc_hash_t *p_hash  Hash表的句柄
 *
 *	[函数返回值]: 
 *              无
 *
 ************************************************************/
mc_void_t mc_hash_value_func_destroy(IN mc_hash_t *p_hash, void (*callback)(void **argv))
{
	mc_list_head_t *head_hash, *pos;
	mc_hash_node_t *p_node; 
	mc_int_t i;
	

	VASSERT_VRV(p_hash != NULL);
	mc_mutex_lock(&p_hash->list_mutex);

	/*循环释放空间*/
	for(i = 0; i<MC_HASH_MASK; i++){   
		head_hash = p_hash->list_head + i;
		if (mc_is_list_empty(head_hash)){
			continue;
		}	    
		for (pos = head_hash->prev; pos != head_hash;){
			p_node = mc_list_entry(pos, mc_hash_node_t, link_hash);     
			pos = pos->prev;
			mc_list_del(&(p_node->link_hash));
                        
                        if (callback != NULL){
                            callback(p_node->value);
                        }
                        
			MC_FREE(p_node->value);
			MC_FREE(p_node);	
		}
	}
	mc_mutex_unlock(&p_hash->list_mutex);
	
 	MC_FREE(p_hash);
}



mc_void_t mc_hash_func_display(IN mc_hash_t *p_hash, void (*callback)(void *argv))
{
	mc_list_head_t *head_hash, *pos;
	mc_hash_node_t *p_node; 
	mc_int_t i;
	

	VASSERT_VRV(p_hash != NULL);
	mc_mutex_lock(&p_hash->list_mutex);

	/*循环空间*/
	for(i = 0; i<MC_HASH_MASK; i++){   
		head_hash = p_hash->list_head + i;
		if (mc_is_list_empty(head_hash)){
			continue;
		}	    
		for (pos = head_hash->prev; pos != head_hash;){
			p_node = mc_list_entry(pos, mc_hash_node_t, link_hash);     
			pos = pos->prev;
                        if (callback != NULL){
                            callback(p_node->value);	
                        }
		}
	}
	mc_mutex_unlock(&p_hash->list_mutex);
	
        
}
