#!/usr/bin/python
# encoding=utf-8

import re

text = "you're no fun anymore..."

#fun替换为entertaining
print re.sub("fun","entertaining",text)

#非字符替换为-
print re.sub("[^\w]+","-",text)

#把非空格替换为-BEEP-
print re.sub("\S+","-BEEP-",text)
