#!/usr/bin/python
# Filenmae:os-remove-ex3.py

import os
import string

def replace(file,search_for,replace_with):

		back = os.path.splitext(file)[0] + ".bak"
		temp = os.path.splitext(file)[0] + ".tmp"

		try:
				os.remove(temp)
		except os.error:
				pass

		fi = open(file)
		fo = open(temp,"w")

		for s in fi.readlines():
				fo.write(string.replace(s,search_for,replace_with))

		fi.close()
		fo.close()

		try:
				os.remove(back)
		except os.error:
				pass

		os.rename(file,back)
		os.rename(temp,file)

file="sample/sample.txt"
replace(file,"hello","tjena")
replace(file,"tjena","hello")
