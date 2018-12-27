
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <strophe.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include "xmppc.h"
#include "libtr69_client.h"
#include "OID.h"
#include "strophe.h"
#include "common.h"
#include "sasl.h"
#include "unix_sock.h"

extern char xmppsess[256];
sem_t c2s_ping_mutex;
sock_msg_t *g_xmpp_msg=NULL;

xmpp_info_t g_xmpp_info = {
    username: "",
    password: "",
    domain: "",
    resource: "",
    server: "",
    port: 5222,
    n_fail_retry : 0,
    KeepAliveInterval: -1,
    ServerConnectAttempts: 16,
    ServerRetryInitialInterval: 60,
    ServerRetryIntervalMultiplier: 2000,
    ServerRetryMaxInterval: 30720,
    UseTLS: 0,
    cur_svrip_id: 0,
    svr_ip_cnt: 0,

};

int xmpp_event_func(int event)
{
    switch (event) {
    case xmpp_msg_event_cnf_update:
        xmpp_info_update();
        xmpp_retry_update();
        break;

    }
    return 0;
}

int set_svrip_cnt(int cnt)
{
    g_xmpp_info.svr_ip_cnt = cnt;

    return g_xmpp_info.svr_ip_cnt;
}


int get_svrip_id()
{
    return g_xmpp_info.cur_svrip_id;
}

int check_xmpp_retry()
{
    if (0 == g_xmpp_info.ServerConnectAttempts) {
        fprintf(stderr,"xmppc ServerConnectAttempts is 0\n");
        g_xmpp_info.n_fail_retry = 0;
    } else if (g_xmpp_info.n_fail_retry > g_xmpp_info.ServerConnectAttempts) {
        /* check if have more server's IP address to retry*/
        g_xmpp_info.cur_svrip_id ++;

        if (g_xmpp_info.cur_svrip_id < g_xmpp_info.svr_ip_cnt) {
            fprintf(stderr, "Try next server's IP address ...\n");
            g_xmpp_info.n_fail_retry = 0;

            return 1;
        } else {
            fprintf(stderr, "No more server's IP address can try\n");
            g_xmpp_info.n_fail_retry = 0;
        }
    }

    return g_xmpp_info.n_fail_retry;
}

int inc_xmpp_retry()
{
    g_xmpp_info.n_fail_retry ++;

    tr69_set_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Status",
                    XMPP_STATUS_DISABLED, TR69_NODE_LEAF_TYPE_STRING);

    return g_xmpp_info.n_fail_retry;
}

void* xmpp_event_thread(void *argv)
{
    g_xmpp_msg = unix_sock_server_create(XMPP_MSG_SOCK_FILE, xmpp_event_func);
    unix_sock_server_service(g_xmpp_msg);

    return NULL;
}

void xmpp_info_update()
{
    char *p_str = NULL;
    int type = 0;

    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Username", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_STRING && p_str != NULL
                && strcmp(g_xmpp_info.username, p_str)!=0 )  {
            snprintf(g_xmpp_info.username, sizeof(g_xmpp_info.username), "%s", p_str);
            fprintf(stderr, "xmpp username: %s\n", g_xmpp_info.username);
            free(p_str);
        }
    }

    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Password", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_STRING && p_str != NULL
                && strcmp(g_xmpp_info.password, p_str)!=0)  {
            snprintf(g_xmpp_info.password, sizeof(g_xmpp_info.password), "%s", p_str);
            fprintf(stderr, "xmpp password: %s\n", g_xmpp_info.password);
            free(p_str);
        }
    }

    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Domain", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_STRING && p_str != NULL
                && strcmp(g_xmpp_info.domain, p_str)!=0)  {
            snprintf(g_xmpp_info.domain, sizeof(g_xmpp_info.domain), "%s", p_str);
            fprintf(stderr, "xmpp domain: %s\n", g_xmpp_info.domain);
            free(p_str);
        }
    }

    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Resource", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_STRING && p_str != NULL
                && strcmp(g_xmpp_info.resource, p_str)!=0)  {
            snprintf(g_xmpp_info.resource, sizeof(g_xmpp_info.resource), "%s", p_str);
            fprintf(stderr, "xmpp resource: %s\n", g_xmpp_info.resource);
            free(p_str);
        }
    }

