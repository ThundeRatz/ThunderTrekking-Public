#!/bin/bash
sudo adb kill-server
sudo adb start-server
sudo adb forward tcp:44148 tcp:44148
sudo killall gpsd
sudo gpsd "tcp://*:44148"
gpsmon
