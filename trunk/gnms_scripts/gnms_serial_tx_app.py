#! /usr/bin/python

import serial
from os import system

# kill the rx process
system('killall gnms_serial_rx_app.py')

# open the serial port
s = serial.Serial(port=0, parity=serial.PARITY_NONE,
 	bytesize=serial.EIGHTBITS, stopbits=serial.STOPBITS_ONE,
	 timeout=2, xonxoff=0, rtscts=0, baudrate=9600)

# send the PWR signal
s.write('P')



