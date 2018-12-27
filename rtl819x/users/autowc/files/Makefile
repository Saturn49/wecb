###########################################################################
# Makefile                                                                #
# Copyright (c) 2005, 2006 by Laddie Group, Inc.  All rights reserved.    #
# Copyright (c) 2012, Actiontec Electronics Inc.  All rights reserved.    #
#                                                                         #
# This is the Makefile for the Actiontec API daemon                       #
###########################################################################

CC=rsdk-linux-gcc
AR=rsdk-linux-ar
RANLIB=rsdk-linux-ranlib
STRIP=rsdk-linux-strip
INSTALL=install

OPT =  -O1 -fPIC -c -I.
DEBUG = -g -ggdb -Werror
CFLAGS = -I. $(OPT) $(DEBUG)
EDOBJS = auto_config.o parse_file.o

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

default: autowc 

autowc: $(EDOBJS)
	$(CC) $(EDOBJS) -o $@
	$(STRIP) $@

clean:
	rm -f autowc $(EDOBJS) 
