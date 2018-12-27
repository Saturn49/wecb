#! /bin/sh

cat >global.def <<_ACEOF
CC=gcc
AR=ar
INSTALL=`/bin/pwd`/target/x86
CFLAGS= -Wall -D_LINUX -g
IFLAGS= -I./ -I/usr/include -I`/bin/pwd`/include -I`/bin/pwd`/include/libxml2
LFLAGS= -L`/bin/pwd`/target/x86/lib 
TARGET_PLATFORM=X86
CTL_DIR=`/bin/pwd`
TARGET_DIR=`/bin/pwd`/target/x86/
_ACEOF


TARGET_DIR=`/bin/pwd`/target/x86/
mkdir -p ${TARGET_DIR}/lib/
mkdir -p ${TARGET_DIR}/bin/
mkdir -p ${TARGET_DIR}/etc/
