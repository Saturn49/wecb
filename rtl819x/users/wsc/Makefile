WSCBIN = wsc_bin
 
all:
	@echo "builded"

clean:
	@echo "It's cleaned"

romfs:
	@echo "#################################install realtek bin##########################################"
	$(ROMFSINST) $(WSCBIN)/wscd /bin/wscd
	$(ROMFSINST) $(WSCBIN)/wscd.conf /etc/wscd.conf
ifdef AEI_WPS_CLIENT
	$(ROMFSINST) $(WSCBIN)/wscc.conf /etc/wscc.conf
endif
