/* tls_openssl.c
** strophe XMPP client library -- TLS abstraction openssl impl.
**
** Copyright (C) 2005-008 Collecta, Inc. 
**
**  This software is provided AS-IS with no warranty, either express
**  or implied.
**
**  This software is distributed under license and may not be copied,
**  modified or distributed except as expressly authorized under the
**  terms of the license contained in the file LICENSE.txt in this
**  distribution.
*/

/** @file
 *  TLS implementation with OpenSSL.
 */

#include <string.h>

#ifndef _WIN32
#include <sys/select.h>
#else
#include <winsock2.h>
#endif

#include <openssl/ssl.h>
#include <openssl/rand.h>

#include "common.h"
#include "tls.h"
#include "sock.h"

struct _tls {
    xmpp_ctx_t *ctx;
    sock_t sock;
    SSL_CTX *ssl_ctx;
    SSL *ssl;
    int lasterror;
};

void tls_initialize(void)
{
    SSL_library_init();
    SSL_load_error_strings();

#ifdef AEI_XMPP
    if (!RAND_load_file("/dev/urandom", 1024))
    { char buf[1024];
      RAND_seed(buf, sizeof(buf));
      while (!RAND_status())
      { int r = rand();
        RAND_seed(&r, sizeof(int));
      }
    }
#endif
}

void tls_shutdown(void)
{
    return;
}

int tls_error(tls_t *tls)
{
    return tls->lasterror;
}

tls_t *tls_new(xmpp_ctx_t *ctx, sock_t sock)
{
    tls_t *tls = xmpp_alloc(ctx, sizeof(*tls));
#ifdef AEI_XMPP
    FILE *fp=NULL;
#endif

    if (tls) {
        int ret;
        memset(tls, 0, sizeof(*tls));

        tls->ctx = ctx;
        tls->sock = sock;
        tls->ssl_ctx = SSL_CTX_new(SSLv23_method());

        SSL_CTX_set_client_cert_cb(tls->ssl_ctx, NULL);
#ifdef AEI_XMPP
        SSL_CTX_set_mode (tls->ssl_ctx, SSL_MODE_AUTO_RETRY);
#else
        SSL_CTX_set_mode (tls->ssl_ctx, SSL_MODE_ENABLE_PARTIAL_WRITE);
#endif

#ifdef AEI_XMPP
        if ( (fp=fopen(XMPP_CERT_FILE, "r"))!=NULL ) {
            fclose(fp);

            if (!(SSL_CTX_load_verify_locations(tls->ssl_ctx, XMPP_CERT_FILE, NULL))) {
                xmpp_error(ctx,"SSL_CTX_load_verify_locations %s failed\n", XMPP_CERT_FILE);
                SSL_CTX_free(tls->ssl_ctx);
                xmpp_free(ctx, tls);
                tls = NULL;
                return tls;
            }
            SSL_CTX_set_verify (tls->ssl_ctx, SSL_VERIFY_PEER, NULL);

        } else {
            SSL_CTX_set_verify (tls->ssl_ctx, SSL_VERIFY_NONE, NULL);
        }

#else
        SSL_CTX_set_verify (tls->ssl_ctx, SSL_VERIFY_NONE, NULL);
#endif

        tls->ssl = SSL_new(tls->ssl_ctx);

        ret = SSL_set_fd(tls->ssl, sock);
        if (ret <= 0) {
            tls->lasterror = SSL_get_error(tls->ssl, ret);
            xmpp_debug(ctx, "tls", "TLS failed, lasterror=%d", tls->lasterror);
            xmpp_debug(ctx,"openssl error reason: %s", ERR_reason_error_string(ERR_peek_last_error()) );
            xmpp_debug(ctx,"openssl error lib: %s", ERR_lib_error_string(ERR_peek_last_error()) );
            xmpp_debug(ctx,"openssl error func: %s", ERR_func_error_string(ERR_peek_last_error()) );
            tls_error(tls);
            tls_free(tls);
            tls = NULL;
        }
    }

    return tls;
}

void tls_free(tls_t *tls)
{
    SSL_free(tls->ssl);
    SSL_CTX_free(tls->ssl_ctx);
    xmpp_free(tls->ctx, tls);
    return;
}

int tls_set_credentials(tls_t *tls, const char *cafilename)
{
    return -1;
}

int tls_start(tls_t *tls)
{
    int ret = -1;

    /* Since we're non-blocking, loop the connect call until it
       succeeds or fails */
    while (ret == -1) {
//        fprintf(stderr, "call SSL_connect\n");
        ret = SSL_connect(tls->ssl);
//        fprintf(stderr, "SSL_connect return %d\n", ret);

        /* wait for something to happen on the sock before looping back */
        if (ret == -1) {
            fd_set fds;
            struct timeval tv;

            tv.tv_sec = 0;
            tv.tv_usec = 1000;

            FD_ZERO(&fds); 
            FD_SET(tls->sock, &fds);

            select(tls->sock + 1, &fds, &fds, NULL, &tv);
        }
    }

    if (ret <= 0) {
        tls->lasterror = SSL_get_error(tls->ssl, ret);
        return 0;
    }

    return 1;

}

int tls_stop(tls_t *tls)
{
    int ret;

    ret = SSL_shutdown(tls->ssl);

    if (ret <= 0) {
        tls->lasterror = SSL_get_error(tls->ssl, ret);
        return 0;
    }

    return 1;
}

int tls_is_recoverable(int error)
{
    return (error == SSL_ERROR_NONE || error == SSL_ERROR_WANT_READ
            || error == SSL_ERROR_WANT_WRITE
            || error == SSL_ERROR_WANT_CONNECT
            || error == SSL_ERROR_WANT_ACCEPT);
}

int tls_pending(tls_t *tls)
{
    return SSL_pending(tls->ssl);
}

int tls_read(tls_t *tls, void * const buff, const size_t len)
{
    int ret = SSL_read(tls->ssl, buff, len);

    if (ret <= 0) {
        tls->lasterror = SSL_get_error(tls->ssl, ret);
    }

    return ret;
}

int tls_write(tls_t *tls, const void * const buff, const size_t len)
{
    int ret = SSL_write(tls->ssl, buff, len);

    if (ret <= 0) {
        tls->lasterror = SSL_get_error(tls->ssl, ret);
    }

    return ret;
}

int tls_clear_pending_write(tls_t *tls)
{
    return 0;
}
