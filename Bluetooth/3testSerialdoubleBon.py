import serial
bonbon1 = serial.Serial("/dev/rfcomm0")
bonbon2 = serial.Serial("/dev/rfcomm1")

# Read line
while True:
	# Temporary Command Line Interface for Testing
	userIn = input("Input Command (emotions h, s, a; quit with q): ")
	if userIn == "q":
		break
	elif userIn == "h":
		bonbon1.write(b'h')
	elif userIn == "s":
		bonbon1.write(b's')
	elif userIn == "a":
		bonbon1.write(b'a')
	elif userIn == "H":
		bonbon2.write(b'h')
	elif userIn == "S":
		bonbon2.write(b's')
	elif userIn == "A":
		bonbon2.write(b'a')
	while bonbon1.in_waiting:
		print(bonbon1.read())
	while bonbon2.in_waiting:
		print(bonbon2.read())
