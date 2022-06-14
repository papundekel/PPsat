#!/bin/bash

mkdir "$2/PPsat-queens/"

i=10
while [ $i -ne 350 ]
do
        "$1" "$i" > "$2/PPsat-queens/$i.cnf"
        
        i=$(($i + 10))
done
