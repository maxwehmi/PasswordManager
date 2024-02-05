import serial
import time

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600)

def arduinoRead():
	data = arduino.read()
	time.sleep(0.05)
	data_left = arduino.inWaiting()
	data += arduino.read(data_left)
	return data

def arduinoEquals(string, data):
	string = string + "\r\n"
	byte_data = string.encode("utf-8")
	return (data == byte_data)

def write(string):
	arduino.write(bytes(string, 'utf-8'))
	data = arduinoRead()
	if (string == "wipe"):
		if (arduinoEquals("Wiped!", data)):
			print("Successfully wiped data!")
		else:
			print("Did not wipe properly!")
	else:
		if (arduinoEquals("I received: " + string, data)):
			print("Successfully transmitted data!")
		else:
			print("Did not send data properly!")

def checkSavingMode():
	data = arduinoRead()
	return arduinoEquals("Entering Saving mode", data)


if (not checkSavingMode()):
	print("Arduino did not enter Saving mode. Exiting")
	exit()

while True:
	data = input("Send data: ")
	write(data)
