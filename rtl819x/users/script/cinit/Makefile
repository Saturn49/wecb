include $(DIR_USERS)/.config
include $(DIR_LINUX)/.config

all :
	echo "script"
romfs :
	@cp *.sh $(DIR_ROMFS)/bin/
	@cp ../script_dbg/* $(DIR_ROMFS)/bin/
ifeq ($(CONFIG_APP_WSC),y)
	$(ROMFSINST) simplecfgservice.xml /etc/simplecfgservice.xml
	$(ROMFSINST) -s /var/wps /etc/simplecfg
endif
