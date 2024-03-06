#!/bin/bash

bluetoothctl -- pair 50:91:99:EE:17:3F
bluetoothctl -- trust 50:91:99:EE:17:3F
bluetoothctl -- connect 50:91:99:EE:17:3F
sleep 5
bluetoothctl -- connect 50:91:99:EE:17:3F

sudo rfcomm bind 0 B4:E6:2D:DA:52:C3
sudo rfcomm bind 1 B4:E6:2D:D9:F1:6F
sleep 5

echo "BTSerial Ports Open on rfcomm0 and rfcomm1"

exit
