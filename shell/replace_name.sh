#!/bin/bash

svn status | grep ^M | awk '{print $2}' | while read filename
do
	purename=`echo ${filename%.*}`
	prefix=`echo ${filename##*.}`
	newname=`echo ${purename}_55.${prefix}`
	if [ -e $filename ];then
		echo $newname
	else
		echo $filename
	fi
done
