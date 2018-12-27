/***********************************************************************
 *
 *  Copyright (c) 2004-2010  Broadcom Corporation
 *  All Rights Reserved
 *
#
#
# This program is the proprietary software of Broadcom Corporation and/or its
# licensors, and may only be used, duplicated, modified or distributed pursuant
# to the terms and conditions of a separate, written license agreement executed
# between you and Broadcom (an "Authorized License").  Except as set forth in
# an Authorized License, Broadcom grants no license (express or implied), right
# to use, or waiver of any kind with respect to the Software, and Broadcom
# expressly reserves all rights in and to the Software and all intellectual
# property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
# NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
# BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
#
# Except as expressly set forth in the Authorized License,
#
# 1. This program, including its structure, sequence and organization,
#    constitutes the valuable trade secrets of Broadcom, and you shall use
#    all reasonable efforts to protect the confidentiality thereof, and to
#    use this information only in connection with your use of Broadcom
#    integrated circuit products.
#
# 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
#    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
#    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
#    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
#    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
#    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
#    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
#    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
#    PERFORMANCE OF THE SOFTWARE.
#
# 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
#    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
#    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
#    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
#    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
#    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
#    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
#    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
#    LIMITED REMEDY.
#
 *
************************************************************************/

#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limits.h"     /* for CHAR_BIT */
#include <sys/stat.h>
#include <ctype.h>

#ifndef TRUE
#define TRUE    1
#endif /* !defined TRUE */

#ifndef FALSE
#define FALSE   0
#endif /* !defined FALSE */

#ifndef TYPE_BIT
#define TYPE_BIT(type)  (sizeof (type) * CHAR_BIT)
#endif /* !defined TYPE_BIT */

#ifndef TYPE_SIGNED
#define TYPE_SIGNED(type) (((type) -1) < 0)
#endif /* !defined TYPE_SIGNED */

#ifndef P
#define P(x)    x
#endif /* !defined P */

#define _(msgid) msgid

#define INITIALIZE(x)   ((x) = 0)

#define is_digit(c) ((unsigned)(c) - '0' <= 9)

#ifndef TM_YEAR_BASE
#define TM_YEAR_BASE    1900
#endif /* !defined TM_YEAR_BASE */

#define TM_SUNDAY       0
#define TM_MONDAY       1
#define TM_TUESDAY      2
#define TM_WEDNESDAY    3
#define TM_THURSDAY     4
#define TM_FRIDAY       5
#define TM_SATURDAY     6

#define TM_JANUARY      0
#define TM_FEBRUARY     1
#define TM_MARCH        2
#define TM_APRIL        3
#define TM_MAY          4
#define TM_JUNE         5
#define TM_JULY         6
#define TM_AUGUST       7
#define TM_SEPTEMBER    8
#define TM_OCTOBER      9
#define TM_NOVEMBER     10
#define TM_DECEMBER     11

#define EPOCH_YEAR      1970
#define EPOCH_WDAY      TM_THURSDAY

/*
** Accurate only for the past couple of centuries;
** that will probably do.
*/

#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

/* Static variables, from zic.c */

static time_t           max_time;
static int              max_year;
static int              max_year_representable;
static time_t           min_time;
static int              min_year;
static int              min_year_representable;
static const char *     progname;

/*
** Private function declarations.
*/
char *  icalloc P((int nelem, int elsize));
char *  icatalloc P((char * old, const char * new));
char *  icpyalloc P((const char * string));
char *  imalloc P((int n));
void *  irealloc P((void * pointer, int size));
void    icfree P((char * pointer));
void    ifree P((char * pointer));
char *  scheck P((const char *string, const char *format));

#define SECSPERMIN      60
#define MINSPERHOUR     60
#define HOURSPERDAY     24
#define DAYSPERWEEK     7
#define DAYSPERNYEAR    365
#define DAYSPERLYEAR    366
#define SECSPERHOUR     (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY      ((long) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR     12

static const int        len_months[2][MONSPERYEAR] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static const int        len_years[2] = {
	DAYSPERNYEAR, DAYSPERLYEAR
};