#if 0
    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Server.1.ServerAddress", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_STRING && p_str != NULL
                && strcmp(g_xmpp_info.server, p_str)!=0)  {
            snprintf(g_xmpp_info.server, sizeof(g_xmpp_info.server), "%s", p_str);
            fprintf(stderr, "xmpp server: %s\n", g_xmpp_info.server);
            free(p_str);
        }
    }

    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Server.1.Port", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_UINT )  {
            g_xmpp_info.port = (unsigned int)p_str;
            fprintf(stderr, "xmpp port: %d\n", g_xmpp_info.port);
        }
    }
#endif

    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.KeepAliveInterval", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_INT )  {
            g_xmpp_info.KeepAliveInterval = (int)p_str;
            fprintf(stderr, "xmpp KeepAliveInterval: %d\n", g_xmpp_info.KeepAliveInterval);
        }
    }

    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.UseTLS", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_BOOL )  {
            g_xmpp_info.UseTLS = (int)p_str;
            fprintf(stderr, "xmpp UseTLS: %d\n", g_xmpp_info.UseTLS);
        }
    }
}

void xmpp_retry_update()
{
    char *p_str = NULL;
    int type = 0;


    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.ServerConnectAttempts", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_UINT )  {
            g_xmpp_info.ServerConnectAttempts = (unsigned int)p_str;
            fprintf(stderr, "xmpp ServerConnectAttempts: %d\n", g_xmpp_info.ServerConnectAttempts);
        }
    }
    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.ServerRetryInitialInterval", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_UINT )  {
            g_xmpp_info.ServerRetryInitialInterval = (unsigned int)p_str;
            fprintf(stderr, "xmpp ServerRetryInitialInterval: %d\n", g_xmpp_info.ServerRetryInitialInterval);
        }
    }
    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.ServerRetryIntervalMultiplier", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_UINT )  {
            g_xmpp_info.ServerRetryIntervalMultiplier = (unsigned int)p_str;
            fprintf(stderr, "xmpp ServerRetryIntervalMultiplier: %d\n", g_xmpp_info.ServerRetryIntervalMultiplier);
        }
    }
    if (tr69_get_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.ServerRetryMaxInterval", (void *)&p_str, &type)==0) {
        if (type ==  TR69_NODE_LEAF_TYPE_UINT )  {
            g_xmpp_info.ServerRetryMaxInterval = (unsigned int)p_str;
            fprintf(stderr, "xmpp ServerRetryMaxInterval: %d\n", g_xmpp_info.ServerRetryMaxInterval);
        }
    }

}

int _handle_missing_pong(xmpp_conn_t * const conn,
                    void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;
    xmpp_debug(conn->ctx, "xmpp", "c2s ping timeout");

    xmpp_stop(ctx);
    sem_post(&c2s_ping_mutex);
    inc_xmpp_retry();

    return 0;
}

