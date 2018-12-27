
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "unix_sock.h"

int unix_sock_client_send(char *p_sock_file, int event)
{
    int sockfd,len;
    struct sockaddr_un addr;

    sockfd=socket(AF_UNIX,SOCK_DGRAM,0);
    if(sockfd<0 || !p_sock_file)  {
        printf("sock create failed!\n");
        return -1;
    }

    bzero(&addr,sizeof(struct sockaddr_un));
    addr.sun_family=AF_UNIX;
    snprintf(addr.sun_path,sizeof(addr.sun_path), "%s", p_sock_file);

    len = strlen(addr.sun_path)+sizeof(addr.sun_family);
    sendto(sockfd, (char *)&event, sizeof(event), 0, (struct sockaddr*)&addr,len);

    return 0;
}

sock_msg_t *unix_sock_server_create(char *p_sock_file, sock_msg_func cb_func)
{
    sock_msg_t *p_msg = NULL;
    if(!p_sock_file)  {
        printf("sock file is NULL!\n");
        return NULL;
    }
    p_msg = (sock_msg_t *)malloc(sizeof(sock_msg_t));
    if(!p_msg)  {
        printf("sock msg create failed!\n");
        return p_msg;
    }
    memset(p_msg, 0, sizeof(sock_msg_t));
    snprintf(p_msg->sock_file, sizeof(p_msg->sock_file), "%s", p_sock_file);
    p_msg->cb_func = cb_func;
    p_msg->quit = 0;

    return p_msg;
}

void unix_sock_server_cleanup(sock_msg_t *p_msg)
{
    p_msg->quit = 1;
    close(p_msg->master_id);
    sleep(1);
    free(p_msg);
}

int unix_sock_server_service(sock_msg_t *p_msg)
{
    int len;
    int socket_fd;
    struct sockaddr_un addr;
    int event = -1;

    bzero(&addr, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", p_msg->sock_file);
    len = strlen(addr.sun_path)+sizeof(addr.sun_family);

    socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(socket_fd<0) {
        printf("server socket error!\n");
        return -1;
    }
    p_msg->master_id = socket_fd;
    unlink(p_msg->sock_file);

    if(bind(socket_fd, (struct sockaddr *)&addr, len)<0) {
        printf("bind error\n");
        return -1;
    }

    p_msg->master_id = socket_fd;

    while(!p_msg->quit)   {
        recvfrom(socket_fd, (char*)&event, sizeof(event), 0, NULL, NULL);
        if (p_msg->cb_func)
            p_msg->cb_func(event);

    }
    return 0;
}