/*
** Memory allocation.
*/

#define emalloc(size)           memcheck(imalloc(size))
#define erealloc(ptr, size)     memcheck(irealloc((ptr), (size)))
#define ecpyalloc(ptr)          memcheck(icpyalloc(ptr))
#define ecatalloc(oldp, newp)   memcheck(icatalloc((oldp), (newp)))

/* BEGIN ialloc.c */

#define nonzero(n)      (((n) == 0) ? 1 : (n))

extern void error(char *msg);

char *
imalloc(n)
const int       n;
{
	return malloc((size_t) nonzero(n));
}

char *
icalloc(nelem, elsize)
int     nelem;
int     elsize;
{
	if (nelem == 0 || elsize == 0)
		nelem = elsize = 1;
	return calloc((size_t) nelem, (size_t) elsize);
}

void *
irealloc(pointer, size)
void * const    pointer;
const int       size;
{
	if (pointer == NULL)
		return imalloc(size);
	return realloc((void *) pointer, (size_t) nonzero(size));
}

char *
icatalloc(old, new)
char * const            old;
const char * const      new;
{
	register char * result;
	register int    oldsize, newsize;

	newsize = (new == NULL) ? 0 : strlen(new);
	if (old == NULL)
		oldsize = 0;
	else if (newsize == 0)
		return old;
	else    oldsize = strlen(old);
	if ((result = irealloc(old, oldsize + newsize + 1)) != NULL)
		if (new != NULL)
			(void) strcpy(result + oldsize, new);
	return result;
}

char *
icpyalloc(string)
const char * const      string;
{
	return icatalloc((char *) NULL, string);
}

void
ifree(p)
char * const    p;
{
	if (p != NULL)
		(void) free(p);
}

void
icfree(p)
char * const    p;
{
	if (p != NULL)
		(void) free(p);
}

/* END ialloc.c */

/* BEGIN scheck.c */

char *
scheck(string, format)
const char * const      string;
const char * const      format;
{
	register char *         fbuf;
	register const char *   fp;
	register char *         tp;
	register int            c;
	register char *         result;
	char                    dummy;
	static char             nada;

	result = &nada;
	if (string == NULL || format == NULL)
		return result;
	fbuf = imalloc((int) (2 * strlen(format) + 4));
	if (fbuf == NULL)
		return result;
	fp = format;
	tp = fbuf;
	while ((*tp++ = c = *fp++) != '\0') {
		if (c != '%')
			continue;
		if (*fp == '%') {
			*tp++ = *fp++;
			continue;
		}
		*tp++ = '*';
		if (*fp == '*')
			++fp;
		while (is_digit(*fp))
			*tp++ = *fp++;
		if (*fp == 'l' || *fp == 'h')
			*tp++ = *fp++;
		else if (*fp == '[')
			do *tp++ = *fp++;
			while (*fp != '\0' && *fp != ']');
		if ((*tp++ = *fp++) == '\0')
			break;
	}
	*(tp - 1) = '%';
	*tp++ = 'c';
	*tp = '\0';
	if (sscanf(string, fbuf, &dummy) != 1)
		result = (char *) format;
	ifree(fbuf);
	return result;
}

/* END scheck.c */

struct rule {
	int             r_month;        /* 0..11 */

	int             r_dycode;       /* see below */
	int             r_dayofmonth;
	int             r_wday;

	long            r_tod;          /* time from midnight */
	int             r_todisstd;     /* above is standard time if TRUE */
	/* or wall clock time if FALSE */
	int             r_todisgmt;     /* above is GMT if TRUE */
	/* or local time if FALSE */
	long            r_stdoff;       /* offset from standard time */
};

/*      r_dycode                r_dayofmonth    r_wday */
#define DC_DOM          0       /* 1..31 */     /* unused */
#define DC_DOWGEQ       1       /* 1..31 */     /* 0..6 (Sun..Sat) */
#define DC_DOWLEQ       2       /* 1..31 */     /* 0..6 (Sun..Sat) */

