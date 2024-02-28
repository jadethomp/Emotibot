import serial

bonbon1 = serial.Serial('/dev/rfcomm0')
bonbon2 = serial.Serial('/dev/rfcomm1')

while(True):
	userIn = input("enter 1 or 2 to read from bonbon1 or bonbon2!")
	serialIn = ""
	if(userIn == "q"):
		break
	elif(userIn == "1"):
		serialIn = bonbon1.read()
	elif(userIn == "2"):
		serialIn == bonbon2.read()
	print(serialIn)

bonbon1.reset_output_buffer()
bonbon2.reset_output_buffer()
