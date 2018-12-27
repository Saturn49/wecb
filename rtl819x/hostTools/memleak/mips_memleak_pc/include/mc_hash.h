/**************************************************************************
 *	版权:	
 *		 
 *
 *	文件名:	
 *		 mc_hash.h
 *
 *	作者:	
 *		 祝红亮 
 *
 *	功能描述:	
 *		 哈希散列表接口函数
 *			
 *              
 *      历史:
 *	         $Id: mc_hash.h,v 1.2 2009/07/03 01:57:40 hzhu Exp $
 *		
 **************************************************************************/
 
#ifndef MC_HASH_H
#define MC_HASH_H

#include "mc_port.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct mc_hash_t mc_hash_t;

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

mc_void_t mc_hash_value_func_destroy(IN mc_hash_t *p_hash, 
                                     IN void (*callback)(void **argv));
    
mc_void_t mc_hash_func_display(IN mc_hash_t *p_hash, 
                               IN void (*callback)(void *argv));

#ifdef __cplusplus
}
#endif

#endif
