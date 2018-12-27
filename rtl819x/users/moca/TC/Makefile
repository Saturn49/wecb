# clnkstat -- clink (MOCA) utility program. 
#

romfs:
	$(ROMFSINST) TC /sbin/TC
	$(ROMFSINST) -s /sbin/TC /sbin/TC.1
	$(ROMFSINST) -s /sbin/TC /sbin/set_const_pwr
	$(ROMFSINST) -s /sbin/TC /sbin/TC.2
	$(ROMFSINST) -s /sbin/TC /sbin/set_scan_freq
	$(ROMFSINST) -s /sbin/TC /sbin/TC.5
	$(ROMFSINST) -s /sbin/TC /sbin/set_moca_passwd
	$(ROMFSINST) -s /sbin/TC /sbin/TC.7
	$(ROMFSINST) -s /sbin/TC /sbin/set_prefer_nc
	$(ROMFSINST) -s /sbin/TC /sbin/set_default_moca
	$(ROMFSINST) -s /sbin/TC /sbin/reset_moca
	$(ROMFSINST) -s /sbin/TC /sbin/Reboot
	$(ROMFSINST) -s /sbin/TC /sbin/show_tc

clean:
	-rm -f $(TARGET) *.elf *.gdb *.o *.d $(OBJ) $(LIBS)
