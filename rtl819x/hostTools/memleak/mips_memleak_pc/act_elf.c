#include "act_common.h"
#include "act_mem_leak.h"
#include <execinfo.h>
#include "act_profile.h"

#define MAX_FILE_COUNT 4096

extern act_char_t g_file_table[MAX_FILE_COUNT][256];
extern act_int_t g_file_max_index;
extern act_memleak_avl_node_t *gp_preload_mem_ref_root;

int g_bind_level = 8;

#define EI_NIDENT 16
#define Elf32_Addr unsigned int
#define Elf32_Half unsigned short
#define Elf32_Off unsigned int
#define Elf32_SWord unsigned int
#define Elf32_Word unsigned int

#define ELF_ST_BIND(x) ((x) >> 4)
#define ELF_ST_TYPE(x) (((unsigned int) x) & 0xf)
#define ELF32_ST_BIND(x) ELF_ST_BIND(x)
#define ELF32_ST_TYPE(x) ELF_ST_TYPE(x)
#define ELF64_ST_BIND(x) ELF_ST_BIND(x)
#define ELF64_ST_TYPE(x) ELF_ST_TYPE(x)

struct Elf32_Ehd{
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
};

struct Elf32_Phdr{
    Elf32_Word p_type;
    Elf32_Off p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
};

struct Elf32_Shdr{
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
};

typedef struct elf32_sym{
    Elf32_Word st_name;
    Elf32_Addr st_value;
    Elf32_Word st_size;
    unsigned char st_info;
    unsigned char st_other;
    Elf32_Half st_shndx;
} Elf32_Sym;

typedef struct dwarf_compile_e_unit{
    char l[4];
    char v[2];
    char o[4];
    char s[1];
}dwarf_compile_e_unit_t;

typedef struct dwarf_compile_unit{
    unsigned int cu_length;
    unsigned short cu_version;
    unsigned int cu_abbrev_offset;
    unsigned char cu_pointer_size;
}dwarf_compile_unit_t;


typedef struct dwarf_debug_line_e_head_s{
    unsigned char li_length          [4];
    unsigned char li_version         [2];
    unsigned char li_prologue_length [4];
    unsigned char li_min_insn_length [1];
    unsigned char li_default_is_stmt [1];
    unsigned char li_line_base       [1];
    unsigned char li_line_range      [1];
    unsigned char li_opcode_base     [1];
}dwarf_debug_line_e_head_t;

typedef struct dwarf_debug_line_head_s{
    unsigned long  li_length;
    unsigned short li_version;
    unsigned int   li_prologue_length;
    unsigned char  li_min_insn_length;
    unsigned char  li_default_is_stmt;
    int            li_line_base;
    unsigned char  li_line_range;
    unsigned char  li_opcode_base;
}dwarf_debug_line_head_t;

typedef struct dwarf_debug_line_state_machine_s{
    unsigned long address;
    unsigned int file;
    unsigned int line;
    unsigned int column;
    int is_stmt;
    int basic_block;
    int end_sequence;
    /* This variable hold the number of the last entry seen
       in the File Table.  */
    unsigned int last_file_entry;
}dwarf_debug_line_state_machine_t;

/* Line number opcodes.  */
enum dwarf_line_number_ops {
    DW_LNS_extended_op = 0,
    DW_LNS_copy = 1,
    DW_LNS_advance_pc = 2,
    DW_LNS_advance_line = 3,
    DW_LNS_set_file = 4,
    DW_LNS_set_column = 5,
    DW_LNS_negate_stmt = 6,
    DW_LNS_set_basic_block = 7,
    DW_LNS_const_add_pc = 8,
    DW_LNS_fixed_advance_pc = 9,
    /* DWARF 3.  */
    DW_LNS_set_prologue_end = 10,
    DW_LNS_set_epilogue_begin = 11,
    DW_LNS_set_isa = 12
};

/* Line number extended opcodes.  */
enum dwarf_line_number_x_ops {
    DW_LNE_end_sequence = 1,
    DW_LNE_set_address = 2,
    DW_LNE_define_file = 3,
};



enum dwarf_call_frame_info
  {
    DW_CFA_advance_loc = 0x40,
    DW_CFA_offset = 0x80,
    DW_CFA_nop = 0x00,
    DW_CFA_set_loc = 0x01,
    DW_CFA_def_cfa = 0x0c,
    DW_CFA_def_cfa_offset = 0x0e,
    DW_CFA_val_offset = 0x14,
    DW_CFA_offset_extended_sf = 0x11,
  };





unsigned long int read_leb128 (unsigned char *data, unsigned int *length_return, int sign)
{
    unsigned long int result = 0;
    unsigned int num_read = 0;
    unsigned int shift = 0;
    unsigned char byte = 0;

    do
        {
            byte = *data++;
            num_read++;

            result |= ((unsigned long int) (byte & 0x7f)) << shift;
        
            shift += 7;
        
        }
    while (byte & 0x80);

    if (length_return != NULL)
        *length_return = num_read;

    if (sign && (shift < 8 * sizeof (result)) && (byte & 0x40))
        result |= -1L << shift;

    return result;
}

 unsigned int int_big2little(unsigned int val)
{

    unsigned int ret;
    unsigned char *b= (unsigned char *)&ret;

    b[3] = val&0xFF;
    b[2] = (val>>8)&0xFF;
    b[1] = (val>>16)&0xFF;
    b[0] = (val>>24)&0xFF;
    
    return ret;
}

 unsigned short short_big2little(unsigned short val)
{

    unsigned short ret;
    unsigned char *b= (unsigned char *)&ret;

    b[1] = val&0xFF;
    b[0] = (val>>8)&0xFF;
    
    return ret;
}

