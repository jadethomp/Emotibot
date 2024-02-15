import time

lastTransmit = time.time()

interval = 0

for i in range(60):
    print("Seconds:", i)
    while interval <= 1:
        now = time.time()
        interval = now - lastTransmit
    lastTransmit = now
    interval = now - lastTransmit