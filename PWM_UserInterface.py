import serial
import time

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

def checkBoot():
	data = arduinoRead()
	if (not arduinoEquals("BOOTED!", data)):
		print("Arduino did not boot properly. Exiting")
		exit()


while (True):
	try:
		arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600)
		print("Connection found! Waiting for Arduino to boot")
		checkBoot()
		print("Arduino booted!")
		break
	except serial.serialutil.SerialException:
		a = 0
	time.sleep(0.5)

if (not checkSavingMode()):
	print("Arduino did not enter Saving mode. Exiting")
	exit()

print("Arduino ready for receiving data!")

while True:
	data = input("Send data: ")
	write(data)
