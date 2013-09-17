#!/usr/bin/python

import os

if os.name == "nt":
		command = "dir"
else:
		command = "ls -lrt"

os.system(command)

print(os.name)
