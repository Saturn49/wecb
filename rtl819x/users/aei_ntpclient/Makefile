# ntpd makefile

LDFLAGS += -Wl,--allow-shlib-undefined
LDFLAGS += -s
LDFLAGS += -lrt --static

CFLAGS += -std=c99 -W -Wall -Wpointer-arith -Wcast-align -Wcast-qual -Wshadow \
 -Waggregate-return -Wnested-externs -Winline -Wwrite-strings -Wstrict-prototypes

VER := 1.20.2
CFLAGS += $(EXTRA_CFLAGS)
CFLAGS += -s -DVERSION='"$(VER)"'
CFLAGS += -D_GNU_SOURCE -D"BB_VER=KBUILD_STR(1.20.2)" -D"KBUILD_STR(s)=\#s"
CFLAGS += -Os -fomit-frame-pointer -fno-strict-aliasing

OBJS_SHARED = bb_strtonum.o default_error_retval.o full_write.o getopt32.o llist.o messages.o perror_msg.o ptr_to_globals.o safe_strncpy.o safe_write.o signals.o udp_io.o verror_msg.o vfork_daemon_rexec.o xatonum.o xconnect.o xfunc_die.o xfuncs.o xfuncs_printf.o dst.o
NTPD_OBJS = ntpd.o

EXEC1 = ntpclient
OBJS1 = $(NTPD_OBJS) $(OBJS_SHARED)

all: $(EXEC1)

.c.o:
	$(CC) -c $(CFLAGS) $<

$(EXEC1): $(OBJS1)
	$(CC) $(LDFLAGS) $(OBJS1) -o $(EXEC1)


romfs:
	$(ROMFSINST) ./ntpclient /sbin/ntpclient
	$(ROMFSINST) -s /var/TZ /etc/TZ

clean:
	rm -f ntpclient *.o 

