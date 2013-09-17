#!/usr/bin/python

import socket
import struct,time,sys

print sys.argv

if sys.argv > 2:
		HOST,PORT = sys.argv[1:3]
else:
		HOST = "localhost"
		PORT = "8037"

TIME1970 = 2208988800L

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,int(PORT)))

t = s.recv(4)

t = struct.unpack("!I",t)[0]

t = int(t - TIME1970)

s.close()

print "server time is",time.ctime(t)
print "local clock is",int(time.time()) - t,"seconds off"
