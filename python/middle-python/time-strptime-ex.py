#!/usr/bin/python

import time

try:
		strptime = time.strptime
except AttributeError:
		print("load from strptime")
		from strptime import strptime

print strptime("2013-09-14 00:00:00","%Y-%m-%d %H:%M:%S")
print strptime("30 Nov 00","%d %b %y")
print strptime("1 Jan 70 1:30pm","%d %b %y %I:%M%p")
