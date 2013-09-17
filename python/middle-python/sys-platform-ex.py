#!/usr/bin/python

import sys

if sys.platform == "win32":
		import ntpath
		pathmodule = ntpath
elif sys.platform == "mac":
		import macpath
		pathmodule = macpath
else:
		import posixpath
		pathmodule = posixpath

print pathmodule
