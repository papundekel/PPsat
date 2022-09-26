#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Invalid argument count."
    echo "PPsat-profiling.sh <solver> <inputs_dir>"
    exit 1
fi

solver="$1"
inputs_dir="$2"

perf record \
    "-g" \
    "-o" "results-profiling.data" \
    "$solver" \
        "-subprogram" "solve" \
        "-output" "none" \
        "-iterations" "5" \
        "-random" "1" \
        "-decision" "deterministic" \
        "-cdcl" "false" \
        "-clause" "watched_literals" \
        "-adjacency" "vector" \
        "-virtual" "true" \
        "$inputs_dir/uuf125-010.cnf"

rm -f "results-profiling.data.old"
