#!/bin/sh

echo "****** reboot.sh ******"

echo "Saving ..."
cli -f

# At least sleep 1 sec here to wait tr69c close the session
echo "Wait daemon cleanup for 3 sec ..."
sleep 3

echo "Reboot Now!"
reboot

echo "***********************"

