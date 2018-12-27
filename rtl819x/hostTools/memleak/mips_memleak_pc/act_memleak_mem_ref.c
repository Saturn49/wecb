#include "act_mem_leak.h"
#include "mc_socket.h"
#include "mc_hash.h"

act_memleak_avl_node_t* insertNode(int key, void *p_value, act_memleak_avl_node_t* root);
act_memleak_avl_node_t *findNode(int key, act_memleak_avl_node_t* root);
act_memleak_avl_node_t *efindNode(int key, act_memleak_avl_node_t* root);
void destroyAVL(act_memleak_avl_node_t* root);
void traverseAVL1(act_memleak_avl_node_t* root);
int searchNode(int key, act_memleak_avl_node_t* root, act_memleak_avl_node_t** parent);
act_memleak_avl_node_t* deleteNode(int key, act_memleak_avl_node_t* root);


static int gb_double_free_key = 1329;
static int gb_overflow_free_key = 7329;
extern int g_bind_pid;

act_rv_t act_memleak_mem_ref_add(act_memleak_avl_node_t **pp_mem_root, act_void_t *p_addr, act_uint_t size, act_uint_t *p_stack_index, act_uint_t pid)
{
    act_memleak_avl_node_t *p_node = NULL;
    act_memleak_mem_ref_t *p_mem_node = NULL;
    act_memleak_mem_ref_t *p_overmem_node = NULL;

    printf("ALLOC MEMORY %d 0x%x \n", pid, p_addr);
    p_node = (act_memleak_avl_node_t *)efindNode((act_uint_t)p_addr,*pp_mem_root);

    if (p_node != NULL){
        p_mem_node = p_node->p_value;
      
        p_mem_node->pid = pid;
        p_mem_node->p_mem = p_addr;
        p_mem_node->mem_size = size;
        p_mem_node->status = ACT_MEMLEAK_MEM_ST_ALLOC;
        time(&p_mem_node->time);

        memcpy(p_mem_node->stack_index, p_stack_index, sizeof(p_mem_node->stack_index));
        
        act_memleak_mem_statics_ref_add(p_mem_node);
        
        return ACT_RV_SUC;
    }
    
    ACT_MALLOC(p_mem_node,act_memleak_mem_ref_t);
    p_mem_node->pid = pid;
    p_mem_node->p_mem = p_addr;
    p_mem_node->mem_size = size;
    p_mem_node->status = ACT_MEMLEAK_MEM_ST_ALLOC;
    time(&p_mem_node->time);
    memcpy(p_mem_node->stack_index, p_stack_index, sizeof(p_mem_node->stack_index));
    
    if (*pp_mem_root == NULL){
        ACT_MALLOC(p_node, act_memleak_avl_node_t );
        p_node->parent = NULL;
        p_node->left = NULL;
        p_node->right = NULL;
        p_node->key = (act_uint_t)p_addr;
        p_node->balance = 0;
        p_node->p_value = p_mem_node;
        *pp_mem_root = p_node;
    }else {
        *pp_mem_root = insertNode((act_uint_t)p_addr, p_mem_node, *pp_mem_root);
    }

    act_memleak_mem_statics_ref_add(p_mem_node);
    
    return ACT_RV_SUC;
}



