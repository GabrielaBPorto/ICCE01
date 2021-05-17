#!/bin/bash

make clean
make

for i in {0..9}
 do
    echo Caso 0$(($i)) >> results/caso-0$(($i)).out
    ./analiseIntervalar < input/caso-0$(($i)).in > results/caso-0$(($i)).out
    echo -------------------------------------------------------
done