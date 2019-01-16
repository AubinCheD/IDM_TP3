#!/bin/bash

for STATUSNUMBER in {1..10}
do
	fileName='status'$STATUSNUMBER'.txt';
	./q3 $fileName >> q4.txt;
done
