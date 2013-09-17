#!/usr/bin/python
# Filename: if.py

number=23
guess=int(raw_input('Enter an integer :'))

if guess == number:
		print 'Congratulations, you guessed it.'
		print "(but you do now win my prizes!)"
elif guess < number:
		print 'No,it is a litter highter than that'
else:
		print 'No,it is a litter lower than tha'

print 'Done'