#define NRULES          2

static struct rule const rules[][NRULES] = {
	/* mon         dycode    dom  wday          tod   isstd  isgmt stdoff */
	{
//         { TM_APRIL,    DC_DOWGEQ,  1, TM_SUNDAY,   7200, FALSE, FALSE, 3600 },
//         { TM_OCTOBER,  DC_DOWLEQ, 31, TM_SUNDAY,   7200, FALSE, FALSE, 0    }
		{ TM_MARCH,    DC_DOWGEQ,  8, TM_SUNDAY,   7200, FALSE, FALSE, 3600 },
		{ TM_NOVEMBER, DC_DOWGEQ,  1, TM_SUNDAY,   7200, FALSE, FALSE, 0    }
	}, /* 0: US */
	{
		{ TM_MARCH,    DC_DOWLEQ, 31, TM_SUNDAY,   3600, FALSE,  TRUE, 3600 },
		{ TM_OCTOBER,  DC_DOWLEQ, 31, TM_SUNDAY,   3600, FALSE,  TRUE, 0    }
	}, /* 1: EU */
	{
		{ TM_APRIL,    DC_DOWGEQ,  9, TM_SUNDAY,  10800, FALSE,  TRUE, 0 },
		{ TM_OCTOBER,  DC_DOWGEQ,  9, TM_SUNDAY,  14400, FALSE,  TRUE, 3600 }
	}, /* 2: Chile */
	{
		{ TM_AUGUST,   DC_DOWLEQ, 31, TM_SUNDAY,   7200, TRUE,  FALSE, 0 },
		{ TM_OCTOBER,  DC_DOWLEQ, 31, TM_SUNDAY,   7200, TRUE,  FALSE, 3600 }
	}, /* 3: Australia */
	{
		{ TM_APRIL,    DC_DOWLEQ, 31, TM_FRIDAY,      0, TRUE,  FALSE, 3600 },
		{ TM_SEPTEMBER,DC_DOWLEQ, 31, TM_THURSDAY,82800, TRUE,  FALSE, 0 }
	}, /* 4: Egypt */
	{
		{ TM_APRIL,    DC_DOM,     7, 0,           3600, FALSE, FALSE, 3600 },
		{ TM_SEPTEMBER,DC_DOM,    22, 0,           3600, FALSE, FALSE, 0 }
	}, /* 5: Zion */
	{
		{ TM_APRIL,    DC_DOM,     1, 0,          10400,  TRUE, FALSE, 3600 },
		{ TM_OCTOBER,  DC_DOM,     1, 0,          10400,  TRUE, FALSE, 0 }
	}, /* 6: Iraq */
	{
		{ TM_MARCH,    DC_DOWLEQ, 31, TM_SUNDAY,   3600,  TRUE, FALSE, 3600 },
		{ TM_OCTOBER,  DC_DOWLEQ, 31, TM_SUNDAY,   0,     TRUE, FALSE, 0 }
	}, /* 7: Russia */
	{
		{ TM_MARCH,    DC_DOM,    21, 0,           0,    FALSE, FALSE, 3600 },
		{ TM_SEPTEMBER,DC_DOM,    21, 0,           0,    FALSE, FALSE, 0 }
	}, /* 8: Iran */
	{
		{ TM_MARCH,    DC_DOWLEQ, 31, TM_SUNDAY,   3600, FALSE, FALSE, 3600 },
		{ TM_OCTOBER,  DC_DOWLEQ, 31, TM_SUNDAY,   3600, FALSE, FALSE, 0 }
	}, /* 9: Azer */
	{
		{ TM_MARCH,    DC_DOWLEQ, 31, TM_SUNDAY,   7200,  TRUE, FALSE, 3600 },
		{ TM_OCTOBER,  DC_DOWGEQ,  1, TM_SUNDAY,   7200,  TRUE, FALSE, 0 }
	}, /* 10: Tasmania */
	{
		{ TM_MARCH,    DC_DOWGEQ, 15, TM_SUNDAY,   7200,  TRUE, FALSE, 0 },
		{ TM_OCTOBER,  DC_DOWGEQ, 15, TM_SUNDAY,   9900,  TRUE, FALSE, 3600 }
	}, /* 11: New Zealand */
	{
		{ TM_MARCH,    DC_DOWLEQ, 31, TM_SUNDAY,   7200,  TRUE, FALSE, 3600 },
		{ TM_OCTOBER,  DC_DOWLEQ, 31, TM_SUNDAY,   7200,  TRUE, FALSE, 0 }
	}, /* 12: RussiaAsia */
	{
		{ TM_APRIL,    DC_DOWGEQ,  1, TM_SUNDAY,   7200, FALSE, FALSE, 3600 },
		{ TM_OCTOBER,  DC_DOWLEQ, 31, TM_SUNDAY,   7200, FALSE, FALSE, 0 }
	}, /* 13: Mexico */
	{
		{ TM_FEBRUARY, DC_DOWGEQ, 15, TM_SUNDAY,   3600, FALSE, FALSE, 3600 },
		{ TM_OCTOBER,  DC_DOWGEQ, 15, TM_SUNDAY,      0, FALSE, FALSE, 0 }
	}, /* 14: Brazil */
};

