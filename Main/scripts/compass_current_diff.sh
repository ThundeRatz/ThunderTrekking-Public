#/bin/bash
initial=`stdbuf -o 0 ./udp_recv_hmc5883l | head -n1 | awk -F ' ' '{print $5}'`
stdbuf -o 0 ./udp_recv_hmc5883l | stdbuf -o 0 awk -F ' ' '{print $5}' | while read bearing ; do
	echo $initial $bearing
done |
stdbuf -o 0 ./compass_diff
