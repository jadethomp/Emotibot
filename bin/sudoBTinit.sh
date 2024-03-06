#!/bin/bash

sudo rfcomm bind 0 B4:E6:2D:D9:F1:6F
sudo rfcomm bind 1 B4:E6:2D:DA:52:C3
sleep 5

echo "BTSerial Ports Open on rfcomm0 and rfcomm1"

exit
