# run me with sudo

import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)

TRIG = 2
ECHO = 3

# distance measurement in progress

GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

GPIO.output(TRIG, False)

# waiting for sensor to settle
time.sleep(2)

# pulse to trigger the module, to start the ranging program
GPIO.output(TRIG, True)
time.sleep(0.0001)
GPIO.output(TRIG, False)

# now listen to input pin
while GPIO.input(ECHO) == 0:
    pulse_start = time.time()

while GPIO.input(ECHO) == 1:
    pulse_end = time.time()

pulse_duration = pulse_end - pulse_start
distance = round(pulse_duration * 17150, 2)
print("Distance:", distance, "cm")
GPIO.cleanup()
