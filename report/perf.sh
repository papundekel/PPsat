#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Invalid argument count."
    echo "PPsat-measure.sh <solver> <inputs_dir>"
    exit 1
fi

solver="$1"
inputs_dir="$2"

parse_output()
{
    sed "s/$/,$option_cdcl,$option_adjacency,$option_clause,$option_decision,$option_virtual,$is_sat/"
}

echo "value,?1,counter,?2,?3,ratio,?5,cdcl,adjacency,clause,decision,virtual,sat"

for option_cdcl in "true" "false"
do
for option_adjacency in "list" "set" "set_unordered"
do
for option_clause in "basic" "counting" "watched_literals"
do
for option_decision in "trivial" "deterministic" "random" "JW_static" "VSIDS"
do
for option_virtual in "true" "false"
do
    options="-random 1 -cdcl $option_cdcl -adjacency $option_adjacency -clause $option_clause -decision $option_decision -virtual $option_virtual -output csv"

    for is_sat in "true" "false"
    do
        if $is_sat
        then
            file_prefix="uf"
        else
            file_prefix="uuf"
        fi
        for i in "10" "11" "12" "13" "14"
        do
            perf stat -x, -e cycles,instructions,page-faults,L1-dcache-loads,L1-dcache-load-misses,branches,branch-misses \
            "$solver" $options "$inputs_dir/${file_prefix}100-0$i.cnf" 2>&1 >/dev/null | parse_output
        done
    done
done
done
done
done
done
