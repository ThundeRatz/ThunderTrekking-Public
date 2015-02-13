#!/bin/bash
set -e
if [ $# -lt 1 ] ; then
	echo "Uso: $0 processo"
	exit 1
fi
sudo ./joystick_kill
sudo killall $1
