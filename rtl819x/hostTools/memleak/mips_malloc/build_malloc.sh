#! /bin/sh

cat >global.def <<_ACEOF
CC=../../../toolchain/rsdk-1.3.6-5281-EB-2.6.30-0.9.30/bin/mips-linux-gcc
AR=../../../toolchain/rsdk-1.3.6-5281-EB-2.6.30-0.9.30/bin/mips-linux-ar
CFLAGS= -Wall -DSUPPORT_WECB
_ACEOF

if [ ! -d ../lib ]; then
	mkdir ../lib
fi

make
