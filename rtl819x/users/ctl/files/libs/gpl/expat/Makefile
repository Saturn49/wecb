include $(CTL_DIR)/global.def

CFLAGS  += -I. -fPIC
LDFLAGS += -shared -fPIC
LIB_SO = $(INSTALL)/lib/libexpat.so

OBJS = xmlparse.o xmlrole.o xmltok.o

all: $(LIB_SO)
	
$(LIB_SO): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $?

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@	

clean:
	rm -f $(OBJS) $(LIB_SO)
