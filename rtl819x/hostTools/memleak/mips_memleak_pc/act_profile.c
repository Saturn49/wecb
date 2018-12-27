#include "act_profile.h"
#include "act_mem_leak.h"

static mc_hash_t *gp_act_profile_root_hash = NULL;
static mc_hash_t *gp_act_profile_stat_hash = NULL;
static act_uint_t g_disp_pid = 0;

extern act_char_t g_file_table[512][256];

extern act_memleak_pid_info_t g_pid_table[256];
extern act_int_t g_pid_max;

act_profile_tree_t *act_memleak_profile_inline_find(act_uint_t pc, act_profile_tree_t *p_profile_head, act_int_t pid);
act_profile_tree_t *act_memleak_profile_inline_travel(act_int_t cleanup, act_profile_tree_t *p_profile_head);

act_void_t act_memleak_profile_inline_travel2(act_profile_tree_t *p_profile_head);

act_void_t act_memleak_profile_inline_cleanup(act_profile_tree_t *p_profile_head);
act_void_t act_memleak_profile_inline_travel_ok(act_int_t disp_level, act_int_t level, act_profile_tree_t *p_profile_head, act_int_t pid);
act_void_t act_memleak_profile_inline_travel_ok2(act_int_t disp_level, act_int_t level, act_profile_tree_t *p_profile_head, act_int_t pid);



act_rv_t act_memleak_profile_func_stat_add(act_uint_t pid,
                                      act_uint_t self_pc,
                                      struct timespec time)
{
    act_memleak_func_ref_t *p_func_node = NULL;
    act_memleak_avl_node_t **pp_func_root = NULL;
    act_memleak_ref_root_t *p_ref_root = NULL;;
    act_profile_statics_t *p_ref_stat = NULL;;
    act_memleak_line_ref_t *p_line_node = NULL;
    act_memleak_avl_node_t **pp_file_line_root;
    
    p_ref_root = act_memleak_ref_root_find(pid);
    pp_func_root = &p_ref_root->p_func_ref_root;
    pp_file_line_root = &p_ref_root->p_file_line_ref_root;

    act_char_t key[128] = "\0";
    
    p_ref_root = act_memleak_ref_root_find(pid);
    pp_func_root = &p_ref_root->p_func_ref_root;
   
    p_func_node = act_memleak_func_ref_find(*pp_func_root, self_pc);
    
    ACT_VASSERT(p_func_node != NULL);
    
    sprintf(key, "%d-%s", pid, p_func_node->func_name);

    if (gp_act_profile_stat_hash == NULL){
        gp_act_profile_stat_hash = mc_hash_create();
    }
    
    p_ref_stat = (act_profile_statics_t *)
        mc_hash_find(key, gp_act_profile_stat_hash);
    
    if (p_ref_stat == NULL){
        ACT_MALLOC(p_ref_stat, act_profile_statics_t);
        mc_hash_add(key, p_ref_stat, gp_act_profile_stat_hash);
    }
    p_line_node = act_memleak_line_ref_find(*pp_file_line_root, self_pc); 
    
    p_ref_stat->pid = pid;
    sprintf(p_ref_stat->function, "%s(%s,%d)", p_func_node->func_name, g_file_table[p_line_node->file_index], p_line_node->line_index); 
    p_ref_stat->total_time.tv_sec += time.tv_sec;
    p_ref_stat->total_time.tv_nsec += time.tv_nsec;
    p_ref_stat->total_count ++;

    return ACT_RV_SUC;
}

act_void_t act_memleak_profile_stat_disp_callback(act_profile_statics_t *p_prof_stat)
{
    unsigned long long lltime;

    if (p_prof_stat->pid != g_disp_pid){
        return;
    }
    
    lltime = ((unsigned long long)p_prof_stat->total_time.tv_sec)*1000000000 +
        (unsigned long long)p_prof_stat->total_time.tv_nsec;
    
    act_yellow_printf("    %-48s ", p_prof_stat->function);
    act_blue_printf("Called: %4u ", p_prof_stat->total_count);
    act_zred_printf("Consume:%14.6fs\n",(float)lltime/1000000000);
}

act_void_t act_memleak_profile_func_stat_disp(act_uint_t pid)
{
    g_disp_pid = pid;
    
    mc_hash_func_display(gp_act_profile_stat_hash, (void *)act_memleak_profile_stat_disp_callback);
}

