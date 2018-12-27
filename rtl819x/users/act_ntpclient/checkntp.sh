#!/bin/sh


creat_checkntp()
{
cat <<EOF
#!/bin/sh

FIRSTUSE=/mnt/rt_conf/firstuse
NTP_BIN=/sbin/ntpclient

#If this is script is already running dont start it again
local pid=\`pidof checkntp.sh\`
[ ! "x\${pid}" == "x\$$" ] && return \${OK}

#If  is not running for any reason start it
while [ 1 ]
do
	local lpid=\`pidof ntpclient\`
	if [ -f "\${FIRSTUSE}" ]; then
		killall ntpclient
		exit \${OK}
	elif [ -z "\${lpid}" ]; then
		\${NTP_BIN} -s -l -i 2 -c 2 -h ntp.actiontec.com>/dev/null 2>/dev/null
	fi
	sleep 8 2>/dev/null
done
EOF
}

[ -e "/mnt/rt_conf/firstuse" ] && return ${OK}

creat_checkntp > /tmp/checkntp.sh 

chmod +x /tmp/checkntp.sh

/tmp/checkntp.sh &
