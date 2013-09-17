#!/usr/bin/python
#Filename builtin-apply-ex2.py

def function(a,b):
		print a,b

apply(function,("crunchy","frog"))
apply(function,("crunchy",),{"b":"frog"})
apply(function,(),{"b":"frog","a":"crunchy"})
