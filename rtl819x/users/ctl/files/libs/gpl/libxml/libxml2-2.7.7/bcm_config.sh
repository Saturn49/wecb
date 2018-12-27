#! /bin/sh

CROSS=/opt/toolchains/uclibc-crosstools-gcc-4.4.2-1

export PATH=$PATH:$CROSS/usr/bin/

./configure --prefix=/opt/toolchains/uclibc-crosstools-gcc-4.4.2-1/usr/ --host=mips-linux --without-python --without-iconv --with-minimum --with-tree --with-writer 
make
cp -f .libs/libxml2.a ../../../target/bcm/lib/


