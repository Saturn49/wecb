
#ifndef _UNIX_SOCK_H
#define _UNIX_SOCK_H

#define XMPP_MSG_SOCK_FILE  "/tmp/xmpp_sock"

typedef int (*sock_msg_func)(int event);

typedef struct {
        char sock_file[32];
        sock_msg_func cb_func;
        int master_id;
        int quit;
} sock_msg_t;

typedef enum {
    xmpp_msg_event_cnf_update = 1,
    xmpp_msg_event_log_start,
    xmpp_msg_event_log_stop,

} xmpp_event_t;

int unix_sock_client_send(char *p_sock_file, int event);
sock_msg_t *unix_sock_server_create(char *p_sock_file, sock_msg_func cb_func);
void unix_sock_server_cleanup(sock_msg_t *p_msg);
int unix_sock_server_service(sock_msg_t *p_msg);

#endif
