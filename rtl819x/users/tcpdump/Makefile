# Makefile for tcpdump tools
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
	if [ ! -f .firsttime ] ; then echo MUST DO FIRSTTIME BUILD FIRST; $(MAKE) firsttime; fi
	@echo DOING UPDATE
	@echo UPDATING tags file

romfs:
	# Install for dropbear
	mkdir -p $(DIR_ROMFS)/sbin
	mkdir -p $(DIR_ROMFS)/lib
	$(ROMFSINST) bin/tcpdump /sbin/tcpdump
	$(ROMFSINST) lib/libpcap.so.1.0.0 /lib/libpcap.so.1.0.0
	$(ROMFSINST) -s libpcap.so.1.0.0 /lib/libpcap.so.1.0
	$(ROMFSINST) -s libpcap.so.1.0.0 /lib/libpcap.so

firsttime:  clean
	echo Did first time inits >.firsttime
	@echo DONE with firsttime

clean:
	@echo DOING CLEAN
	-rm -rf install tags .firsttime
	#-find . -name 'junk*' -exec rm -rf '{}' ';'
	#-find . -name '.deps' -exec rm -rf '{}' ';'
	#-find . -name '.dirstamp' -exec rm -rf '{}' ';'
	#-find . -name '.obj' -exec rm -rf '{}' ';'
	#-find . -name '.moc' -exec rm -rf '{}' ';'
	#-find . -name '.ui' -exec rm -rf '{}' ';'
	#-find . -name '.rnd' -exec rm -rf '{}' ';'
	#-find . -name '.*.swp' -exec rm -rf '{}' ';'
	#-find . -name '*.log' -exec rm -rf '{}' ';'
	@echo All cleaned
