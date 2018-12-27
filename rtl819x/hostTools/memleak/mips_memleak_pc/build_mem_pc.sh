#!/bin/sh

if [ ! -d ./debug ]; then
	mkdir ./debug
fi

make

cp -f ../../../users/ctl/files/target/wecb/bin/* debug/
cp -f ../../../users/ctl/files/target/wecb/lib/*.so debug/
cp -f ../../../toolchain/rsdk-1.3.6-5281-EB-2.6.30-0.9.30/lib/ld-uClibc-0.9.30.so debug/
cp -f ../../../toolchain/rsdk-1.3.6-5281-EB-2.6.30-0.9.30/lib/libgcc_s_5281.so.1 debug/
