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

    printf("map exit\n");
}

void init_maps()
{
    if (g_stack_max != 0){
        return;
    }

    maps();
}

act_memleak_mq_profile_t profile_ref;

void __mcount(unsigned int pc1, unsigned int pc2)
{
        init_maps();
        
        memset(&profile_ref, 0, sizeof(act_memleak_mq_profile_t));
        profile_ref.pid = getpid();
        profile_ref.u.profile.caller_pc = pc1;
        profile_ref.u.profile.self_pc = pc2;
        
        clock_gettime(CLOCK_REALTIME, &profile_ref.u.profile.time);
        
        profile_ref.mq_opcode = ACT_MEMLEAK_MQ_OP_PROFILE_ADD;
        
        memleak_tcp_write(MQ_SERVER_PROFILE_PORT, (char *)&profile_ref, sizeof(act_memleak_mq_profile_t));
}

#if defined SUPPORT_BHR3

#define MCOUNT asm(           \
".align 2\n\t"                \
".global __gnu_mcount_nc\n\t"         \
".type __gnu_mcount_nc, %function\n\t"\
"__gnu_mcount_nc:\n\t"                \
"pop {r4}\n\t"\
"push {r0, r1, r2, r3, fp, lr}\n\t"\
"add fp, sp, #4\n\t"\
"mov r3, lr\n\t"\
"mov r2, r4\n\t"\
"mov r0, r2\n\t"\
"mov r1, r3\n\t"\
"bl	__mcount\n\t"\
"mov lr, r4\n\t"\
"pop {r0, r1, r2, r3, fp, pc}\n\t"\
".size	__gnu_mcount_nc, .-__gnu_mcount_nc\n\t" \
);


MCOUNT

#endif

