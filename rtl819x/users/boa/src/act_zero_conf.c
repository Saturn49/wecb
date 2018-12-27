 /*
  *----------------------------------------------------------------
  *
  * act_zero_conf.c
  *
  * Feb, 15, 2013 Jerome Zhao: This file is for GUI notify start sync wifi parameter and auto upagrade
  *
  * Copyright (c) 2012 Actiontec Electronics Inc.
  * All rights reserved.
  *
  *----------------------------------------------------------------
  */


#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>

#include "boa.h"
#include "asp_page.h"
#include "apmib.h"
#include "apform.h"
#include "utility.h"
#include "act_tr69.h"

/*
* Function: get_enable_status()
*
* Description: Check this feature is enable or not
*
*/
static int actIsZeroConfEnabled(void)
{
    char buf[8] = {0};
    int enable;

    if( tr69GetUnfreshLeafData( buf,
                "Device.X_ACTIONTEC_COM_ZeroConf.Extender",
                "Enable") == 0) {
        enable = atoi( buf );
        if( enable  == 0) {
            return 0;
        } else {
            return 1;
        }
    }
    
    return 0;
}

/*
* Function: actZeroConfNotify()
*
* Description: This function is called by GUI. When Telus notify WCB3000, then call this function.
*
* Input Parameters
* argv[0] = AutoSync: Start Sync WiFi parameter
* argv[0] = Upgrade: Start Check Upgrade
*
*/
void actZeroConfNotify(request *wp, int argc, char **argv)
{
    if( ! actIsZeroConfEnabled()) {
        return;
    }

    if(strcmp(argv[0], "AutoSync") == 0)
    {
        system("zero_conf_telus AutoSync &");
    }
    else if(strcmp(argv[0], "Upgrade") == 0)
    {
        system("zero_conf_telus Upgrade &");
    }

	return;
}

