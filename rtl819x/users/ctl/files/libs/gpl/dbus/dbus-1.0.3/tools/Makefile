include $(CTL_DIR)/global.def

TARGET=dbus-send dbus-launch dbus-monitor dbus-cleanup-sockets dbus-uuidgen
#		dbus-print-message.h \
#		dbus-launch.h \
#		dbus-print-message.h

OBJS_dbus-send = dbus-send.o dbus-print-message.o 
OBJS_dbus-launch = dbus-launch.o dbus-launch-x11.o	
OBJS_dbus-monitor = dbus-monitor.o dbus-print-message.o
OBJS_dbus-cleanup-sockets = dbus-cleanup-sockets.o
OBJS_dbus-uuidgen = dbus-uuidgen.o

CFLAGS += -Wall -I./ -I../ -shared -DDBUS_COMPILATION -DDBUS_MACHINE_UUID_FILE=\""/etc/dbus/machine-id"\"
LDFLAGS += -L../dbus -ldbus -lpthread

all: $(TARGET)

ifeq ($(TARGET_PLATFORM),BHR2)
dbus-send: $(OBJS_dbus-send)
	$(RCC) -o $@ $^ $(LDFLAGS)
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-send: $(OBJS_dbus-send)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
dbus-launch: $(OBJS_dbus-launch)
	$(RCC) -o $@ $^ $(LDFLAGS)
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-launch: $(OBJS_dbus-launch)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
dbus-monitor: $(OBJS_dbus-monitor)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-monitor: $(OBJS_dbus-monitor)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
dbus-cleanup-sockets: $(OBJS_dbus-cleanup-sockets)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-cleanup-sockets: $(OBJS_dbus-cleanup-sockets)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
dbus-uuidgen: $(OBJS_dbus-uuidgen)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
dbus-uuidgen: $(OBJS_dbus-uuidgen)
	$(CC) -o $@ $^ $(LDFLAGS)
endif
	
clean:
	rm -f *.o $(TARGET)
	
	
