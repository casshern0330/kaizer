#!/usr/bin/python

import time

now = time.localtime(time.time())


print now
print time.asctime(now)
print time.strftime("%y%m%d %H:%M",now)
print time.strftime("%a %b %d",now)
print time.strftime("%c",now)
print time.strftime("%I %p",now)
print time.strftime("%Y-%m-%d %H:%M:%S %Z",now)