act_rv_t act_memleak_mem_ref_del(act_memleak_avl_node_t **pp_mem_root, act_uint_t addr, act_uint_t *p_stack_index, act_uint_t pid)
{
    act_memleak_avl_node_t *p_node = NULL;
    act_memleak_mem_ref_t *p_mem_node = NULL;
    
    ACT_VASSERT(*pp_mem_root != NULL);

    p_node = efindNode(addr,*pp_mem_root);
    
    if (p_node == NULL){
    db_free:       /*mis freed including double freed*/
        ACT_MALLOC(p_mem_node,act_memleak_mem_ref_t);
        p_mem_node->pid = pid;
        p_mem_node->p_mem = (act_void_t *)addr;
        p_mem_node->status = ACT_MEMLEAK_MEM_ST_DFREE;
        memcpy(p_mem_node->stack_index, p_stack_index, sizeof(p_mem_node->stack_index));
        
        if (*pp_mem_root == NULL){
            ACT_MALLOC(p_node, act_memleak_avl_node_t );
            p_node->parent = NULL;
            p_node->left = NULL;
            p_node->right = NULL;
            p_node->key = (act_uint_t)addr;
            p_node->balance = 0;
            p_node->p_value = p_mem_node;
            *pp_mem_root = p_node;
        }else {
            *pp_mem_root = insertNode((act_uint_t)(gb_double_free_key++), p_mem_node, *pp_mem_root);
        }
        printf("DOUBLE FREE MEMORY %d 0x%x\n", pid, addr);
        return ACT_RV_ERR;
    }

    ACT_VASSERT(p_node != NULL && p_node->p_value != NULL);
    
    p_mem_node = p_node->p_value;
   
#if 0 
    if (p_mem_node->pid != pid){
        printf("PID NOT CORRECT %d %d\n", p_mem_node->pid, pid);
        return ACT_RV_ERR;
    }
#endif
    
    if (p_mem_node->status == ACT_MEMLEAK_MEM_ST_DFREE){
        return ACT_RV_ERR;
    }else if(p_mem_node->status &ACT_MEMLEAK_MEM_ST_FREED){
        printf("DB FREE MEMORY %d 0x%x\n", pid, addr);
        goto db_free;
        /**/
    }else if (p_mem_node->status & ACT_MEMLEAK_MEM_ST_ALLOC){
        p_mem_node->status &= ~ACT_MEMLEAK_MEM_ST_ALLOC;
        p_mem_node->status |= ACT_MEMLEAK_MEM_ST_FREED;
        
    }

    act_memleak_mem_statics_ref_del(p_mem_node);

    printf("FREE MEMORY %d 0x%x\n", pid, addr);
    
    return ACT_RV_SUC;
}


act_rv_t act_memleak_mem_ref_mod(act_memleak_avl_node_t **pp_mem_root, act_uint_t addr, act_char_t status)
{
    act_memleak_avl_node_t *p_node = NULL;
    act_memleak_mem_ref_t *p_mem_node = NULL;
    
    ACT_VASSERT(*pp_mem_root != NULL);
    act_printf("%d\n", __LINE__);
    p_node = efindNode(addr,*pp_mem_root);
    act_printf("%d\n", __LINE__);
    ACT_VASSERT(p_node != NULL && p_node->p_value != NULL);
    act_printf("%d\n", __LINE__);
    p_mem_node = p_node->p_value;
    
    p_mem_node->status |= status;

    if (status & ACT_MEMLEAK_MEM_ST_OFLOW){
            p_mem_node->p_mem_statics_ref->status = ACT_MEMLEAK_MEM_ST_OFLOW;
            act_printf("OVERFLOW MEMORY\n");
    }else if (status & ACT_MEMLEAK_MEM_ST_UOFLOW){
            p_mem_node->p_mem_statics_ref->status = ACT_MEMLEAK_MEM_ST_UOFLOW;
            act_printf("UNDERFLOW MEMORY\n");
    }


    return ACT_RV_SUC;
}

act_rv_t act_memleak_mem_ref_cleanup(act_memleak_avl_node_t **pp_mem_root)
{
    printf("CLEANUP MEMORY\n");
    destroyAVL(*pp_mem_root);
    *pp_mem_root = NULL;
    return ACT_RV_SUC;
}

static mc_hash_t *gp_act_memleak_mem_ref_root_hash = NULL;


act_memleak_avl_node_t **act_memleak_mem_ref_root_find(act_uint_t pid)
{
    act_memleak_avl_node_t **pp_mem_root = NULL;
    act_char_t key[32] = "\0";
    
    sprintf(key, "%d", pid);
    
    if (gp_act_memleak_mem_ref_root_hash == NULL){
        gp_act_memleak_mem_ref_root_hash = mc_hash_create();
    }
    
    ACT_VASSERT_RV(gp_act_memleak_mem_ref_root_hash != NULL, NULL);
    
    pp_mem_root = (act_memleak_avl_node_t **)mc_hash_find(key, gp_act_memleak_mem_ref_root_hash);
    
    if (pp_mem_root == NULL){
        ACT_MALLOC_RV(pp_mem_root, act_memleak_avl_node_t *, NULL);
        *pp_mem_root = NULL;
        mc_hash_add(key, pp_mem_root, gp_act_memleak_mem_ref_root_hash);
    }
    
    return pp_mem_root;
    
}

