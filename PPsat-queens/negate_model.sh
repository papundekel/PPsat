#!/bin/bash

file_formula="$1"

sed 's/ \([1-9][0-9]*\)/+\1/g' < "$file_formula" | sed 's/-\([1-9][0-9]*\)/ \1/g' | sed 's/+\([1-9][0-9]*\)/-\1/g'
