#! /bin/sh

CROSS=/usr/local/openrg/mips64-octeon-linux-gnu/

export PATH=$PATH:$CROSS/bin/

./configure --prefix=/usr/local/openrg/mips64-octeon-linux-gnu/ --host=mips64-octeon-linux-gnu --without-python --without-iconv --with-minimum --with-tree --with-writer --with-debug #--without-threads --without-thread-alloc
cp config.h.bhr2 config.h
make

cp -f .libs/libxml2.a ../../../target/bhr2/lib/


