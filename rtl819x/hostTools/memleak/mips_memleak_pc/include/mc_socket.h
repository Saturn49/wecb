/**************************************************************************
 *
 *	版权:	
 *              
 *
 *	文件名:	
 *              mc_socket.h
 *
 *	作者:	
 *              祝红亮 
 *
 *	功能描述:	
 *		
 *            
 *	函数列表:	
 *             
 *              
 *      历史:
 *              $Id: mc_socket.h,v 1.2 2009/07/03 01:57:40 hzhu Exp $ 
 *		
 **************************************************************************/
#ifndef MC_SOCKET_H
#define MC_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mc_port.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if 1
//linux
#if 0
#include <pcap/pcap.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/filter.h>
#endif
#else  
//solaris
#include <pcap.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#endif




/**************************************************************************
 *	[函数名]:
 *	        mc_socket_udp_bind
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/

mc_int_t
mc_socket_udp_bind(mc_int_t port);


/**************************************************************************
 *	[函数名]:
 *	        mc_socket_udp_read
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/
mc_char_t *
mc_socket_udp_read(IN mc_int_t sock_id, 
		 OUT struct sockaddr_in *p_cli_addr);


/**************************************************************************
 *	[函数名]:
 *	        mc_socket_udp_read_buf
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/

mc_rv
mc_socket_udp_read_buf(IN mc_int_t sock_id,
		     IN mc_char_t *p_buf, 
		     IN mc_int_t buf_size);

/**************************************************************************
 *	[函数名]:
 *	        mc_socket_udp_send
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/
mc_rv 
mc_socket_udp_send(IN mc_int_t sock_id,
		 IN mc_char_t *p_buf,
		 IN struct sockaddr_in *p_dst_addr);



/**************************************************************************
 *	[函数名]:
 *	        mc_socket_udp_write
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/
mc_rv
mc_socket_udp_write(IN mc_int_t sock_id, 
                    IN mc_char_t *p_buf, 
                    IN mc_int_t buf_size,
                    IN mc_char_t *p_dest,
                    IN mc_int_t d_port );




/**************************************************************************
 *	[函数名]:
 *	        mc_socket_raw_udp_bind
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/
mc_int_t 
mc_socket_raw_udp_bind(mc_int_t port);



/**************************************************************************
 *	[函数名]:
 *	        mc_socket_raw_udp_read
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/
mc_rv
mc_socket_raw_udp_read(IN mc_int_t sock_id,
		     IN mc_char_t *p_buf, 
		     IN mc_int_t buf_size);



/**************************************************************************
 *	[函数名]:
 *	        mc_socket_raw_udp_open
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/
mc_int_t mc_socket_raw_udp_open();



/**************************************************************************
 *	[函数名]:
 *	        mc_socket_raw_udp_write
 *
 *	[函数功能]:
 *              
 *
 *	[函数参数]
 *  		空
 *            
 *	[函数返回值]
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/
mc_int_t
mc_socket_raw_udp_write(IN mc_int_t sock_id, 
		      IN mc_char_t *p_buf, 
		      IN mc_char_t *p_dest,
		      IN mc_int_t d_port );


/**************************************************************************
 *	[函数名]:
 *	        mc_socket_tcp_open
 *
 *	[函数功能]:
 *              开启TCP/IP连接
 *
 *	[函数参数]
 *  		IN mc_char_t ip    对方IP地址
 *              IN mc_int_t  port  对方端口号
 *
 *	[函数返回值]
 *              >=1       TCP/IP连接句柄
 *              MC_RV_ERR 失败
 *
 *************************************************************************/
mc_int_t mc_socket_tcp_open(IN mc_char_t *ip,
			    IN mc_int_t  port);

/**************************************************************************
 *	[函数名]:
 *	        mc_socket_tcp_close
 *
 *	[函数功能]:
 *              关闭TCP/IP连接
 *
 *	[函数参数]
 *  		IN mc_int_t  socket_id 
 *
 *	[函数返回值]
 *              MC_RV_SUC 成功
 *              MC_RV_ERR 失败
 *
 *************************************************************************/
mc_rv mc_socket_tcp_close(IN mc_int_t  socket_id);


/**************************************************************************
 *	[函数名]:
 *	        mc_socket_tcp_bind
 *
 *	[函数功能]:
 *              监听TCP/IP端口
 *
 *	[函数参数]
 *  		IN mc_int_t port 端口号
 *            
 *	[函数返回值]
 *              >=1         返回SOCKET句柄
 *              MC_RV_ERR   失败
 *
**************************************************************************/
mc_int_t 
mc_socket_tcp_bind(mc_int_t port);


/**************************************************************************
 *	[函数名]:
 *	        mc_socket_tcp_write
 *
 *	[函数功能]:
 *              写TCP/IP数据
 *
 *	[函数参数]
 *  		IN mc_int_t  socket_id   SOCKET句柄
 *              IN mc_char_t *p_buf      传送字符串
 *              IN mc_int_t  buf_size    字符串长度
 *              IN mc_int_t  flag        1 表示循环读写直到满足长度 0表示只读写
 * 
 *	[函数返回值]
 *              >= 1       写入数据长度
 *              MC_RV_ERR  失败
 *
**************************************************************************/
mc_int_t mc_socket_tcp_write(IN mc_int_t sock_id, 
			     IN mc_char_t *p_buf, 
			     IN mc_int_t buf_size,
			     IN mc_int_t flag);


/**************************************************************************
 *	[函数名]:
 *	        mc_socket_tcp_read
 *
 *	[函数功能]:
 *              读取TCP/IP数据
 *
 *	[函数参数]
 *  		IN    mc_int_t  socket_id   SOCKET句柄
 *              INOUT mc_char_t *p_buf      读取字符串
 *              IN    mc_int_t  buf_size    字符串长度
 *              IN mc_int_t  flag           1 表示循环读写直到满足长度 0表示只读写
 *            
 *	[函数返回值]
 *              >= 1       读取数据长度
 *              MC_RV_ERR  失败
 *
**************************************************************************/
mc_int_t mc_socket_tcp_read(IN    mc_int_t sock_id,
			    INOUT mc_char_t *p_buf, 
			    IN    mc_int_t buf_size,
			    IN    mc_int_t flag);
	






	
#ifdef __cplusplus
}
#endif

#endif






