#!/bin/bash

generator="$1"
solver="$2"
size="$3"

file_formula=`mktemp`
file_model=`mktemp`

"$generator" "$size" > "$file_formula"

while true
do
        $solver "$file_formula" | grep "^\(s\|v\)" > "$file_model"

        if grep "UNSATISFIABLE" < "$file_model"
        then
                break
        fi

        sed -ni 's/^v //p' "$file_model"
        
        sed 's/\([1-9][0-9]*\)/-\1/g' < "$file_model" | sed 's/--//g' >> "$file_formula"

        sed 's/ \([1-9][0-9]*\)/  \1/g' < "$file_model" | sed 's/^\([1-9]\)/ \1/'
        
        clause_count=`grep 'p cnf' < "$file_formula" | grep -o '[0-9]\+$'`
        clause_count=$(($clause_count + 1))

        sed -i "s/\(.*p *cnf *[0-9]\+ *\)[0-9]\+\(.*\)/\1$clause_count\2/" "$file_formula"
done

rm "$file_formula" "$file_model"
