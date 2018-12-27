include $(CTL_DIR)/global.def

TARGET=test-service test-shell-service shell-test spawn-test test-segfault test-exit test-sleep-forever \
	libdbussend_msg.so libdbusrecv_msg.so \
	sample test-recv-performance

#test-utils.h dbus_define.h

OBJS_test-service = test-service.o test-utils.o
#OBJS_test-names = test-names.o test-utils.o
OBJS_test-shell-service = test-shell-service.o test-utils.o
OBJS_shell-test = shell-test.o test-utils.o
OBJS_spawn-test = spawn-test.o test-utils.o
OBJS_test-segfault = test-segfault.o test-utils.o
OBJS_test-exit = test-exit.o test-utils.o
OBJS_test-sleep-forever = test-sleep-forever.o test-utils.o
OBJS_pmd = pmd.o test-utils.o
OBJS_dbussend = dbussend_msg.o
OBJS_dbusrecv = dbusrecv_msg.o test-utils.o
OBJS_sample = sample.o
OBJS_test-recv-performance = test-recv-performance.o

CFLAGS+=-I./ -I../ -I$(CTL_DIR)/include -DDBUS_API_SUBJECT_TO_CHANGE=1
LDFLAGS+=-L../dbus -ldbus

all: $(TARGET)

ifeq ($(TARGET_PLATFORM),BHR2)
test-service: $(OBJS_test-service)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
test-service: $(OBJS_test-service)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


#ifeq ($(TARGET_PLATFORM),BHR2)
#test-names: $(OBJS_test-names)
#	$(RCC) -o $@ $^ $(LDFLAGS)        
#.c.o:
#	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
#else
#test-names: $(OBJS_test-names)
#	$(CC) -o $@ $^ $(LDFLAGS)
#endif


ifeq ($(TARGET_PLATFORM),BHR2)
test-shell-service: $(OBJS_test-shell-service)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
test-shell-service: $(OBJS_test-shell-service)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
shell-test: $(OBJS_shell-test)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
shell-test: $(OBJS_shell-test)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
spawn-test: $(OBJS_spawn-test)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
spawn-test: $(OBJS_spawn-test)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
test-segfault: $(OBJS_test-segfault)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
test-segfault: $(OBJS_test-segfault)
	$(CC) -o $@ $^ $(LDFLAGS)
endif

ifeq ($(TARGET_PLATFORM),BHR2)
test-exit: $(OBJS_test-exit)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
test-exit: $(OBJS_test-exit)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
test-sleep-forever: $(OBJS_test-sleep-forever)
	$(RCC) -o $@ $^ $(LDFLAGS)        
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
test-sleep-forever: $(OBJS_test-sleep-forever)
	$(CC) -o $@ $^ $(LDFLAGS)
endif


ifeq ($(TARGET_PLATFORM),BHR2)
libdbussend_msg.a: $(OBJS_dbussend)
	$(AR) crv $@ $^
	mkdir -p $(TARGET_DIR)/lib/
	cp $@ $(TARGET_DIR)/lib/
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
libdbussend_msg.so: $(OBJS_dbussend)
	$(AR) -rcs $@ $^
	mkdir -p $(TARGET_DIR)/lib/
	cp $@ $(TARGET_DIR)/lib/
endif


ifeq ($(TARGET_PLATFORM),BHR2)
libdbusrecv_msg.a: $(OBJS_dbusrecv)
	$(AR) crv $@ $^
	mkdir -p $(TARGET_DIR)/lib/
	cp $@ $(TARGET_DIR)/lib/
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
libdbusrecv_msg.so: $(OBJS_dbusrecv)
	$(AR) -rcs $@ $^
	mkdir -p $(TARGET_DIR)/lib/
	cp $@ $(TARGET_DIR)/lib/
endif


ifeq ($(TARGET_PLATFORM),BHR2)
sample: $(OBJS_sample)
	$(RCC) -o $@ $^ $(LDFLAGS) -L./ -ldbussend_msg -ldbusrecv_msg
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
sample: $(OBJS_sample)
	$(CC) -o $@ $^ $(LDFLAGS) -L./ -ldbussend_msg -ldbusrecv_msg
endif

ifeq ($(TARGET_PLATFORM),BHR2)
test-recv-performance: $(OBJS_test-recv-performance)
	$(RCC) -o $@ $^ $(LDFLAGS) -L./ -ldbussend_msg -ldbusrecv_msg
.c.o:
	$(RCC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
else
test-recv-performance: $(OBJS_test-recv-performance)
	$(CC) -o $@ $^ $(LDFLAGS) -L./ -ldbussend_msg -ldbusrecv_msg
endif


clean:
	rm -f *.o $(TARGET)
	rm -f $(TARGET_DIR)/$(TARGET)
	



