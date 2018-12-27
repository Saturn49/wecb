#!/bin/sh

OPTION1=$1
OPTION2=$2

if [ $# -ne 1 ]; then 
if [ $# -ne 2 ]; then 
echo "Usage: $0 [dir] [clean|*]"; exit 1;
fi
fi


CTL_DIR=`pwd`

echo "start building..."

PARAMS="CTL_DIR=$CTL_DIR"

case $OPTION2 in
clean)
cd $OPTION1 && make clean $PARAMS
;;

*)
echo "cd $OPTION1 && make $PARAMS"
cd $OPTION1 && make $PARAMS
;;
esac

