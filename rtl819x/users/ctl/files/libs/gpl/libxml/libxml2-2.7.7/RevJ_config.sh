#! /bin/sh

export PATH=$PATH:/home/michelle/x86_openwrt/staging_dir/toolchain-i386_gcc-linaro_uClibc-0.9.32/bin/

TMP_PATH=$(pwd)
./configure --prefix=${TMP_PATH%ctl*}../../staging_dir/toolchain-i386_gcc-linaro_uClibc-0.9.32/bin/ --host=i486-openwrt-linux-uclibc --without-python --without-iconv --with-minimum --with-tree --with-writer 
make
cp -f .libs/libxml2.a ../../../target/revj/lib/
cp -f .libs/libxml2.a ../../../../lib/

