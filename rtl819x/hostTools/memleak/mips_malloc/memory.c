#include <signal.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <time.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include "malloc.h"

#define ACT_MEMLEAK_MAX_STACK    30
#define ACT_MEM_LEAK_MEM_TAG_B   121
#define ACT_MEM_LEAK_MEM_TAG_E   99

#define ACT_MEMLEAK_MQ_OP_ADD    1
#define ACT_MEMLEAK_MQ_OP_DEL    2
#define ACT_MEMLEAK_MQ_OP_OVL    4
#define ACT_MEMLEAK_MQ_OP_CLR    8
#define ACT_MEMLEAK_MQ_OP_UOVL   16

#define ACT_MEMLEAK_MQ_OP_SEGV  126

#define ACT_MEMLEAK_MQ_OP_PROFILE_ADD  80
#define ACT_MEMLEAK_MQ_OP_PROFILE_DISP 81
#define ACT_MEMLEAK_MQ_OP_PROFILE_END  82


#define ACT_MEMLEAK_MQ_OP_MAPS  111

#define MQ_SERVER_IP "192.168.1.80"
#define MQ_SERVER_COMMAND_PORT 4000
#define MQ_SERVER_MAPS_PORT    4001
#define MQ_SERVER_MEM_PORT     4002
#define MQ_SERVER_PROFILE_PORT 4003

#define MALLOC(p_ptr, size) p_ptr = dlmalloc(size)
#define CALLOC(p_ptr, size, nb) p_ptr = dlcalloc(size, nb)
#define REALLOC(p_ptr, size) p_ptr = dlrealloc(p_ptr, size)
#define FREE(p_ptr) dlfree(p_ptr)

#define MAPS_STACK 0
#define MAPS_LIB_CMS_CORE 1
#define MAPS_LIB_CMS_UTIL 2
#define MAPS_LIB_CMS_DAL 3
#define MAPS_LIB_CMS_MSG 4
#define MAPS_MAIN        5
#define MAPS_MAX         14

typedef struct act_memleak_maps_s{
    unsigned int add1;
    unsigned int add2;
    unsigned int add3;
    unsigned int add4;
    char name[32];
}act_memleak_maps_t;

typedef struct act_profile_mq_s{
    unsigned int caller_pc;
    unsigned int self_pc;
    struct timespec time;
}act_profile_mq_t;


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

void mcount(unsigned int pc1, unsigned int pc2);
void profile_end();

static inline void segv_handle_init();
void init_maps();


unsigned int gstack_addr_max = 0;
unsigned int gstack_addr_min = 0;

static unsigned int g_stack_max = 0;

static int g_maps_sock = -1;
static int g_memref_sock = -1;
static int g_profile_sock = -1;




static inline int mc_socket_tcp_open(char *ip,
                                     int  port)
{
    struct sockaddr_in 	address;
    int 		handle = -1;
    
    signal(13, SIG_IGN);
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons((unsigned short)port);
    
    
    handle = socket(AF_INET, SOCK_STREAM, 0);
    
    if( connect(handle, (struct sockaddr *)&address, sizeof(address)) < 0) {
        close(handle);
        return -1;
    }else {
        return handle;
    }
}

static inline int mc_socket_tcp_close(int  socket_id)
{
    return close(socket_id);
}


static inline int mc_socket_tcp_write(int sock_id, 
                                      char *buf, 
                                      int buf_size,
                                      int flag)
{
    int 	size = 0;
    int 	res  = 0;
    
    
    if (sock_id < 0)
        return -1;
    if (buf == NULL)
        return -1;
    if (buf_size < 0)
        return -1;
	
    while(size < buf_size){
        res = send(sock_id, (char *)buf + size, buf_size - size, 0);
        if (res <= 0){
            return -1;
        }
        size += res;
        if (!flag){
            return size;
        }
    }
    
    return size;
    
}