void *c2s_ping_thread(void *argv)
{
    xmpp_conn_t * conn = (xmpp_conn_t *) argv;
    xmpp_ctx_t *ctx = xmpp_conn_get_context(conn);
    xmpp_stanza_t *xmpp_ping;
    xmpp_stanza_t *body;
    struct timespec tv;
    int ret = -1;

    xmpp_ping = xmpp_stanza_new(ctx);  //p_xmpp_conn->ctx);

    fprintf(stderr, "from=%s\n", xmpp_conn_get_bound_jid(conn));

    xmpp_stanza_set_name(xmpp_ping, "iq");
    xmpp_stanza_set_type(xmpp_ping, "get");

    xmpp_stanza_set_attribute(xmpp_ping, "from", xmpp_conn_get_bound_jid(conn));
    xmpp_stanza_set_id(xmpp_ping, "c2s");

    body = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(body, "ping");
    xmpp_stanza_set_ns(body, "urn:xmpp:ping");

    xmpp_stanza_add_child(xmpp_ping, body);

    while (1) {
        if (g_xmpp_info.KeepAliveInterval == -1)
            tv.tv_sec = time(NULL) + CUSTOM_PING_INTERVAL;
        else if (g_xmpp_info.KeepAliveInterval>0)
            tv.tv_sec = time(NULL) + g_xmpp_info.KeepAliveInterval;

        tv.tv_nsec = 0;

        ret = sem_timedwait(&c2s_ping_mutex, &tv);
        if (ret == -1) {
            if (errno == ETIMEDOUT)
                fprintf(stderr, "send xmpp ping ...\n");
        } else {
            fprintf(stderr, "not recv pong from server, break.\n");
            break;
        }

        if (ctx->loop_status != XMPP_LOOP_RUNNING)
            break;

        xmpp_send(conn, xmpp_ping);
        xmpp_debug(conn->ctx, "xmpp", "add ping timer");
        xmpp_timed_handler_add(conn, _handle_missing_pong, C2S_PING_TIMEOUT, ctx);
    }

    xmpp_stanza_release(xmpp_ping);

    return NULL;
}

