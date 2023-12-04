import serial

ser = serial.Serial('/dev/rfcomm0')

while(True):
	userIn = input()
	if(userIn == "q"):
		break
	data = ser.read()
	print(data)

ser.reset_output_buffer()
