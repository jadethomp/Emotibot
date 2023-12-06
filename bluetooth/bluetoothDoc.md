# Bluetooth Doc

This file contains documentation of the discoveries made by Kyler Smith for the HRI Student Design Challenge in reference to bluetooth connection between a raspberry pi and an ESP32.

## ESP32 Code

The ESP32 can present itself as a bluetooth device using the arduino library "BluetoothSerial". It was simplest to initialize a connection by using the example project "SerialToSerialBT" available in the menu File->Examples->BluetoothSerial.

## Raspberry Pi Code

As of writing, I am still manually pairing and connecting the esp32 to a serial communications port (/dev/rfcomm0) using a series of commands described in the section "connecting to bluetooth device," but this could be automated using a rfcomm.conf file.

### Connecting to a Bluetooth Device

To connect a raspberry pi to a bluetooth device (in this particular case, we want to connect to the now discoverable "ESP32-Slave"), it is helpful to know the MAC Address, but it is possible to connect without it. We will utilize the bluetoothctl command line interface to pair the device, and the rfcomm command to open a port.

#### Bluetoothctl

Type "bluetoothctl" into the command line to open a CLI to view and connect to bluetooth devices. Use the series of commands below to find and connect to a device, where <MAC_ADDRESS> is the mac address of the device you intend to pair:

    bluetoothctl
    scan on
    pair <MAC_ADDRESS>
    trust <MAC_ADDRESS>
    scan off

When you enter "scan on" you should wait until you see the MAC address and / or device name of the ESP32 come up. The scan will list every available bluetooth device, so it may be hard to read with high traffic (as I experienced).

#### rfcomm

Update: There is an easier way to connect a device using rfcomm that doesn't require ctrl+c afterwards, and makes a little more sense:

    sudo rfcomm bind 0 <MAC_ADDRESS>

Where 0 is the comm port number (bonbon1 is typically 0, bonbon2 is typically 1).

Use the following command to open a communications port with the now paired and trusted bluetooth device.

    sudo rfcomm connect /dev/rfcomm0 <MAC_ADDRESS> 1 &

The "&" seems to be essential so that the program remains running and ready to use the port.

Then "hangup" with CTRL+C. rfcomm will remain "running" so that we can read and write to the serial port. We cannot initiate bluetooth comms through rfcomm (for some reason), so we can either use the command line tool screen or a python script. Details are available here:

[https://askubuntu.com/questions/248817/how-to-i-connect-a-raw-serial-terminal-to-a-bluetooth-connection]

#### Interfacing with the port: Screen

Install screen with:

    sudo apt-get install screen

Use screen with:

    sudo screen /dev/rfcomm0

Exit screen with:

    ctrl+A
    ":quit"
    Enter

#### Interfacing with the port: Python

Dependency: pyserial

pip3 install pyserial