static inline void memleak_tcp_write(int port, char *buf, int buf_size)
{
    int fd = -1;
    
    switch(port){
    case MQ_SERVER_MAPS_PORT:
        if (g_maps_sock < 0){
            g_maps_sock = mc_socket_tcp_open(MQ_SERVER_IP, port);
        }
        fd = g_maps_sock;
        break;
    case MQ_SERVER_MEM_PORT:    
        if (g_memref_sock < 0){
            g_memref_sock = mc_socket_tcp_open(MQ_SERVER_IP, port);
        }
        fd = g_memref_sock;
        break;
    case MQ_SERVER_PROFILE_PORT:
        if (g_profile_sock < 0){
            g_profile_sock = mc_socket_tcp_open(MQ_SERVER_IP, port);
        }
        fd = g_profile_sock;
        break;
    default:
        break;
    }
    
    if (fd > 0){
        if (mc_socket_tcp_write(fd, buf, buf_size, 1) < 0){
            //printf("TCP WRITE FAILED !!!!!!!!!!\n");
            close(fd);
        }
    }else {
        //printf("TCP OPEN FAILED !!!!!!!!!!\n");
    }
}



void maps()
{
    FILE *fp = NULL;
    int len;
    char buf[8192] = "\0";
    char file[64] = "\0";
    char *ptr = NULL;
    char *line = NULL;
    
    unsigned int add1;
    unsigned int add2;
    char attr[16];
    unsigned int offset;
    char time[16];
    unsigned int inode;
    char name[128];
    char program_name[128] = "\0";
    act_memleak_mq_maps_t mq_ref;
    act_memleak_mq_maps_t *p_mq_ref1 = &mq_ref;

    
    if (g_stack_max != 0){
        return;
    }
    
    g_stack_max = 1;
    segv_handle_init();
    
    memset(p_mq_ref1, 0, sizeof(act_memleak_mq_maps_t));
    
    p_mq_ref1->pid =  getpid();
    
    sprintf(file, "/proc/%d/status", p_mq_ref1->pid);
    
    fp = fopen(file, "rb"); 
    if(fp == NULL){
        return ;
    }

    fread(buf,1, 8192, fp);

    fclose(fp);

    fp = NULL;

    ptr = buf;
    
    line = strsep(&ptr, "\r\n");

    sscanf(line, "%s\t%s", name, program_name);
 
    sprintf(file, "/proc/%d/maps", p_mq_ref1->pid);
    
    fp = fopen(file, "rb"); 
    if(fp == NULL){
        return ;
    }
    
    fread(buf,1, 8192, fp);
    fclose(fp);
    
    len = strlen(buf);
    ptr = buf;
    
    while(ptr != NULL && ptr < buf + len){
        memset(name, 0, sizeof(name));
        line = strsep(&ptr, "\r\n");

        sscanf(line, "%x-%x %s %x %s %x %s", &add1, &add2, attr, &offset, time, &inode, name);
        
        printf("name %s addr1 %x addr2 %x\n", name, add1, add2);
        
        if (strstr(name, "[stack]")){
            strcpy(p_mq_ref1->u.maps[0].name, "[stack]");
            p_mq_ref1->u.maps[0].add1 = add1;
            p_mq_ref1->u.maps[0].add2 = add2;
            g_stack_max = add2;
            
            gstack_addr_min = add1;
            gstack_addr_max = add2;
            
        }else if (strstr(name, "libdbus.so")){
            if (strstr(p_mq_ref1->u.maps[1].name, "libdbus.so")){
                p_mq_ref1->u.maps[1].add3 = add1;
                p_mq_ref1->u.maps[1].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[1].name, "libdbus.so");
                p_mq_ref1->u.maps[1].add1 = add1;
                p_mq_ref1->u.maps[1].add2 = add2;
            }
        }else if (strstr(name, "libuClibc-0.9.30.1.so")){
            if (strstr(p_mq_ref1->u.maps[2].name, "libuClibc-0.9.30.1.so")){
                p_mq_ref1->u.maps[2].add3 = add1;
                p_mq_ref1->u.maps[2].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[2].name, "libuClibc-0.9.30.1.so");
                p_mq_ref1->u.maps[2].add1 = add1;
                p_mq_ref1->u.maps[2].add2 = add2;
            }
        }else if (strstr(name, "libcms_core.so")){
            if (strstr(p_mq_ref1->u.maps[1].name, "libcms_core.so")){
                p_mq_ref1->u.maps[1].add3 = add1;
                p_mq_ref1->u.maps[1].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[1].name, "libcms_core.so");
                p_mq_ref1->u.maps[1].add1 = add1;
                p_mq_ref1->u.maps[1].add2 = add2;
            }
        }else if (strstr(name, "libcms_util.so")){
            if (strstr(p_mq_ref1->u.maps[2].name, "libcms_util.so")){
                p_mq_ref1->u.maps[2].add3 = add1;
                p_mq_ref1->u.maps[2].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[2].name, "libcms_util.so");
                p_mq_ref1->u.maps[2].add1 = add1;
                p_mq_ref1->u.maps[2].add2 = add2;
            }
        }else if (strstr(name, "libcms_dal.so")){
            if (strstr(p_mq_ref1->u.maps[3].name, "libcms_dal.so")){
                p_mq_ref1->u.maps[3].add3 = add1;
                p_mq_ref1->u.maps[3].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[3].name, "libcms_dal.so");
                p_mq_ref1->u.maps[3].add1 = add1;
                p_mq_ref1->u.maps[3].add2 = add2;
            }
        }else if (strstr(name, "libcms_msg.so")){
            if (strstr(p_mq_ref1->u.maps[4].name, "libcms_msg.so")){
                p_mq_ref1->u.maps[4].add3 = add1;
                p_mq_ref1->u.maps[4].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[4].name, "libcms_msg.so");
                p_mq_ref1->u.maps[4].add1 = add1;
                p_mq_ref1->u.maps[4].add2 = add2;
            }
        }else if (strstr(name, "libnanoxml.so")){
            if (strstr(p_mq_ref1->u.maps[6].name, "libnanoxml.so")){
                p_mq_ref1->u.maps[6].add3 = add1;
                p_mq_ref1->u.maps[6].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[6].name, "libnanoxml.so");
                p_mq_ref1->u.maps[6].add1 = add1;
                p_mq_ref1->u.maps[6].add2 = add2;
            }
        }else if (strstr(name, "libxdslctl.so")){
            if (strstr(p_mq_ref1->u.maps[7].name, "libxdslctl.so")){
                p_mq_ref1->u.maps[7].add3 = add1;
                p_mq_ref1->u.maps[7].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[7].name, "libxdslctl.so");
                p_mq_ref1->u.maps[7].add1 = add1;
                p_mq_ref1->u.maps[7].add2 = add2;
            }
        }else if (strstr(name, "libatmctl.so")){
            if (strstr(p_mq_ref1->u.maps[8].name, "libatmctl.so")){
                p_mq_ref1->u.maps[8].add3 = add1;
                p_mq_ref1->u.maps[8].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[8].name, "libatmctl.so");
                p_mq_ref1->u.maps[8].add1 = add1;
                p_mq_ref1->u.maps[8].add2 = add2;
            }
        }else if (strstr(name, "libcms_boardctl.so")){
            if (strstr(p_mq_ref1->u.maps[9].name, "libcms_boardctl.so")){
                p_mq_ref1->u.maps[9].add3 = add1;
                p_mq_ref1->u.maps[9].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[9].name, "libcms_boardctl.so");
                p_mq_ref1->u.maps[9].add1 = add1;
                p_mq_ref1->u.maps[9].add2 = add2;
            }                  
        }else if (strstr(name, "libwlmngr.so")){
            if (strstr(p_mq_ref1->u.maps[10].name, "libwlmngr.so")){
                p_mq_ref1->u.maps[10].add3 = add1;
                p_mq_ref1->u.maps[10].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[10].name, "libwlmngr.so");
                p_mq_ref1->u.maps[10].add1 = add1;
                p_mq_ref1->u.maps[10].add2 = add2;
            }
        }else if (strstr(name, "libwlctl.so")){
            if (strstr(p_mq_ref1->u.maps[11].name, "libwlctl.so")){
                p_mq_ref1->u.maps[11].add3 = add1;
                p_mq_ref1->u.maps[11].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[11].name, "libwlctl.so");
                p_mq_ref1->u.maps[11].add1 = add1;
                p_mq_ref1->u.maps[11].add2 = add2;
            }
        }else if (strstr(name, "libnvram.so")){
            if (strstr(p_mq_ref1->u.maps[12].name, "libnvram.so")){
                p_mq_ref1->u.maps[12].add3 = add1;
                p_mq_ref1->u.maps[12].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[12].name, "libnvram.so");
                p_mq_ref1->u.maps[12].add1 = add1;
                p_mq_ref1->u.maps[12].add2 = add2;
            }
        }else if (strstr(name, "libcgi.so")){
            if (strstr(p_mq_ref1->u.maps[13].name, "libcgi.so")){
                p_mq_ref1->u.maps[13].add3 = add1;
                p_mq_ref1->u.maps[13].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[13].name, "libcgi.so");
                p_mq_ref1->u.maps[13].add1 = add1;
                p_mq_ref1->u.maps[13].add2 = add2;
            }
        }else if (strstr(name, program_name)){
            if (strstr(p_mq_ref1->u.maps[5].name, program_name)){
                p_mq_ref1->u.maps[5].add3 = add1;
                p_mq_ref1->u.maps[5].add4 = add2;
            }else {
                strcpy(p_mq_ref1->u.maps[5].name, program_name);
                p_mq_ref1->u.maps[5].add1 = add1;
                p_mq_ref1->u.maps[5].add2 = add2;
            }
        }
    }

    if (strlen(p_mq_ref1->u.maps[5].name) <= 1){
        strcpy(p_mq_ref1->u.maps[5].name, program_name);
        p_mq_ref1->u.maps[5].add1 = 0x40000;
    }
    
    p_mq_ref1->mq_opcode = ACT_MEMLEAK_MQ_OP_MAPS;
    memleak_tcp_write(MQ_SERVER_MAPS_PORT, (char *)p_mq_ref1, sizeof(act_memleak_mq_maps_t));
    
    printf("map exit %d\n", sizeof(act_memleak_mq_maps_t));
}

