include $(CTL_DIR)/global.def

LIB_A = libdbus.a
LIB_SO = libdbus.so
#dbus-credentials-util.o dbus-credentials.o dbus-file-unix.o dbus-file.o dbus-misc.o dbus-nonce.o dbus-pipe-unix.o
#dbus-pipe.o dbus-server-socket.o dbus-sysdeps-unix.o dbus-sysdeps-util-unix.o dbus-transport-socket.o dbus-uuidgen.o
#sd-daemon.o dbus-sysdeps-pthread.o
####dbus-test-main.o dbus-test.o
#OBJS=dbus-address.o dbus-auth-script.o dbus-auth-util.o dbus-auth.o dbus-bus.o dbus-connection.o \
#		dbus-dataslot.o dbus-errors.o dbus-hash.o dbus-internals.o dbus-keyring.o \
#		dbus-list.o dbus-mainloop.o dbus-marshal-basic.o dbus-marshal-byteswap-util.o dbus-marshal-byteswap.o dbus-marshal-header.o \
#		dbus-marshal-recursive-util.o dbus-marshal-recursive.o dbus-marshal-validate-util.o dbus-marshal-validate.o dbus-memory.o \
#		dbus-mempool.o dbus-message-factory.o dbus-message-util.o dbus-message.o dbus-object-tree.o \
#		dbus-pending-call.o dbus-resources.o dbus-server-debug-pipe.o \
#		dbus-server-unix.o dbus-server.o dbus-sha.o dbus-shell.o dbus-signature.o dbus-spawn.o dbus-string-util.o dbus-string.o \
#		dbus-sysdeps-util.o dbus-sysdeps.o \
#		dbus-threads.o dbus-timeout.o dbus-transport-unix.o dbus-transport.o dbus-userdb-util.o \
#		dbus-userdb.o dbus-watch.o	

##################################################
#Cut some .o
#dbus-auth-script.o dbus-auth-util.o dbus-marshal-byteswap-util.o dbus-marshal-recursive-util.o 
#dbus-marshal-validate-util.o dbus-message-factory.o dbus-message-util.o dbus-server-debug-pipe.o 
##################################################
OBJS=dbus-address.o dbus-auth.o dbus-bus.o dbus-connection.o \
		dbus-dataslot.o dbus-errors.o dbus-hash.o dbus-internals.o dbus-keyring.o \
		dbus-list.o dbus-mainloop.o dbus-marshal-basic.o dbus-marshal-byteswap.o dbus-marshal-header.o \
		dbus-marshal-recursive.o dbus-marshal-validate.o dbus-memory.o \
		dbus-mempool.o dbus-message.o dbus-object-tree.o \
		dbus-pending-call.o dbus-resources.o \
		dbus-server-unix.o dbus-server.o dbus-sha.o dbus-shell.o dbus-signature.o dbus-spawn.o dbus-string-util.o dbus-string.o \
		dbus-sysdeps-util.o dbus-sysdeps.o \
		dbus-threads.o dbus-timeout.o dbus-transport-unix.o dbus-transport.o dbus-userdb-util.o \
		dbus-userdb.o dbus-watch.o
		
#		dbus.h dbus-address.h dbus-arch-deps.h dbus-auth-script.h dbus-auth.h dbus-bus.h dbus-connection-internal.h \
#		dbus-connection.h dbus-credentials.h dbus-dataslot.h dbus-errors.h dbus-file.h dbus-hash.h dbus-internals.h \
#		dbus-keyring.h dbus-list.h dbus-macros.h dbus-mainloop.h dbus-marshal-basic.h dbus-marshal-byteswap.h \
#		dbus-marshal-header.h dbus-marshal-recursive.h dbus-marshal-validate.h dbus-memory.h dbus-mempool.h \
#		dbus-message-factory.h dbus-message-internal.h dbus-message-private.h dbus-message.h dbus-misc.h dbus-nonce.h \
#		dbus-object-tree.h dbus-pending-call-internal.h dbus-pending-call.h dbus-pipe.h dbus-protocol.h dbus-resources.h \
#		dbus-server-debug-pipe.h dbus-server-protected.h dbus-server-socket.h dbus-server-unix.h dbus-server.h dbus-sha.h \
#		dbus-shared.h dbus-shell.h dbus-signature.h dbus-spawn.h dbus-string-private.h dbus-string.h dbus-sysdeps-unix.h \
#		dbus-sysdeps.h dbus-test.h dbus-threads-internal.h dbus-threads.h dbus-timeout.h dbus-transport-protected.h \
#		dbus-transport-socket.h dbus-transport-unix.h dbus-transport.h dbus-types.h dbus-userdb.h dbus-uuidgen.h \
#		dbus-watch.h sd-daemon.h

CFLAGS+= -Os -Wall -fPIC -I./ -I../ -DDBUS_COMPILATION -DDBUS_API_SUBJECT_TO_CHANGE=1 -DDAEMON_NAME=\"dbus-daemon\"
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
endif
	
#$(LIB_A): $(OBJS)
#	$(AR) -rcs $@ $^

clean:
	rm -f $(OBJS) $(LIB_SO) *.a
	rm -f $(TARGET_DIR)/lib/$(LIB_SO)

#include $(BUILD_DIR)/make.deprules

#-include $(OBJS:.o=.d)
