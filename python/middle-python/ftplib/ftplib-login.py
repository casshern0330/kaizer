#!/usr/bin/python

import ftplib

ftp = ftplib.FTP("172.21.0.36")
ftp.login("wngcc","wngcc")

print ftp.dir()

ftp.quit()