#define TAG_LEN 10
#define TAG_PTR 8

static inline void set_mem_tag(void *p_addr, unsigned int size)
{
        *((char *)p_addr) = ACT_MEM_LEAK_MEM_TAG_B;
        *(int *)((char *)p_addr + 1) = size;
        *((char *)p_addr+ 5) = ACT_MEM_LEAK_MEM_TAG_E;
        *((char *)p_addr+size + TAG_PTR) = ACT_MEM_LEAK_MEM_TAG_E;
}

static inline int val_mem_tag(void *p_addr)
{
        int size = 0;
        
        if (*((char *)p_addr) != ACT_MEM_LEAK_MEM_TAG_B){
                return -1;
        }
        if (*((char *)p_addr+ 5) != ACT_MEM_LEAK_MEM_TAG_E){
                return -1;
        }
        
        size = *(int *)((char *)p_addr + 1);
        
        if (size > 1000000){
                return -2;
        }
        
        if (*((char *)p_addr + size + TAG_PTR) != ACT_MEM_LEAK_MEM_TAG_E){
                return -2;
        }
        
    return 0;
}


#if defined SUPPORT_WECB
void test()
{

}


int backtrace(unsigned int *arr, int maxsize, int ra)
{
        long fp = 0 ;
        int i = 0;
        long lr = 0;
        long nextfp = 0;


        asm("mov %0, fp":"=r"(fp));
        asm("mov %0, lr":"=r"(lr));
        
        //printf("fp: %x lr: %x\n", fp, lr);
        
        for (i = 0; i < maxsize; i++){
#if 1
                if ((fp - 4) <= gstack_addr_min || (fp - 4)>= gstack_addr_max ){
                        break;
                }
#endif           
                lr = *((long *)(fp));
                nextfp = *((long *)(fp - 4));
                
                //printf("#lr :%x nextfp: %x\n", lr, nextfp);
                
                fp = nextfp;
                
                arr[i] = lr;
                
                if (fp == 0){
                        break;
                }
        }
        
        test();
        
        return i;
}
#endif