int big2little = 0;


act_rv_t dwarf_read_debug_frame(act_memleak_avl_node_t *p_func_ref_root,
                                act_memleak_avl_node_t *p_line_ref_root,
                                act_uchar_t *p_debug_frame, 
                                act_uint_t size, 
                                unsigned int base_address1,
                                unsigned int max_base_address1,
                                unsigned int base_address2)
{
    act_uint_t length;
    act_uint_t cid;
    act_uchar_t version;
    act_uchar_t augument;
    act_uchar_t code_factor;
    act_char_t data_factor;
    act_uchar_t ra_ind;
    act_uint_t pc_base = 0;
    act_uint_t pc_range = 0;
    act_uchar_t op;
    act_uchar_t opa;
    act_uint_t cfa_reg = 0;
    act_uint_t cfa_offset = 0;
    act_uint_t def_cfa_offset = 0;

    act_uint_t byte_read;
    
    act_uchar_t *p_start = p_debug_frame;
    act_uchar_t *p_end = p_debug_frame + size;
    act_uchar_t *p_block_end = NULL;

    act_uint_t bool_add_frame = 0;
    act_uint_t pc_base_val = 0;
    act_uint_t ra_off_val = 0;
    act_uint_t fp_off_val = 0;
    act_uint_t frame_val = 0;

    int max_base_diff = max_base_address1 - base_address1;
    unsigned int base_address;    
    

    while(p_start < p_end){
        memcpy(&length, p_start, sizeof(act_uint_t));
        p_start += 4;
        
        if (big2little){
            length = int_big2little(length);
        }
        
        p_block_end = p_start + length;

        memcpy(&cid, p_start, sizeof(act_uint_t));
        p_start += 4;
        
        if (cid == 0xffffffff){
             version = *p_start++;
            augument = *p_start++;
            code_factor = read_leb128(p_start, &byte_read, 0);
            p_start += byte_read;

            data_factor = read_leb128(p_start, &byte_read, 1);
            p_start += byte_read;
            
            ra_ind = *p_start++;
        }else{
            memcpy(&pc_base, p_start, sizeof(act_uint_t));
            p_start += 4;
            
            memcpy(&pc_range, p_start, sizeof(act_uint_t));
            p_start += 4;
            
            if (big2little){
                pc_base = int_big2little(pc_base);
                pc_range = int_big2little(pc_range);
            }
            bool_add_frame = 1;

            pc_base_val = pc_base;
            ra_off_val = 0;
            fp_off_val = 0;
            frame_val = 0;
        }

        printf("length %d version:%d augu:%d code_factor:%d data_factor:%d ra_ind:%d pc_base:0x%x pc_range:%d\n", 
               length, version, augument, code_factor, data_factor, ra_ind, pc_base, pc_range);
        
        while(p_start < p_block_end){
            op = *p_start++;
            opa = op&0x3f;
            
            if (op & 0xc0){
                op &= 0xc0;
            }
            
            switch(op){
            case DW_CFA_def_cfa:
                cfa_reg = *p_start++;
                cfa_offset =  read_leb128(p_start, &byte_read, 0);
                p_start += byte_read;


                printf("DW_CFA_def_cfa: %d ofs %d\n", cfa_reg, cfa_offset);
                break;
                
            case DW_CFA_advance_loc:
                printf("DW_CFA_Advance_loc: %d to 0x%x\n", opa * code_factor, pc_base + opa * code_factor);
                pc_base += opa * code_factor;
                break;
                
            case DW_CFA_def_cfa_offset:
                def_cfa_offset =  read_leb128(p_start, &byte_read, 0);
                p_start += byte_read;
                printf("DW_CFA_def_cfa_offset: %d\n", def_cfa_offset);
                
                frame_val = def_cfa_offset;
                break;
                
            case DW_CFA_offset:
                cfa_offset =  read_leb128(p_start, &byte_read, 0);
                p_start += byte_read;
                printf("DW_CFA_offset: %d at cfa %d \n", opa, cfa_offset*data_factor);

                if (opa == 30){
                    fp_off_val = (frame_val + cfa_offset*data_factor)/4;
                }else if (opa == ra_ind){
                    ra_off_val = (frame_val + cfa_offset*data_factor)/4;
                }
                
                break;

            case DW_CFA_offset_extended_sf:
                opa = read_leb128(p_start, &byte_read, 0);
                p_start += byte_read;

                cfa_offset =  read_leb128(p_start, &byte_read, 1);
                p_start += byte_read;
                printf("DW_CFA_offset_extended_sf: %d at cfa %d \n", opa, cfa_offset*data_factor);

                if (opa == 30){
                    fp_off_val = (frame_val + cfa_offset*data_factor)/4;
                }else if (opa == ra_ind){
                    ra_off_val = (frame_val + cfa_offset*data_factor)/4;
                }
                
                break;                

            case DW_CFA_nop:
                printf("DW_CFA_nop\n");

            default:
                break;
            }
        }
        
        if (bool_add_frame){
            bool_add_frame = 0;

            if (pc_base_val > max_base_diff){
                base_address = base_address2;
            }else {
                base_address = base_address1;
            }
            
            printf(" pc_base_val = 0x%x da_base_val = %d fp_base_val = %d fp_offset_val = %d\n",  (act_uint_t)((char *)pc_base_val + base_address), ra_off_val, fp_off_val, frame_val);
            act_memleak_func_frame_ref_add(p_func_ref_root, (act_uint_t)((char *)pc_base_val + base_address), frame_val, fp_off_val, ra_off_val);
        }
    }
    
    return ACT_RV_SUC;
}




