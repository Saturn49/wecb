#include "act_common.h"
#include "act_mem_leak.h"

act_memleak_avl_node_t* insertNode(int key, void *p_value, act_memleak_avl_node_t* root);
act_memleak_avl_node_t *findNode(int key, act_memleak_avl_node_t* root);
act_memleak_avl_node_t *efindNode(int key, act_memleak_avl_node_t* root);
void destroyAVL(act_memleak_avl_node_t* root);
void traverseAVL1(act_memleak_avl_node_t* root);
int searchNode(int key, act_memleak_avl_node_t* root, act_memleak_avl_node_t** parent);

#define MAX_FILE_COUNT 4096
act_char_t g_file_table[MAX_FILE_COUNT][256];
act_int_t g_file_max_index = 0;



act_rv_t act_memleak_func_ref_add(act_memleak_avl_node_t **pp_func_root, act_void_t *p_addr, act_char_t *p_func_name)
{
    act_memleak_avl_node_t *p_node = NULL;
    act_memleak_avl_node_t *parent = NULL;
    act_memleak_func_ref_t *p_func_node = NULL;

    if(searchNode((act_uint_t )p_addr, *pp_func_root, &parent) == 1){
        return ACT_RV_ERR;
    }
   
    ACT_MALLOC(p_func_node,act_memleak_func_ref_t);
    sprintf(p_func_node->func_name, "%s", p_func_name);
    
    if (*pp_func_root == NULL){
        ACT_MALLOC(p_node, act_memleak_avl_node_t );
        p_node->parent = NULL;
        p_node->left = NULL;
        p_node->right = NULL;
        p_node->key = (act_uint_t)p_addr;
        p_node->balance = 0;
        p_node->p_value = p_func_node;
        *pp_func_root = p_node;
    }else {
        *pp_func_root = insertNode((act_uint_t)p_addr, p_func_node, *pp_func_root);
    }
    return ACT_RV_SUC;
}

act_memleak_func_ref_t *act_memleak_func_ref_find(act_memleak_avl_node_t *p_func_root, act_uint_t addr)
{
    act_memleak_avl_node_t *p_node = NULL;

    if (p_func_root == NULL){
        return NULL;
    }
    
    p_node = findNode(addr,p_func_root);
    if (p_node != NULL){
        return p_node->p_value;
    }
    return NULL;

}

act_rv_t act_memleak_func_frame_ref_add(act_memleak_avl_node_t *p_func_root, act_uint_t addr, act_uint_t frame_size, act_uint_t frame_fp_off, act_uint_t frame_ra_off)
{
    act_memleak_avl_node_t *p_node = NULL;
    act_memleak_func_ref_t *p_func_ref = NULL;
    
    ACT_VASSERT (p_func_root != NULL);
    
    p_node = findNode(addr,p_func_root);
    if (p_node != NULL){
        p_func_ref = (act_memleak_func_ref_t *)p_node->p_value;
    }
    
    ACT_VASSERT (p_func_ref != NULL);
    
    p_func_ref->frame_ref.frame_size = frame_size;
    p_func_ref->frame_ref.frame_fp_offset = frame_fp_off;
    p_func_ref->frame_ref.frame_ra_offset = frame_ra_off;
    
    return ACT_RV_SUC;
}

act_memleak_frame_ref_t *act_memleak_func_frame_ref_find(act_uint_t pid, act_uint_t addr)
{
    act_memleak_func_ref_t *p_func_ref = NULL;
    act_memleak_avl_node_t **pp_func_root;
    act_memleak_ref_root_t *p_ref_root;
    
    p_ref_root = act_memleak_ref_root_find(pid);
    pp_func_root = &p_ref_root->p_func_ref_root;
    
    p_func_ref = (act_memleak_func_ref_t *)act_memleak_func_ref_find(*pp_func_root, addr);
    
    ACT_VASSERT_RV(p_func_ref != NULL, NULL);
    
    return &(p_func_ref->frame_ref);
}



act_rv_t act_memleak_func_frame_ra_find(act_uint_t pid, 
                                        act_uint_t *p_stack_frame, 
                                        act_uint_t *p_stack_frame_end, 
                                        act_uint_t stack_base, 
                                        act_uint_t cur_func_addr, 
                                        act_uint_t *p_save_ra,
                                        act_uint_t *p_save_end_ra)
{
    act_uint_t ra;
    act_uint_t fp;
    act_memleak_frame_ref_t *p_frame_ref = NULL;
    act_uint_t *p_frame = NULL;
    
    ACT_VASSERT(p_stack_frame < p_stack_frame_end);

    p_frame_ref = act_memleak_func_frame_ref_find(pid, cur_func_addr);
    
    ACT_VASSERT(p_frame_ref != NULL);
    
    if (p_stack_frame + p_frame_ref->frame_ra_offset <= p_stack_frame_end){
        ra = *(p_stack_frame + p_frame_ref->frame_ra_offset);
    }else {
        return ACT_RV_ERR;
    }
    
    if (p_stack_frame + p_frame_ref->frame_fp_offset <= p_stack_frame_end){
        fp = *(p_stack_frame + p_frame_ref->frame_fp_offset);
    }else{
        *p_save_ra++ = ra;
        return ACT_RV_ERR;
    }
    
    *p_save_ra++ = ra;
    
    if (fp != stack_base + p_frame_ref->frame_size){
        fp = stack_base + p_frame_ref->frame_size;
    }


    p_frame = (act_uint_t *)((act_uchar_t *)p_stack_frame + fp - stack_base);
    
    ACT_VASSERT(p_frame > p_stack_frame && p_frame < p_stack_frame_end);
    
    if (p_save_ra > p_save_end_ra){
        printf("ACT_REUTRN\n");
        return ACT_RV_SUC;
    }

    act_memleak_func_frame_ra_find(pid, p_frame, p_stack_frame_end, fp, ra, p_save_ra, p_save_end_ra);
    return ACT_RV_SUC;
}


