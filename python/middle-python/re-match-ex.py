#!/usr/bin/python
# coding=UTF-8

# match从字符串的其实位置开始匹配

import re

text = "The Attila the Sun Show,787"

m = re.match(".",text)
if m:
		print repr("."),"=>",repr(m.group(0))

m = re.match(".*",text)
if m:
		print repr(".*"),"=>",repr(m.group(0))

m = re.match("\w+",text)
if m:
		print repr("\w+"),"=>",repr(m.group(0))

m = re.match("\d+",text)
if m:
		print repr("\d+"),"=>",repr(m.group(0))

