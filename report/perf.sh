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
    sed "s/$/,$option_cdcl,$option_adjacency,$option_clause,$option_virtual/"
}

echo "value,?1,counter,?2,?3,ratio,?5,cdcl,adjacency,clause,virtual"

for option_cdcl in "true" "false"
do
for option_adjacency in "vector" "list" "set" "set_unordered"
do
for option_clause in "basic" "counting" "watched_literals"
do
for option_virtual in "true" "false"
do
    options="-random 1 -cdcl $option_cdcl -adjacency $option_adjacency -clause $option_clause -decision deterministic -virtual $option_virtual -output csv"

    for i in "10" "11" "12" "13" "14" "15" "16" "17" "18" "19"
    do
        perf stat -x, -e cycles,instructions,page-faults \
        "$solver" $options "$inputs_dir/uuf100-0$i.cnf" 2>&1 >/dev/null | parse_output

        perf stat -x, -e L1-dcache-loads,L1-dcache-load-misses,branches,branch-misses \
        "$solver" $options "$inputs_dir/uuf100-0$i.cnf" 2>&1 >/dev/null | parse_output
    done
done
done
done
done
