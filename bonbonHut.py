# Author: Kyler Smith for the Student Design Challenge
# The main execution script for bonbon control, using the Grove Ultrasonic Ranger (which uses the same pin for trigger and receive)
# The sensor's sig pin is connected to the raspberry pi on gpio pin GPIO_SIG (defaults to 6)

# Printing to the console could be removed but is left as a debug / control PC heartbeat signal

import sys
import os

import RPi.GPIO as GPIO
import time

# this package must be from pip package "pyserial"
import serial
bonbon1BT = serial.Serial("/dev/rfcomm0")
bonbon2BT = serial.Serial("/dev/rfcomm1")

lastTransmit = time.time()

userPresent = False

# The signal pin of the ultrasonic sensor
# Labeled as the Raspberry pi's numerical GPIO pin
GPIO_SIG = 18

# Checks if the ultrasonic sensor detects an obstacle within range "low" to "high"
def checkRange(low, high):
    global userPresent
    global lastTransmit
    print("SeeedStudio Grove Ultrasonic get data and print")

    shouldSend = False
    sendVal = b'h'

    # check distance
    distanceCM = measurementInCM()

    print("Distance : %.1f CM" % distanceCM)

    if distanceCM > low and distanceCM < high and not userPresent:
        print("Person within distance!")
        # bonbon1BT.write(b'h')
        # bonbon2BT.write(b'h')
        sendVal = b'h'
        shouldSend = True
        userPresent = True
    elif distanceCM < low or distanceCM > high and userPresent:
        print("Person out of distance!")
        # bonbon1BT.write(b'n')
        # bonbon2BT.write(b'n')
        sendVal = b'n'
        shouldSend = True
        userPresent = False

    # Enforce an interval
    now = time.time()
    transmitInterval = now - lastTransmit
    if transmitInterval > 3:
        if shouldSend:
            print("Transmitting")
            bonbon1BT.write(sendVal)
            bonbon2BT.write(sendVal)
            shouldSend = False
            lastTransmit = now

def measurementInCM():
    # rpi board gpio or bcm gpio
    GPIO.setmode(GPIO.BCM)

    # setup the GPIO_SIG as output
    GPIO.setup(GPIO_SIG, GPIO.OUT)

    GPIO.output(GPIO_SIG, GPIO.LOW)
    time.sleep(0.2)
    GPIO.output(GPIO_SIG, GPIO.HIGH)
    time.sleep(0.5)
    GPIO.output(GPIO_SIG, GPIO.LOW)
    start = time.time()

    # setup GPIO_SIG as input
    GPIO.setup(GPIO_SIG, GPIO.IN)

    # get duration from Ultrasonic SIG pin
    while GPIO.input(GPIO_SIG) == 0:
        start = time.time()

    while GPIO.input(GPIO_SIG) == 1:
        stop = time.time()

    return calcDistance(start, stop)


def calcDistance(start, stop):
    # Calculate pulse length
    elapsed = stop-start

    # Distance pulse travelled in that time is time
    # multiplied by the speed of sound (cm/s)
    distance = elapsed * 34300

    # That was the distance there and back so halve the value
    distance = distance / 2

    return distance

def main():
    while True:
        checkRange(0, 100)

if __name__ == '__main__':
    try:
        print("Begin Student Design Challenge Loop")
        print("Press CTRL+C to stop")
        main()
    except KeyboardInterrupt:
        GPIO.cleanup()
        print("Keyboard Interrupt! GPIO Cleaned Up\n")
        try:
            sys.exit(130)
        except SystemExit:
            os._exit(130)