#!/bin/bash
if [ $# -lt 1 ] ; then
	echo "Usage: $0 ip_addr"
	exit 1
fi
echo "8888" | ./udp_send $1 1414
echo "8889" | ./udp_send $1 1415
echo "8890" | ./udp_send $1 1416
# echo "1" | ./udp_send $1 1417
