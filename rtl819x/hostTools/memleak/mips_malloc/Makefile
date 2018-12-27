include ./global.def

all:
	$(CC) $(CFLAGS) -c memory.c  -g
#	$(CC) $(CFLAGS) -c profile.c  -g
	$(CC) $(CFLAGS) -c malloc.c 
	$(AR) -cruv libmalloc.a memory.o malloc.o
#	$(AR) -cruv libprofile.a profile.o
ifneq ($(strip $(RCC)),)
#	$(CC) test.c -c -g
#	$(RCC) test.o -o test1 -L./ -lmalloc -g /tmp/ld-uClibc-0.9.29.so /tmp/libuClibc-0.9.29.so
else
#	$(CC) test.c -o test -g -L./ -lmalloc
#	$(CC) test.c -c -p -g
#	$(CC) test.o -o	profile -g -L./ -lprofile

#	cp test /tftpboot/
#	cp test ../debug/
#	cp profile /tftpboot/
#	cp profile ../debug/
	mv -f libmalloc.a ../lib/
#	cp libprofile.a /tmp/libmalloc.a
endif
clean:
	rm -f *.o
	rm -f libmalloc.a
	rm -f test
	rm -f libprofile.a
	rm -f profile
	rm -f ../lib/libmalloc.a
