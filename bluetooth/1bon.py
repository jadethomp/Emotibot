import serial
ser = serial.Serial("/dev/rfcomm0")

# Read line
while True:
	# Temporary Command Line Interface for Testing
	userIn = input("Input Command (emotions h, s, a; quit with q): ")
	if userIn == "q":
		break
	elif userIn == "h":
		ser.write(b'h')
	elif userIn == "s":
		ser.write(b's')
	elif userIn == "a":
		ser.write(b'a')
	while ser.in_waiting:
		print(ser.read())
