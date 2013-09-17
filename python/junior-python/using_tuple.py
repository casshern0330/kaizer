#!/usr/bin/python
#Filename using_tuple.py

zoo = ('wolf','elephant','penguin')
print 'Number of animals in the zoo is',len(zoo)
print 'Number of zoo.count',zoo.count('wolf')

new_zoo = ('money','dolphin',zoo)
print 'Number of animals in the new_zoo is',len(new_zoo)
print 'All animal in new_zoo are',new_zoo
print 'Animate brought from old zoo are',new_zoo[2]
print 'Last animal brought from old zoo are',new_zoo[2][2]
