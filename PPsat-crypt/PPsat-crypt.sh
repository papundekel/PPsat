#!/bin/bash

if [ "$1" == "-help" ]
then
    echo -e "Description:"
    echo -e "\tPrints all models for the input cryptarithm."
    echo -e "\tAccepts the cryptarithm through stdin."
    echo
    echo -e "Usage:"
    echo -e "\tPPsat-crypt.sh <convertor-cmd> <smt-solver-cmd>"
    echo -e "\tPPsat-crypt.sh -help"
    echo
    echo -e "Arguments:"
    echo -e "\tconvertor-cmd\tA command that converts a cryptarithm input into an SMT-LIB formula."
    echo -e "\tsmt-solver-cmd\tAn SMT solver command which accepts SMT-LIB on stdin."
    echo -e "\t\t\tThe model of the formula shall be represented as a list of two element lists"
    echo -e "\t\t\twhich represent the variable assignment."
    echo
    echo -e "Examples:"
    echo -e "\tPPsat-crypt.sh \"PPsat-crypt-generate -repeating\" \"z3 -in\" < input"

    exit 0
fi

generator="$1"
solver="$2"
file_formula=`mktemp`
file_model=`mktemp`
file_models=`mktemp`
trap "rm $file_formula $file_model $file_models" EXIT INT TERM

if [ $# -ge 3 ]
then
    file_formula="$3"
fi

if [ $# -ge 4 ]
then
    file_model="$4"
fi

if [ $# -ge 5 ]
then
    file_models="$5"
fi

$generator > "$file_formula" || { echo "<PPsat-crypt>: The generator failed, quitting." >&2; exit 1; }

get_value=`grep "get-value" < "$file_formula"`

tmp=`mktemp` && grep -v "get-value\|check-sat" < "$file_formula" > "$tmp" && echo >> "$tmp" && mv "$tmp" "$file_formula"

while true
do
    echo "(check-sat) $get_value" | cat "$file_formula" - | $solver > "$file_model" 

    if grep "unsat" < "$file_model" > /dev/null
    then
        break
    fi

    grep -v "sat" < "$file_model" | tr -d "\n" | sed 's/))/))\n/g' >> "$file_models"

    echo "(assert" >> "$file_formula"

    grep -v "sat" < "$file_model"\
    | sed 's/([[:space:]]*\([a-zA-Z]\)/(not (= \1/g'\
    | sed 's/\([0-9]\)[[:space:]]*)/\1))/g'\
    | sed 's/((not/(or (not/' >> "$file_formula"
    

    echo ")" >> "$file_formula"
done

cat - < "$file_models"