struct zone {
	long            z_gmtoff; /* zone's basic GMT offset */
	int             z_rules;  /* index of rules to use */
};

#define N_ZONES 75
/* This must match sntpcfg.html and sntp.c */
static struct zone const zones[N_ZONES] = {
	/* offset rules */
	{ -43200, -1 }, /* (GMT-12:00) International Date Line West */
	{ -39600, -1 }, /* (GMT-11:00) Midway Island, Samoa */
	{ -36000, -1 }, /* (GMT-10:00) Hawaii */
	{ -32400,  0 }, /* (GMT-09:00) Alaska */
	{ -28800,  0 }, /* (GMT-08:00) Pacific Time, Tijuana */
	{ -25200, -1 }, /* (GMT-07:00) Arizona, Mazatlan */
	{ -25200, 13 }, /* (GMT-07:00) Chihuahua, La Paz */
	{ -25200,  0 }, /* (GMT-07:00) Mountain Time */
	{ -21600,  0 }, /* (GMT-06:00) Central America */
	{ -21600,  0 }, /* (GMT-06:00) Central Time */
	{ -21600, 13 }, /* (GMT-06:00) Guadalajara, Mexico City, Monterrey */
	{ -21600, -1 }, /* (GMT-06:00) Saskatchewan */
	{ -18000, -1 }, /* (GMT-05:00) Bogota, Lima, Quito */
	{ -18000,  0 }, /* (GMT-05:00) Eastern Time */
	{ -18000, -1 }, /* (GMT-05:00) Indiana */
	{ -14400,  0 }, /* (GMT-04:00) Atlantic Time */
	{ -14400, -1 }, /* (GMT-04:00) Caracas, La Paz */
	{ -14400,  2 }, /* (GMT-04:00) Santiago */
	{ -12600,  0 }, /* (GMT-03:30) Newfoundland */
	{ -10800, 14 }, /* (GMT-03:00) Brasilia */
	{ -10800, -1 }, /* (GMT-03:00) Buenos Aires, Georgetown */
	{ -10800, -1 }, /* (GMT-03:00) Greenland */
	{  -7200, -1 }, /* (GMT-02:00) Mid-Atlantic */
	{  -3600,  1 }, /* (GMT-01:00) Azores */
	{  -3600, -1 }, /* (GMT-01:00) Cape Verde Is. */
	{      0, -1 }, /* (GMT) Casablanca, Monrovia */
	{      0,  1 }, /* (GMT) Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
	{   3600,  1 }, /* (GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna */
	{   3600,  1 }, /* (GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague */
	{   3600,  1 }, /* (GMT+01:00) Brussels, Copenhagen, Madrid, Paris */
	{   3600,  1 }, /* (GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb */
	{   3600, -1 }, /* (GMT+01:00) West Central Africa */
	{   7200,  1 }, /* (GMT+02:00) Athens, Istanbul, Minsk */
	{   7200,  1 }, /* (GMT+02:00) Bucharest */
	{   7200,  4 }, /* (GMT+02:00) Cairo */
	{   7200, -1 }, /* (GMT+02:00) Harare, Pretoria */
	{   7200,  1 }, /* (GMT+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius */
	{   7200,  5 }, /* (GMT+02:00) Jerusalem */
	{  10800,  6 }, /* (GMT+03:00) Baghdad */
	{  10800, -1 }, /* (GMT+03:00) Kuwait, Riyadh */
	{  10800,  7 }, /* (GMT+03:00) Moscow, St. Petersburg, Volgograd */
	{  10800, -1 }, /* (GMT+03:00) Nairobi */
	{  12600,  8 }, /* (GMT+03:30) Tehran */
	{  14400, -1 }, /* (GMT+04:00) Abu Dhabi, Muscat */
	{  14400,  9 }, /* (GMT+04:00) Baku, Tbilisi, Yerevan */
	{  16200, -1 }, /* (GMT+04:30) Kabul */
	{  18000,  7 }, /* (GMT+05:00) Ekaterinburg */
	{  18000, -1 }, /* (GMT+05:00) Islamabad, Karachi, Tashkent */
	{  19800, -1 }, /* (GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi */
	{  20700, -1 }, /* (GMT+05:45) Kathmandu */
	{  21600, 12 }, /* (GMT+06:00) Almaty, Novosibirsk */
	{  21600, -1 }, /* (GMT+06:00) Astana, Dhaka */
	{  21600, -1 }, /* (GMT+06:00) Sri Jayawardenepura */
	{  23400, -1 }, /* (GMT+06:30) Rangoon */
	{  25200, -1 }, /* (GMT+07:00) Bangkok, Hanoi, Jakarta */
	{  25200,  7 }, /* (GMT+07:00) Krasnoyarsk */
	{  28800, -1 }, /* (GMT+08:00) Beijing, Chongquing, Hong Kong, Urumqi */
	{  28800, -1 }, /* (GMT+08:00) Irkutsk, Ulaan Bataar */
	{  28800, -1 }, /* (GMT+08:00) Kuala Lumpur, Singapore */
	{  28800, -1 }, /* (GMT+08:00) Perth */
	{  28800, -1 }, /* (GMT+08:00) Taipei */
	{  32400, -1 }, /* (GMT+09:00) Osaka, Sapporo, Tokyo */
	{  32400, -1 }, /* (GMT+09:00) Seoul */
	{  32400,  7 }, /* (GMT+09:00) Yakutsk */
	{  34200,  3 }, /* (GMT+09:30) Adelaide */
	{  34200, -1 }, /* (GMT+09:30) Darwin */
	{  36000, -1 }, /* (GMT+10:00) Brisbane */
	{  36000,  3 }, /* (GMT+10:00) Canberra, Melbourne, Sydney */
	{  36000, -1 }, /* (GMT+10:00) Guam, Port Moresby */
	{  36000, 10 }, /* (GMT+10:00) Hobart */
	{  36000,  7 }, /* (GMT+10:00) Vladivostok */
	{  39600, -1 }, /* (GMT+11:00) Magadan */
	{  39600,  7 }, /* (GMT+11:00) Solomon Is., New Caledonia */
	{  43200, 11 }, /* (GMT+12:00) Auckland, Wellington */
	{  43200, -1 }, /* (GMT+12:00) Fiji, Kamchatka, Marshall Is. */
};