act_void_t act_memleak_mem_ref_root_cleanup()
{
    mc_hash_value_func_destroy(gp_act_memleak_mem_ref_root_hash, 
                               (void *)act_memleak_mem_ref_cleanup);
    gp_act_memleak_mem_ref_root_hash = NULL;
    
}


static mc_hash_t *gp_act_memleak_mem_statics_ref_root_hash = NULL;


act_rv_t act_memleak_mem_statics_ref_add(act_memleak_mem_ref_t *p_mem_node)
{
    act_char_t key[256] = "\0";
    act_memleak_mem_statics_ref_t *p_mem_statics_ref;
    act_uint_t i = 0;
    act_char_t tmp[16] = "\0";
    
 
    
    if (gp_act_memleak_mem_statics_ref_root_hash == NULL){
        gp_act_memleak_mem_statics_ref_root_hash = mc_hash_create();
    }
    ACT_VASSERT(gp_act_memleak_mem_statics_ref_root_hash != NULL);
    
    
    act_memleak_func_ra_translate(p_mem_node->pid, p_mem_node->stack_index, p_mem_node->stack_ra);
    
    sprintf(key, "%d ", p_mem_node->pid);
    for (i = 0; i < 24; i++){
        if (p_mem_node->stack_ra[i] != 0){
            sprintf(tmp, "%x ",p_mem_node->stack_ra[i]);
            strcat(key, tmp);
        }
    }
    
    printf("key:%s\n", key);
    p_mem_statics_ref = (act_memleak_mem_statics_ref_t *)mc_hash_find(key, gp_act_memleak_mem_statics_ref_root_hash);
    
    if (p_mem_statics_ref == NULL){
        ACT_MALLOC(p_mem_statics_ref, act_memleak_mem_statics_ref_t);
        memcpy(p_mem_statics_ref->stack_ra, p_mem_node->stack_ra, sizeof(p_mem_statics_ref->stack_ra));
        mc_hash_add(key, p_mem_statics_ref, gp_act_memleak_mem_statics_ref_root_hash);
    }
    
    p_mem_statics_ref->pid = p_mem_node->pid;
    p_mem_statics_ref->mem_total_alloc += p_mem_node->mem_size;

    p_mem_node->p_mem_statics_ref = p_mem_statics_ref;

    return ACT_RV_SUC;
}

act_rv_t act_memleak_mem_statics_ref_del(act_memleak_mem_ref_t *p_mem_node)
{
    ACT_VASSERT(p_mem_node != NULL);
    ACT_VASSERT(p_mem_node->p_mem_statics_ref != NULL);
    
    p_mem_node->p_mem_statics_ref->mem_total_free += p_mem_node->mem_size;
    
    return ACT_RV_SUC;
}

act_rv_t act_memleak_mem_statics_ref_show(act_memleak_mem_ref_t *p_mem_node)
{
    
    
    ACT_VASSERT(p_mem_node != NULL);
    ACT_VASSERT(p_mem_node->p_mem_statics_ref != NULL);
    
    act_printf("ref_statics_total_alloc %u\n", p_mem_node->p_mem_statics_ref->mem_total_alloc);
    act_printf("ref_statics_total_free %u\n", p_mem_node->p_mem_statics_ref->mem_total_free);
    
    return ACT_RV_SUC;
}


extern act_char_t g_file_table[512][256];

static int g_stat_ind = 1;

unsigned long long g_cpe_memory_total_alloc = 0;
unsigned long long g_cpe_memory_total_free = 0;

int g_statics_display_unfree = 0;