act_rv_t act_memleak_func_ra_display(act_uint_t pid, act_uint_t *p_stack_frame)
{
    act_memleak_line_ref_t *p_line_node = NULL;
    act_memleak_func_ref_t *p_func_node = NULL;
    act_uint_t ra_stack[24] = {0};
    act_int_t i = 0;
    act_uint_t ra = 0;
    act_uint_t fp = 0;
    
    act_memleak_avl_node_t **pp_func_root;
    act_memleak_avl_node_t **pp_file_line_root;
    act_memleak_ref_root_t *p_ref_root;


    p_ref_root = act_memleak_ref_root_find(pid);
    pp_func_root = &p_ref_root->p_func_ref_root;
    pp_file_line_root = &p_ref_root->p_file_line_ref_root;
    
    fp = p_stack_frame[1];
    ra = p_stack_frame[2];

    ra_stack[0] = p_stack_frame[0];
    ra_stack[1] = ra;

    act_memleak_func_frame_ra_find(pid, &p_stack_frame[3], &p_stack_frame[ACT_MEMLEAK_MAX_STACK -1], fp, ra, &ra_stack[2], &ra_stack[23]);

    for(i = 0; i < 24 ; i++){
        if (ra_stack[i] == 0){
            continue;
        }
        
        p_func_node = NULL;
        p_line_node = NULL;

        act_printf("\t%d:0x%x ", i, ra_stack[i]);
        p_func_node = act_memleak_func_ref_find(*pp_func_root, ra_stack[i] - 4);          
        if (p_func_node != NULL){                                                                      
            act_printf("%s", p_func_node->func_name);                                                 
            p_line_node = act_memleak_line_ref_find(*pp_file_line_root, ra_stack[i] - 4); 
            if (p_line_node != NULL){                                                                 
                act_printf("(%s:%d)",g_file_table[p_line_node->file_index], p_line_node->line_index); 
            }                                                                                         
        }
        act_printf("\n");                                                                             
    }
    
    return ACT_RV_SUC;

}

int g_ra_notrans = 1;
act_rv_t act_memleak_func_ra_translate(act_uint_t pid, act_uint_t *p_stack_frame, act_uint_t *p_ra_stack)
{
    act_uint_t ra_stack[24] = {0};
    act_uint_t ra = 0;
    act_uint_t fp = 0;

    if (g_ra_notrans == 1){
       memcpy(p_ra_stack, p_stack_frame, sizeof(ra_stack));
       return;
    }
    
    fp = p_stack_frame[1];
    ra = p_stack_frame[2];

    ra_stack[0] = p_stack_frame[0];
    ra_stack[1] = ra;

    act_memleak_func_frame_ra_find(pid, &p_stack_frame[3], &p_stack_frame[ACT_MEMLEAK_MAX_STACK -1], fp, ra, &ra_stack[2], &ra_stack[23]);
    
    memcpy(p_ra_stack, ra_stack, sizeof(ra_stack));
    
    return ACT_RV_SUC;
}




act_rv_t act_memleak_func_ref_cleanup(act_memleak_avl_node_t **p_func_root)
{
    destroyAVL(*p_func_root);
    *p_func_root = NULL;
    return ACT_RV_SUC;
}



act_rv_t act_memleak_line_ref_add(act_memleak_avl_node_t **pp_line_root, act_uint_t addr, int file_index, int line_index)
{
    act_memleak_avl_node_t *p_node = NULL;
    act_memleak_avl_node_t *parent = NULL;
    act_memleak_line_ref_t *p_line_node = NULL;

    ACT_VASSERT(searchNode(addr, *pp_line_root, &parent) == 0);
    
    ACT_MALLOC(p_line_node,act_memleak_line_ref_t);
    p_line_node->file_index = file_index;
    p_line_node->line_index = line_index;
    
    if (*pp_line_root == NULL){
        ACT_MALLOC(p_node, act_memleak_avl_node_t );
        p_node->parent = NULL;
        p_node->left = NULL;
        p_node->right = NULL;
        p_node->key = addr;
        p_node->balance = 0;
        p_node->p_value = p_line_node;
        *pp_line_root = p_node;
    }else {
        *pp_line_root = insertNode(addr, p_line_node, *pp_line_root);
    }
    return ACT_RV_SUC;
}

act_memleak_line_ref_t *act_memleak_line_ref_find(act_memleak_avl_node_t *p_line_root, act_uint_t addr)
{
    act_memleak_avl_node_t *p_node = NULL;
    if (p_line_root == NULL){
        return NULL;
    }
    
    p_node = findNode(addr,p_line_root);
    if (p_node != NULL){
        return p_node->p_value;
    }
    return NULL;

}
act_rv_t act_memleak_line_ref_cleanup(act_memleak_avl_node_t **p_line_root)
{
    destroyAVL(*p_line_root);
    *p_line_root = NULL;
    return ACT_RV_SUC;
}

