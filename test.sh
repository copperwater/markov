#!/bin/bash

./test quotes.txt -l | \
while read line
do
    if grep -Fxq "$line" quotes.txt; then
	:
    else
	echo $line
    fi
done
