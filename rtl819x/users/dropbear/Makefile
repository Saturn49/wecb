# Makefile for dropbear tools
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

SRC_DIR = dropbear-0.52
CONFIGURE_ARGS = --host=mips-linux \
	--with-shared \
        --disable-pam \
        --enable-openpty \
        --enable-syslog \
        --disable-shadow \
        --disable-lastlog \
        --disable-utmp \
        --disable-utmpx \
        --disable-wtmp \
        --disable-wtmpx \
        --disable-loginfunc \
        --disable-pututline \
        --disable-pututxline \
        --disable-zlib

all:
	if [ ! -f .firsttime ] ; then echo MUST DO FIRSTTIME BUILD FIRST; $(MAKE) firsttime; fi
	@echo DOING UPDATE
	@echo UPDATING tags file
	make -C $(SRC_DIR) PROGRAMS="dropbear dbclient dropbearkey dropbearconvert scp" MULTI=1 SCPPROGRESS=1
	make -C $(SRC_DIR) PROGRAMS="dropbearconvert"

romfs:
	# Install for dropbear
	mkdir -p $(DIR_ROMFS)/usr/sbin
	$(ROMFSINST) $(SRC_DIR)/dropbearmulti /usr/sbin/dropbear
	mkdir -p $(DIR_ROMFS)/usr/bin
	ln -sf /usr/sbin/dropbear $(DIR_ROMFS)/usr/bin/scp
	ln -sf /usr/sbin/dropbear $(DIR_ROMFS)/usr/bin/ssh
	ln -sf /usr/sbin/dropbear $(DIR_ROMFS)/usr/bin/dbclient
	ln -sf /usr/sbin/dropbear $(DIR_ROMFS)/usr/bin/dropbearkey
	mkdir -p $(DIR_ROMFS)/etc/config
	$(ROMFSINST) ./files/dropbear.config /etc/config/dropbear
	mkdir -p $(DIR_ROMFS)/etc/init.d
	$(ROMFSINST) ./files/dropbear.init /etc/init.d/dropbear
	mkdir -p $(DIR_ROMFS)/usr/lib/opkg/info
	mkdir -p $(DIR_ROMFS)/etc/dropbear
	touch $(DIR_ROMFS)/etc/dropbear/dropbear_rsa_host_key
	touch $(DIR_ROMFS)/etc/dropbear/dropbear_dss_host_key
	# Install for dropbearconvert
	mkdir -p $(DIR_ROMFS)/usr/bin
	$(ROMFSINST) $(SRC_DIR)/dropbearconvert /usr/bin/dropbearconvert
	cp dropbear_wecb_rsa $(DIR_ROMFS)/etc/dropbear/ -a 

firsttime:  clean
	echo Did first time inits >.firsttime
#	tar zxvf dropbear-0.52.tar.gz
	# Add patches
#	patch -p1 -d $(SRC_DIR) < 100-pubkey_path.patch
#	patch -p1 -d $(SRC_DIR) < 110-change_user.patch
#	patch -p1 -d $(SRC_DIR) < 130-ssh_ignore_o_and_x_args.patch
#	patch -p1 -d $(SRC_DIR) < 150-dbconvert_standalone.patch
	# Configure macro definition
#	sed -i 's,^/\* #define PKG_MULTI.*,#define PKG_MULTI,g' $(SRC_DIR)/options.h
#	sed -i 's,^#define DO_HOST_LOOKUP,/* & */,g' $(SRC_DIR)/options.h
#	sed -i 's,^#define DEFAULT_PATH .*,#define DEFAULT_PATH "/sbin:/usr/sbin:/bin:/usr/bin",g' $(SRC_DIR)/options.h
	cd $(SRC_DIR) && ./configure $(CONFIGURE_ARGS)
	#cd -
	@echo DONE with firsttime

clean:
	@echo DOING CLEAN
	-rm -rf install tags .firsttime
#	-rm -rf $(SRC_DIR)
	#-find . -name 'junk*' -exec rm -rf '{}' ';'
	#-find . -name '.deps' -exec rm -rf '{}' ';'
	#-find . -name '.dirstamp' -exec rm -rf '{}' ';'
	#-find . -name '.obj' -exec rm -rf '{}' ';'
	#-find . -name '.moc' -exec rm -rf '{}' ';'
	#-find . -name '.ui' -exec rm -rf '{}' ';'
	#-find . -name '.rnd' -exec rm -rf '{}' ';'
	#-find . -name '.*.swp' -exec rm -rf '{}' ';'
	#-find . -name '*.log' -exec rm -rf '{}' ';'
	#-(cd wget-1.12 && $(MAKE) clean)
	@echo All cleaned
