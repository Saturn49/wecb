CC = mips-linux-gcc
AR = mips-linux-ar
RANLIB = mips-linux-ranlib
apmibpath = ../boa/apmib/

CFLAGS += -DHAVE_SYS_IOCTL_H -DHAVE_NETINET_IN_H -DHAVE_NET_IF_H -DSIOCGIFHWADDR
CFLAGS += -I$(apmibpath)
TARGET = libuuid.a
OBJS = compare.o gen_uuid.o pack.o parse.o unpack.o unparse.o \
       uuid_time.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) -cruv $@ $(OBJS)
	$(RANLIB) $@

clean:
	rm -f *.o
	rm -f $(TARGET)

romfs:
	$(ROMFSINST) $(TARGET) /lib/

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<