act_int_t parse_elf(act_memleak_avl_node_t **pp_func_ref_root,
                    act_memleak_avl_node_t **pp_line_ref_root,
                    act_char_t *p_file, 
                    unsigned int base_address1,
                    unsigned int max_base_address1,
                    unsigned int base_address2 )
{
    struct Elf32_Ehd ehd;
    struct Elf32_Shdr shdr;
    struct Elf32_Shdr sym_shdr;
    struct Elf32_Shdr sec_shdr;

    Elf32_Sym *p_sym_element; 

    FILE *fp = NULL;
    act_uint_t len = -1;
    act_int_t i = 0;

    act_long_t file_point = 0;
    act_char_t *p_symname_table = NULL;
    act_char_t *p_sym_table = NULL;
    act_int_t sym_count = 0;
    act_uchar_t *p_debug_info = NULL;
    act_uchar_t *p_frame_info = NULL;
    act_uint_t frame_info_size = 0;
    act_char_t *p_secname_table = NULL;
    

    dwarf_compile_unit_t dw_cu;
    dwarf_compile_e_unit_t e_dw_cu;

    dwarf_debug_line_head_t dw_dl;
    dwarf_debug_line_e_head_t e_dw_dl;

    act_uchar_t *p_debug_line = NULL;
    act_uchar_t *p_data = NULL;
    act_uchar_t *p_std_opcodes = NULL;

    act_int_t j = 0;
    act_uchar_t *p_dir_table = NULL;
    act_uchar_t *p_file_table = NULL;
    act_uchar_t *p_numb_table = NULL;
    act_uint_t byte_read; 
    
    act_int_t dir_index;
    act_uchar_t op_code;
    act_ulong_t uladv;
    act_int_t adv;
    
    unsigned int ext_op_code_len;
    unsigned int bytes_read;
    unsigned char ext_op_code;
    unsigned char *op_code_data;
    act_int_t cur_file_index = 0;
    
    int max_base_diff = max_base_address1 - base_address1;
    unsigned int base_address;    
    act_int_t k;

    dwarf_debug_line_state_machine_t stm;

    memset(&sym_shdr, 0, sizeof(struct Elf32_Shdr));

    fp = fopen(p_file, "rb"); 
    ACT_VASSERT(fp != NULL);

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    ACT_VASSERT(len >= sizeof(struct Elf32_Ehd));

    rewind(fp);
    printf ("read \n");
    
    fread(&ehd,1, sizeof(struct Elf32_Ehd), fp);

    ACT_VASSERT (ehd.e_ident[0]== 0x7f &&
                 ehd.e_ident[1]== 'E' && 
                 ehd.e_ident[2]== 'L' && 
                 ehd.e_ident[3]== 'F');
    
    if (ehd.e_ident[5] == 2){
        big2little = 1;
    }
    
    if (big2little){
        /*
          Elf32_Half e_type;
          Elf32_Half e_machine;
          Elf32_Word e_version;
          Elf32_Addr e_entry;
          Elf32_Off e_phoff;
          Elf32_Off e_shoff;
          Elf32_Word e_flags;
          Elf32_Half e_ehsize;
          Elf32_Half e_phentsize;
          Elf32_Half e_phnum;
          Elf32_Half e_shentsize;
          Elf32_Half e_shnum;
          Elf32_Half e_shstrndx;
        */
        ehd.e_type = short_big2little(ehd.e_type);
        ehd.e_machine = short_big2little(ehd.e_machine);
        ehd.e_version = int_big2little(ehd.e_version);
        ehd.e_entry = int_big2little(ehd.e_entry);
        ehd.e_phoff = int_big2little(ehd.e_phoff);
        ehd.e_shoff = int_big2little(ehd.e_shoff);
        ehd.e_flags = int_big2little(ehd.e_flags);
        ehd.e_ehsize = short_big2little(ehd.e_ehsize);
        ehd.e_phentsize = short_big2little(ehd.e_phentsize);
        ehd.e_phnum = short_big2little(ehd.e_phnum);
        ehd.e_shentsize = short_big2little(ehd.e_shentsize);
        ehd.e_shnum = short_big2little(ehd.e_shnum);
        ehd.e_shstrndx= short_big2little(ehd.e_shstrndx);
    }

    printf("ELF %d:type %d machine %d version %d entry %d %d %d %d %d %d %d %d shnum %d %d\n",
           sizeof(struct Elf32_Ehd),
           ehd.e_type,
           ehd.e_machine,
           ehd.e_version, 
           ehd.e_entry, 
           ehd.e_phoff,
           ehd.e_shoff,
           ehd.e_flags,
           ehd.e_ehsize,
           ehd.e_phentsize,
           ehd.e_phnum,
           ehd.e_shentsize,
           ehd.e_shnum,
           ehd.e_shstrndx);

    /*parse section str table*/
    rewind(fp);
    fseek(fp, ehd.e_shoff+ehd.e_shentsize*ehd.e_shstrndx, SEEK_SET);
    fread(&sec_shdr,sizeof(struct Elf32_Shdr) , 1, fp);

    if (big2little){
        /*
        struct Elf32_Shdr{
            Elf32_Word sh_name;
            Elf32_Word sh_type;
            Elf32_Word sh_flags;
            Elf32_Addr sh_addr;
            Elf32_Off sh_offset;
            Elf32_Word sh_size;
            Elf32_Word sh_link;
            Elf32_Word sh_info;
            Elf32_Word sh_addralign;
            Elf32_Word sh_entsize;
        };
        */
        sec_shdr.sh_name = int_big2little(sec_shdr.sh_name);
        sec_shdr.sh_type = int_big2little(sec_shdr.sh_type);
        sec_shdr.sh_flags = int_big2little(sec_shdr.sh_flags);
        sec_shdr.sh_addr = int_big2little(sec_shdr.sh_addr);
        sec_shdr.sh_offset = int_big2little(sec_shdr.sh_offset);
        sec_shdr.sh_size = int_big2little(sec_shdr.sh_size);
        sec_shdr.sh_link = int_big2little(sec_shdr.sh_link);
        sec_shdr.sh_info = int_big2little(sec_shdr.sh_info);
        sec_shdr.sh_addralign = int_big2little(sec_shdr.sh_addralign);
        sec_shdr.sh_entsize = int_big2little(sec_shdr.sh_entsize);
    }
    


    ACT_SMALLOC(p_secname_table, act_char_t, sec_shdr.sh_size);
    
    rewind(fp);
    fseek(fp, sec_shdr.sh_offset, SEEK_SET);
    fread(p_secname_table,sec_shdr.sh_size,1,fp);
    



    if(ehd.e_shnum != 0){
        ACT_VASSERT(ehd.e_shentsize == sizeof(struct Elf32_Shdr));
        rewind(fp);
        fseek(fp,ehd.e_shoff,SEEK_SET);
        for (i = 0; i < ehd.e_shnum; i++){
            fread(&shdr,ehd.e_shentsize,1,fp);
            
            if (big2little){
                /*
                  struct Elf32_Shdr{
                  Elf32_Word sh_name;
                  Elf32_Word sh_type;
                  Elf32_Word sh_flags;
                  Elf32_Addr sh_addr;
                  Elf32_Off sh_offset;
                  Elf32_Word sh_size;
                  Elf32_Word sh_link;
                  Elf32_Word sh_info;
                  Elf32_Word sh_addralign;
                  Elf32_Word sh_entsize;
                  };
                */
                shdr.sh_name = int_big2little(shdr.sh_name);
                shdr.sh_type = int_big2little(shdr.sh_type);
                shdr.sh_flags = int_big2little(shdr.sh_flags);
                shdr.sh_addr = int_big2little(shdr.sh_addr);
                shdr.sh_offset = int_big2little(shdr.sh_offset);
                shdr.sh_size = int_big2little(shdr.sh_size);
                shdr.sh_link = int_big2little(shdr.sh_link);
                shdr.sh_info = int_big2little(shdr.sh_info);
                shdr.sh_addralign = int_big2little(shdr.sh_addralign);
                shdr.sh_entsize = int_big2little(shdr.sh_entsize);
            }            
            
            

            printf("SEC_NAME %s %x\n", p_secname_table+shdr.sh_name, shdr.sh_type);
            
            if (shdr.sh_type == 2){
                memcpy(&sym_shdr,&shdr, sizeof(struct Elf32_Shdr));
            }else if (shdr.sh_type == 3 && 
                      !strcmp(p_secname_table+ shdr.sh_name, ".strtab")){
                file_point=ftell(fp);
                fseek(fp, shdr.sh_offset, SEEK_SET);
                
                ACT_SMALLOC(p_symname_table, act_char_t, shdr.sh_size);
                fread(p_symname_table, shdr.sh_size, 1, fp);
                fseek(fp,file_point,SEEK_SET);
            }else if ((shdr.sh_type == 1 || shdr.sh_type == 0x7000001e) && 
                      !strcmp(p_secname_table +shdr.sh_name, ".debug_info")){
                file_point=ftell(fp);
                fseek(fp, shdr.sh_offset, SEEK_SET);
                
                ACT_SMALLOC(p_debug_info, act_uchar_t, shdr.sh_size);
                fread(p_debug_info, shdr.sh_size, 1, fp);
              
                memset(&e_dw_cu, 0, sizeof(dwarf_compile_e_unit_t));
                memcpy(&e_dw_cu, p_debug_info, sizeof(dwarf_compile_e_unit_t));
                
                memcpy(&dw_cu.cu_length, e_dw_cu.l, sizeof(int));
                memcpy(&dw_cu.cu_version, e_dw_cu.v, sizeof(short));
                memcpy(&dw_cu.cu_abbrev_offset, e_dw_cu.o, sizeof(int));
                memcpy(&dw_cu.cu_pointer_size, e_dw_cu.s, sizeof(char));
                                
                printf("%d length: %d version:%d abbrev_offset:%d point_size:%d\n",
                       sizeof(dwarf_compile_e_unit_t),
                       dw_cu.cu_length, dw_cu.cu_version, 
                       dw_cu.cu_abbrev_offset, dw_cu.cu_pointer_size);
                fseek(fp,file_point,SEEK_SET);
            }else if ((shdr.sh_type == 1 || shdr.sh_type == 0x7000001e) && 
                      !strcmp(p_secname_table +shdr.sh_name, ".debug_frame")){
                file_point=ftell(fp);
                fseek(fp, shdr.sh_offset, SEEK_SET);
                
                ACT_SMALLOC(p_frame_info, act_uchar_t, shdr.sh_size);
                fread(p_frame_info, shdr.sh_size, 1, fp);
                
                frame_info_size = shdr.sh_size;
                
                fseek(fp,file_point,SEEK_SET);
            }else if ((shdr.sh_type == 1 || shdr.sh_type == 0x7000001e) && 
                      !strcmp(p_secname_table +shdr.sh_name, ".debug_line")){
                file_point=ftell(fp);
                fseek(fp, shdr.sh_offset, SEEK_SET);
                
                ACT_SMALLOC(p_debug_line, act_uchar_t, shdr.sh_size);
                fread(p_debug_line, shdr.sh_size, 1, fp);
                fseek(fp,file_point,SEEK_SET);

                p_data = p_debug_line;
               
                while(p_data < p_debug_line + shdr.sh_size){
                
                    memset(&e_dw_dl, 0, sizeof(dwarf_debug_line_e_head_t));
                    
                    memcpy(&e_dw_dl, p_data, sizeof(dwarf_debug_line_e_head_t));
                    memcpy(&dw_dl.li_length, e_dw_dl.li_length, sizeof(e_dw_dl.li_length));
                    memcpy(&dw_dl.li_version, e_dw_dl.li_version, sizeof(e_dw_dl.li_version));
                    memcpy(&dw_dl.li_prologue_length, e_dw_dl.li_prologue_length, sizeof(e_dw_dl.li_prologue_length));
                    memcpy(&dw_dl.li_min_insn_length, e_dw_dl.li_min_insn_length, sizeof(e_dw_dl.li_min_insn_length));
                    memcpy(&dw_dl.li_default_is_stmt, e_dw_dl.li_default_is_stmt, sizeof(e_dw_dl.li_default_is_stmt));
                    memcpy(&dw_dl.li_line_base, e_dw_dl.li_line_base, sizeof(e_dw_dl.li_line_base));
                    memcpy(&dw_dl.li_line_range, e_dw_dl.li_line_range, sizeof(e_dw_dl.li_line_range));
                    memcpy(&dw_dl.li_opcode_base, e_dw_dl.li_opcode_base, sizeof(e_dw_dl.li_opcode_base));
                    
                    if (big2little){
                        /*
                        typedef struct dwarf_debug_line_head_s{
                            unsigned long  li_length;
                            unsigned short li_version;
                            unsigned int   li_prologue_length;
                            unsigned char  li_min_insn_length;
                            unsigned char  li_default_is_stmt;
                            int            li_line_base;
                            unsigned char  li_line_range;
                            unsigned char  li_opcode_base;
                        }dwarf_debug_line_head_t;
                        */
                        dw_dl.li_length  = int_big2little(dw_dl.li_length);
                        dw_dl.li_version = short_big2little(dw_dl.li_version);
                        dw_dl.li_prologue_length = int_big2little(dw_dl.li_prologue_length);
                        //   dw_dl.li_line_base = int_big2little(dw_dl.li_line_base);
                    }
                    
                    dw_dl.li_line_base <<=24;
                    dw_dl.li_line_base >>=24;

                    printf("DW_DL :\n%ld %d %d %d %d %d %d %d\n", 
                           dw_dl.li_length, 
                           dw_dl.li_version,
                           dw_dl.li_prologue_length, 
                           dw_dl.li_min_insn_length, 
                           dw_dl.li_default_is_stmt, 
                           dw_dl.li_line_base, 
                           dw_dl.li_line_range, 
                           dw_dl.li_opcode_base);
                    
                    stm.address = 0;
                    stm.file = 1;
                    stm.line = 1;
                    stm.column = 0;
                    stm.is_stmt = dw_dl.li_default_is_stmt;
                    stm.basic_block = 0;
                    stm.end_sequence = 0;
                    stm.last_file_entry = 0;
                    
                    p_std_opcodes = p_data + sizeof(dwarf_debug_line_e_head_t);
                    
                    printf("\n Opcodes :\n");
                    
                    for(j = 1; j < dw_dl.li_opcode_base; j++){
                        printf("Opcode %d has %d args\n", j, p_std_opcodes[j-1]);
                    }
                    
                    p_dir_table = p_std_opcodes + dw_dl.li_opcode_base -1;
                    
                    if (*p_dir_table == 0){
                        printf("directory table is null\n");
                    }else {
                        printf("directory table:\n");
                        while(*p_dir_table != 0){
                            printf("%s\n", p_dir_table);
                            p_dir_table += strlen((char *)p_dir_table) + 1;
                        }
                    }
                    p_file_table = p_dir_table + 1;
                    
                    if (*p_file_table == 0){
                        printf("file table is null\n");
                    }else {
                        cur_file_index = -1;
                        for(k = 0; k < MAX_FILE_COUNT; k++){
                            if (!strcmp(g_file_table[k], (act_char_t *)p_file_table) || strlen(g_file_table[k]) == 0){
                                cur_file_index = k;
                                break;
                            }
                        }
                        if (cur_file_index < 0){
                            cur_file_index = g_file_max_index++;
                            if (g_file_max_index > MAX_FILE_COUNT){
                                g_file_max_index = 0;
                            }
                        }
                        
                        sprintf(g_file_table[cur_file_index],"%s", p_file_table);
                        
                        printf("file table:\n");
                        while(*p_file_table != 0){
                            ++stm.last_file_entry;
                            printf("%d\t", stm.last_file_entry);
                            printf("%s\t", p_file_table);
                            p_file_table += strlen((char *)p_file_table) +1;
                            
                            read_leb128(p_file_table, &byte_read, 0);
                            p_file_table += byte_read;
                            
                            read_leb128(p_file_table, &byte_read, 0);
                            p_file_table += byte_read;
                            
                            read_leb128(p_file_table, &byte_read, 0);
                            p_file_table += byte_read;
                            
                            printf("\n");
                        }
                    }
                    
                    p_numb_table = p_file_table + 1;

                    printf("Line number statement:\n");
                    
                    while (p_numb_table < p_data + dw_dl.li_length + 4){
                        op_code = *p_numb_table++;
    
                        if (op_code >= dw_dl.li_opcode_base){
                            op_code -= dw_dl.li_opcode_base;
                            uladv = (op_code/dw_dl.li_line_range)*dw_dl.li_min_insn_length;
                            stm.address += uladv;
                            adv = (op_code%dw_dl.li_line_range) + dw_dl.li_line_base;
                            stm.line += adv;
        
                            printf("Special opcode %d advance Address by %lu to 0x%lx and line by %d to %d \n",
                                   op_code, uladv, stm.address, adv, stm.line );
                            
                            if (stm.address > max_base_diff){
                                base_address = base_address2;
                            }else {
                                base_address = base_address1;
                            }
                            act_memleak_line_ref_add(pp_line_ref_root, (act_uint_t)((act_char_t *)stm.address + base_address), cur_file_index, stm.line);

                        }else {
                            switch(op_code){
                            case DW_LNS_extended_op:
            
                                op_code_data = p_numb_table;
                                
                                ext_op_code_len = read_leb128 (op_code_data, &bytes_read, 0);
                                op_code_data += bytes_read;
            
                                if (ext_op_code_len == 0) {
                                    printf("badly formed extended line op encountered!\n");
                                    break;
                                }
                                ext_op_code_len += bytes_read;
                                ext_op_code = *op_code_data++;
            
                                switch (ext_op_code){
                                case DW_LNE_end_sequence:
                                    stm.address = 0;
                                    stm.file = 1;
                                    stm.line = 1;
                                    stm.column = 0;
                                    stm.is_stmt = dw_dl.li_default_is_stmt;
                                    stm.basic_block = 0;
                                    stm.end_sequence = 0;
                                    stm.last_file_entry = 0;
                
                                    break;
                                case DW_LNE_set_address:
                                    memcpy(&stm.address, op_code_data, ext_op_code_len - bytes_read - 1);
                                    
                                    if (big2little){
                                        stm.address = int_big2little(stm.address);
                                    }
                                    
                                    printf("extend Set Address, 0x%lx\n", stm.address);
                                    break;
                                case DW_LNE_define_file:
                                    ++stm.last_file_entry;
                                    op_code_data += strlen ((char *) op_code_data) + 1;
                                    dir_index = read_leb128 (op_code_data, & bytes_read, 0);
                                    op_code_data += bytes_read;
                                    read_leb128 (op_code_data, & bytes_read, 0);
                                    op_code_data += bytes_read;
                                    read_leb128 (op_code_data, & bytes_read, 0);
                
                                    printf ("%s:\n", (char *)p_dir_table+dir_index);
                                    break;
            
                                default:
                                    printf ("UNKNOWN: length %d\n", ext_op_code_len - bytes_read);
                                    break;
                                }
            
                                p_numb_table += ext_op_code_len;
  
                                break;
                                
                            case DW_LNS_copy:
                                printf("Copy\n");
                                if (stm.address > max_base_diff){
                                    base_address = base_address2;
                                }else {
                                    base_address = base_address1;
                                }
                                act_memleak_line_ref_add(pp_line_ref_root, (act_uint_t)((act_char_t *)stm.address + base_address), cur_file_index, stm.line);
                                break;
                                
                            case DW_LNS_advance_pc:
                                uladv = read_leb128 (p_numb_table, & byte_read, 0);
                                uladv *= dw_dl.li_min_insn_length;
                                p_numb_table += byte_read;
                                stm.address += uladv;
                                printf("Advance PC %lu to 0x%lx\n", uladv, stm.address);
                                break;
        
                            case DW_LNS_advance_line:
                                adv = read_leb128 (p_numb_table, & byte_read, 1);
                                p_numb_table += byte_read;
                                stm.line += adv;
                                printf("Advance Line by %d to %d\n", adv, stm.line);
                                break;
        
                            case DW_LNS_set_file:
                                adv = read_leb128 (p_numb_table, & byte_read, 0);
                                p_numb_table += byte_read;
                                printf ("  Set File Name to entry %d in the File Name Table\n",
                                        adv);
                                stm.file = adv;
                                break;
                                
                            case DW_LNS_set_column:
                                uladv = read_leb128 (p_numb_table, & byte_read, 0);
                                p_numb_table += byte_read;
                                printf ("  Set column to %lu\n", uladv);
                                stm.column = uladv;
                                break;
                                
                            case DW_LNS_negate_stmt:
                                adv = stm.is_stmt;
                                adv = ! adv;
                                printf ("  Set is_stmt to %d\n", adv);
                                stm.is_stmt = adv;
                                break;
                                
                            case DW_LNS_set_basic_block:
                                printf ("  Set basic block\n");
                                stm.basic_block = 1;
                                break;
                                
                            case DW_LNS_const_add_pc:
                                uladv = (((255 - dw_dl.li_opcode_base) / dw_dl.li_line_range)
                                         * dw_dl.li_min_insn_length);
                                stm.address += uladv;
                                printf("Advance PC by constant %lu to 0x%lx\n", uladv, stm.address);
                                break;
        
                            case DW_LNS_fixed_advance_pc:
                                memcpy(&uladv, p_numb_table, 2);
                                p_numb_table += 2;
                                stm.address += uladv;
                                printf("Advance PC by fixed size %lu to 0x%lx\n", uladv, stm.address);
                                break;
                                
                            case DW_LNS_set_prologue_end:
                                printf ("  Set prologue_end to true\n");
                                break;
                                
                            case DW_LNS_set_epilogue_begin:
                                printf ("  Set epilogue_begin to true\n");
                                break;
                                
                            case DW_LNS_set_isa:
                                uladv = read_leb128 (p_numb_table, & byte_read, 0);
                                p_numb_table += byte_read;
                                printf ("  Set ISA to %lu\n", uladv);
                                break;
                                
                            default:
                                printf("Unknown opcode %d with operands:", op_code);
            
                                for(j = p_std_opcodes[op_code =1]; j > 0; --j){
                                    read_leb128(p_numb_table, &byte_read, 0);
                                    //printf("0x%lx%s", read_leb128(p_numb_table, &byte_read, 0), i==1?"":",");
                                    p_numb_table += byte_read;
                                }
                            }
                        }
                    }
                    /*line number statement*/

                    p_data += dw_dl.li_length + 4;
                    //break;
                }
            }
        }

        if (sym_shdr.sh_offset != 0){

            fseek(fp, sym_shdr.sh_offset, SEEK_SET);
            ACT_SMALLOC(p_sym_table, act_char_t, sym_shdr.sh_size);
            fread( p_sym_table, sym_shdr.sh_size, 1, fp);
            
            sym_count = sym_shdr.sh_size/sizeof(Elf32_Sym);
            
            for (i = 0; i < sym_count; i++){
                p_sym_element = (Elf32_Sym *)(p_sym_table + i*sizeof(Elf32_Sym));
                
                if (big2little){
                    /*
                      typedef struct elf32_sym{
                      Elf32_Word st_name;
                      Elf32_Addr st_value;
                      Elf32_Word st_size;
                      unsigned char st_info;
                      unsigned char st_other;
                      Elf32_Half st_shndx;
                      } Elf32_Sym;
                    */
                    p_sym_element->st_name = int_big2little(p_sym_element->st_name);
                    p_sym_element->st_value = int_big2little(p_sym_element->st_value);
                    p_sym_element->st_size = int_big2little(p_sym_element->st_size);
                    p_sym_element->st_shndx = short_big2little(p_sym_element->st_shndx);
                }
                
                if (ELF32_ST_TYPE(p_sym_element->st_info ) != 2 
                    || p_sym_element->st_value  == 0){
                    continue;
                }
                
                if (p_sym_element->st_value > max_base_diff){
                    base_address = base_address2;
                }else {
                    base_address = base_address1;
                }
                
                printf(" 0x%x ",(char *)p_sym_element->st_value + base_address);
                printf("\t %s \n", p_symname_table + p_sym_element->st_name);
                act_memleak_func_ref_add(pp_func_ref_root, (char *)p_sym_element->st_value + base_address, p_symname_table + p_sym_element->st_name);
            }
        }
    }
    
    dwarf_read_debug_frame(*pp_func_ref_root, *pp_line_ref_root,p_frame_info, frame_info_size, base_address1, max_base_address1, base_address2);
    
    ACT_FREE(p_frame_info);
    ACT_FREE(p_debug_line);
    ACT_FREE(p_debug_info);
    ACT_FREE(p_secname_table);
    ACT_FREE(p_symname_table);
    ACT_FREE(p_sym_table);

    fclose(fp);

    return ACT_RV_SUC;
}