int handle_reply(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
//    char *type;
    char *name;
    xmpp_stanza_t *reply;
    xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;
    char *p_str = NULL;
    int type = 0;
    char connusername[256]={0};
    char connpassword[256]={0};
    xmpp_stanza_t *body;
    xmpp_stanza_t *connreq, *error;
    xmpp_stanza_t *user_stanza, *pwd_stanza;
    xmpp_stanza_t *noauth_stanza;
    xmpp_stanza_t *text;

    body = xmpp_stanza_get_children(stanza);
    name = xmpp_stanza_get_name(body);
//    fprintf(stderr,"child ns=%s, name=%s\n", ns, name);

    if (strcmp(name, "ping")==0) {

        reply = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(reply, "iq");
        xmpp_stanza_set_type(reply, "result");

        xmpp_stanza_set_attribute(reply, "from", xmpp_stanza_get_attribute(stanza, "to"));
        xmpp_stanza_set_attribute(reply, "to", xmpp_stanza_get_attribute(stanza, "from"));
        xmpp_stanza_set_id(reply, xmpp_stanza_get_id(stanza));

        xmpp_send(conn, reply);
        xmpp_stanza_release(reply);
    } else if (strcmp(name, "connectionRequest")==0) {
        char *xmpprequser=NULL;
        char *xmppreqpwd=NULL;

        xmpp_debug(conn->ctx, "xmpp", "receive connectionRequest from xmppserver");
        xmpprequser = xmpp_stanza_get_text(xmpp_stanza_get_child_by_name(body, "username"));
        xmpp_debug(conn->ctx, "xmpp", "xmpp request username: %s", xmpprequser);

        xmppreqpwd = xmpp_stanza_get_text(xmpp_stanza_get_child_by_name(body, "password"));
        xmpp_debug(conn->ctx, "xmpp", "xmpp request password: %s", xmppreqpwd);

        if (tr69_get_unfresh_leaf_data(TR69_OID_MANAGEMENT_SERVER".ConnectionRequestUsername", (void *)&p_str, &type)==0) {
            if (type ==  TR69_NODE_LEAF_TYPE_STRING && p_str != NULL)  {
                snprintf(connusername, sizeof(connusername), "%s", p_str);
                xmpp_debug(conn->ctx, "xmpp", "get tr69 ConnectionRequestUsername: %s", connusername);
                free(p_str);
            }
        }
        if (tr69_get_unfresh_leaf_data(TR69_OID_MANAGEMENT_SERVER".ConnectionRequestPassword", (void *)&p_str, &type)==0) {
            if (type ==  TR69_NODE_LEAF_TYPE_STRING && p_str != NULL)  {
                snprintf(connpassword, sizeof(connpassword), "%s", p_str);
                xmpp_debug(conn->ctx, "xmpp", "get tr69 ConnectionRequestPassword: %s", connpassword);
                free(p_str);
            }
        }

        if (!xmpprequser || !xmppreqpwd) {
            xmpp_error(conn->ctx, "xmpp","xmpp request username/password is NULL!");
        } else if (strcmp(connusername, xmpprequser)==0 &&
                        strcmp(connpassword, xmppreqpwd)==0) {
            xmpp_debug(conn->ctx, "xmpp","xmpp request username/password is valid");

            xmpp_debug(conn->ctx, "xmpp", "send connectionRequest to tr69");
            system("msg -m");

            /* Connection Request Successful Response  */
            reply = xmpp_stanza_new(ctx);
            xmpp_stanza_set_name(reply, "iq");
            xmpp_stanza_set_type(reply, "result");

            xmpp_stanza_set_attribute(reply, "from", xmpp_stanza_get_attribute(stanza, "to"));
            xmpp_stanza_set_attribute(reply, "to", xmpp_stanza_get_attribute(stanza, "from"));
            xmpp_stanza_set_id(reply, xmpp_stanza_get_id(stanza));

            xmpp_send(conn, reply);
            xmpp_stanza_release(reply);

        } else {
            xmpp_error(conn->ctx, "xmpp", "xmpp request username/password is invalid!");

            reply = xmpp_stanza_new(ctx);
            xmpp_stanza_set_name(reply, "iq");
            xmpp_stanza_set_type(reply, "error");

            xmpp_stanza_set_attribute(reply, "from", xmpp_stanza_get_attribute(stanza, "to"));
            xmpp_stanza_set_attribute(reply, "to", xmpp_stanza_get_attribute(stanza, "from"));
            xmpp_stanza_set_id(reply, xmpp_stanza_get_id(stanza));

            connreq = xmpp_stanza_new(ctx);
            xmpp_stanza_set_name(connreq,"connectionRequest");
            xmpp_stanza_set_ns(connreq, "urn:broadband-forum-org:cwmp:xmppConnReq-1-0");

            user_stanza = xmpp_stanza_new(ctx);
            xmpp_stanza_set_name(user_stanza, "username");
            text = xmpp_stanza_new(ctx);
            xmpp_stanza_set_text(text, xmpprequser);
            xmpp_stanza_add_child(user_stanza, text);
            xmpp_stanza_add_child(connreq, user_stanza);

            pwd_stanza = xmpp_stanza_new(ctx);
            xmpp_stanza_set_name(pwd_stanza, "password");
            text = xmpp_stanza_new(ctx);
            xmpp_stanza_set_text(text, xmppreqpwd);
            xmpp_stanza_add_child(pwd_stanza, text);
            xmpp_stanza_add_child(connreq, pwd_stanza);
            xmpp_stanza_add_child(reply, connreq);

            error = xmpp_stanza_new(ctx);
            xmpp_stanza_set_name(error, "error");
            xmpp_stanza_set_type(error, "cancel");
            noauth_stanza = xmpp_stanza_new(ctx);
            xmpp_stanza_set_name(noauth_stanza, "not-authorized");
            xmpp_stanza_set_ns(noauth_stanza, "urn:ietf:params:xml:ns:xmpp-stanzas");
            xmpp_stanza_add_child(error, noauth_stanza);
            xmpp_stanza_add_child(reply, error);

            xmpp_error(conn->ctx, "xmpp","send connectionRequest not-authorized reply");
            xmpp_send(conn, reply);
            xmpp_stanza_release(reply);
        }

        xmpp_free(conn->ctx,xmpprequser);
        xmpp_free(conn->ctx,xmppreqpwd);
    }

    return 1;
}

