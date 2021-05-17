#!/bin/bash

make clean
make

for i in {0..9}
 do
	echo $(($i))
    echo Caso 0$(($i)) >> results/caso-0$(($i)).out
    ./analiseIntervalar < input/caso-0$(($i)).in > results/caso-0$(($i)).out
	diff output/caso-0$(($i)).out results/caso-0$(($i)).out > results/diffcaso-0$(($i))
done