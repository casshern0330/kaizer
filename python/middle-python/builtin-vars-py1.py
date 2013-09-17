#!/usr/bin/python
# Filename : builtin-vars-ex1.py

book="library2"
pages=250
scripts=350

print "the %(book)s book contains more than %(scripts)s scripts" % vars()