int handle_result(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
    xmpp_debug(conn->ctx, "xmpp", "get iq result");
    xmpp_debug(conn->ctx, "xmpp", "delete ping timer");
    xmpp_timed_handler_delete(conn, _handle_missing_pong);

    return 1;
}

int message_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
    xmpp_stanza_t *reply, *body, *text;
    char *intext, *replytext;
    xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;

    if(!xmpp_stanza_get_child_by_name(stanza, "body"))
        return 1;
    if(!strcmp(xmpp_stanza_get_attribute(stanza, "type"), "error"))
        return 1;

    intext = xmpp_stanza_get_text(xmpp_stanza_get_child_by_name(stanza, "body"));

    printf("Incoming message from %s: %s\n", xmpp_stanza_get_attribute(stanza, "from"), intext);
    reply = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(reply, "message");
    xmpp_stanza_set_type(reply, xmpp_stanza_get_type(stanza)?xmpp_stanza_get_type(stanza):"chat");
    xmpp_stanza_set_attribute(reply, "to", xmpp_stanza_get_attribute(stanza, "from"));

    body = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(body, "body");

    replytext = malloc(strlen(intext) + 1);
    strcpy(replytext, intext);

    text = xmpp_stanza_new(ctx);
    xmpp_stanza_set_text(text, replytext);
    xmpp_stanza_add_child(body, text);
    xmpp_stanza_add_child(reply, body);

    xmpp_send(conn, reply);
    xmpp_stanza_release(reply);
    free(replytext);
    xmpp_free(conn->ctx,intext);

    return 1;
}

int _connect_unlock(xmpp_conn_t * const conn)
{
    xmpp_debug(conn->ctx, "xmpp","forced quit from connect lock");

    int xmpptrigger = 1;
    tr69_set_leaf_data(TR69_OID_XMPP_CONNECTION".1.X_ACTIONTEC_COM_Trigger", (void *)&xmpptrigger,
                    TR69_NODE_LEAF_TYPE_BOOL);

    exit(1);
}

/* define a handler for connection events */
void conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, 
		  const int error, xmpp_stream_error_t * const stream_error,
		  void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;
    char alias[256]={0};
    char *pos=NULL;
    pthread_t tid_ping=-1;

    if (status == XMPP_CONN_CONNECT) {
        xmpp_stanza_t *pres;
        xmpp_stanza_t *body, *text;

        fprintf(stderr, "DEBUG: connected\n");
        g_xmpp_info.n_fail_retry = 0;

        xmpp_handler_add(conn,message_handler, NULL, "message", NULL, ctx);
        xmpp_handler_add(conn,handle_reply, NULL, "iq", "get", ctx);
        xmpp_handler_add(conn,handle_result, NULL, "iq", "result", ctx);

        if (g_xmpp_info.KeepAliveInterval)
            pthread_create(&tid_ping, NULL, c2s_ping_thread, conn);

        /* Send initial <presence/> so that we appear online to contacts */
        pres = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(pres, "presence");
        fprintf(stderr, "bound jid=%s\n", xmpp_conn_get_bound_jid(conn));

        tr69_set_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.JabberID",
                        xmpp_conn_get_bound_jid(conn), TR69_NODE_LEAF_TYPE_STRING);

        tr69_set_unfresh_leaf_data(TR69_OID_MANAGEMENT_SERVER".ConnReqJabberID",
                        xmpp_conn_get_bound_jid(conn), TR69_NODE_LEAF_TYPE_STRING);

        tr69_set_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Status",
                        XMPP_STATUS_ENABLED, TR69_NODE_LEAF_TYPE_STRING);

        if (g_xmpp_info.UseTLS) {
            int tlsconn = 1;
            tr69_set_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.TLSEstablished", (void *)&tlsconn,
                            TR69_NODE_LEAF_TYPE_BOOL);
        }

        fprintf(stderr, "jid=%s\n", xmpp_conn_get_jid(conn));

        pos = strstr(xmpp_conn_get_bound_jid(conn),"@");
        fprintf(stderr, "pos=%s\n", pos);
        if (pos)
            snprintf(alias, sizeof(alias), "%s%s", xmpp_conn_get_jid(conn), pos);

        xmpp_stanza_set_attribute(pres, "id", xmpp_conn_get_jid(conn));
        xmpp_stanza_set_attribute(pres, "from", alias);

        body = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(body, "priority");
        text = xmpp_stanza_new(ctx);
        xmpp_stanza_set_text(text, "1");
        xmpp_stanza_add_child(body, text);
        xmpp_stanza_add_child(pres, body);

        xmpp_send(conn, pres);
        xmpp_stanza_release(pres);

    }
    else {
        fprintf(stderr, "DEBUG: disconnected\n");
        xmpp_stop(ctx);
    }
}

