#!/usr/bin/python
# Filename : reference.py

print 'Simple Assignment'
shoplist = ['apple','mango','carrot','banana']
mylist = shoplist

del shoplist[0]

print 'shoplist is',shoplist
print 'mylist is',mylist

print 'Copy by making a full slice'
mylist = shoplist[:]

del mylist[0]

print 'shoplist is',shoplist
print 'mylist is',mylist

zoo = ['monkey','phalip']

new_zoo = ['tiger','wolf']

new_zoo.append(zoo)

zoo.append('dog')

print 'new_zoo is',new_zoo
print 'zoo is',zoo
