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
    grep "^c " | sed 's/c //' | tr -d "\n"
    echo ",$option_cdcl,$option_adjacency,$option_clause,$option_decision,$is_sat,$var_count"
}

echo "parse,solve,decision,unit,visit,restart,cdcl,adjacency,clause,decision,sat,var"

for option_cdcl in "true" "false"
do
for option_adjacency in "list" "set" "set_unordered"
do
for option_clause in "basic" "counting" "watched_literals"
do
for option_decision in "trivial" "random" "JW_static" "VSIDS"
do
    options="-random 1 -cdcl $option_cdcl -adjacency $option_adjacency -clause $option_clause -decision $option_decision -output csv"

    for var_count in "50" "75" "100" "125"
    do
        for is_sat in "true" "false"
        do
            if $is_sat
            then
                file_prefix="uf"
            else
                file_prefix="uuf"
            fi
            for i in "10" "11" "12" "13" "14" "15" "16" "17" "18" "19"
            do
                "$solver" "$inputs_dir/$file_prefix$var_count-0$i.cnf" $options | parse_output
            done
        done
    done
done
done
done
done
