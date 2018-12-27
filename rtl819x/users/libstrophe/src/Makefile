
target = libstrophe.so

expatpath = $(DIR_USERS)/ctl/files/libs/gpl/expat
openssl_lib_path = $(DIR_USERS)/openssl-1.0.1h
openssl_inc_path = $(DIR_USERS)/openssl-1.0.1h/include

CFLAGS += -I. -g -Wall -fPIC -I$(openssl_inc_path) -I$(expatpath)
LDFLAGS += -shared -fPIC

LIB_OBJS = auth.o ctx.o event.o hash.o md5.o snprintf.o stanza.o tls_openssl.o \
	conn.o  dns.o  handler.o  jid.o parser_expat.o  sasl.o sock.o  thread.o util.o

all:  $(target)

$(target): $(LIB_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $?

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(target)

	
