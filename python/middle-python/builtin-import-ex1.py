#!/usr/bin/python
# Filename: builtin-import-ex1.py

import glob,os

modules = []

for module_file in glob.glob("*-plugin.py"):
				try:
						module_name,ext=os.path.splitext(os.path.basename(module_file))
						module = __import__(module_name)
						modules.append(module)
				except ImportError:
						pass

for module in modules:
		module.hello()