/*
** Prototypes, from zic.c
*/
static long     eitol P((int i));
static time_t   rpytime P((const struct rule * rp, int wantedy));

#define MAX_BITS_IN_FILE        32
#define TIME_T_BITS_IN_FILE     ((TYPE_BIT(time_t) < MAX_BITS_IN_FILE) ? TYPE_BIT(time_t) : MAX_BITS_IN_FILE)

static int              sflag = FALSE;

static void
setboundaries P((void))
{
	if (TYPE_SIGNED(time_t)) {
		min_time = ~ (time_t) 0;
		min_time <<= TIME_T_BITS_IN_FILE - 1;
		max_time = ~ (time_t) 0 - min_time;
		if (sflag)
			min_time = 0;
	} else {
		min_time = 0;
		max_time = 2 - sflag;
		max_time <<= TIME_T_BITS_IN_FILE - 1;
		--max_time;
	}
	min_year = TM_YEAR_BASE + gmtime(&min_time)->tm_year;
	max_year = TM_YEAR_BASE + gmtime(&max_time)->tm_year;
	min_year_representable = min_year;
	max_year_representable = max_year;
}

/* Returns the offset, in seconds, for zone_n at gmtime. */
int
get_offset(long gm_time,int zone_n)
{
	int i, year;
	long current_offset = 0;
	struct zone const * zone;
	struct rule const * rule;
	struct tm *tm;

	/* Initialize */
	setboundaries();

	if (zone_n >= N_ZONES) {
		printf("ERROR: zone %d is too large.\n", zone_n);
		exit(1);
	}

	/* Look up zone */
	zone = &zones[zone_n];

	/* rule -1: no DST */
	if (zone->z_rules == -1)
		return zone->z_gmtoff;

	/* Get year */
	tm = gmtime( &gm_time );
	if (tm) {
		year = tm->tm_year + 1900;
	} else {
		year = 2004;
	}

	/* Check the last rule, last year, to handle countries that are in
	   DST on Jan 1. */
	rule = &rules[zone->z_rules][NRULES-1];
	if (rpytime(rule, year-1) < (gm_time + zone->z_gmtoff))
		current_offset = rule->r_stdoff;

	for (i = 0; i < NRULES; i++) {
		rule = &rules[zone->z_rules][i];

		if (rpytime(rule, year) < (gm_time + zone->z_gmtoff))
			current_offset = rule->r_stdoff;
	}

#ifdef AEI_WECB
		return zone->z_gmtoff;
#else
        return zone->z_gmtoff + current_offset;
#endif

}

