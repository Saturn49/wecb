
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#include <strophe.h>
#include "xmppc.h"

static unsigned long my_rand(void )
{
    int fd = -1;
    unsigned long rand_value = 0;
    fd =open("/dev/urandom", O_RDONLY);
    if (fd < 0)
        return rand();
    if (read(fd, (char *)&rand_value, sizeof(unsigned long)) <= 0)
        return rand();
    close(fd);

    return (rand_value % RAND_MAX);
}

static int gen_random()
{
    static int rand_init = 0;

    if( ! rand_init ) {
        rand_init = 1;

        srand(time(NULL));
    }

    return rand();
}

/*
 * Generate a random number, begin <= random_num < end
 */
int random_num(int begin, int end)
{
    // RANDMAX being 2147483647, (RAND_MAX + 1) will cause integer overflow,
    // then return a negative value.
    return (((double)gen_random()) / (RAND_MAX) * (end - begin)) + begin;
}

int get_retry_time(xmpp_info_t * p_xmpp_info)
{
    int retry_time = 0;
    static int retry_interval = 60;
    int retry_multiplier = p_xmpp_info->ServerRetryIntervalMultiplier;

    if (p_xmpp_info->n_fail_retry<1)
        return 0;

    if (p_xmpp_info->n_fail_retry == 1)
        retry_interval = p_xmpp_info->ServerRetryInitialInterval;
    else
        retry_interval = retry_interval * ((float) retry_multiplier /1000);

    if ( retry_interval > p_xmpp_info->ServerRetryMaxInterval ) {
        retry_interval = p_xmpp_info->ServerRetryMaxInterval;
    }

    retry_time = random_num(0, retry_interval);

    fprintf(stderr, "xmpp fail=%d, retry_time %d\n", p_xmpp_info->n_fail_retry, retry_time);

    return retry_time;
}

