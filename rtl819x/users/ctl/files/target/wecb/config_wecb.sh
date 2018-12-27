#! /bin/sh

cat >global.def <<_ACEOF

CROSS=$DIR_RSDK/bin/
CC=$DIR_RSDK/bin/mips-linux-gcc
AR=$DIR_RSDK/bin/mips-linux-ar
INSTALL=`/bin/pwd`/target/wecb
CFLAGS+= -Wall -D_LINUX -g -DCONFIG_RTL_92D_SUPPORT
IFLAGS+= -I./ -I`/bin/pwd`/include -I`/bin/pwd`/include/libxml2
LFLAGS+= -L`/bin/pwd`/target/wecb/lib -lpthread
TARGET_PLATFORM=WECB
CTL_DIR=`/bin/pwd`
TARGET_DIR=`/bin/pwd`/target/wecb/
_ACEOF

TARGET_DIR=`/bin/pwd`/target/wecb/
mkdir -p ${TARGET_DIR}/lib/
mkdir -p ${TARGET_DIR}/bin/
mkdir -p ${TARGET_DIR}/etc/

