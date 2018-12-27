#ifndef __CTL_STRCONV_H__
#define __CTL_STRCONV_H__
#include "tsl_common.h"

/*!
 *  * \brief defines for ppp auto method in number
 *   */
#define PPP_AUTH_METHOD_AUTO        0
#define PPP_AUTH_METHOD_PAP         1
#define PPP_AUTH_METHOD_CHAP        2
#define PPP_AUTH_METHOD_MSCHAP      3


#define DSL_LINK_DESTINATION_PREFIX_PVC   "PVC:"
#define DSL_LINK_DESTINATION_PREFIX_SVC   "SVC:"

typedef enum 
{
    URL_PROTO_HTTP=0, /**< http */
    URL_PROTO_HTTPS=1, /**< https */                                                                               
    URL_PROTO_FTP=2,   /**< ftp */
    URL_PROTO_TFTP=3   /**< tftp */   
} UrlProto;
/** Given a ppp  auth method string, return the equivalent auth method number.
 *  *
 *   *
 *    * @param authStr (IN) The auth method string.
 *     *
 *      * @return the  auth method number
 *       */
tsl_rv_t ctlUtl_macStrToNum(const char *macStr, tsl_u8_t *macNum); 
tsl_bool_t ctlUtl_isValidMacAddress(const char* input);
tsl_32_t cmsUtl_pppAuthToNum(const char *authStr);
tsl_32_t cmsUtl_fwSecurityLevelToNum(const char *levelStr);
char * cmsUtl_numToFwSecurityLevelString(tsl_32_t level); 
tsl_bool_t ctlUtl_isValidIpAddress(tsl_32_t af, const char* address);
tsl_bool_t ctlUtl_isValidIpv4Address(const char* input);
tsl_rv_t ctlUtl_parseDNS(const char *inDsnServers, char *outDnsPrimary, char *outDnsSecondary);
tsl_rv_t ctlUtl_strtoul(const char *str, char **endptr, tsl_32_t base, tsl_u32_t *val);
void rut_doSystemAction(const char* from, char *cmd);

void AEI_freeArgs(char **argv);
int AEI_parseArgs(const char *cmd, const char *args, char ***argv);
void AEI_close_open_fds();
void AEI_tsl_child_dupfds(char *filename);
int AEI_tsl_system(char *p_path, char *p_argv);
int AEI_tsl_system_pwd(char *p_path, char *p_argv, char *filename, char *fin_file);



#endif
