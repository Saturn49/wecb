#! /bin/sh

LIBFILE=../../../target/x86/lib/libxml2.a
if [ ! -f $LIBFILE ]; then
./configure --without-python --without-iconv --with-minimum --with-tree --with-writer 
make
cp -f .libs/libxml2.a ../../../target/x86/lib/
fi