void  handle_stack(unsigned int *p_call_stack, unsigned int ra)
{
#if 0
        backtrace(p_call_stack, 30, ra);

#else       
    int *cur_fp = NULL;
    int i = 0;

    asm("move %0, $fp":"=r"(cur_fp));


    memset(&p_call_stack[0], 0xffffffff, ACT_MEMLEAK_MAX_STACK*4);
    
    p_call_stack[0] = (unsigned int)__builtin_return_address(0);
    p_call_stack[1] = (unsigned int)((unsigned char*)cur_fp + 48 + 1264);
    p_call_stack[2] = (unsigned int)ra;

    i = (g_stack_max-(unsigned int)(cur_fp+12+316)-4);

    if (i > (ACT_MEMLEAK_MAX_STACK-3)*4){
        i = (ACT_MEMLEAK_MAX_STACK-3)*4;
    }

    memcpy(&p_call_stack[3], cur_fp + 12 + 316, i);
#endif
}

void signal_segv(int signum, siginfo_t* info, void*ptr)
{
    act_memleak_mq_mem_t mq_mem_ref;
    
    memset(&mq_mem_ref, 0, sizeof(act_memleak_mq_mem_t));
    mq_mem_ref.pid = getpid();
    
    handle_stack(mq_mem_ref.u.stack_index, (unsigned int)__builtin_return_address(0)); 

    mq_mem_ref.mq_opcode = ACT_MEMLEAK_MQ_OP_SEGV;
    memleak_tcp_write(MQ_SERVER_MEM_PORT, (char *)&mq_mem_ref, sizeof(act_memleak_mq_mem_t));

    exit(-1);
}