int main(int argc, char **argv)
{
    xmpp_ctx_t *ctx;
    xmpp_conn_t *conn;
    xmpp_log_t *log;
    pthread_t tid_event;
    int retry_time = 0;

    /* Get xmpp info from data_center */
    fprintf(stderr,"update xmpp info\n");
    xmpp_info_update();
    xmpp_retry_update();

#if 0
    if (argc > 3) {
        snprintf(g_xmpp_info.username, sizeof(g_xmpp_info.username), "%s", argv[1]);
        snprintf(g_xmpp_info.password, sizeof(g_xmpp_info.password), "%s", argv[2]);
        snprintf(g_xmpp_info.domain, sizeof(g_xmpp_info.domain), "%s", argv[3]);
    }
#endif

    /* init library */
    xmpp_initialize();

    pthread_create(&tid_event, NULL, xmpp_event_thread, NULL);

    do {
        sem_init(&c2s_ping_mutex, 0, 0);

        log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG);

        /* create a context */
        ctx = xmpp_ctx_new(NULL, log);

        /* create a connection */
        conn = xmpp_conn_new(ctx);

        /* setup authentication information */
        xmpp_conn_set_jid(conn, g_xmpp_info.username);
        xmpp_conn_set_pass(conn, g_xmpp_info.password);
        xmpp_conn_set_domain(conn, g_xmpp_info.domain);
        xmpp_conn_set_resource(conn, g_xmpp_info.resource);

        if (! g_xmpp_info.UseTLS)
            xmpp_conn_disable_tls(conn);

        tr69_set_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.Status",
                        XMPP_STATUS_CONNECTING, TR69_NODE_LEAF_TYPE_STRING);
        int tlsconn = 0;
        tr69_set_unfresh_leaf_data(TR69_OID_XMPP_CONNECTION".1.TLSEstablished", (void *)&tlsconn,
                        TR69_NODE_LEAF_TYPE_BOOL);


        xmpp_debug(conn->ctx, "xmpp", "add check sock connect lock");
        handler_add_timed(conn, _connect_unlock, XMPP_CONNECT_TIMEOUT, NULL);

        /* initiate connection */
        xmpp_connect_client(conn, NULL , g_xmpp_info.port, conn_handler, ctx);

        xmpp_debug(conn->ctx, "xmpp", "delete check sock connect lock");
        xmpp_timed_handler_delete(conn, _connect_unlock);

        if (conn->sock != -1)
            xmpp_run(ctx);

        /* release our connection and context */
        xmpp_conn_release(conn);
        xmpp_ctx_free(ctx);

        if (check_xmpp_retry()) {
            retry_time = get_retry_time(&g_xmpp_info);
            sleep(retry_time);
        } else {
            fprintf(stderr,"xmppc quit!\n");
            break;
        }

    } while (1);

    /* final shutdown of the library */
    xmpp_shutdown();

    int xmpptrigger = 1;
    tr69_set_leaf_data(TR69_OID_XMPP_CONNECTION".1.X_ACTIONTEC_COM_Trigger", (void *)&xmpptrigger,
                    TR69_NODE_LEAF_TYPE_BOOL);
    return 0;
}
