#! /usr/bin/python

import os

diskname = os.listdir('/media')
dirname = []
if diskname:
	dirname  = os.listdir('/media/'+diskname[0])
if dirname:
	usbDir  = '/media/'+diskname[0]+'/'
	tileDir = '/home/rick/Documents/Spring2012/ECE477/477grp6/trunk/gnms_os/apps/tiles/'
	binDir  = '/home/rick/Documents/Spring2012/ECE477/477grp6/trunk/gnms_os/apps/bin/'
	userContent = '~/UserContent/'

	commandStr = 'mkdir -p ' + userContent + ' && cp -r ' + usbDir + '/*' + \
	 ' ' + userContent + ' && cd ' + userContent + dirname[0] + \
	 ' && make clean && make && cp ./tile/* ' + tileDir +  \
	' && cp ./bin/* ' + binDir + ' && chmod +x ' + binDir + '*'

	print 'Loading from %s' % usbDir
	os.system(commandStr)
else:
	print 'No USB inserted'
