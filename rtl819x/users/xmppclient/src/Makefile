
xmppbin = xmppclient

expatpath = ./expat
expatlib = $(expatpath)/libexpat.so
msgpath = ./sockmsg
msglib = $(msgpath)/libsockmsg.a
strophepath = $(DIR_USERS)/libstrophe/src/
strophelib = $(strophepath)/libstrophe.so

ctlpath = $(DIR_USERS)/ctl/files
opensslpath = $(DIR_USERS)/openssl-1.0.1h/include
xmlpath = $(ctlpath)/libs/private/framework/include/libxml2

CFLAGS += -I. -g -Wall -I$(ctlpath)/include -I$(opensslpath) -I$(strophepath) \
	-I$(expatpath) -I$(msgpath) -I$(xmlpath)

LDFLAGS =  -L$(ctlpath)/target/wecb/lib -L$(ctlpath)/exlib -L$(expatpath) -L$(strophepath) \
	-lssl -lcrypto -lresolv -lpthread -ltr69c_dl -lexpat -lstrophe

ACT_XMPP_OBJS = retry.o xmppc.o

all:  $(expatlib) $(msglib) $(xmppbin)

$(xmppbin):  $(strophelib) $(expatlib) $(msglib) $(ACT_XMPP_OBJS)
	$(CC) $(CFLAGS) -o $@  $(ACT_XMPP_OBJS) $(msglib) $(LDFLAGS)

$(expatlib):
	make -C $(expatpath)

$(msglib):
	make -C $(msgpath)

clean:
	rm -f *.o
	rm -f $(xmppbin)
	make -C $(expatpath) clean
	make -C $(msgpath) clean	
	
