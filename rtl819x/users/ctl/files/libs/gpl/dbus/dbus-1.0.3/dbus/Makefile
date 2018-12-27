include $(CTL_DIR)/global.def

LIB_A = libdbus.a
LIB_SO = libdbus.so
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))

CFLAGS+= -Wall -fPIC -I./ -I../ -DDBUS_COMPILATION -DDBUS_MACHINE_UUID_FILE=\""/etc/dbus/machine-id"\" \
		-DDBUS_SYSTEM_CONFIG_FILE=\""/etc/dbus/system.conf"\"	\
		-DDBUS_SESSION_CONFIG_FILE=\""/etc/dbus/session.conf"\" \
		-DDBUS_SESSION_BUS_DEFAULT_ADDRESS=\""unix:tmpdir=/tmp"\"
#DBUS_SESSION_BUS_ADDRESS	

all: $(LIB_SO)

ifeq ($(TARGET_PLATFORM),BHR2)
$(LIB_SO): $(OBJS)
	$(RCC) -shared -o $@ $^
	mkdir -p $(TARGET_DIR)/lib/
	cp $(LIB_SO) $(TARGET_DIR)/lib/
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<

else
$(LIB_SO): $(OBJS)
	$(CC) -shared -o $@ $^
	mkdir -p $(TARGET_DIR)/lib/
	cp $(LIB_SO) $(TARGET_DIR)/lib/
.c.o:
	$(CC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
endif
	
clean:
	rm -f $(OBJS) $(LIB_SO) *.a
	rm -f $(TARGET_DIR)/lib/$(LIB_SO)

#include $(BUILD_DIR)/make.deprules

#-include $(OBJS:.o=.d)
