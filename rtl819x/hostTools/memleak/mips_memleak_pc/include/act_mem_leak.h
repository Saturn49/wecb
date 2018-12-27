/* FILE NAME: act_common.h
 * PURPOSE: the header file 
 * NOTES:
 * REASON:
 *      DESCRIPTION:  
 *      CREATOR: Yunhai Zhu  
 *      DATE: 2009/3/31
 * Copyright (C), Yunhai Zhu
 */

#ifndef ACT_MEM_LEAK_H
#define ACT_MEM_LEAK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "act_common.h"

typedef struct act_memleak_avl_node_s{
    struct act_memleak_avl_node_s *parent;
    struct act_memleak_avl_node_s *left;
    struct act_memleak_avl_node_s *right;
    act_int_t   balance;  
    act_uint_t  key;
    act_void_t *p_value;
}act_memleak_avl_node_t;


#define ACT_MEMLEAK_MAX_STACK    30
#define ACT_MEM_LEAK_MEM_TAG_B   121
#define ACT_MEM_LEAK_MEM_TAG_E   99


#define ACT_MEMLEAK_MEM_ST_ALLOC 1
#define ACT_MEMLEAK_MEM_ST_FREED 2
#define ACT_MEMLEAK_MEM_ST_DFREE 4
#define ACT_MEMLEAK_MEM_ST_OFLOW 8
#define ACT_MEMLEAK_MEM_ST_UOFLOW 16

typedef struct act_memleak_mem_statics_ref_s{
    act_uint_t pid;
    act_char_t name[32];
    act_uint_t stack_ra[24];
    act_uint_t mem_total_alloc;
    act_uint_t mem_total_free;
    act_char_t status;
}act_memleak_mem_statics_ref_t;

typedef struct act_memleak_mem_ref_s{
    act_uint_t pid;
    act_void_t *p_mem;
    act_uint_t mem_size;
    time_t time;
    act_uint_t stack_index[ACT_MEMLEAK_MAX_STACK];
    act_char_t status;
    act_uint_t stack_ra[24];
    act_memleak_mem_statics_ref_t *p_mem_statics_ref;
}act_memleak_mem_ref_t;



typedef struct act_memleak_frame_ref_s{
    act_uint_t frame_size;
    act_uint_t frame_fp_offset;
    act_uint_t frame_ra_offset;
}act_memleak_frame_ref_t;


typedef struct act_memleak_func_ref_s{
    act_char_t func_name[MAX_NAME_SIZE];//need to change
    act_memleak_frame_ref_t frame_ref;
}act_memleak_func_ref_t;

typedef struct act_memleak_line_ref_s{
    act_int_t file_index;
    act_int_t line_index;
}act_memleak_line_ref_t;


typedef struct act_memleak_ref_root_s{
    act_memleak_avl_node_t *p_mem_ref_root;
    act_memleak_avl_node_t *p_func_ref_root;
    act_memleak_avl_node_t *p_file_line_ref_root;
}act_memleak_ref_root_t;


#define ACT_MEMLEAK_MQ_OP_ADD    1
#define ACT_MEMLEAK_MQ_OP_DEL    2
#define ACT_MEMLEAK_MQ_OP_OVL    4
#define ACT_MEMLEAK_MQ_OP_CLR    8
#define ACT_MEMLEAK_MQ_OP_UOVL    16

#define ACT_MEMLEAK_MQ_OP_DISP_UNFREE 17
#define ACT_MEMLEAK_MQ_OP_DISP_DBFREE 32
#define ACT_MEMLEAK_MQ_OP_DISP_OVFLOW 64

#define ACT_MEMLEAK_MQ_OP_DISP  125
#define ACT_MEMLEAK_MQ_OP_SEGV  126
#define ACT_MEMLEAK_MQ_OP_QUIT  127

#define ACT_MEMLEAK_MQ_OP_MAPS  111


#define ACT_MEMLEAK_MQ_OP_PROFILE_ADD  80
#define ACT_MEMLEAK_MQ_OP_PROFILE_DISP 81
#define ACT_MEMLEAK_MQ_OP_PROFILE_END  82
#define ACT_MEMLEAK_MQ_OP_PROFILE_GEO  83

#define ACT_MEMLEAK_MQ_MEM_TYPE_PUBLIC  1
#define ACT_MEMLEAK_MQ_MEM_TYPE_PRIVATE 2

#define MQ_SERVER_IP "192.168.1.80"

#define MQ_SERVER_PORT 1998

#define MQ_SERVER_COMMAND_PORT 4000
#define MQ_SERVER_MAPS_PORT    4001
#define MQ_SERVER_MEMREF_PORT     4002
#define MQ_SERVER_PROFILE_PORT 4003

    
#define MAPS_STACK 0
#define MAPS_LIB_CMS_CORE 1
#define MAPS_LIB_CMS_UTIL 2
#define MAPS_LIB_CMS_DAL  3
#define MAPS_LIB_CMS_MSG  4
#define MAPS_MAIN         5
#define MAPS_LIB_NANO_XML 6
#define MAPS_LIB_XDSL_CTL  7
#define MAPS_LIB_ATM_CTL  8
#define MAPS_LIB_CMS_BOARD_CTL 9
#define MAPS_LIB_WL_MNGR 10
#define MAPS_LIB_WL_CTL  11
#define MAPS_LIB_NVRAM   12
#define MAPS_LIB_CGI     13

