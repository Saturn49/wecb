#! /bin/sh

LIBFILE=../../../target/pk5000/lib/libxml2.a
if [ ! -f $LIBFILE ]; then
CROSS=/usr/local/mipsel-toolchain/toolchain_mipsel/bin/
export PATH=$PATH:$CROSS

./configure --prefix=/usr/local/mipsel-toolchain/toolchain_mipsel/ --host=mipsel-linux --without-python --without-iconv --with-minimum --with-tree --with-writer --includedir=/usr/include/ CFLAGS=-DSUPPORT_PK5000

make
cp -f .libs/libxml2.a ../../../target/pk5000/lib/
cp -f .libs/libxml2.a ../../../../lib/
fi

