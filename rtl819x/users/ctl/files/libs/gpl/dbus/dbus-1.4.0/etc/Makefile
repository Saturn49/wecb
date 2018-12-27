
all dynamic install: etcfiles
etcfiles:
	mkdir -p $(INSTALL_DIR)/etc/dbus
	mkdir -p $(INSTALL_DIR)/etc/dbus/session.d
	mkdir -p $(INSTALL_DIR)/etc/dbus/system.d
	mkdir -p $(INSTALL_DIR)/etc/dbus/run
	cp -rf ./passwd $(INSTALL_DIR)/etc
	cp -rf ./session.conf $(INSTALL_DIR)/etc/dbus
	cp -rf ./system.conf $(INSTALL_DIR)/etc/dbus

#include $(BUILD_DIR)/make.deprules

#-include $(OBJS:.o=.d)
