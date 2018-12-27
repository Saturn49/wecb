#ifndef __TR69_CMS_OBJECT_H__
#define __TR69_CMS_OBJECT_H__
#include "ctl.h"
#include "ctl_log.h"
#include "tsl_common.h"
#include "ctl_mem.h"
#include "OID.h"
#include "ctl_objectid.h"
#include "ctl_validstrings.h"
#include "ctl_params.h"
/** Invalid file descriptor number */
#define CMS_INVALID_FD  (-1)     


/** Invalid process id.          
 *  *
 *   * Management entities should not need to use this constant.  It is used
 *    * by OS dependent code in the OAL layer.  But I put this constant here
 *     * to make it easier to find.
 *      */
#define CMS_INVALID_PID   0
/********************hardware related****************/
/* Socket configuration controls. */
#define SIOCGIFNAME 0x8910      /* get iface name       */
#define SIOCSIFLINK 0x8911      /* set iface channel        */
#define SIOCGIFCONF 0x8912      /* get iface list       */
#define SIOCGIFFLAGS    0x8913      /* get flags            */
#define SIOCSIFFLAGS    0x8914      /* set flags            */
//#define SIOCGIFADDR 0x8915      /* get PA address       */
#define SIOCSIFADDR 0x8916      /* set PA address       */
#define SIOCGIFDSTADDR  0x8917      /* get remote PA address    */
#define SIOCSIFDSTADDR  0x8918      /* set remote PA address    */
#define SIOCGIFBRDADDR  0x8919      /* get broadcast PA address */
#define SIOCSIFBRDADDR  0x891a      /* set broadcast PA address */
#define SIOCGIFNETMASK  0x891b      /* get network PA mask      */
#define SIOCSIFNETMASK  0x891c      /* set network PA mask      */
#define SIOCGIFMETRIC   0x891d      /* get metric           */
#define SIOCSIFMETRIC   0x891e      /* set metric           */
#define SIOCGIFMEM  0x891f      /* get memory address (BSD) */
#define SIOCSIFMEM  0x8920      /* set memory address (BSD) */
//#define SIOCGIFMTU  0x8921      /* get MTU size         */
#define SIOCSIFMTU  0x8922      /* set MTU size         */
#define SIOCSIFNAME 0x8923      /* set interface name */
#define SIOCSIFHWADDR   0x8924      /* set hardware address     */
#define SIOCGIFENCAP    0x8925      /* get/set encapsulations       */
#define SIOCSIFENCAP    0x8926      
//#define SIOCGIFHWADDR   0x8927      /* Get hardware address     */
#define SIOCGIFSLAVE    0x8929      /* Driver slaving support   */
#define SIOCSIFSLAVE    0x8930
#define SIOCADDMULTI    0x8931      /* Multicast address lists  */
#define SIOCDELMULTI    0x8932
#define SIOCGIFINDEX    0x8933      /* name -> if_index mapping */
#define SIOGIFINDEX SIOCGIFINDEX    /* misprint compatibility :-)   */
#define SIOCSIFPFLAGS   0x8934      /* set/get extended flags set   */
#define SIOCGIFPFLAGS   0x8935
#define SIOCDIFADDR 0x8936      /* delete PA address        */
#define SIOCSIFHWBROADCAST  0x8937  /* set hardware broadcast addr  */
#define SIOCGIFCOUNT    0x8938      /* get number of devices */

#define SIOCGIFBR   0x8940      /* Bridging support     */
#define SIOCSIFBR   0x8941      /* Set bridging options     */

#define SIOCGIFTXQLEN   0x8942      /* Get the tx queue length  */
#define SIOCSIFTXQLEN   0x8943      /* Set the tx queue length  */

enum { 
    ETHGETNUMTXDMACHANNELS = 0,  
    ETHSETNUMTXDMACHANNELS,      
    ETHGETNUMRXDMACHANNELS,      
    ETHSETNUMRXDMACHANNELS,      
    ETHGETSOFTWARESTATS,         
    ETHCLEARSTATSINFO,           
    ETHGETSTATSINFO,             
};

/*general data*/
struct ethGeneral_data {    
    int op;
    int data;
    int len;
    int reserved;
};

typedef struct AtmAddr
{
        tsl_u32_t ulPortMask;
            tsl_u16_t usVpi;
                tsl_u16_t usVci;
} ATM_ADDR, *PATM_ADDR;

typedef struct PtmAddr
{  
        tsl_u32_t ulPortMask;
            tsl_u32_t ulPtmPriority;
} PTM_ADDR, *PPTM_ADDR;
   
typedef struct GenAddr
{     
        tsl_u32_t ulPortMask;
            tsl_u32_t ulPtmPriority;
} GEN_ADDR, *PGEN_ADDR;
   