act_void_t act_memleak_mem_statics_ref_callback(act_memleak_mem_statics_ref_t *p_mem_statics_ref)
{
    act_int_t i;
    act_memleak_line_ref_t *p_line_node = NULL;
    act_memleak_func_ref_t *p_func_node = NULL;
    
    act_memleak_avl_node_t **pp_func_root;
    act_memleak_avl_node_t **pp_file_line_root;
    act_memleak_ref_root_t *p_ref_root;

    ACT_VASSERT_RV(p_mem_statics_ref != NULL, );
    ACT_VASSERT_RV(g_bind_pid == 0 || g_bind_pid == p_mem_statics_ref->pid, );
    
    if ( (p_mem_statics_ref->mem_total_alloc -p_mem_statics_ref->mem_total_free) == 0 && g_statics_display_unfree){
            return;
    }

    p_ref_root = act_memleak_ref_root_find(p_mem_statics_ref->pid);
    pp_func_root = &p_ref_root->p_func_ref_root;
    pp_file_line_root = &p_ref_root->p_file_line_ref_root;

    act_printf("\nmemory statics ref:%d pid:%d app_name:%s\n", g_stat_ind, p_mem_statics_ref->pid, act_memleak_find_pid(p_mem_statics_ref->pid));
    g_stat_ind++;

    for(i = 0; i < 24 ; i++){
        if (p_mem_statics_ref->stack_ra[i] == 0){
            continue;
        }

        p_func_node = NULL;
        p_line_node = NULL;

        act_printf("\t%d:0x%x ", i, p_mem_statics_ref->stack_ra[i]);
        p_func_node = act_memleak_func_ref_find(*pp_func_root, p_mem_statics_ref->stack_ra[i] - 4);          
        if (p_func_node != NULL){                                                                      
            act_printf("%s", p_func_node->func_name);                                                 
            p_line_node = act_memleak_line_ref_find(*pp_file_line_root, p_mem_statics_ref->stack_ra[i] - 4); 
            if (p_line_node != NULL){                                                                 
                act_printf("(%s:%d)",g_file_table[p_line_node->file_index], p_line_node->line_index); 
            }                                                                                         
        }
        act_printf("\n");                                                                             
    }
    
    g_cpe_memory_total_alloc += p_mem_statics_ref->mem_total_alloc;
    g_cpe_memory_total_free += p_mem_statics_ref->mem_total_free;
    
    act_printf("\tref_statics_total_alloc %u\n", p_mem_statics_ref->mem_total_alloc);
    act_printf("\tref_statics_total_free %u\n", p_mem_statics_ref->mem_total_free);
    act_printf("end mem statics:\n");
}

act_void_t act_memleak_mem_statics_ref_all_display()
{
    g_stat_ind = 1;
    g_cpe_memory_total_alloc = 0;
    g_cpe_memory_total_free = 0;

    act_printf("MEM_STATICS_REF_DISPLAY\n");
    mc_hash_func_display(gp_act_memleak_mem_statics_ref_root_hash, (void *)act_memleak_mem_statics_ref_callback);

    act_printf("CPE_MEMORY_TOTAL_ALLOC:%llu bytes %lluK \n", g_cpe_memory_total_alloc, g_cpe_memory_total_alloc/1024);
    act_printf("CPE_MEMORY_TOTAL_FREE:%llu bytes %lluK \n", g_cpe_memory_total_free, g_cpe_memory_total_free/1024);

    act_printf("END_MEM_STATICS_REF_DISPLAY\n");
}