int g_segv_handle_init = 0;
static inline void segv_handle_init()
{
    struct sigaction action;
    
    if (g_segv_handle_init == 0){
        g_segv_handle_init = 1;
        memset(&action, 0, sizeof(action));
        action.sa_sigaction = signal_segv;
        action.sa_flags = SA_SIGINFO;
        sigaction(SIGSEGV, &action, NULL);
        sigaction(SIGABRT, &action, NULL);
    }
}

act_memleak_mq_mem_t send_buf;

void pre_act_memleak_send_mq_op(int op_code, void *p_mem, unsigned int mem_size)
{
    act_memleak_mq_mem_t *p_mq = NULL;
    
    p_mq = &send_buf;
    
    memset(p_mq, 0, sizeof(act_memleak_mq_mem_t));
    p_mq->pid = getpid();
    p_mq->p_mem = p_mem;
    p_mq->mem_size = mem_size;

    handle_stack(p_mq->u.stack_index, (unsigned int)__builtin_return_address(0)); 

    p_mq->mq_opcode = op_code;

    memleak_tcp_write(MQ_SERVER_MEM_PORT, (char *)p_mq, sizeof(act_memleak_mq_mem_t));
}



void init_maps()
{
    if (g_stack_max != 0){
        return;
    }

    maps();
}





void *malloc(unsigned int size)
{
    void *p_ptr;
    
    MALLOC(p_ptr, size + TAG_LEN);
    
    if (g_stack_max == 1){
            return (void *)((char*)p_ptr + TAG_PTR);
    }

    init_maps();

    set_mem_tag(p_ptr, size);
    
    pre_act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_ADD, (void *)((char*)p_ptr + TAG_PTR), size);

    return (void *)((char*)p_ptr + TAG_PTR);
}       

void *calloc(unsigned int size, unsigned int nb)
{
    void *p_ptr;
    
    CALLOC(p_ptr, size, nb + TAG_LEN);
    
    if (g_stack_max == 1){
            return (void *)((char*)p_ptr + TAG_PTR);
    }
    
    init_maps();
    
    set_mem_tag(p_ptr, size*nb);
    
    pre_act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_ADD, (void *)((char*)p_ptr + TAG_PTR), size*nb);

    return (void *)((char*)p_ptr + TAG_PTR);
}       


void *realloc(void *p_ptr, unsigned int size)
{
    if (g_stack_max == 1){
        REALLOC(p_ptr, size+TAG_LEN);
        return (void *)((char*)p_ptr + TAG_PTR);
    }
    
    init_maps();
    
    if (p_ptr != NULL){
            if (val_mem_tag((void *)((char*)p_ptr - TAG_PTR)) != 0){
                    pre_act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_OVL, p_ptr, size);
            }
            
            pre_act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_DEL, p_ptr, size);
            
            p_ptr = (void *)((char*)p_ptr - TAG_PTR);

            REALLOC(p_ptr, size+TAG_LEN);
    }else {
            
            REALLOC(p_ptr, size+TAG_LEN);
    }
    
    set_mem_tag(p_ptr, size);
    
    pre_act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_ADD, (void *)((char*)p_ptr + TAG_PTR), size);
    
    return (void *)((char*)p_ptr + TAG_PTR);
}

void free(void *p_ptr)
{
    int rv = 0;
    
    if (p_ptr == NULL){
        return;
    }
    
    
    if (g_stack_max == 1){
        FREE((void *)((char*)p_ptr - TAG_PTR));
        return;
    }
    
    pre_act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_DEL, p_ptr, 0);
    if ((rv = val_mem_tag((void *)((char*)p_ptr - TAG_PTR))) != 0){
            if (rv == -1){
                    pre_act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_UOVL, p_ptr, 0);
                    
                    printf("UNDER FLOW\n");
            }else if (rv == -2){
                    pre_act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_OVL, p_ptr, 0);
                    printf("OVER FLOW\n");
            }
    }

    
    FREE((void *)((char*)p_ptr - TAG_PTR));
}
