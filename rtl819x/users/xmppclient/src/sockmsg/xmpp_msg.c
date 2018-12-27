
#include <stdio.h>
#include "unix_sock.h"

int main(int argc, char **argv)
{
    char cmd[32] = {0};

    if (argc < 2){
        return 0;
    }

    snprintf(cmd, sizeof(cmd)-1, "%s", argv[1]);

    switch(cmd[1]){
        case 'c':
            unix_sock_client_send(XMPP_MSG_SOCK_FILE, xmpp_msg_event_cnf_update);
            fprintf(stdout, "xmpp config info update\n");
            break;

    }

    return 0;
}
