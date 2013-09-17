#!/usr/bin/python

import os,sys

program="python"
arguments = ["hello.py"]

print os.execvp(program,(program,) + tuple(arguments))

print "goodbaby"
