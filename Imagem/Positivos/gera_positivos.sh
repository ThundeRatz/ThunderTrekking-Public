#!/bin/bash

while read file ; do
	echo "$file 1 0 0 `identify -format "%w %h" $file`"
done
