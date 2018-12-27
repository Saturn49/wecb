# clinkd -- clink (MOCA) daemon program. 
#

CLINKD_CONF_DIR = ../../../drivers/moca/host/examples
CLINKD_CCPU_DIR = ../../../drivers/moca/ccpu


romfs:
	$(ROMFSINST) clinkd /sbin/clinkd
	cp $(CLINKD_CONF_DIR)/clink.backup $(DIR_ROMFS)/etc/clink.backup
	$(ROMFSINST) -s /var/clink.conf /etc/clink.conf
	cp $(CLINKD_CONF_DIR)/feic.conf $(DIR_ROMFS)/etc/feic.conf
	mkdir $(DIR_ROMFS)/etc/ -p
	cp $(CLINKD_CCPU_DIR)/microcode/EN2510.1.10.01.21_dic_gphy.bin $(DIR_ROMFS)/etc/EN2510.1.10.01.21_dic_gphy.bin
	$(ROMFSINST) -S $(CLINKD_CCPU_DIR)/EN2510.1.10.01.21_soc.bin /etc/EN2510.1.10.01.21_soc.bin

clean:
	rm -f core junk* *~ *.o *.d *.a *.so $(OBJ) $(WINALL) $(INSTALL_ROOT)/sbin/clinkd *.o.flags
