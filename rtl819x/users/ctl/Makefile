# Makefile for Data_center and TR69 software
#
# This Makefile may be run "standalone" to create native software
# for a Linux PC.
# For cross-compiling, some pre-definitions are needed,
# see "buildmips" script as an example.
# (You can still do "make clean" even if cross-compiling).

# Pseudo-targets:
#      clean -- remove all(?) created files
#      firsttime -- do clean, then initialize all subdirectories
#      update (default if .firsttime exists) -- limited rebuild
#                      suitable for typical source file changes
# With no arguments, does update (and firsttime if .firsttime does not exist)
#
# Environmental variables used:
# INSTALL_ROOT=...  where to put output files
#       If missing, uses install subdirectory (useful for development
#       on a linux PC).
#
# Output files:
#      .firsttime -- writtten after "firsttime" make
#              If missing, "update" will do "firsttime"
#      $(INSTALL_ROOT) -- important run-time files copied here,
#              and also some files for cross-module compilation.
#              (By default this is "install").

INSTALL_ROOT = $(DIR_ROMFS)
INSTALL_ROOT	?=	$(shell pwd)/install


all:
	make -C ../boa ctl_config
	if [ ! -f .firsttime ] ; then echo MUST DO FIRSTTIME BUILD FIRST; $(MAKE) firsttime; fi
	@echo DOING UPDATE
	@echo UPDATING tags file
	cd files && make wecb

romfs:
	mkdir -p $(DIR_ROMFS)/sbin
	mkdir -p $(DIR_ROMFS)/etc
	mkdir -p $(DIR_ROMFS)/lib
ifeq ($(AEI_CRISTAL_ACCESS),1)
	mkdir -p $(DIR_ROMFS)/nagios
endif
	mkdir -p $(DIR_ROMFS)/var/rt_conf
	mkdir -p $(DIR_ROMFS)/mnt/rt_conf
	$(ROMFSINST) files/target/wecb/bin/data_center	/sbin/data_center
	$(ROMFSINST) files/target/wecb/bin/ssd	/sbin/ssd
	$(ROMFSINST) files/target/wecb/bin/zip	/bin/zip 
	$(ROMFSINST) files/target/wecb/bin/cli	/sbin/cli 
	$(ROMFSINST) files/target/wecb/bin/system_monitor	/sbin/system_monitor
ifeq ($(AEI_UPGRADE),1)
	$(ROMFSINST) files/target/wecb/bin/wanupdate  /sbin/wanupdate
endif 
ifeq ($(AEI_ZERO_CONF),1)
	$(ROMFSINST) files/target/wecb/bin/zero_conf    /sbin/zero_conf
	ln -sf /sbin/zero_conf $(DIR_ROMFS)/sbin/zero_conf_telus
	ln -sf /sbin/zero_conf $(DIR_ROMFS)/sbin/zero_conf_vz
endif
ifeq ($(AEI_TR69),1)
	$(ROMFSINST) files/target/wecb/bin/tr69	/sbin/tr69
	$(ROMFSINST) files/target/wecb/bin/msg	/sbin/msg
	$(ROMFSINST) -s /mnt/rt_conf/cert_der /etc/cacerts.pem
endif
	$(ROMFSINST) files/target/wecb/bin/tr69_cfg	/sbin/tr69_cfg 
	$(ROMFSINST) files/target/wecb/bin/mynetwork	/sbin/mynetwork
	$(ROMFSINST) files/target/wecb/bin/setbasemac	/sbin/setbasemac
ifeq ($(AEI_DBUS),1)
	$(ROMFSINST) files/target/wecb/lib/libdbus.so	/lib/libdbus.so
	$(ROMFSINST) files/target/wecb/lib/libdbusrecv_msg.so	/lib/libdbusrecv_msg.so
	$(ROMFSINST) files/target/wecb/lib/libdbussend_msg.so	/lib/libdbussend_msg.so
endif
	$(ROMFSINST) files/target/wecb/lib/libtr69c_dl.so	/lib/libtr69c_dl.so
	$(ROMFSINST) files/target/wecb/lib/libtslutil_dl.so	/lib/libtslutil_dl.so
	$(ROMFSINST) files/target/wecb/lib/libxml2.so.2	  /lib/libxml2.so.2	
	$(ROMFSINST) -s libxml2.so.2  /lib/libxml2.so
	$(ROMFSINST) files/target/wecb/lib/libdal.so	/lib/libdal.so
	$(ROMFSINST) files/exlib/libcrypto.so.1.0.0	/lib/libcrypto.so.1.0.0
	$(ROMFSINST) files/exlib/libssl.so.1.0.0	/lib/libssl.so.1.0.0
	$(ROMFSINST) -s libssl.so.1.0.0	/lib/libssl.so
	$(ROMFSINST) -s libcrypto.so.1.0.0	/lib/libcrypto.so
	$(ROMFSINST) files/target/wecb/etc/protype.xml	/etc/protype.xml
	$(ROMFSINST) files/target/wecb/etc/cfg.xml	/etc/cfg.xml
	$(ROMFSINST) files/target/wecb/etc/instoid.lst	/etc/instoid.lst
	$(ROMFSINST) files/target/wecb/lib/libgsoap2.so   /lib/libgsoap2.so
	$(ROMFSINST) files/target/wecb/lib/libdnssrv.so   /lib/libdnssrv.so
	$(ROMFSINST) files/target/wecb/lib/libexpat.so   /lib/libexpat.so
ifeq ($(AEI_CRISTAL_ACCESS),1)
	$(ROMFSINST) files/target/wecb/bin/factoryctl  /sbin/factoryctl
endif
firsttime:  clean
	echo Did first time inits >.firsttime
	@echo DONE with firsttime

clean:
	@echo DOING CLEAN
	-rm -rf install tags .firsttime
	-find . -name 'junk*' -exec rm -rf '{}' ';'
	-find . -name '.deps' -exec rm -rf '{}' ';'
	-find . -name '.dirstamp' -exec rm -rf '{}' ';'
	-find . -name '.obj' -exec rm -rf '{}' ';'
	-find . -name '.moc' -exec rm -rf '{}' ';'
	-find . -name '.ui' -exec rm -rf '{}' ';'
	-find . -name '.rnd' -exec rm -rf '{}' ';'
	-find . -name '.*.swp' -exec rm -rf '{}' ';'
	-find . -name '*.log' -exec rm -rf '{}' ';'
	-(cd files && $(MAKE) clean PLAT=wecb)
	@echo All cleaned
