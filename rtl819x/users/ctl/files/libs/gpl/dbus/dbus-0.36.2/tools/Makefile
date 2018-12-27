include $(CTL_DIR)/global.def

TARGET=dbus-send dbus-launch dbus-monitor dbus-cleanup-sockets
#		dbus-print-message.h \
#		dbus-launch.h \
#		dbus-print-message.h

OBJS_dbus-send = dbus-send.o dbus-print-message.o 
OBJS_dbus-launch = dbus-launch.o
OBJS_dbus-monitor = dbus-monitor.o dbus-print-message.o
OBJS_dbus-cleanup-sockets = dbus-cleanup-sockets.o

CFLAGS += -Wall -I./ -I../ -shared -DDBUS_COMPILATION -DDBUS_API_SUBJECT_TO_CHANGE=1
LDFLAGS += -L../dbus -ldbus

all: $(TARGET)

ifeq ($(TARGET_PLATFORM),BHR2)
dbus-send: $(OBJS_dbus-send)
	$(RCC) -o $@ $^ $(LDFLAGS)
	$(STRIP) $@
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-send: $(OBJS_dbus-send)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
dbus-launch: $(OBJS_dbus-launch)
	$(RCC) -o $@ $^ $(LDFLAGS)
	$(STRIP) $@
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-launch: $(OBJS_dbus-launch)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
dbus-monitor: $(OBJS_dbus-monitor)
	$(RCC) -o $@ $^ $(LDFLAGS)
	$(STRIP) $@
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-monitor: $(OBJS_dbus-monitor)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
dbus-cleanup-sockets: $(OBJS_dbus-cleanup-sockets)
	$(RCC) -o $@ $^ $(LDFLAGS)
	$(STRIP) $@        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-cleanup-sockets: $(OBJS_dbus-cleanup-sockets)
	$(CC) -o $@ $^ $(LDFLAGS)
endif

	
clean:
	rm -f *.o $(TARGET)
	
	
