include $(CTL_DIR)/global.def

TARGET = dbus-daemon 
#dbus-daemon-launch-helper dbus-daemon-launch-helper-test

#	activation.h	\
#	activation-exit-codes.h			\
#	activation-helper.h	\
#	bus.h					\
#	config-parser.h				\
#	config-parser-common.h			\
#	config-parser-trivial.h	\
#	connection.h				\
#	desktop-file.h				\
#	dir-watch.h				\
#	dispatch.h				\
#	driver.h				\
#	expirelist.h				\
#	policy.h				\
#	selinux.h				\
#	services.h				\
#	signals.h				\
#	test.h					\
#	utils.h

OBJS_dbus-daemon = activation.o \
										 bus.o \
										 desktop-file.o \
										 config-parser.o \
										 connection.o \
										 dispatch.o \
										 driver.o \
										 expirelist.o \
										 policy.o \
										 selinux.o \
										 services.o \
										 signals.o \
										 config-loader-expat.o \
										 main.o \
										 dir-watch.o \
										 utils.o
#										 config-parser-common.o \
#										 dir-watch-inotify.o \
###										 test.o \

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

CFLAGS += -Wall -I./ -I../ -I$(CTL_DIR)/include/expat -DDBUS_COMPILATION  -DDBUS_API_SUBJECT_TO_CHANGE=1 \
			-DDAEMON_NAME=\"dbus-daemon\"  -DDBUS_SYSTEM_CONFIG_FILE=\""/etc/dbus/system.conf"\" \
			-DDBUS_SESSION_CONFIG_FILE=\"/etc/session.conf\"
LDFLAGS += -L../dbus -ldbus -L$(TARGET_DIR)/lib -lexpat

all: $(TARGET)


all: $(TARGET)

ifeq ($(TARGET_PLATFORM),BHR2)
$(TARGET): $(OBJS_dbus-daemon)
	$(RCC) -o $@ $^ $(LDFLAGS)
	$(STRIP) $@
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<

else
$(TARGET): $(OBJS_dbus-daemon)
	$(CC) -o $@ $^ $(LDFLAGS)
endif

clean:
	rm -f $(TARGET) *.o 

#include $(BUILD_DIR)/make.deprules

#-include $(OBJS:.o=.d)
