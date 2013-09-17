#!/usr/bin/python

import os

cwd = os.getcwd()
print cwd

os.chdir("sample")
print "2",os.getcwd()

os.chdir(os.pardir)
print "3",os.getcwd()
