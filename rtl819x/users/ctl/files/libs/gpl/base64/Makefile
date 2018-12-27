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
	cp base64.h $(CTL_DIR)/include/

ARCH	= $(INSTALL)/lib/lib64.a

# Dependencies
DEPEND_FILES	= base64.o

#CFLAGS	= -DHAVE_CONFIG_H
#DEBUG	= -g -Wall

compile: $(ARCH) 

#
#	Build archive of objects
#
$(ARCH): $(DEPEND_FILES)
	$(AR) -cruv $(ARCH) $?

#
#	Primary link
#
clean:
	rm -f $(ARCH) $(DEPEND_FILES)

#
#	Transition rules (add -o to put object in right directory)
#
.c.o:
	$(CC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
