#! /usr/bin/python

import serial
from os import system
from string import strip

s = serial.Serial(port=0, parity=serial.PARITY_NONE,
 	bytesize=serial.EIGHTBITS, stopbits=serial.STOPBITS_ONE,
	 timeout=2, xonxoff=0, rtscts=0, baudrate=9600)

f = open("/home/grp6/477grp6/trunk/gnms_scripts/serial.log", "w")
f.write("GNMS Serial Listener\n")
f.close()

# run forever
while 1:
	inputStr = s.readline()
	if not inputStr is '':
		f = open("/home/grp6/477grp6/trunk/gnms_scripts/serial.log", "a")
		f.write(inputStr)
		if strip(inputStr) == 'V++':
			# turn up the volume
			f.write("turning up volume\n")
			system('aumix -v+')
		elif strip(inputStr) == 'V--':
			# turn down the volume
			f.write("turning down volume\n")
			system('aumix -v-')
		f.close()