#ifdef DSTTEST
int
main(argc, argv)
int     argc;
char *  argv[];
{
	long gmtime;
	long zone_n;
	long offset;

	if (argc == 2 && strcmp(argv[1], "test") == 0) {
		gmtime = 1073260800; /* Jan 5, 2004 0:00 UTC */
		printf("Jan 5, Atlantic Time: %d\n", get_offset(gmtime, 15));
		printf("Jan 5, Caracas, La Paz: %d\n", get_offset(gmtime, 16));
		printf("Jan 5, Santiago: %d\n", get_offset(gmtime, 17));
		printf("Jan 5, Fiji, Kamchatka, Marshall Is.: %d\n", get_offset(gmtime, 74));

		printf("\n");

		gmtime = 1088985600; /* Jul 5, 2004 0:00 UTC */
		printf("Jul 5, Atlantic Time: %d\n", get_offset(gmtime, 15));
		printf("Jul 5, Caracas, La Paz: %d\n", get_offset(gmtime, 16));
		printf("Jul 5, Santiago: %d\n", get_offset(gmtime, 17));
		printf("Jul 5, Fiji, Kamchatka, Marshall Is.: %d\n", get_offset(gmtime, 74));
	} else {
		if (argc != 3) exit(1);

		gmtime = strtol(argv[1], NULL, 10);
		zone_n = strtol(argv[2], NULL, 10);
		printf("gmtime: %d\n", gmtime);
		printf("zone_n: %d\n", zone_n);

		printf("\n", zone_n);

		offset = get_offset(gmtime, zone_n);

		printf("offset: %d\n", offset);
		printf("localtime: %d\n", gmtime+offset);
	}

	exit (0);
}
#endif /*DSTTEST*/

extern int      getopt P((int argc, char * const argv[],
                          const char * options));
extern int      link P((const char * fromname, const char * toname));
extern char *   optarg;
extern int      optind;

