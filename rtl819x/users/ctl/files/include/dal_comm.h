#ifndef DAL_COMM__H
#define DAL_COMM_H

#include <fcntl.h>
#include "ctl.h"
#include "tsl_common.h"
#include "tr69_cms_object.h"

int dal_lock_reg(int fd, int cmd, int type, 
                        off_t offset, int whence, off_t len);

#define read_lock(fd, offset, whence, len) \
    dal_lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))

#define readw_lock(fd, offset, whence, len) \
    dal_lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))

#define write_lock(fd, offset, whence, len) \
    dal_lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))

#define writew_lock(fd, offset, whence, len) \
    dal_lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))

#define un_lock(fd, offset, whence, len) \
    dal_lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

/*************************************************************/

#define DAL_MAX_PARAM_NUM 64 
#define DAL_MAX_ERR_MSG_LEN  128
#define DAL_INST_OID	DAL_MAX_PARAM_NUM -1
//param[DAL_INST_OID] means oid of pointed interface name

typedef struct {
    tsl_rv_t ret;
    char *errorMsg;
    int param_num;
    char *param[DAL_MAX_PARAM_NUM]; 
} dal_ret_t;

/*************************************************************/

char *oid_to_fullname(tr69_oid obj_oid, const tr69_oid_stack_id *iidStack, char *param);
char *fullname_to_oid(char* full_name, tr69_oid_stack_id *p_iidStack);

/*************************************************************/

void dump_dal_ret(dal_ret_t *dal_ret);
void free_dal_ret(dal_ret_t **ret);

#define DAL_RET_CONTINUOUS 1
#define DAL_RET_DISTRIBUTE 2
void dump_multiple_dal_ret(dal_ret_t **dal_ret, int num, int flag);
void free_multiple_dal_ret(dal_ret_t **dal_ret, int num, int flag);

char *itoa(int num);
char **str_printf(char **s, char *fmt, ...);
char **str_cat(char **s, char *str);
char **str_free(char **s);

/*************************************************************/

/* Convert the iidStack into a buffer which is printable */
char *dal_dumpIidStack(const tr69_oid_stack_id *iidStack);

/* Convert to the full name of a parameter */
char *oid_to_fullname(tr69_oid obj_oid, const tr69_oid_stack_id *iidStack, char *param);

/* Get Parameter Value based on OID, iid, and Parameter Name
 * Input:
 *      obj_oid:  the macros defined in OID.h,
 *      iidStack: a valid tr69_oid_stack_id,
 *      param:    a valid parameter name,
 *      getFlags: non-zero: call tr69_get_unfresh_leaf_data()
 *                zero: call tr69_get_leaf_data()
 *      mdmValue: the *mdmValue can be NULL, the memroy will be alloced
 *                if it is NULL, the memory will be realloced if it is non-NULL
 * Output:
 *      mdmValue: will be alloced for the valid string */
tsl_rv_t dal_ParamValueGet(tr69_oid obj_oid, const tr69_oid_stack_id *iidStack,
        char *param, int getFlags, char **mdmValue);

/* Set Parameter Value based on OID, iid, and Parameter Name/Value
 * Input:
 *      obj_oid:  the macros defined in OID.h,
 *      iidStack: a valid tr69_oid_stack_id,
 *      param:    a valid parameter name,
 *      setFlags: non-zero: call tr69_set_leaf_data()
 *                zero: call tr69_commit_set_leaf_data()
 *      value:    a valid string, it will be convert to corresponding type
 * Output:
 *      None */
tsl_rv_t dal_ParamValueSet(tr69_oid obj_oid, const tr69_oid_stack_id *iidStack,
        char *param, int setFlags, char *value);

/* Find the first Parameter in the whole data model which its OID, iid, 
 * and Parameter Name are matched if iid is EMPTY_INSTANCE_ID_STACK.
 * Find the next parameter if iid isn't empty.
 * Input:
 *      obj_oid:  the macros defined in OID.h,
 *      iidStack: a valid tr69_oid_stack_id or a EMPTY_INSTANCE_ID_STACK,
 *      param:    a valid parameter name,
 *      getFlags: non-zero: call tr69_get_unfresh_leaf_data()
 *                zero: call tr69_get_leaf_data()
 *      mdmValue: the *mdmValue can be NULL, the memroy will be alloced
 *                if it is NULL, the memory will be realloced if it is non-NULL
 * Output:
 *      mdmValue: will be alloced for the valid string */
tsl_rv_t dal_ParamValueGetNextFlags(tr69_oid obj_oid, tr69_oid_stack_id *iidStack,
        char *param, tsl_u32_t getFlags, char **mdmValue);
tsl_rv_t dal_ParamValueGetNext(tr69_oid obj_oid, tr69_oid_stack_id *iidStack,
        char *param, char **mdmValue);

/* Find the first Parameter in the whole data model which its OID, iid, 
 * and Parameter Name are matched if iid is EMPTY_INSTANCE_ID_STACK.
 * Find the next parameter if iid isn't empty.
 * The base_oid and base_iidStack are used to locate the reference object.
 * The obj_oid can be the ancestor or child object of reference object.
 * Input:
 *      obj_oid:  the macros defined in OID.h,
 *      iidStack: a valid tr69_oid_stack_id or a EMPTY_INSTANCE_ID_STACK,
 *      base_oid: the macros defined in OID.h,
 *      base_iidStack: a valid tr69_oid_stack_id, it can'be be empty
 *      param:    a valid parameter name,
 *      getFlags: non-zero: call tr69_get_unfresh_leaf_data()
 *                zero: call tr69_get_leaf_data()
 *      mdmValue: the *mdmValue can be NULL, the memroy will be alloced
 *                if it is NULL, the memory will be realloced if it is non-NULL
 * Output:
 *      mdmValue: will be alloced for the valid string */
