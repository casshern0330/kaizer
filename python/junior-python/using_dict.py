#!/usr/bin/python
# Filename : using_dict.py

ab = {	'Swaroop':'swaroop@byteofpython.info',
		'Larry':'larr@wall.org',
		'Matsumoto':'matz@ruby-lang.org',
		'Spammer':'spammer@hotmail.com'
		}

print "Swaroop's address is %s" % ab['Swaroop']

ab['Guido'] = 'guido@python.org'

del ab['Spammer']

print '\nThera are %d contacts in address-book\n' % len(ab)

for name,address in ab.items():
		print 'Contact %s at %s' % (name,address)

if 'Guido' in ab:
		print "\nGuido's address is %s" % ab['Guido']