typedef struct XtmAddr
{
        tsl_u32_t ulTrafficType;
            union
            {
                ATM_ADDR  Vcc;                      
                PTM_ADDR  Flow;
                GEN_ADDR  Conn;
            } u;
} XTM_ADDR, *PXTM_ADDR;

typedef struct _adslCfgProfile { 
    long        adslAnnexCParam; 
    long        adslAnnexAParam; 
    long        adslTrainingMarginQ4; 
    long        adslShowtimeMarginQ4; 
    long        adslLOMTimeThldSec;   
    long        adslDemodCapMask;
    long        adslDemodCapValue;    
    long        adsl2Param;      
    long        adslPwmSyncClockFreq; 
    long        adslHsModeSwitchTime; 
    long        adslDemodCap2Mask;    
    long        adslDemodCap2Value;   
    long        vdslParam;       
    long        vdslParam1;      
    long        xdslAuxFeaturesMask;  
    long        xdslAuxFeaturesValue; 
    long        vdslCfgFlagsMask;
    long        vdslCfgFlagsValue;    
    long        xdslCfg1Mask;    
    long        xdslCfg1Value;   
    long        xdslCfg2Mask;    
    long        xdslCfg2Value;   
    long        xdslCfg3Mask;
    long        xdslCfg3Value;
    long        xdslCfg4Mask;
    long        xdslCfg4Value;
} adslCfgProfile;

#define BPA_FLOW_INVALID_HDL    0xFFFFFFFF
#define BPA_MAX_FLOWS           512
#define BPA_PBITS_MAX               8
#define BPA_DSCP_MAX                64
#define BPA_DSCPTBL_INVALID_HDL     0xFFFFFFFF
#define BPA_DSCPTBL_MAXNUM          BPA_MAX_UNI
#define BPA_DSCPENT_MAXNUM          64
typedef union BpaFlowHdl         
{
        tsl_u32_t   u32;              
            struct 
            {  
                tsl_u32_t   resvd: 16;   /**< can be used for incarnation  */
                tsl_u32_t   id   : 16;   /**< right now max of 512 flows   */
            } f1;
} BpaFlowHdl_t;

typedef struct BpaFCBHits
{
        tsl_u32_t   numFlows;        /**< number of valid flows (1 or 8) of an FCB */
        tsl_u32_t   hits[BPA_PBITS_MAX];  /**< hits count array for FCB flows      */
} BpaFCBHits_t;

/** 6816 ports */
typedef enum BpaPort
{
    e_PORT_GEPHY0,               
    e_PORT_GEPHY1,               
    e_PORT_RGMII0,               
    e_PORT_RGMII1,
    e_PORT_SERDES_GMII,          
    e_PORT_MOCA,
    e_PORT_USB,
    e_PORT_GPON,
    e_PORT_MIPS,
    e_PORT_MAX
} BpaPort_t;

typedef enum BpaApiRet
{
    e_RET_SUCCESS,
    e_RET_ERR_FAILURE,
    e_RET_MAX
} BpaApiRet_t;

#define PT_MAX_ENTRIES  100      
#define PT_MAX_PORTS    1000     
#define PT_MAX_EXPECTED 1000     
#define PT_TIMEOUT  180          

#define SKBMARK_Q_S             0
#define SKBMARK_Q_M             (0x1F << SKBMARK_Q_S)
#define SKBMARK_GET_Q(MARK)     ((MARK & SKBMARK_Q_M) >> SKBMARK_Q_S)
#define SKBMARK_SET_Q(MARK, Q)  ((MARK & ~SKBMARK_Q_M) | (Q << SKBMARK_Q_S))

#define cmsLed_setWanConnected()   do {\
					system("echo 104 > /proc/simple_config/internet_config_led ");\
					system("echo 204 > /proc/simple_config/internet_config_led ");\
					system("echo 102 > /proc/simple_config/internet_config_led ");\
					system("echo 201 > /proc/simple_config/internet_config_led ");\
					system("echo 105 > /proc/simple_config/internet_config_led ");\
				} while(0)
#define cmsLed_setWanDisconnected()   do {\
					system("echo 104 > /proc/simple_config/internet_config_led ");\
					system("echo 204 > /proc/simple_config/internet_config_led ");\
					system("echo 101 > /proc/simple_config/internet_config_led ");\
					system("echo 201 > /proc/simple_config/internet_config_led ");\
					system("echo 106 > /proc/simple_config/internet_config_led ");\
				} while(0)
#define cmsLed_setWanConnecting()   do {\
					system("echo 104 > /proc/simple_config/internet_config_led ");\
					system("echo 203 > /proc/simple_config/internet_config_led ");\
					system("echo 102 > /proc/simple_config/internet_config_led ");\
					system("echo 201 > /proc/simple_config/internet_config_led ");\
					system("echo 106 > /proc/simple_config/internet_config_led ");\
				} while(0)
