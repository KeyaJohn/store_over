#!/bin/bash

for i in 1 2 3 4 5 6 
do
    cd disk$i
    rm  -rf disk$i.dat
    cd ..
done