act_void_t act_memleak_profile_func_stat_cleanup()
{
    mc_hash_value_destroy(gp_act_profile_stat_hash);
    gp_act_profile_stat_hash = NULL;
}


act_rv_t act_memleak_profile_func_add(act_uint_t caller_pc, 
                                      act_uint_t self_pc,
                                      struct timespec time,
                                      act_int_t pid)
{
    act_profile_root_t *p_profile_root = NULL;
    act_profile_tree_t *p_profile_parent = NULL;
    act_profile_tree_t *p_profile = NULL;
    act_char_t key[128] = "\0";
    
    printf("ADD caller:0x%x self:0x%x\n", caller_pc, self_pc);
    
    if (gp_act_profile_root_hash == NULL){
        gp_act_profile_root_hash = mc_hash_create();
    }
    
    ACT_VASSERT(gp_act_profile_root_hash != NULL);
    
    sprintf(key, "%d", pid);
    p_profile_root = (act_profile_root_t *)mc_hash_find(key, gp_act_profile_root_hash);
 
    ACT_MALLOC(p_profile, act_profile_tree_t);
    printf("alloc:%p\n", p_profile);

    if (p_profile_root == NULL){
        ACT_MALLOC(p_profile_root, act_profile_root_t);
        mc_hash_add(key, p_profile_root, gp_act_profile_root_hash);
        p_profile_root->p_tree_root = p_profile;
        p_profile_root->pid = pid;
        p_profile_root->flow_count = 1;
    }else {
        p_profile_parent = act_memleak_profile_inline_find(caller_pc, p_profile_root->p_tree_root, pid);
        ACT_VASSERT(p_profile_parent != NULL);
        MC_INIT_LIST_HEAD(&p_profile->list_node);
        mc_list_add(&p_profile->list_node, &p_profile_parent->list_head);
    }
    
    p_profile->parent_pc = caller_pc;
    p_profile->self_pc = self_pc;
    p_profile->flow_count = p_profile_root->flow_count++;
    p_profile->time = time;
    //p_profile->end_time = time;
    
    MC_INIT_LIST_HEAD(&p_profile->list_head);
    


    return ACT_RV_SUC;

}

act_void_t act_memleak_profile_func_end(act_int_t pid, struct timespec time)
{
    act_profile_root_t *p_profile_root = NULL;
    act_profile_tree_t *p_profile = NULL;
    act_char_t key[128] = "\0";
        
    ACT_VASSERT_RV(gp_act_profile_root_hash != NULL, );
    
    sprintf(key, "%d", pid);
    p_profile_root = (act_profile_root_t *)mc_hash_find(key, gp_act_profile_root_hash);
    
    ACT_VASSERT_RV(p_profile_root != NULL, );
    
    p_profile = act_memleak_profile_inline_travel(1, p_profile_root->p_tree_root);
    p_profile->end_time = time;

}



act_void_t act_memleak_profile_func_cleanup()
{
    act_profile_root_t *p_profile_root = NULL;
    act_char_t key[128] = "\0";
    act_int_t i;

    ACT_VASSERT_RV(gp_act_profile_root_hash != NULL, );
    
    for (i = 0; i < g_pid_max; i++){
        sprintf(key, "%d", g_pid_table[i].pid);
        p_profile_root = (act_profile_root_t *)mc_hash_find(key, gp_act_profile_root_hash);
        
        ACT_VASSERT_RV_ACT(p_profile_root != NULL, ,continue;);
        
        act_memleak_profile_inline_cleanup(p_profile_root->p_tree_root);
        
        ACT_FREE(p_profile_root);
    }
    
    mc_hash_destroy(gp_act_profile_root_hash);
    gp_act_profile_root_hash = NULL;
}


act_void_t act_memleak_profile_inline_cleanup(act_profile_tree_t *p_profile_head)
{
    act_profile_tree_t *p_profile = NULL;
    mc_list_head_t *p_list = NULL;

    for (p_list = p_profile_head->list_head.prev; p_list != &p_profile_head->list_head;){
        p_profile = mc_list_entry(p_list, act_profile_tree_t, list_node);
        p_list = p_list->prev;
        act_memleak_profile_inline_cleanup(p_profile);
    }
    
    printf("freed:%p\n", p_profile_head);
    ACT_FREE(p_profile_head);
    p_profile_head = NULL;
}



act_profile_tree_t *act_memleak_profile_func_find(act_uint_t pc, 
                                                  act_int_t pid)

