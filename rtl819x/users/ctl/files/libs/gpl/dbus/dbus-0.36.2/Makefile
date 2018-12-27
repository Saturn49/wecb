DBUS_DIRS = dbus bus tools test

all:
#	$(foreach dir, $(DBUS_DIRS), \
#	if [ -e $(dir) ]; then make -C $(dir); fi;)
	make -C dbus
	make -C bus
	make -C tools
	make -C test 
	
clean:
	$(foreach dir, $(DBUS_DIRS), \
	if [ -e $(dir) ]; then make -C $(dir) clean; fi;\
	if [ -L $(dir) ]; then rm -f $(dir); fi; )
          
