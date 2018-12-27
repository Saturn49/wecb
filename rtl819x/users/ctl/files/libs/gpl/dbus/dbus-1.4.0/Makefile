include $(CTL_DIR)/global.def
DBUS_DIRS = dbus bus tools test

PASSWD="etc/passwd"
SESSION="etc/session.conf"
SYSTEM="etc/system.conf"

all:
#	$(foreach dir, $(DBUS_DIRS), \
#	if [ -e $(dir) ]; then make -C $(dir); fi;)
	make -C dbus
	make -C bus
	make -C tools
	make -C test 
	cp ${PASSWD} $(TARGET_DIR)/etc/
	cp ${SESSION} $(TARGET_DIR)/etc/
	cp ${SYSTEM} $(TARGET_DIR)/etc/
	
clean:
	echo "==== dbus clean ====="
	rm -f $(TARGET_DIR)/${PASSWD}
	rm -f $(TARGET_DIR)/${SESSION}
	rm -f $(TARGET_DIR)/${SYSTEM}
	$(foreach dir, $(DBUS_DIRS), \
	if [ -e $(dir) ]; then make -C $(dir) clean; fi;\
	if [ -L $(dir) ]; then rm -f $(dir); fi; )
		