{
    act_profile_root_t *p_profile_root = NULL;
    act_char_t key[128] = "\0";
    
    ACT_VASSERT_RV(gp_act_profile_root_hash != NULL, NULL);

    sprintf(key, "%d", pid);
    p_profile_root = (act_profile_root_t *)mc_hash_find(key, gp_act_profile_root_hash);

    ACT_VASSERT_RV(p_profile_root != NULL, NULL);

    return act_memleak_profile_inline_find(pc, p_profile_root->p_tree_root, pid);
}



act_void_t act_memleak_profile_func_travel(act_int_t disp_level, act_int_t pid)
{
    act_profile_root_t *p_profile_root = NULL;
    act_profile_tree_t *p_profile = NULL;
    act_char_t key[128] = "\0";


    ACT_VASSERT_RV(gp_act_profile_root_hash != NULL, );
    
    sprintf(key, "%d", pid);
    p_profile_root = (act_profile_root_t *)mc_hash_find(key, gp_act_profile_root_hash);
    
    ACT_VASSERT_RV(p_profile_root != NULL, );
    
    p_profile = act_memleak_profile_inline_travel(1, p_profile_root->p_tree_root);

    if (p_profile != NULL){
        p_profile->last_show = 1;
        p_profile->end_time = p_profile->time;
        act_printf("last_show:%d\n", p_profile->flow_count);
    }

    act_memleak_profile_inline_travel2(p_profile_root->p_tree_root);
    
    act_memleak_profile_inline_travel_ok2(disp_level, 1, p_profile_root->p_tree_root, pid);
}




act_void_t act_memleak_profile_inline_travel_ok(act_int_t disp_level, act_int_t level, act_profile_tree_t *p_profile_head, act_int_t pid)
{
    act_profile_tree_t *p_profile = NULL;
    mc_list_head_t *p_list = NULL;
    act_int_t i = 1;

    act_memleak_line_ref_t *p_line_node = NULL;
    act_memleak_func_ref_t *p_func_node = NULL;
    
    act_memleak_avl_node_t **pp_func_root;
    act_memleak_avl_node_t **pp_file_line_root;
    act_memleak_ref_root_t *p_ref_root;
    unsigned long long lltime;
    float ftime;
    

    p_ref_root = act_memleak_ref_root_find(pid);
    pp_func_root = &p_ref_root->p_func_ref_root;
    pp_file_line_root = &p_ref_root->p_file_line_ref_root;
   
    p_func_node = act_memleak_func_ref_find(*pp_func_root, p_profile_head->self_pc);

    if (level <= disp_level){ 
        act_printf("\033[32m%-6u\033[0m", p_profile_head->flow_count);
        for(i = 1; i <= level; i++){
            act_printf("  ");
        }
        
        if (p_func_node != NULL){                                                                      
            act_printf("\033[34m%s", p_func_node->func_name);                                                 
            p_line_node = act_memleak_line_ref_find(*pp_file_line_root, p_profile_head->self_pc); 
            if (p_line_node != NULL){                                                                 
                act_printf("(%s:%d)\033[0m ",g_file_table[p_line_node->file_index], p_line_node->line_index); 
            }                                                                                         
        }else {
            act_printf("(0x%x) ",p_profile_head->self_pc);
        }
        
        if (p_profile_head->end_time.tv_nsec != 0){
            lltime =  ((unsigned long long)p_profile_head->end_time.tv_sec - (unsigned long long)p_profile_head->time.tv_sec)*1000000000 +
                (unsigned long long)p_profile_head->end_time.tv_nsec - (unsigned long long)p_profile_head->time.tv_nsec;
            
            if (mc_is_list_empty(&p_profile_head->list_head)){
                act_printf("\033[35mTotal::%fs\033[0m\n", 
                           (float)lltime/1000000000);
            }else{
                act_printf("\n");
            }
        }else {
            act_printf("NO T\n");
        }
    }

    p_profile_head->child_time.tv_sec = 0;
    p_profile_head->child_time.tv_nsec = 0;

    for (p_list = p_profile_head->list_head.prev; p_list != &p_profile_head->list_head;){
        p_profile = mc_list_entry(p_list, act_profile_tree_t, list_node);
               
        act_memleak_profile_inline_travel_ok(disp_level, level + 1, p_profile, pid);
        p_profile_head->child_time.tv_sec += p_profile->end_time.tv_sec - p_profile->time.tv_sec + p_profile->child_time.tv_sec;
        p_profile_head->child_time.tv_nsec += p_profile->end_time.tv_nsec - p_profile->time.tv_nsec + p_profile->child_time.tv_nsec;
        
        p_list = p_list->prev;
    }

    if (level <= disp_level){
        if (!mc_is_list_empty(&p_profile_head->list_head)){
            act_printf("      ");
            for(i = 1; i <= level; i++){
                act_printf("  ");
            }
            
            if (p_func_node != NULL && p_line_node != NULL){
                act_printf("\033[1;34m%s", p_func_node->func_name);
                act_printf("(%s:%d)\033[0m ",g_file_table[p_line_node->file_index], p_line_node->line_index); 
            }else{
                act_printf("(0x%x) ",p_profile_head->self_pc);
            }
            
            if (p_profile_head->end_time.tv_nsec != 0){
                lltime = ((unsigned long long)p_profile_head->child_time.tv_sec + (unsigned long long)p_profile_head->end_time.tv_sec - (unsigned long long)p_profile_head->time.tv_sec)*1000000000 +
                    (unsigned long long)p_profile_head->child_time.tv_nsec + (unsigned long long)p_profile_head->end_time.tv_nsec - (unsigned long long)p_profile_head->time.tv_nsec;
                
                ftime = (float)lltime/1000000000;
                act_printf("\033[1;35mTotal::%fs\033[0m\n", 
                           ftime);
            }else {
               
                act_printf("NO T\n");
            }
        }
    }
}

