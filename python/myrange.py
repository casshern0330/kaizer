#!/usr/bin/python

class MyRange:
		def __init__(self,begin,end):
				self.list=[begin]
				temp = begin
				while(begin<end):
						begin += 1
						self.list.append(begin)
