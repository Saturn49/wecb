# A long time ago, far, far away, under Solaris, you needed to
#    CFLAGS += -xO2 -Xc
#    LDLIBS += -lnsl -lsocket
# To cross-compile
#    CC = arm-linux-gcc
# To check for lint
# -Wundef not recognized by gcc-2.7.2.3
CFLAGS += -std=c99 -W -Wall -Wpointer-arith -Wcast-align -Wcast-qual -Wshadow \
 -Waggregate-return -Wnested-externs -Winline -Wwrite-strings -Wstrict-prototypes

CFLAGS += -Os 
CFLAGS += $(EXTRA_CFLAGS)
LDFLAGS += -s
# CFLAGS += -DPRECISION_SIOCGSTAMP
CFLAGS += -DENABLE_DEBUG
CFLAGS += -DENABLE_REPLAY
CFLAGS += -DACT_FIRST_USE
#CFLAGS += -DUSE_OBSOLETE_GETTIMEOFDAY

LDFLAGS += -lrt --static

all: ntpclient checkntp

test: ntpclient
	./ntpclient -d -r <test.dat

ntpclient: ntpclient.o phaselock.o

checkntp: checkntp.o

ntpclient.o phaselock.o checkntp.o: ntpclient.h

#adjtimex: adjtimex.o

romfs:
	$(ROMFSINST) ./ntpclient /sbin/ntpclient
	$(ROMFSINST) ./checkntp /sbin/checkntp
	$(ROMFSINST) -s /var/TZ /etc/TZ

clean:
	rm -f ntpclient adjtimex *.o 