act_void_t act_memleak_mem_statics_ref_overflow_callback(act_memleak_mem_statics_ref_t *p_mem_statics_ref)
{
    act_int_t i;
    act_memleak_line_ref_t *p_line_node = NULL;
    act_memleak_func_ref_t *p_func_node = NULL;
    
    act_memleak_avl_node_t **pp_func_root;
    act_memleak_avl_node_t **pp_file_line_root;
    act_memleak_ref_root_t *p_ref_root;

    ACT_VASSERT_RV(p_mem_statics_ref != NULL, );
    ACT_VASSERT_RV(g_bind_pid == 0 || g_bind_pid == p_mem_statics_ref->pid, );

    ACT_VASSERT_RV((p_mem_statics_ref->status == ACT_MEMLEAK_MEM_ST_OFLOW) ||
                   (p_mem_statics_ref->status == ACT_MEMLEAK_MEM_ST_UOFLOW), );
        
    
    p_ref_root = act_memleak_ref_root_find(p_mem_statics_ref->pid);
    pp_func_root = &p_ref_root->p_func_ref_root;
    pp_file_line_root = &p_ref_root->p_file_line_ref_root;
    

    act_printf("\nmemory %s statics ref:%d pid:%d app_name:%s\n", 
               p_mem_statics_ref->status == ACT_MEMLEAK_MEM_ST_OFLOW?"overflow":"underflow",
               g_stat_ind, p_mem_statics_ref->pid, act_memleak_find_pid(p_mem_statics_ref->pid));
    g_stat_ind++;

    for(i = 0; i < 24 ; i++){
        if (p_mem_statics_ref->stack_ra[i] == 0){
            continue;
        }

        p_func_node = NULL;
        p_line_node = NULL;

        act_printf("\t%d:0x%x ", i, p_mem_statics_ref->stack_ra[i]);
        p_func_node = act_memleak_func_ref_find(*pp_func_root, p_mem_statics_ref->stack_ra[i] - 4);          
        if (p_func_node != NULL){                                                                      
            act_printf("%s", p_func_node->func_name);                                                 
            p_line_node = act_memleak_line_ref_find(*pp_file_line_root, p_mem_statics_ref->stack_ra[i] - 4); 
            if (p_line_node != NULL){                                                                 
                act_printf("(%s:%d)",g_file_table[p_line_node->file_index], p_line_node->line_index); 
            }                                                                                         
        }
        act_printf("\n");                                                                             
    }
    
    act_printf("end overflow mem statics:\n");
}

act_void_t act_memleak_mem_statics_ref_all_overflow_display()
{
    g_stat_ind = 1;
    g_cpe_memory_total_alloc = 0;
    g_cpe_memory_total_free = 0;

    act_printf("MEM_STATICS_REF_OVERFLOW_DISPLAY\n");
    mc_hash_func_display(gp_act_memleak_mem_statics_ref_root_hash, (void *)act_memleak_mem_statics_ref_overflow_callback);


    act_printf("END_MEM_STATICS_REF_OVERFLOW_DISPLAY\n");
}



act_void_t act_memleak_mem_statics_ref_cleanup()
{
   
    mc_hash_value_destroy(gp_act_memleak_mem_statics_ref_root_hash);
    gp_act_memleak_mem_statics_ref_root_hash = NULL;
}





static mc_hash_t *gp_act_memleak_ref_root_hash = NULL;

act_memleak_ref_root_t *act_memleak_ref_root_find(act_uint_t pid)
{
    act_memleak_ref_root_t *p_ref_root = NULL;
    act_char_t key[32] = "\0";
    
    sprintf(key, "%d", pid);
    
    if (gp_act_memleak_ref_root_hash == NULL){
        gp_act_memleak_ref_root_hash = mc_hash_create();
    }
    
    ACT_VASSERT_RV(gp_act_memleak_ref_root_hash != NULL, NULL);
    
    p_ref_root = (act_memleak_ref_root_t *)mc_hash_find(key, gp_act_memleak_ref_root_hash);
    
    if (p_ref_root == NULL){
        ACT_MALLOC_RV(p_ref_root, act_memleak_ref_root_t, NULL);
        mc_hash_add(key, p_ref_root, gp_act_memleak_ref_root_hash);
    }
    
    return p_ref_root;
}

act_rv_t act_memleak_ref_cleanup_callback(act_memleak_ref_root_t *p_ref_root)
{
    printf("CLEANUP MEMORY\n");
    destroyAVL(p_ref_root->p_mem_ref_root);
    destroyAVL(p_ref_root->p_func_ref_root);
    destroyAVL(p_ref_root->p_file_line_ref_root);

    p_ref_root->p_mem_ref_root = NULL;
    p_ref_root->p_func_ref_root = NULL;
    p_ref_root->p_file_line_ref_root = NULL;

    return ACT_RV_SUC;
}


act_void_t act_memleak_ref_root_cleanup()
{
    mc_hash_value_func_destroy(gp_act_memleak_ref_root_hash, 
                               (void *)act_memleak_ref_cleanup_callback);
    gp_act_memleak_ref_root_hash = NULL;
    
}