#define cmsLed_setWanFailed()   do {\
					system("echo 104 > /proc/simple_config/internet_config_led ");\
					system("echo 204 > /proc/simple_config/internet_config_led ");\
					system("echo 101 > /proc/simple_config/internet_config_led ");\
					system("echo 202 > /proc/simple_config/internet_config_led ");\
					system("echo 106 > /proc/simple_config/internet_config_led ");\
				} while(0)
#define cmsLed_setWanTraffic()   do {\
					fprintf(stdout,"call led wan traffic\n");\
				} while(0)
				
#define SKBMARK_FLOW_ID_S       11
#define SKBMARK_FLOW_ID_M       (0xFF << SKBMARK_FLOW_ID_S)
#define SKBMARK_GET_FLOW_ID(MARK) \
        ((MARK & SKBMARK_FLOW_ID_M) >> SKBMARK_FLOW_ID_S)
#define SKBMARK_SET_FLOW_ID(MARK, FLOW) \
        ((MARK & ~SKBMARK_FLOW_ID_M) | (FLOW << SKBMARK_FLOW_ID_S))
/*******************end hardware related*************/
/*******************syslog related****************/
/** indicate first read */       
#define SYSLOG_FIRST_READ           -2

/** indicates error */           
#define SYSLOG_READ_BUFFER_ERROR    -1

/** indicates last line was read */
#define SYSLOG_READ_BUFFER_END      -3

/** max log buffer length */     
#define SYSLOG_MAX_LINE_SIZE        255
/*******************syslog related end****************/
#ifdef DMP_X_ACTIONTEC_COM_AUTODETECTION_1
#define STOP_PPPD(n) \
   ((n)->X_ACTIONTEC_COM_X_StopPppD == TSL_B_TRUE)
#else
#define STOP_PPPD(n) (TSL_B_FALSE)
#endif   

#ifdef DMP_X_ACTIONTEC_COM_AUTODETECTION_1
#define AUTO_DETECT_SEND_UPDATE_MSG(oid, iidStack, isDeleted) \
rutWan_sendConnectionUpdateMsg(oid, iidStack, TSL_B_FALSE, TSL_B_FALSE, isDeleted, TSL_B_FALSE);
#else
/* do nothing if auto detect is not defined */
#define AUTO_DETECT_SEND_UPDATE_MSG(oid, iidStack, isDeleted) \
ctllog_debug("AUTO_DETECT_SEND_UPDATE_MSG do nothing if auto is not defined");
#endif   
#define INIT_INSTANCE_ID_STACK(s)  (memset((void *)(s),0,sizeof(tr69_oid_stack_id)))
#define EMPTY_INSTANCE_ID_STACK {0,{0,0,0,0,0,0}}
#define PEEK_INSTANCE_ID(s) \
       ((s)->currentDepth > 0) ? (s)->instance[(s)->currentDepth - 1] : 0
/** Macro to push another instance id onto the instance stack. */
#define PUSH_INSTANCE_ID(s, id) \
       do {if ((s)->currentDepth < MAX_INSTANCE_DEPTH ) { \
                    (s)->instance[(s)->currentDepth] = (id);    \
                    (s)->currentDepth++; }                       \
              } while(0)


/** Macro to pop the topmost instance id from the stack. */
#define POP_INSTANCE_ID(s) \
   ((s)->currentDepth > 0) ? (s)->instance[(((s)->currentDepth)--) - 1] : 0
typedef struct
{
       char       oid[100];        /**< Object Identifier.*/
       tr69_oid_stack_id   iidStack;   /**< Instance Id Stack.*/
       char paramName[MAX_PARAM_NAME_LENGTH + 1];
}tr69_path_desc;
tsl_rv_t oid_to_iidstack(char *oid,tr69_oid_stack_id *p_iidStack);
/** Macro to determine if this is a new MdmObject creation case.
 */
#define ADD_NEW(n, c) (((n) != NULL) && (c == NULL))


/** Macro to determine if this is a MdmObject deletion case
 */
#define DELETE_EXISTING(n, c) (((n) == NULL) && (c != NULL))

/** Macro to determine if this is a MdmObject parameter change case
 */
#define UPDATE_EXISTING(n, c) (((n) != NULL) && (c != NULL))

/** Macro to determine if an MdmObject is being added and enabled,
 *  or if an existing MdmObject that is currently disabled is now is being enabled.
 */
#define ENABLE_NEW_OR_ENABLE_EXISTING(n, c) \
   (((n) != NULL && (n)->enable && (c) == NULL) || \
    ((n) != NULL && (n)->enable && (c) != NULL && !((c)->enable)))
    
