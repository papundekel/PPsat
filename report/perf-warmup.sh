#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Invalid argument count."
    echo "PPsat-warmup.sh <solver> <inputs_dir>"
    exit 1
fi

solver="$1"
inputs_dir="$2"

parse_output()
{
    grep "^c " | sed 's/^c //' | sed "s/$/,$option_adjacency,$option_virtual/"
}

echo "parse,solve,decision,unit,visit,restart,adjacency,virtual,iterations"

for option_adjacency in "vector" "list" "set" "set_unordered"
do
for option_virtual in "true" "false"
do
    options=" -iterations 5 -random 1 -subprogram solve -output csv -decision deterministic -cdcl false -clause watched_literals -adjacency $option_adjacency -virtual $option_virtual"

    for i in "10" "11" "12" "13" "14"
    do
        "$solver" $options "$inputs_dir/uuf100-0$i.cnf" | parse_output
    done
done
done
