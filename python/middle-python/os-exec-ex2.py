#!/usr/bin/python

import os
import sys
import time

def run(program,*args):
		pid = os.fork()
		if not pid:
				os.execvp(program,(program,) + args)
		return os.wait()[0]

print run("python","hello.py")

time.sleep(10)

print "goodbaby"
