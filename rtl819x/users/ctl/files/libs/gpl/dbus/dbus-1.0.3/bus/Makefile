include $(CTL_DIR)/global.def

ifeq ($(TARGET_PLATFORM),pk5000)
CFLAGS += -DDAEMON_NAME=\"dbus-daemon\" -DDBUS_SESSION_CONFIG_FILE=\""/etc/dbus/session.conf"\"
endif

TARGET = dbus-daemon 

OBJS_dbus-daemon = 	main.o	\
		 	config-loader-expat.o	\
			activation.o \
			bus.o \
        		config-parser.o \
			connection.o \
        		desktop-file.o \
			dir-watch-default.o \
			dispatch.o \
        		driver.o \
			expirelist.o \
			policy.o \
			selinux.o  \
			services.o \
			signals.o \
        		test.o \
			utils.o 

OBJS_dbus-daemon-launch-helper = config-loader-expat.o \
										 config-parser-common.o \
										 config-parser-trivial.o \
										 desktop-file.o \
										 utils.o \
										 activation-helper.o \
										 activation-helper-bin.o
										 
OBJS_dbus-daemon-launch-helper-test = activation-helper-bin.o	\
										 config-loader-expat.o \
										 config-parser-common.o \
										 config-parser-trivial.o \
										 desktop-file.o \
										 utils.o \
										 activation-helper.o

CFLAGS += -Wall -I./ -I../ -I$(CTL_DIR)/include/expat -DDBUS_COMPILATION -DDBUS_SYSTEM_CONFIG_FILE=\""/etc/dbus/system.conf"\"	
LDFLAGS += -L../dbus -ldbus -L$(TARGET_DIR)/lib -lexpat -lpthread

all: $(TARGET)


all: $(TARGET)

ifeq ($(TARGET_PLATFORM),BHR2)
$(TARGET): $(OBJS_dbus-daemon)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<

else
$(TARGET): $(OBJS_dbus-daemon)
	$(CC) -o $@ $^ $(LDFLAGS)
endif

clean:
	rm -f $(TARGET) *.o 

