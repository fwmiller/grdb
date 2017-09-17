#!/bin/bash
for f in *.exp
do
	echo 
	echo "==> " . $f
	./$f
done
