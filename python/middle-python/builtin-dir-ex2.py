#!/usr/bin/python

class A:
		def a(self):
				pass
		def b(self):
				pass

class B(A):
		def c(self):
				pass
		def d(self):
				pass

def getmembers(klass,member=None):
		if member == None:
				member = [];

		for k in klass.__bases__:
				getmembers(k,member);

		for m in dir(klass):
				if m not in member:
						member.append(m)
		return member

print getmembers(A)
print getmembers(B)
print getmembers(IOError)
