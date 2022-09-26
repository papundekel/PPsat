#!/bin/bash

if [ $# -ne 3 ]
then
    echo "Invalid argument count."
    echo "PPsat-instr.sh <solver> <pintool> <inputs_dir>"
    exit 1
fi

solver="$1"
pintool="$2"
inputs_dir="$3"

echo "count,adjacency,virtual"

for option_adjacency in "vector" "list" "set" "set_unordered"
do
for option_virtual in "true" "false"
do
for i in "10" "11" "12" "13" "14"
do
    pin \
        "-t" "$pintool" \
        "--" \
        "$solver" \
            "-subprogram" "solve" \
            "-output" "none" \
            "-iterations" "5" \
            "-random" "1" \
            "-decision" "deterministic" \
            "-cdcl" "false" \
            "-clause" "watched_literals" \
            "-adjacency" "$option_adjacency" \
            "-virtual" "$option_virtual" \
            "$inputs_dir/uuf50-0$i.cnf"
    echo ",$option_adjacency,$option_virtual"
done
done
done
