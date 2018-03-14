#!/bin/bash

for ((i=1; i<=6; i++)) 
do
    cd disk$i
    rm disk$i.dat
    cd ..
done
