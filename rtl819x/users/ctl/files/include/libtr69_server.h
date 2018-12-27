#ifndef LIBTR69_SERVER_H
#define LIBTR69_SERVER_H
#ifdef AEI_PARAM_PERSIST
int tr69_register_persist(void *p_inst_tb, int inst_tb_count, void *p_reg_tb, int reg_tb_count);
#endif
/**************************************************************************
 *
 *      Following Function only used for DATA_CENTER  
 *
 **************************************************************************/

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tr69_register
 *
 *	[DESCRIPTION]:
 *	        register NumberOfEntries table and RegisterFunc table
 *              which exist in autocreate files tr69_inst.h and tr69_st.h.
 *              make sure call this function before calling tr69_init
 *	[ARGUMENT]:
 *	        void *p_inst_tb
 *	        int inst_tb_count
 *              void *p_reg_tb
 *              int reg_tb_count
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tr69_register(void *p_inst_tb, int inst_tb_count, void *p_reg_tb, int reg_tb_count);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tr69_init
 *
 *	[DESCRIPTION]:
 *	        initiate tr69, make sure called tr69_register. 
 *
 *	[ARGUMENT]:
 *	        char *p_protype_xml 
 *	        char *p_cfg_xml
 *	        char *p_ori_cfg_xml
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tr69_init(char *p_protype_xml, char *p_cfg_xml, char *p_ori_cfg_xml);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tr69_cleanup
 *
 *	[DESCRIPTION]:
 *	        cleanup tr69
 *
 *	[ARGUMENT]:
 *	        
 *	        
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
void tr69_cleanup();


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        tf69_save_xml
 *
 *	[DESCRIPTION]:
 *	        save tr69 trees into xml
 *
 *	[ARGUMENT]:
 *	        char *p_file_name
 *	        
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int tr69_save_xml(char *p_file_name);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        libtr69_server
 *
 *	[DESCRIPTION]:
 *	        only used for data_center and save cfg.xml in default  
 *              periodic time  
 *
 *	[ARGUMENT]:
 *	        char *p_save_xml
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
int libtr69_server(char *p_save_xml, int def_wait_timeout);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        libtr69_server_stop
 *
 *	[DESCRIPTION]:
 *	        only used for data_center to stop service  
 *              
 *
 *	[ARGUMENT]:
 *	        char *p_save_xml
 *
 *	[RETURN]
 *              >= 0          SUCCESS
 *              <  0          ERROR
 **************************************************************************/
void libtr69_server_stop();

#endif