act_void_t act_memleak_profile_inline_travel_ok2(act_int_t disp_level, act_int_t level, act_profile_tree_t *p_profile_head, act_int_t pid)
{
    act_profile_tree_t *p_profile = NULL;
    mc_list_head_t *p_list = NULL;
    act_int_t i = 1;

    act_memleak_line_ref_t *p_line_node = NULL;
    act_memleak_func_ref_t *p_func_node = NULL;
    
    act_memleak_avl_node_t **pp_func_root;
    act_memleak_avl_node_t **pp_file_line_root;
    act_memleak_ref_root_t *p_ref_root;


    if (level <= disp_level){ 
        act_green_printf("%-6u", p_profile_head->flow_count);
        for(i = 1; i <= level; i++){
            act_printf("  ");
        }
        
        
        p_ref_root = act_memleak_ref_root_find(pid);
        pp_func_root = &p_ref_root->p_func_ref_root;
        pp_file_line_root = &p_ref_root->p_file_line_ref_root;


        p_func_node = act_memleak_func_ref_find(*pp_func_root, p_profile_head->parent_pc);
               
        if (p_func_node != NULL){                                                                      
            p_line_node = act_memleak_line_ref_find(*pp_file_line_root, p_profile_head->parent_pc); 
            if (p_line_node != NULL){                                                                 
                act_yellow_printf("(%s:%d):",g_file_table[p_line_node->file_index], p_line_node->line_index); 
            }                                                                                         
        }else {
            act_yellow_printf("(0x%x) ",p_profile_head->self_pc);
        }
        
        p_func_node = act_memleak_func_ref_find(*pp_func_root, p_profile_head->self_pc);

        if (p_func_node != NULL){                                                                      
            act_lblue_printf("%s", p_func_node->func_name);                                                 
            p_line_node = act_memleak_line_ref_find(*pp_file_line_root, p_profile_head->self_pc); 
            if (p_line_node != NULL){                                                                 
                act_lblue_printf("(%s:%d) ",g_file_table[p_line_node->file_index], p_line_node->line_index); 
            }                                                                                         
        }else {
            act_blue_printf("(0x%x) ",p_profile_head->self_pc);
        }
        
        if (p_profile_head->end_time.tv_nsec != 0){
            act_zred_printf("Total::%fs\n", 
                       p_profile_head->stat_total_ftime);
            
            if (act_memleak_func_ref_find(*pp_func_root,p_profile_head->self_pc) != act_memleak_func_ref_find(*pp_func_root,p_profile_head->parent_pc)){
                act_memleak_profile_func_stat_add(pid, p_profile_head->self_pc, p_profile_head->total_time);
            }
        }else {
            act_zred_printf("NO T\n");
        }
    }

    for (p_list = p_profile_head->list_head.prev; p_list != &p_profile_head->list_head;){
        p_profile = mc_list_entry(p_list, act_profile_tree_t, list_node);
        act_memleak_profile_inline_travel_ok2(disp_level, level + 1, p_profile, pid);
        p_list = p_list->prev;
    }
}