/** Macro to determine if an existing MdmObject is being enabled.
 */
#define ENABLE_EXISTING(n, c) \
   ((n) != NULL && (n)->enable && (c) != NULL && !((c)->enable))


/** Macro to determine if an MdmObject is being deleted (which implies disble),
 *  or if an existing MdmObject that is currently enabled is now being disabled.
 */
#define DELETE_OR_DISABLE_EXISTING(n, c) \
   (((n) == NULL) ||                                                    \
    ((n) != NULL && !((n)->enable) && (c) != NULL && (c)->enable))

/* ACTION_TEC */
#define DISABLE_EXISTING(n, c) \
   (((n) != NULL && !((n)->enable) && (c) != NULL && (c)->enable))

/** Macro to determine if this is POTENTIALLY a modification of an existing
 * and enabled object.  
 * 
 * This macro must be used in conjunction with another function which
 * you provide that determines if any of the fields in the object has
 * changed.  This macro only verifies that the object currently exists
 * and is enabled. Example usage:
 * 
 * if (POTENTIAL_CHANGE_OF_EXISTING(newObj, currObj) && fieldsHaveChanged(newObj, currObj)
 * 
 */
#define POTENTIAL_CHANGE_OF_EXISTING(n, c) \
   (((n) != NULL && (n)->enable && (c) != NULL && (c)->enable))

/** Macro to determine if an MdmObject is being added and enabled,
 *  *  or if an existing MdmObject that is currently disabled is now is being enabled.
 *   */
#define CTL_ENABLE_NEW_OR_ENABLE_EXISTING(n, c) \
       (((n) != NULL && (n)->enable && (c) == NULL) || \
            ((n) != NULL && (n)->enable && (c) != NULL && !((c)->enable)))

/** Macro to determine if this is POTENTIALLY a modification of an existing
 *  * and enabled object.  
 *   * 
 *    * This macro must be used in conjunction with another function which
 *     * you provide that determines if any of the fields in the object has
 *      * changed.  This macro only verifies that the object currently exists
 *       * and is enabled. Example usage:
 *        * 
 *         * if (POTENTIAL_CHANGE_OF_EXISTING(newObj, currObj) && fieldsHaveChanged(newObj, currObj)
 *     * 
 *      */
#define CTL_POTENTIAL_CHANGE_OF_EXISTING(n, c) \
       (((n) != NULL && (n)->enable && (c) != NULL && (c)->enable))
//#define GET_MDM_OBJECT_ID(obj)  (*((tr69_oid *) (obj)))
tsl_rv_t tr69_oid_to_fullpath(tr69_path_desc *pathDesc, char **fullpath);
tsl_rv_t tr69_fullpath_to_oid(const char *fullpath, tr69_path_desc *pathDesc);
char *tr69_dump_stackid(const tr69_oid_stack_id *iidStack);
tsl_32_t tr69_compare_stackid(const tr69_oid_stack_id *iidStack1, const tr69_oid_stack_id *iidStack2);
tsl_rv_t tr69_get_obj_by_oid(tr69_oid oid, tr69_oid_stack_id *iidStack, int getFlags, void **obj);
tsl_rv_t tr69_set_obj_by_oid(tr69_oid oid, void *obj, tr69_oid_stack_id *iidStack);
tsl_rv_t tr69_free_obj_by_oid(tr69_oid oid, tr69_oid_stack_id *iidStack, void **obj);
tsl_rv_t tr69_get_next_obj_by_oid(tr69_oid oid, tr69_oid_stack_id *iidStack, void **obj);
tsl_rv_t tr69_get_next_obj_by_oid_flag(tr69_oid oid, tr69_oid_stack_id *iidStack, tsl_u32_t getFlags, void **obj);
tsl_rv_t tr69_get_next_sub_obj_by_oid(tr69_oid oid, tr69_oid_stack_id *ParentiidStack, tr69_oid_stack_id *iidStack, void **obj);
tsl_rv_t tr69_get_next_sub_obj_by_oid_flag(tr69_oid oid, tr69_oid_stack_id *ParentiidStack, tr69_oid_stack_id *iidStack, tsl_u32_t getFlags, void **obj);
tsl_rv_t tr69_get_parent_obj_by_oid(tr69_oid parent_oid, tr69_oid oid, tr69_oid_stack_id *iidStack, void **obj);
tsl_rv_t tr69_get_parent_obj_by_oid_flag(tr69_oid parent_oid, tr69_oid oid, tr69_oid_stack_id *iidStack,tsl_u32_t getFlags, void **obj);
tsl_rv_t tr69_add_inst_by_oid(tr69_oid oid, tr69_oid_stack_id *iidStack);
tsl_rv_t tr69_del_inst_by_oid(tr69_oid oid, tr69_oid_stack_id *iidStack);
#endif
