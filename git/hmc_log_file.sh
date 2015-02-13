#!/bin/bash
timestr="logs/hmc_log-`date '+%Y-%m-%d %H:%M:%S'`"
sudo ./hmc5883l-log > "$timestr"
echo $timestr
cut -s -f1 --complement < "$timestr" > trekking.dat