void print_trace (void)
{

    void *array[20];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace (array, 20);
    strings = (char **)backtrace_symbols (array, size);

    act_printf ("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++){
        act_printf ("%s\n", strings[i]);
    }

    free (strings);
}



void signal_segv(int signum, siginfo_t* info, void*ptr)
{
    print_trace();
    exit(0);
}


void segv_handle_init()
{
    struct sigaction action;

    memset(&action, 0, sizeof(action));
    action.sa_sigaction = signal_segv;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGPIPE, &action, NULL);
    sigaction(SIGINT, &action, NULL);
}

int g_bind_pid = 0;
FILE *g_log_fd = NULL;

void *act_memleak_server(void *argv)
{
    int port;
    sbs_func_status_bind(port);
    return NULL;
}

#define COMMAND_INPUT() \
act_lred_printf("\t\t\tTHE MEMORY LEAK & PROFILE TOOLKIT \n");\
act_red_printf("\t\t\t\t\t\t\t Ver 1.0\n");\
act_green_printf("Please Input Char\n");\
act_yellow_printf("\t'q'      ");     act_blue_printf("%-64s\n", "means quit");\
act_yellow_printf("\t'r'      ");     act_blue_printf("%-64s\n", "means clear");\
act_yellow_printf("\t'a'      ");     act_blue_printf("%-64s\n", "means display all monitored process's application name");\
act_yellow_printf("\t'p xxx'  ");     act_blue_printf("%-64s\n", "means bind pid xxx, in this case only display pid xxx's information");\
act_yellow_printf("\t'd'      ");     act_blue_printf("%-64s\n", "means display the memory usage information");\
act_yellow_printf("\t'u'      ");     act_blue_printf("%-64s\n", "means display the memory error information about unfreed memory list");\
act_yellow_printf("\t'b'      ");     act_blue_printf("%-64s\n", "means display the memory error information about double freed memory list");\
act_yellow_printf("\t'o 1..8' ");     act_blue_printf("%-64s\n", "means display the profile information about function call graph with level 1..8");\
act_yellow_printf("\t'g'      ");     act_blue_printf("%-64s\n", "means display the profile information about function statics");


