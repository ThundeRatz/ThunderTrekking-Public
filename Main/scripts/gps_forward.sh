#!/bin/bash
sudo adb kill-server
sudo adb start-server
sleep 1
sudo adb forward tcp:44148 tcp:44148
sleep 1
sudo killall gpsd
sudo gpsd "tcp://*:44148"
gpsmon
