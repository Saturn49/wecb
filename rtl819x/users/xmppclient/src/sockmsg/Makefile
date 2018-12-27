
CFLAGS += -I. -I../ 
OBJECTS =  unix_sock.o

lib_LTLIBRARIES = libsockmsg.a
bin_xmppmsg = xmpp_msg

all: $(lib_LTLIBRARIES) $(bin_xmppmsg)

%.o : %.c
	$(CC) -c $(CFLAGS) $(DEFS) $< -o $@

$(lib_LTLIBRARIES): $(OBJECTS)
	$(AR) cru $@ $(OBJECTS)
	$(RANLIB) $@

$(bin_xmppmsg): xmpp_msg.o
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) xmpp_msg.o
	
clean:
	rm -f $(OBJECTS) xmpp_msg.o 
	rm -f $(lib_LTLIBRARIES)
	rm -f $(bin_xmppmsg)