tsl_rv_t dal_ParamValueGetNextByBaseOid(tr69_oid obj_oid, tr69_oid base_oid,
        tr69_oid_stack_id *obj_iidStack, tr69_oid_stack_id *base_iidStack,
        char *param, tsl_u32_t getFlags, char **mdmValue);

/* Find the first object instance which has X_ACTIONTEC_COM_X_IfName defined
 * Input:
 *      intfname: the device interface name, e.g., eth0, ppp0, wifi0 ...
 * Ooutput:
 *      obj_oid:  the object OID which defined in OID.h
 *      iidStack: the iid correspoinding */
tsl_rv_t dal_intfnameToFullPath(const char *intfname,
        int layer2, tr69_oid *obj_oid, tr69_oid_stack_id *iidStack);

/*************************************************************/
typedef struct dal_gen dal_gen_t;
typedef tsl_rv_t (*get_base_cb)(
        tr69_oid base_oid, tr69_oid_stack_id *base_iidStack, void * customerStruct);
typedef tsl_rv_t (*get_next_cb)(
        tr69_oid base_oid, tr69_oid_stack_id *base_iidStack,
        tr69_oid obj_oid, tr69_oid_stack_id *obj_iidStack, void * customerStruct);
typedef tsl_rv_t (*gpv_cb)(dal_ret_t *dal_ret,
        tr69_oid obj_oid, tr69_oid_stack_id *iidStack);
typedef tsl_rv_t (*match_cb)(dal_gen_t *context,
        tr69_oid obj_oid, tr69_oid_stack_id *iidStack, void * customerStruct);
typedef tsl_rv_t (*spv_cb)(dal_ret_t *dal_ret,
        tr69_oid obj_oid, tr69_oid_stack_id *iidStack);

typedef struct {
    int param_idx;
    char *param_name;
} dal_param_t;

typedef struct dal_param_group{
    tr69_oid obj_oid;
    tr69_oid base_oid;
    tr69_oid_stack_id obj_iidStack;
    tr69_oid_stack_id base_iidStack;
    dal_param_t *param_list;

    get_base_cb get_base;
    get_next_cb get_next;

    gpv_cb custom_gpv_cb;
    match_cb get_match;
    match_cb match_cb;
    spv_cb custom_spv_cb;
    int operationPID;

} dal_param_group_t;

struct dal_gen{
    dal_param_group_t *param_list;
    dal_ret_t **dal_ret;
    int *dal_ret_num;
} ;

#define GEN_DAL_GET 1
#define GEN_DAL_SET 2
#define GEN_DAL_ADD 3
#define GEN_DAL_DEL 4
tsl_rv_t dal_genDALHandler(dal_gen_t *context, void * customerStruct, int flag);

#if 0
#if 0
#define fw_v4Def_invalue_low        "79FFFFFFFFFFFF"
#define fw_v4Def_invalue_medium     "1E0FC"
#define fw_v4Def_invalue_high       "1E0DC"
#define fw_v4Def_outvalue_low       "7FFFFFFFFFFFFF"
#define fw_v4Def_outvalue_medium    "7FFFFFFFFFFFFF"
#define fw_v4Def_outvalue_high      "7FFFFFFFFFFFDF"
#else
#define fw_v4Def_invalue_low        "F9FFFFFFFFFFFF"
#define fw_v4Def_invalue_medium     "8000000001E0FC"
#define fw_v4Def_invalue_high       "8000000001E0DC"
#define fw_v4Def_outvalue_low       "FFFFFFFFFFFFFF"
#define fw_v4Def_outvalue_medium    "FFFFFFFFFFFFFF"
#define fw_v4Def_outvalue_high      "FFFFFFFFFFFFDF"
#endif
#else
#define fw_v4Def_invalue_low        "F3FFFFFFFFFFFF"
#define fw_v4Def_invalue_medium     "800000001C783C"
#define fw_v4Def_invalue_high       "800000001C783C"
#define fw_v4Def_outvalue_low       "FFFFFFFFFFFFFF"
#define fw_v4Def_outvalue_medium    "FFFFFFFFFFFFFF"
#define fw_v4Def_outvalue_high      "FFFFFFFFFFFFFF"
#endif

/*************************************************************/

dal_ret_t* dal_get_oid_by_ifname_sample(char *ifname);
dal_ret_t* dal_get_lan_intf_list_sample(void);
dal_ret_t* dal_get_wan_conn_list_sample(void);
dal_ret_t* dal_get_bridge_list_sample(void);
dal_ret_t* dal_get_phydev_list_sample(void);
dal_ret_t* dal_get_enslaved_bridge_by_port_sample(char *port_name);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        dal_SaveConfig
 *
 *	[DESCRIPTION]:
 *	        save configuration
 *
 *	[PARAMETER]:
 *	       NONE
 *
 *	[RETURN]
 *              TSL_RV_SUC :          SUCCESS
 *              other		  :          FAIL
 **************************************************************************/
tsl_rv_t dal_SaveConfig(void);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        dal_getDateTime
 *
 *	[DESCRIPTION]:
 *	       get current datetime. 
 *		format: y-m-d h:m:s:z			
 *
 *	[PARAMETER]:
 *	       NONE
 *
 *	[RETURN]
 *              TSL_RV_SUC :          SUCCESS
 *              other		  :          FAIL
 **************************************************************************/
tsl_rv_t dal_getDateTime(tsl_char_t *buf, tsl_int_t bufLen);
int AEI_compare_version(const char * old_version, const char * new_version);
#endif /* DAL_COMM_H */
