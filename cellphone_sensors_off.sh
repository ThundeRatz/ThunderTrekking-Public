#!/bin/bash
# É interessante deixar o GPS ativo durante competições
# (ele demora alguns segundos para reconectar com satélites)
echo | ./udp_send $1 1414
echo | ./udp_send $1 1415
echo | ./udp_send $1 1416
