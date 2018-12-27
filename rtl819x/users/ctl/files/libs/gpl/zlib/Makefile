#
# Makefile for the GoAhead web server reference source base
#  for the Linux OS
#
# Copyright (c) GoAhead Software, Inc. 1995-2000
#
# $Id: Makefile,v 1.2 2002/11/01 16:20:19 mmitchell Exp $
#
include $(CTL_DIR)/global.def

all:	compile 

ARCH	= $(INSTALL)/lib/libzlib.a
NAME	= $(INSTALL)/bin/zip

# User Management switch
UMSW	= -DUSER_MANAGEMENT_SUPPORT

# Digest Access switch
DASW	= -DDIGEST_ACCESS_SUPPORT

# Dependencies
DEPEND_FILES	= adler32.o  compress.o  crc32.o  deflate.o  gzio.o  infblock.o  infcodes.o  inffast.o  inflate.o  inftrees.o  infutil.o  trees.o  uncompr.o  zutil.o

#CFLAGS	= -DHAVE_CONFIG_H
#DEBUG	= -g -Wall

compile: $(ARCH) $(NAME)

#
#	Build archive of objects
#
$(ARCH): $(DEPEND_FILES)
	$(AR) -cruv $(ARCH) $?

#
#	Primary link
#
$(NAME): Makefile $(ARCH)
ifneq ($(strip $(RCC)),)
	$(CC) -c $(CFLAGS) $(IFLAGS) minigzip.c
	$(RCC) -o $(NAME) $(DEBUG) \
		$(CFLAGS) $(IFLAGS) \
		minigzip.o $(ARCH) 
else
	$(CC) -o $(NAME) $(DEBUG) \
		$(CFLAGS) $(IFLAGS) \
		minigzip.c $(ARCH)
endif

clean:
	rm -f $(NAME) $(ARCH) $(DEPEND_FILES)

#
#	Transition rules (add -o to put object in right directory)
#
.c.o:
	$(CC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
