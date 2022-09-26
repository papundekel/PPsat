#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Invalid argument count."
    echo "PPsat-perf.sh <solver> <inputs_dir>"
    exit 1
fi

solver="$1"
inputs_dir="$2"

parse_output()
{
    sed "s/$/,$option_adjacency,$option_virtual/"
}

echo "value,?1,counter,?2,?3,ratio,?5,adjacency,virtual"

for option_adjacency in "vector" "list" "set" "set_unordered"
do
for option_virtual in "true" "false"
do
    options="-random 1 -iterations 5 -subprogram solve -output none -decision deterministic -cdcl false -clause watched_literals -adjacency $option_adjacency -virtual $option_virtual"

    for i in "10" "11" "12" "13" "14"
    do
        perf stat -x, -e cycles,page-faults \
        "$solver" $options "$inputs_dir/uuf100-0$i.cnf" 2>&1 >/dev/null | parse_output

        perf stat -x, -e L1-dcache-loads,L1-dcache-load-misses,branches,branch-misses \
        "$solver" $options "$inputs_dir/uuf100-0$i.cnf" 2>&1 >/dev/null | parse_output
    done
done
done