#define MAPS_MAX         14

typedef struct act_memleak_maps_s{
    unsigned int add1;
    unsigned int add2;
    unsigned int add3;
    unsigned int add4;
    char name[32];
}act_memleak_maps_t;

typedef struct act_profile_mq_s{
    act_uint_t caller_pc;
    act_uint_t self_pc;
    struct timespec time;
}act_profile_mq_t;
    
typedef struct act_memleak_mq_command{
    char mq_opcode;
}act_memleak_mq_command_t;

typedef struct act_memleak_mq_maps{
    char mq_opcode;
    unsigned int pid;
    
    union{
        act_memleak_maps_t maps[MAPS_MAX];
    }u;
}act_memleak_mq_maps_t;

typedef struct act_memleak_mq_mem{
    char mq_opcode;
    unsigned int pid;
    void *p_mem;
    unsigned int mem_size;
    union{
        unsigned int stack_index[ACT_MEMLEAK_MAX_STACK];
    }u;
}act_memleak_mq_mem_t;

typedef struct act_memleak_mq_profile{
    char mq_opcode;
    unsigned int pid;
    union{
        act_profile_mq_t profile;
    }u;
}act_memleak_mq_profile_t;
    

typedef struct {
    act_int_t pid;
    act_char_t name[128];
}act_memleak_pid_info_t;

act_rv_t act_memleak_mem_ref_add(act_memleak_avl_node_t **pp_mem_root, act_void_t *p_ptr, act_uint_t size, act_uint_t *p_stack_index, act_uint_t pid);
act_rv_t act_memleak_mem_ref_del(act_memleak_avl_node_t **pp_mem_root, act_uint_t addr, act_uint_t *p_stack_index, act_uint_t pid);
act_rv_t act_memleak_mem_ref_mod(act_memleak_avl_node_t **pp_mem_root, act_uint_t addr, act_char_t status);
act_rv_t act_memleak_mem_ref_cleanup(act_memleak_avl_node_t **pp_mem_root);

act_rv_t act_memleak_func_ref_add(act_memleak_avl_node_t **pp_func_root, act_void_t *p_addr, act_char_t *p_func_name);
act_memleak_func_ref_t *act_memleak_func_ref_find(act_memleak_avl_node_t *p_func_root, act_uint_t addr);
act_rv_t act_memleak_func_ref_cleanup(act_memleak_avl_node_t **pp_func_root);

act_rv_t act_memleak_line_ref_add(act_memleak_avl_node_t **pp_line_root, act_uint_t addr, int file_index, int line_index);
act_memleak_line_ref_t *act_memleak_line_ref_find(act_memleak_avl_node_t *p_line_root, act_uint_t addr);
act_rv_t act_memleak_line_ref_cleanup(act_memleak_avl_node_t **pp_line_root);

act_void_t *act_memleak_recv_mq_thread(void *argv);
act_void_t act_memleak_send_mq_op(act_int_t op_code);

act_rv_t act_memleak_func_frame_ref_add(act_memleak_avl_node_t *p_func_root, act_uint_t addr, act_uint_t frame_size, act_uint_t frame_fp_off, act_uint_t frame_ra_off);


act_rv_t act_memleak_mem_statics_ref_add(act_memleak_mem_ref_t *p_mem_node);
act_rv_t act_memleak_mem_statics_ref_del(act_memleak_mem_ref_t *p_mem_node);
act_void_t act_memleak_mem_statics_ref_cleanup();
act_void_t act_memleak_mem_statics_ref_all_display();
act_rv_t act_memleak_mem_statics_ref_show(act_memleak_mem_ref_t *p_mem_node);


act_memleak_ref_root_t *act_memleak_ref_root_find(act_uint_t pid);
act_void_t act_memleak_ref_root_cleanup();

act_rv_t act_memleak_func_ra_display(act_uint_t pid, act_uint_t *p_stack_frame);
act_rv_t act_memleak_func_ra_translate(act_uint_t pid, act_uint_t *p_stack_frame, act_uint_t *p_ra_stack);

act_int_t parse_elf(act_memleak_avl_node_t **pp_func_ref_root,
                    act_memleak_avl_node_t **pp_line_ref_root,
                    act_char_t *p_file, 
                    unsigned int base_address1,
                    unsigned int max_base_address1,
                    unsigned int base_address2 );

act_rv_t sbs_func_status_bind(act_int_t port);

act_char_t *act_memleak_find_pid(act_int_t pid);
act_void_t act_memleak_dispaly_all_pid();

#ifdef __cplusplus
}
#endif

#endif
