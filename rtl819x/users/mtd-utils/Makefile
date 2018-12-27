# clinkd -- clink (MOCA) daemon program. 
#
OBJ = flash_erase

romfs: all
	mkdir -p $(DIR_ROMFS)
	mkdir -p $(DIR_ROMFS)/sbin
	$(ROMFSINST) flash_erase /sbin/flash_erase

all: flash_erase

CFLAGS := $(CFLAGS) -DWITHOUT_XATTR -I./include/

flash_erase: flash_erase.c
	$(CC) -o $@ $^

clean:
	rm -f core junk* *~ *.o *.d *.a *.so $(OBJ) $(WINALL) *.o.flags
