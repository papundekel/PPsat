#!/bin/bash

mkdir -p "$2/PPsat-queens/glucose" "$2/PPsat-queens/cadical" "$2/PPsat-queens/PPsat"

i=10
while [ $i -ne 350 ]
do
        #glucose "$1/PPsat-queens/$i.cnf" > "$2/PPsat-queens/glucose/$i.out"
        cadical "$1/PPsat-queens/$i.cnf" > "$2/PPsat-queens/cadical/$i.out"
        #PPsat "$1/PPsat-queens/$i.cnf" > "$2/PPsat-queens/PPsat/$i.out"

        i=$(($i + 10))
done
