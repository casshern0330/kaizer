#!/usr/bin/python

import fileinput
import sys

for line in fileinput.input("samples/sample.txt"):
		if fileinput.isfirstline():
				sys.stderr.write("--reading %s--\n" % fileinput.filename)
		sys.stdout.write("->")
		sys.stdout.write(line)
