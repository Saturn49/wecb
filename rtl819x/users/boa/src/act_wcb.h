/*
 *      WCB  Define 
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#ifndef ACT_WCB_H
#define ACT_WCB_H

#include "act_common.h"
#include "act_log.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "boa.h"
#include "asp_page.h"
#include "apmib.h"
#include "apform.h"
#include "utility.h"

#define HOME_PAGE        						"/wcb_home.html"
#define HOME_PAGE_K1     						"/wcb_home.html?k=1"
#define MAIN_PAGE        						"/wcb_main.html"
#define THANKYOU_PAGE                           "/wcb_thankyou.html"
#define WIFI_SECURITY_PAGE                      "/wl_security.html"
#define FORM_INPUT_SPECIAL                      '|'

#define SWAP_COUNTDOWN_TIME                     90
#define RESTORE_COUNTDOWN_TIME                  90
#define REBOOT_COUNTDOWN_TIME                   90
#define WIRELESS_SETTING_COUNTDOWN_TIME    		20
#define WIRELESS_CLIENTCONNECT_COUNTDOWN_TIME   10

/* 131072 , 8192, 12288 */
#define LIMIT_MSGHEAD							12288

/* 20480, 8192 */
#define WEB_BUF_SIZE_MAX    					8192

#define LIMIT_PARSEHTML  						1024

void thankyouPage(request *wp);
void nothankyouPage(request *wp);
void backPage(request *wp);

#endif

