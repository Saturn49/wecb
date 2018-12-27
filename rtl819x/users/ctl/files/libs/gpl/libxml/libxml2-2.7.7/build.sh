#!/bin/sh

TARGET_DIR=$2
TARGET_PLATFORM=$1
CROSS=$3

LIBFILE=$TARGET_DIR/lib/libxml2.so.2.7.7

if [ "$TARGET_PLATFORM" = "WECB" ]; then
#echo "##########  build WECB libxml #############"
if [ ! -f $LIBFILE ]; then
export PATH=$PATH:$CROSS
./configure --prefix=$CROSS --host=mips-linux  --enable-shared CFLAGS=-Os host_alias=mips-linux CC=mips-linux-gcc LD=mips-linux-ld RANLIB=mips-linux-ranlib --without-python --without-iconv --with-minimum --with-tree --with-writer
make
if [ $? -ne 0 ]; then
echo "make libxml2 fail, platform:$TARGET_PLATFORM" 
exit 1
fi
cp -f .libs/libxml2.so.2.7.7 $TARGET_DIR/lib/
ln -sf $TARGET_DIR/lib/libxml2.so.2.7.7 $TARGET_DIR/lib/libxml2.so.2
ln -sf $TARGET_DIR/lib/libxml2.so.2.7.7 $TARGET_DIR/lib/libxml2.so
fi

else
echo "warning: PLATFORM: $TARGET_PLATFORM is not recognized, did you build it by yourself?"
fi

