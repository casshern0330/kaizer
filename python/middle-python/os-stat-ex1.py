#!/usr/bin/python

import os
import time

file="sample/sample.txt"

st = os.stat(file)

def dump(st):
		mode,ino,dev,nlink,uid,gid,size,atime,mtime,ctime = st
		print "-size:",size,"bytes"
		print "-owner:",uid,gid
		print "-created:",time.ctime(ctime)
		print "-last accessed:",time.ctime(atime)
		print "-last modified:",time.ctime(mtime)
		print "-mode:",oct(mode)
		print "-iode/dev:",ino,dev


print "stat",st
dump(st)
print

#get stats for an open file
fp = open(file)
st = os.fstat(fp.fileno())

print "fstat",st
dump(st)

st = os.stat(file)
print "open stat",st
dump(st)
fp.close()
