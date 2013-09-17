#!/usr/bin/python

import os
import stat,time

infile="sample/sample.txt"
outfile="sample/out.txt"

fi = open(infile,"r")
fo = open(outfile,"w")

while 1:
	s = fi.readline()
	if not s:
			break
	fo.write(s)

fi.close()
fo.close()

st = os.stat(infile)
os.chmod(outfile,stat.S_IMODE(st[stat.ST_MODE]))
os.utime(outfile,(st[stat.ST_ATIME],st[stat.ST_MTIME]))

print "original","=>"
print "mode",oct(st[stat.ST_MODE])
print "atime",time.ctime(st[stat.ST_ATIME])
print "mtime",time.ctime(st[stat.ST_MTIME])

print "copy","=>"
st = os.stat(outfile)
print "mode",oct(st[stat.ST_MODE])
print "atime",time.ctime(st[stat.ST_ATIME])
print "mtime",time.ctime(st[stat.ST_MTIME])