int main(int argc, char *argv[])
{
    pthread_t tid;
    char ch = ' ';
    int i;
    char id_name[16];
    
    segv_handle_init();
    
    big2little = 1;
    
    pthread_create(&tid, NULL, act_memleak_server, NULL); 

            
    COMMAND_INPUT();

    while((ch=getchar()) != EOF){
        switch (ch){
        case 'r':
            fclose(g_log_fd);
            g_log_fd = NULL;
            break;
            
        case 'd':
            act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_DISP);
            break;
            
        case 'u':  
            act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_DISP_UNFREE);
            break;

        case 'b':
            act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_DISP_DBFREE);
            break;

        case 'e':
            act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_DISP_OVFLOW);
            break;

        case 'p':
            i = 0;
            ch=getchar();
            
            while ((ch >= '0' && ch <='9')|| ch == ' '){
                id_name[i++] = ch;
                ch=getchar();
            }
            id_name[i] = '\0';
            act_printf("bind pid:%d\n", atoi(id_name));
            g_bind_pid = atoi(id_name);
            break;

        case 'a':
            act_memleak_dispaly_all_pid();
            break;
            
        case 'o':
            i = 0;
            ch=getchar();
            
            while ((ch >= '0' && ch <='9')|| ch == ' '){
                id_name[i++] = ch;
                ch=getchar();
            }
            id_name[i] = '\0';
            act_printf("bind level:%d\n", atoi(id_name));
            if (atoi(id_name) != 0){
                g_bind_level = atoi(id_name);
            }
            
            act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_PROFILE_DISP);
            break;

        case 'g':
            act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_PROFILE_GEO);
            break;

        case 'q':
            act_memleak_send_mq_op(ACT_MEMLEAK_MQ_OP_QUIT);
            goto end;

        case 'h':
            COMMAND_INPUT();
            continue;
        }
    }
 end:
    
    pthread_join(tid, NULL);
  
    act_memleak_mem_statics_ref_cleanup();
    act_memleak_ref_root_cleanup();
    act_memleak_mem_ref_cleanup(&gp_preload_mem_ref_root);
    act_memleak_profile_func_cleanup();
    act_memleak_profile_func_stat_cleanup();
    
    act_printf("program exit\n");
    
    if (g_log_fd != NULL){
        fclose(g_log_fd);
    }
    g_log_fd = NULL;

    return 0;
}

