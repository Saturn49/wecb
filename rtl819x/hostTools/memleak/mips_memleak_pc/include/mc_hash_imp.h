/**************************************************************************
 *	版权:	
 *		 
 *
 *	文件名:	
 *		 mc_hash_imp.h
 *
 *	作者:	
 *		 祝红亮 
 *
 *	功能描述:	
 *		 哈希散列表内部实现头文件
 *			
 *              
 *      历史:
 *	         $Id: mc_hash_imp.h,v 1.1 2009/06/05 04:34:32 lliu Exp $
 *		
 **************************************************************************/
 
#ifndef MC_HASH_IMP_H
#define MC_HASH_IMP_H

#include "mc_port.h"
#include "mc_list.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MC_HASH_KEY   256
#define MC_HASH_MASK  1024     //hash模
#define MAX_HASH_KEY  128


/*定义Hash表结构*/
typedef struct {
	mc_list_head_t	 list_head[MC_HASH_MASK];
	mc_mutex_t       list_mutex;
}mc_hash_t;

typedef struct mclist_struct{
	mc_char_t        key[MC_HASH_KEY];   //关键字
	mc_void_t        *value;             //对应的值
	mc_list_head_t   link_hash;	     //HASH链表
}mc_hash_node_t;


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
mc_hash_t * mc_hash_create();


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
mc_rv mc_hash_add(IN mc_char_t *key, 
		  IN mc_void_t *value, 
		  IN mc_hash_t *p_hash);


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
mc_void_t *mc_hash_find(IN mc_char_t *key, 
			IN mc_hash_t *p_hash);


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
mc_rv mc_hash_del(IN mc_char_t *key, 
		  IN mc_hash_t *p_hash);


/***********************************************************
 *	[函数名]:
 *		mc_hash_destroy
 *
 *	[函数功能]:
 *		释放Hash表
 *
 *	[函数参数]
 *		IN mc_hash_t *p_hash  Hash表的句柄
 *
 *	[函数返回值]:
 *              无
 *
 ************************************************************/
mc_void_t mc_hash_destroy(IN mc_hash_t *p_hash);


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
mc_void_t mc_hash_value_destroy(IN mc_hash_t *p_hash);

#ifdef __cplusplus
}
#endif

#endif
