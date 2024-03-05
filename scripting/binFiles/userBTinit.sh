#!/bin/bash

bluetoothctl -- pair 50:91:99:EE:17:3F
bluetoothctl -- trust 50:91:99:EE:17:3F
bluetoothctl -- connect 50:91:99:EE:17:3F
sleep 5
bluetoothctl -- connect 50:91:99:EE:17:3F

exit
