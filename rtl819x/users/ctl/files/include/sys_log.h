/*********************************************************
 * Descp :
 *     This file support system log interface for all module
 * Author :
 *     Rayofox(lhu@actiontec.com)
 * Revision :
 *     1.0 : 2011/9/20 Initial Version
 *
 *
 ********************************************************/
#ifndef __SYS_LOG_H_
#define __SYS_LOG_H_

#include <unistd.h>
#include <syslog.h>
#include <sys/time.h>


enum OMT_Log_Level
{
    OMT_LOG_ERR     =   3,
    OMT_LOG_WARNING =   4,
    OMT_LOG_INFO    =   6,
    OMT_LOG_DEBUG   =   7,
    OMT_LOG_NUM   =   9,
};

#define STR_OMT_LOG_ERR "error" 
#define STR_OMT_LOG_WARNING "warning" 
#define STR_OMT_LOG_INFO "info" 
#define STR_OMT_LOG_DEBUG "debug" 

#define STR_SYSLOG_LEVEL(n) STR_##n
/* 
 *


static const char* log_levelstr[] =
{
    "Emerg",
    "Alert",
    "Crit",
    "Error",
    "Warning",
    "Notice",
    "Info",
    "Debug",
    "None"
};


const char* sys_module[] = 
{
    {"other","Other"},
     "bridge: Bridge
      "cli: Command Line Interface
      "dhcp: DHCP
      "dns: DNS
      cwmp: DSL Home
      ddns: Dynamic DNS
      firewall: Firewall
      generic_proxy: Generic Proxy
      hss: HSS
      http: HTTP
      hw_switch: HW Switch
      igmp: IGMP
      image: Image Library
      util: LibJutil
      mail_client: Mail Client
      main: Main Task
      ppp: PPP
      pppoe_relay: PPPoE Relay
      permst: Permanent Storage
      qos: QoS
      rip: RIP
      rmt_mng: Remote Management
      rmt_upd: Remote Update
      route: Route
      ssi: SSI
      syslog: Syslog
      tod: Time of Day
      voatm: VoATM
      web_filtering: Web Filtering
      wbm: Web-Based Management
      wget: Wget
      xml: XML
}
**/
/*******************************************************
  *
  * Interface for dependent application, 
  * the module name can be specified with openlog ,
  * or default is the application name.
  *
  *
 **************************************************** */ 
#define APP_SYSLOG(level,errDesc,arg...)    \
    do { \
        syslog(level,"[PID:%d] [LVL:%s]:"errDesc ,\
                getpid(),STR_SYSLOG_LEVEL(level),##arg); \
    }while(0);


#define APP_DEBUG_SYSLOG(level,errDesc,arg...)    \
    do { \
        syslog(level,"[PID:%d] [LVL:%s] [FILE:%s] [LINE:%d]:"errDesc ,\
                getpid(),STR_SYSLOG_LEVEL(level),__FILE__,__LINE__,##arg); \
    }while(0);

//For Module in independent application (the module name is the application name)
#define DEBUG_SYSLOG(msg,arg...)    APP_DEBUG_SYSLOG(OMT_LOG_DEBUG,msg,##arg)
#define INFO_SYSLOG(msg,arg...)     APP_SYSLOG(OMT_LOG_INFO,msg,##arg)
#define WARNING_SYSLOG(msg,arg...)  APP_SYSLOG(OMT_LOG_WARNING,msg,##arg)
#define ERR_SYSLOG(msg,arg...)      APP_SYSLOG(OMT_LOG_ERR,msg,##arg)

/*******************************************************
  *
  * Interface for module in lib, 
  * the module name can not be specified with openlog ,
  * so add the module info in content.
  *
  *
 **************************************************** */ 
#define COMMON_SYSLOG(mod_name,level,errDesc,arg...)    \
    do { \
        syslog(level,"[PID:%d] [MID:%s] [LVL:%s]:"errDesc ,\
                getpid(),mod_name,STR_SYSLOG_LEVEL(level),##arg); \
    }while(0);

#define COMMON_DEBUG_SYSLOG(mod_name,level,errDesc,arg...)    \
    do { \
        syslog(level,"[PID:%d] [MID:%s] [LVL:%s] [FILE:%s] [LINE:%d]:"errDesc ,\
                getpid(),mod_name,STR_SYSLOG_LEVEL(level),__FILE__,__LINE__,##arg); \
    }while(0);

//For Module Firewall in lib
#define DEBUG_FW_SYSLOG(msg,arg...)     COMMON_DEBUG_SYSLOG("firewall",OMT_LOG_DEBUG,msg,##arg)
#define INFO_FW_SYSLOG(msg,arg...)      COMMON_SYSLOG("firewall",OMT_LOG_INFO,msg,##arg)
#define WARNING_FW_SYSLOG(msg,arg...)   COMMON_SYSLOG("firewall",OMT_LOG_WARNING,msg,##arg)
#define ERR_FW_SYSLOG(msg,arg...)       COMMON_SYSLOG("firewall",OMT_LOG_ERR,msg,##arg)

//For Module cwmp in lib
#define DEBUG_CWMP_SYSLOG(msg,arg...)     COMMON_DEBUG_SYSLOG("cwmp",OMT_LOG_DEBUG,msg,##arg)
#define INFO_CWMP_SYSLOG(msg,arg...)      COMMON_SYSLOG("cwmp",OMT_LOG_INFO,msg,##arg)
#define WARNING_CWMP_SYSLOG(msg,arg...)   COMMON_SYSLOG("cwmp",OMT_LOG_WARNING,msg,##arg)
#define ERR_CWMP_SYSLOG(msg,arg...)       COMMON_SYSLOG("cwmp",OMT_LOG_ERR,msg,##arg)




#endif//#ifndef __SYS_LOG_H_


