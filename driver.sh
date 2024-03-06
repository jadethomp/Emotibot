#!/bin/bash

echo "driver is active"

export PATH=$PATH:/usr/local/bin/
export XDG_RUNTIME_DIR="/run/user/1000"

#sleep 30

echo "about to start pulse audio"

pulseaudio --start > /home/pi/logs/pulseOutput.log

echo "pulse audio started"

sleep 5

/home/pi/bon/bonbonHut.py > /home/pi/logs/bonbonHut.log 2>&1

echo "python started"

exit
