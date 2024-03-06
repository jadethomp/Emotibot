# bonbon

Codebase for a smart tool used for multimodal affect communication. HRI 2024 Student Design Challenge: "Emotibot: A Tool for Multi-Sensory Affect Communication"

This README is a work in progress!

## Setup

Install dependencies:

? sudo apt-get install libsdl-ttf2.0-0
? sudo apt-get install libsdl2-mixer-2.0-0
sudo apt install python3-pip
pip install pyserial
(there may be more...brute force)

Configure crontab by running the command `crontab -e`. Add the following lines to your user's crontab:

`@reboot sleep 20 && /home/pi/bin/userBTinit.sh > /home/pi/logs/userBTinit.log

@reboot sleep 30 && /home/pi/driver.sh > /home/pi/logs/driverOutput.log && echo "done" > /home/pi/logs/done.log`

## Running the code

Load bonbon1.ino onto one ESP32, and bonbon2.ino onto another ESP32. 

Load the bon directory, bin directory, and driver.sh file into /home/pi/ on a Raspberry Pi (we used Raspberry Pi OS (Legacy, 32-bit) Lite - Debian Bullseye).

NOTE: The sudoBTinit.sh and userBTinit.sh scripts must be configured with the MAC addresses of your specific ESP's and speaker unit.

Reboot your Raspberry Pi. Ensure that the two speakers are on and connected to each other, and that the two ESPs are on and running normally.

If configured correctly, crontab should initialize all Bluetooth connections and begin the main loop.
