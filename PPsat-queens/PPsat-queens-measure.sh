#!/bin/bash

if [ $# -ne 5 ]
then
    echo "Invalid argument count."
    echo "PPsat-queens-measure.sh <generator> <solver> <max_size>"
    exit 1
fi

generator="$1"
solver="$2"
solver_name="$3"
size_begin="$4"
size_end="$5"
temp=`mktemp`
trap "rm $temp" EXIT INT TERM

parse_output()
{
    if [ "$1" == "PPsat" ]
    then
        grep "^c "\
        | sed 's/c //'\
        | cut -d, -f2\
        | sed 's/\([0-9]\)\([0-9][0-9][0-9]\)$/\1.\2/'\
        | sed 's/^\([0-9][0-9][0-9]\)$/0.\1/'\
        | sed 's/^\([0-9][0-9]\)$/0.0\1/'\
        | sed 's/^\([0-9]\)$/0.00\1/' > "$temp"
    fi

    if [ "$1" == "cadical" ]
    then
        grep "^c.*%.*solve.*" | gawk '{ print $2 }' > "$temp"
    fi

    if [ "$1" == "glucose" ]
    then
        grep "^c CPU time" | gawk '{ print $5 }' > "$temp"
    fi

    for arg in $@
    do
        echo ",$arg" >> "$temp"
    done

    tr -d "\n" < "$temp"
    echo
}

echo "time,solver,size"

for size in `seq $size_begin $size_end`
do
    $generator "$size" | $solver | parse_output "$solver_name" "$size"
done
