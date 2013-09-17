#!/usr/bin/python

import sys

print("path type",type(sys.path))
print("path has",len(sys.path),"members")

sys.path.insert(0,"samples")
import sample

del sample
sys.path = []
import math

