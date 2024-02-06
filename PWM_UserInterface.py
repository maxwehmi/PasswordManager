import serial
import os.path
import time

filename = "credentials.csv"

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
	elif (string == "sendAll"):
		return
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

def sendFile():
	if (not ensureFile()):
		print("File was empty, so nothing was sent!")
		return
	file = open(filename, 'r')
	temp = file.read().splitlines()
	for line in temp:
		print(line)
		write(line)
	file.close()

def getData():
	ensureFile()
	write("sendAll")
	data = arduinoRead()
	file = open(filename, 'a')
	print(data) # save to file instead
	file.close()

def importData(): 
	# TODO
	return

def enterCredentials():
	code = input("Please enter the Code for the encryption:")
	name = input("Please enter the name of the new set of credentials:")
	username = input("Please enter the username:")
	pw = input("Please enter the password:")
	pw = encrypt(pw,code)
	ensureFile()
	file = open(filename, 'a')
	cred = name + "	" + username + "	" + pw + "\n"
	file.write(cred)
	file.close()
	print("Added the new set of credentials!")

def encrypt(string,code):
	# TODO
	return string

def ensureFile():
	if (os.path.isfile(filename)):
		return True
	else: 
		file = open(filename, 'w')
		file.close()
		return False


print("Please connect the Arduino.")

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
	command = input("What would you like to do?")
	if (command == "S"): # (s)end file to stick
		sendFile()
	elif (command == "G"): # (g)et all data from the stick
		getData()
	elif (command == "I"): # (i)mport data from file
		importData()
	elif (command == "E"): # (e)nter new credentials
		enterCredentials()
	else:
		print("Command not understood. Please try again.")
