#! /bin/sh

TARGET_DIR=$1
LIBFILE=$TARGET_DIR/lib/libxml2.a

rm -f $LIBFILE
make clean
