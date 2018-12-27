#!/bin/sh

#
# Cleanup existing bridge
#
brctl delif br0 eth0
brctl delif br0 wlan0
brctl delif br0 wlan1
brctl delif br0 wlan0-va0
brctl delif br0 wlan1-va0
# brctl delbr br0

#
# Global
#
echo 1 > /proc/net/vlan/vlanEnable
echo 1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,3 > /proc/net/vlan/pvid
# 1)Not use 0 as pvid
# 2)set wlan0-va3 and wlan1-va3's pvid to 3
# 3)Set all the others to 1

#
# For SSID1, no VLAN, included in br0
#
ip link add link eth0 name eth0.1 type vlan id 1
ip link add link wlan0-va0 name w0-0.1 type vlan id 1
ip link add link wlan1-va0 name w1-0.1 type vlan id 1

ifconfig eth0.1 up
ifconfig w0-0.1 up
ifconfig w1-0.1 up

# brctl addbr br0
brctl addif br0 eth0.1
brctl addif br0 w0-0.1
brctl addif br0 w1-0.1

echo 1 > /proc/net/vlan/groupIndex
echo 1,10423,10423,1 > /proc/net/vlan/vlanGroup
     # 10423: Eth(P0,p1), MoCA (p5), WLAN0_VA0, WLAN1_VA0
     #     0: no VLAN tag

#
# For SSID2, with VLAN tag 2, included in br1
#
ifconfig wlan0-va1 up
ifconfig wlan1-va1 up
ip link add link eth0 name eth0.2 type vlan id 2
ip link add link wlan0-va1 name w0-1.2 type vlan id 2
ip link add link wlan1-va1 name w1-1.2 type vlan id 2

ifconfig eth0.2 up
ifconfig w0-1.2 up
ifconfig w1-1.2 up

brctl addbr br1
brctl addif br1 eth0.2
brctl addif br1 w0-1.2
brctl addif br1 w1-1.2

echo 2 > /proc/net/vlan/groupIndex
echo 1,20823,20823,2 > /proc/net/vlan/vlanGroup
    # 1st 20823: Eth(P0,p1), MoCA (p5), WLAN0_VA1, WLAN1_VA1
    # 2nd 20823: with VLAN tag

echo "***********************"
echo "Setup VLAN successfully"
echo "***********************"


