#!/bin/bash

for N in $1
do
	echo $N
 	echo "(3-2*x1)*x1-2*x2+1"
	for i in $(seq 2 $((N-1)))
	do
		echo "(3-2*x$((i)))*x$((i))-x$((i-1))-2*x$((i+1))+1"
	done
 	echo "(3-2*x$((N)))*x$((N))-x$((N-1))+1"
 	for i in $(seq 1 $N)
	do
		echo -n "-1 "
	done
 	echo "\n0 20\n"
done
