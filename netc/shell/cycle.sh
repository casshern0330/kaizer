#!/bin/bash

for i in 1 2 3 4 5
do
	echo $i
done

i=0
while [ "$i" -lt 5 ]
do
	echo "while cycle $i"
	((i=i+1))
done
