#!/bin/bash

if [ "$1" == "-help" ]
then
    echo -e "Description:"
    echo -e "\tPrints all models of the N queens puzzle."
    echo -e "\tThe size N should be a parameter of the generator of the CNF formula."
    echo
    echo -e "Usage:"
    echo -e "\tPPsat-queens.sh <generator-cmd> <sat-solver-cmd>"
    echo -e "\tPPsat-queens.sh -help"
    echo
    echo -e "Arguments:"
    echo -e "\tgenerator-cmd\tA command that generates a CNF formula in the DIMACS format"
    echo -e "\t\t\tthat represents the N queens puzzle."
    echo -e "\tsat-solver-cmd\tA SAT solver command which accepts DIMACS on stdin."
    echo -e "\t\t\tSatisfiability of the formula shall be represented with output containing a line \"s UNSATISFIABLE\" or \"s SATISFIABLE\"."
    echo -e "\t\t\tThe model of the formula shall be represented with output containing a line \"v <LITERAL>... 0\"."
    echo
    echo -e "Examples:"
    echo -e "\tPPsat-queens.sh \"PPsat-queens-generate 4\" cadical"
    echo -e "\tPPsat-queens.sh \"PPsat-queens-generate 5\" \"glucose -model\""
    echo -e "\tPPsat-queens.sh \"PPsat-queens-generate 6\" PPsat"

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

$generator > "$file_formula" || { echo "<PPsat-queens>: The generator failed, quitting." >&2; exit 1; }

while true
do
    $solver < "$file_formula" | grep "^\(s\|v\)" > "$file_model" || { echo "<PPsat-queens>: The solver failed, quitting." >&2; exit 2; }

    if grep "UNSATISFIABLE" < "$file_model" > /dev/null
    then
            break
    fi

    tmp=`mktemp` && sed -n 's/^v //p' < "$file_model" | tr '\n' ' ' > "$tmp" && echo >> "$tmp" && mv "$tmp" "$file_model"
    
    sed 's/\([1-9][0-9]*\)/-\1/g' < "$file_model" | sed 's/--//g' >> "$file_formula"

    sed 's/ \([1-9][0-9]*\)/  \1/g' < "$file_model" | sed 's/^\([1-9]\)/ \1/' >> "$file_models"
    
    clause_count=`grep 'p cnf' < "$file_formula" | grep -o '[0-9]\+$'`
    clause_count=$(($clause_count + 1))

    sed -i "s/\(.*p *cnf *[0-9]\+ *\)[0-9]\+\(.*\)/\1$clause_count\2/" "$file_formula"
done

sort < "$file_models"