static long
oadd(const long t1,const long t2)
{
	register long   t;

	t = t1 + t2;
	if ((t2 > 0 && t <= t1) || (t2 < 0 && t >= t1)) {
		error(_("time overflow"));
		(void) exit(EXIT_FAILURE);
	}
	return t;
}

static time_t
tadd(const time_t t1,const long t2)
{
	register time_t t;

	if (t1 == max_time && t2 > 0)
		return max_time;
	if (t1 == min_time && t2 < 0)
		return min_time;
	t = t1 + t2;
	if ((t2 > 0 && t <= t1) || (t2 < 0 && t >= t1)) {
		error(_("time overflow"));
		(void) exit(EXIT_FAILURE);
	}
	return t;
}

/*
** Given a rule, and a year, compute the date - in seconds since January 1,
** 1970, 00:00 LOCAL time - in that year that the rule refers to.
*/

static time_t
rpytime(register const struct rule * const rp,register const int wantedy)
{
	register int    y, m, i;
	register long   dayoff;                 /* with a nod to Margaret O. */
	register time_t t;

	if (wantedy == INT_MIN)
		return min_time;
	if (wantedy == INT_MAX)
		return max_time;
	dayoff = 0;
	m = TM_JANUARY;
	y = EPOCH_YEAR;
	while (wantedy != y) {
		if (wantedy > y) {
			i = len_years[isleap(y)];
			++y;
		} else {
			--y;
			i = -len_years[isleap(y)];
		}
		dayoff = oadd(dayoff, eitol(i));
	}
	while (m != rp->r_month) {
		i = len_months[isleap(y)][m];
		dayoff = oadd(dayoff, eitol(i));
		++m;
	}
	i = rp->r_dayofmonth;
	if (m == TM_FEBRUARY && i == 29 && !isleap(y)) {
		if (rp->r_dycode == DC_DOWLEQ)
			--i;
		else {
			error(_("use of 2/29 in non leap-year"));
			(void) exit(EXIT_FAILURE);
		}
	}
	--i;
	dayoff = oadd(dayoff, eitol(i));
	if (rp->r_dycode == DC_DOWGEQ || rp->r_dycode == DC_DOWLEQ) {
		register long   wday;

#define LDAYSPERWEEK    ((long) DAYSPERWEEK)
		wday = eitol(EPOCH_WDAY);
		/*
		** Don't trust mod of negative numbers.
		*/
		if (dayoff >= 0)
			wday = (wday + dayoff) % LDAYSPERWEEK;
		else {
			wday -= ((-dayoff) % LDAYSPERWEEK);
			if (wday < 0)
				wday += LDAYSPERWEEK;
		}
		while (wday != eitol(rp->r_wday))
			if (rp->r_dycode == DC_DOWGEQ) {
				dayoff = oadd(dayoff, (long) 1);
				if (++wday >= LDAYSPERWEEK)
					wday = 0;
				++i;
			} else {
				dayoff = oadd(dayoff, (long) -1);
				if (--wday < 0)
					wday = LDAYSPERWEEK - 1;
				--i;
			}
		if (i < 0 || i >= len_months[isleap(y)][m]) {
			error(_("no day in month matches rule"));
			(void) exit(EXIT_FAILURE);
		}
	}
	if (dayoff < 0 && !TYPE_SIGNED(time_t))
		return min_time;
	if (dayoff < min_time / SECSPERDAY)
		return min_time;
	if (dayoff > max_time / SECSPERDAY)
		return max_time;
	t = (time_t) dayoff * SECSPERDAY;
	return tadd(t, rp->r_tod);
}

static long
eitol(const int i)
{
	long    l;

	l = i;
	if ((i < 0 && l >= 0) || (i == 0 && l != 0) || (i > 0 && l <= 0)) {
		(void) fprintf(stderr,
		               _("%s: %d did not sign extend correctly\n"),
		               progname, i);
		(void) exit(EXIT_FAILURE);
	}
	return l;
}

/*
** UNIX was a registered trademark of UNIX System Laboratories in 1993.
*/
