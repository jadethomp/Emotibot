import serial

bonbon1 = serial.Serial('/dev/rfcomm0')
bonbon2 = serial.Serial('/dev/rfcomm1')

while(True):
	userIn = input("enter h or n to send to bonbon1, and H or N to send to bonbon2!")
	if(userIn == "q"):
		break
	elif(userIn == "h"):
		bonbon1.write(b'h')
	elif(userIn == "n"):
		bonbon1.write(b'n')
	elif(userIn == "H"):
		bonbon2.write(b'h')
	elif(userIn == "N"):
		bonbon2.write(b'n')

bonbon1.reset_output_buffer()
bonbon2.reset_output_buffer()
