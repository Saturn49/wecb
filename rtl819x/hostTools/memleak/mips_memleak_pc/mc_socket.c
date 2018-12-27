/**************************************************************************
 *
 *	版权:	
 *              
 *
 *	文件名:	
 *              mc_socket.c
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
 *              $Id: mc_socket.c,v 1.3 2009/07/03 02:07:19 hzhu Exp $
 *		
 **************************************************************************/
#include "mc_port.h"
#include "mc_socket.h"





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
 *                成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/

mc_int_t
mc_socket_udp_bind(mc_int_t port)
{
        struct sockaddr_in s_in;
	int sock_id;

	memset(&s_in, 0, sizeof(s_in));
	s_in.sin_family = AF_INET;
	s_in.sin_addr.s_addr = INADDR_ANY;
	s_in.sin_port = htons((unsigned short)port);

	FVASSERT((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) > 0);
	FVASSERT((bind(sock_id, (struct sockaddr *)&s_in, sizeof(s_in)) >= 0));
	
	return sock_id;
}


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
		 OUT struct sockaddr_in *p_cli_addr)
{
  //len must be static var
        static socklen_t len;
 	mc_char_t *p_buf;
	mc_int_t buf_size = 0;
	
  
	VASSERT_RV(p_cli_addr != NULL, NULL);
	
	MC_SMALLOC_RV(p_buf, mc_char_t, MAX_BUF_SIZE, NULL);
	FVASSERT_RV_ACT((buf_size = recvfrom(sock_id, p_buf, MAX_BUF_SIZE, 0, (struct sockaddr *)p_cli_addr, &len)) >= 0, NULL, MC_FREE(p_buf);perror("recvfrom\n "););
	p_buf[buf_size] = '\0';
	return p_buf;
}

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
		     IN mc_int_t buf_size)
{
  //len must be static var
        struct sockaddr_in cli_addr;
        static socklen_t len;
 	
	VASSERT(p_buf != NULL);
	VASSERT(buf_size > 0);
		
	FVASSERT((buf_size = recvfrom(sock_id, p_buf, buf_size, 0, (struct sockaddr *)&cli_addr, &len)) >= 0);
	p_buf[buf_size] = '\0';
	
	return MC_RV_SUC;
}


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
		 IN struct sockaddr_in *p_dst_addr)
{
         FASSERT(sendto(sock_id, p_buf, strlen(p_buf), MSG_DONTWAIT, (struct sockaddr *)p_dst_addr, sizeof(struct sockaddr)));
	 
	 return MC_RV_SUC;
}


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
mc_int_t
mc_socket_udp_write(IN mc_int_t sock_id, 
                    IN mc_char_t *p_buf, 
                    IN mc_int_t buf_size,
                    IN mc_char_t *p_dest,
                    IN mc_int_t d_port )
{
         struct sockaddr_in dst_addr;
  
	 if (sock_id <= 0){
	   FVASSERT((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) > 0);
	 }
	 
	 memset(&dst_addr, 0, sizeof(dst_addr));
	 dst_addr.sin_family =AF_INET;
	 dst_addr.sin_addr.s_addr = inet_addr(p_dest);
	 dst_addr.sin_port = htons(d_port);
	 
	 FVASSERT(sendto(sock_id, p_buf, buf_size, MSG_DONTWAIT, (struct sockaddr *)&dst_addr, sizeof(struct sockaddr )) >= 0);
	 
	 return sock_id;
}


/**************************************************************************
 *
 *              TCP/IP功能函数实现
 *
 *************************************************************************/

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
mc_int_t 
mc_socket_tcp_open(IN mc_char_t *ip,
		   IN mc_int_t  port)
{
	struct sockaddr_in 	address;
	mc_int_t 		handle = -1;


#ifdef _WIN32
	WORD    dVer;
	WSADATA dData;
	dVer = MAKEWORD(2, 2);
	WSAStartup(dVer, &dData);
#endif 
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons((unsigned short)port);

	handle = socket(AF_INET, SOCK_STREAM, 0);
	
	if( connect(handle, (struct sockaddr *)&address, sizeof(address)) == -1) {
		return MC_RV_ERR;
	}else {
		return handle;
	}
}


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
mc_rv
mc_socket_tcp_close(IN mc_int_t  socket_id)
{
	return close(socket_id);
}

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
mc_socket_tcp_bind(mc_int_t port)
{
  	static struct sockaddr_in s_in;
	mc_int_t sock_id;
	mc_int_t                opt_val   = 1;

	memset(&s_in, 0, sizeof(s_in));
	s_in.sin_family = AF_INET;

#define MQ_SERVER_IP "192.168.1.80"

	s_in.sin_addr.s_addr = inet_addr(MQ_SERVER_IP);//INADDR_ANY;
	s_in.sin_port = htons(port);

	FVASSERT((sock_id = socket(AF_INET, SOCK_STREAM, 0)) > 0);
	FASSERT(setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR,  (char *)&opt_val, sizeof(int)));
	FVASSERT(bind(sock_id, (struct sockaddr *)&s_in, sizeof(s_in)) >= 0);
	FVASSERT(listen(sock_id, 64 ) >= 0);

	return sock_id;
}


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
 *                        成功
 *              MC_RV_ERR 失败
 *
**************************************************************************/
mc_int_t
mc_socket_tcp_write(mc_int_t sock_id, 
		    mc_char_t *buf, 
		    mc_int_t buf_size,
		    mc_int_t flag)
{
        int 	size = 0;
        int 	res  = 0;

	
	VASSERT(sock_id > 0);
	VASSERT(buf != NULL);
	VASSERT(buf_size > 0);
	
	while(size < buf_size){
        	res = send(sock_id, (char *)buf + size, buf_size - size, 0);
   		if (res <= 0){
			return MC_RV_ERR;
		}
        	size += res;
		if (!flag){
			return size;
		}
        }
	
	return size;
	
}


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
			    INOUT mc_char_t *buf, 
			    IN    mc_int_t buf_size,
			    IN    mc_int_t flag)
{
	int 	size = 0;
	int 	res  = 0;

	
	VASSERT(sock_id > 0);
	VASSERT(buf != NULL);
	VASSERT(buf_size > 0);
	
	while(size < buf_size){
		res = recv(sock_id, (char *)buf + size, buf_size - size, 0); 
        	if(res <= 0){
               		return -1;
               	}
		size += res;
		if (!flag){
			return size;
		}
	}
	
        return size;
}