act_void_t act_memleak_profile_inline_travel2(act_profile_tree_t *p_profile_head)
{
    act_profile_tree_t *p_profile = NULL;
    mc_list_head_t *p_list = NULL;
    unsigned long long lltime;


    p_profile_head->child_time.tv_sec = 0;
    p_profile_head->child_time.tv_nsec = 0;

    for (p_list = p_profile_head->list_head.prev; p_list != &p_profile_head->list_head;){
        p_profile = mc_list_entry(p_list, act_profile_tree_t, list_node);
               
        act_memleak_profile_inline_travel2(p_profile);
        p_profile_head->child_time.tv_sec += p_profile->end_time.tv_sec - p_profile->time.tv_sec + p_profile->child_time.tv_sec;
        p_profile_head->child_time.tv_nsec += p_profile->end_time.tv_nsec - p_profile->time.tv_nsec + p_profile->child_time.tv_nsec;
        
        p_list = p_list->prev;
    }
    
    p_profile_head->total_time.tv_sec =  p_profile_head->child_time.tv_sec + p_profile_head->end_time.tv_sec - p_profile_head->time.tv_sec;
    p_profile_head->total_time.tv_nsec =  p_profile_head->child_time.tv_nsec + p_profile_head->end_time.tv_nsec - p_profile_head->time.tv_nsec;
    
    lltime = ((unsigned long long)p_profile_head->child_time.tv_sec + (unsigned long long)p_profile_head->end_time.tv_sec - (unsigned long long)p_profile_head->time.tv_sec)*1000000000 +
        (unsigned long long)p_profile_head->child_time.tv_nsec + (unsigned long long)p_profile_head->end_time.tv_nsec - (unsigned long long)p_profile_head->time.tv_nsec;
    
    p_profile_head->stat_total_ftime = (float)lltime/1000000000;


}




act_profile_tree_t *act_memleak_profile_inline_travel(act_int_t cleanup, act_profile_tree_t *p_profile_head)
{
    act_profile_tree_t *p_profile = NULL;
    mc_list_head_t *p_list = NULL;
    static act_profile_tree_t *ps_last_profile = NULL;
    
    if (cleanup == 1){
        ps_last_profile = NULL;
    }

    if (ps_last_profile != NULL){
        ps_last_profile->end_time = p_profile_head->time;
    }

    if (ps_last_profile != NULL && ps_last_profile->last_show == 1){
        ps_last_profile->end_time = ps_last_profile->time;
    }
    
    ps_last_profile = p_profile_head;

    for (p_list = p_profile_head->list_head.prev; p_list != &p_profile_head->list_head;){
        p_profile = mc_list_entry(p_list, act_profile_tree_t, list_node);
        act_memleak_profile_inline_travel(0, p_profile);
        p_list = p_list->prev;
    }
    
    return ps_last_profile;
}



act_profile_tree_t *act_memleak_profile_inline_find(act_uint_t pc, act_profile_tree_t *p_profile_head, act_int_t pid)
{
    act_profile_tree_t *p_profile = NULL;
    mc_list_head_t *p_list = NULL;
    static act_profile_tree_t *ps_last_profile = NULL;
    
    act_memleak_func_ref_t *p_func_node = NULL;
    act_memleak_func_ref_t *p_func_node1 = NULL;
    
    act_memleak_avl_node_t **pp_func_root;
    act_memleak_avl_node_t **pp_file_line_root;
    act_memleak_ref_root_t *p_ref_root;


    p_ref_root = act_memleak_ref_root_find(pid);
    pp_func_root = &p_ref_root->p_func_ref_root;
    pp_file_line_root = &p_ref_root->p_file_line_ref_root;

    p_func_node = act_memleak_func_ref_find(*pp_func_root, p_profile_head->self_pc); 
    p_func_node1 = act_memleak_func_ref_find(*pp_func_root, pc); 
    
    if (p_profile_head->self_pc == pc || p_func_node == p_func_node1){
        ps_last_profile = p_profile_head;
    }
    
    for (p_list = p_profile_head->list_head.prev; p_list != &p_profile_head->list_head;){
        p_profile = mc_list_entry(p_list, act_profile_tree_t, list_node);
        act_memleak_profile_inline_find(pc, p_profile, pid);
        p_list = p_list->prev;
    }

    return ps_last_profile;
}



