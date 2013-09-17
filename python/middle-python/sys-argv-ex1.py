#!/usr/bin/python

import sys

print "sys.argv type",type(sys.argv)

print("script name is",sys.argv[0])

if(len(sys.argv) > 1):
		print ("there are",len(sys.argv)-1,"arguments")
		for arg in sys.argv[1:]:
				print arg
else:
		print "there are no arguments!"
