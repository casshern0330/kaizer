#!/usr/bin/python

import os

try:
		fp=open("test/multiple/levels/file1","w")
except IOError:
		print "open error"
		pass

os.makedirs("test/multiple/levels")

fp = open("test/multiple/levels/file1","w")
fp.write("inspector praline")
fp.close()

os.remove("test/multiple/levels/file1")

os.removedirs("test/multiple/levels")
