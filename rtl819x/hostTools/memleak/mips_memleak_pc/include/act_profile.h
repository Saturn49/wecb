#ifndef ACT_PROFILE_H
#define ACT_PROFILE_H

#include "act_common.h"
#include "mc_port.h"
#include "mc_list.h"
#include "mc_hash.h"


typedef struct act_profile_tree_s{
    act_char_t function[MAX_NAME_SIZE];
    act_uint_t parent_pc;
    act_uint_t self_pc;
    act_uint_t flow_count;
    struct timespec time;
    struct timespec end_time;
    struct timespec child_time;
    mc_list_head_t list_head;
    mc_list_head_t list_node;
    act_uint_t last_show;
    struct timespec total_time;
    float stat_total_ftime;
}act_profile_tree_t;


typedef struct act_profile_root_s{
    act_uint_t pid;
    act_profile_tree_t *p_tree_root;
    act_uint_t flow_count;
}act_profile_root_t;


typedef struct act_profile_static_s{
    act_uint_t pid;
    act_char_t function[MAX_NAME_SIZE];
    struct timespec total_time;
    act_uint_t total_count;
}act_profile_statics_t;


act_rv_t act_memleak_profile_func_add(act_uint_t caller_pc, 
                                      act_uint_t self_pc,
                                      struct timespec time,
                                      act_int_t pid);

act_void_t act_memleak_profile_func_end(act_int_t pid, struct timespec time);

act_void_t act_memleak_profile_func_cleanup();

act_void_t act_memleak_profile_func_travel(act_int_t disp_level, act_int_t pid);

act_void_t act_memleak_profile_func_stat_disp(act_uint_t pid);

act_void_t act_memleak_profile_func_stat_cleanup();

#endif
