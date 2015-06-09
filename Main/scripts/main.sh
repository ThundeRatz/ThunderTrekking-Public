#!/bin/bash
set -e
echo Rodando GPS...
#udp_recv_gps_cell | head -n1
echo Rodando sonares...
#udp_recv_hc_sr04 | head -n1
echo Rodando bússola...
#udp_recv_hmc5883l | head -n1
echo Rodando câmera...
#./pixy_test | head -n1

echo Tomada 1
read
echo Iniciada
nohup ./main

echo Tomada 2
read
echo Iniciada
nohup ./main

echo Tomada 3
read
echo Iniciada
nohup ./main
