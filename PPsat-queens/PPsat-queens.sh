#!/bin/bash

# Example usage
# PPsat-queens.sh "PPsat-queens-generate 8" cadical

create_temp_file()
{
        local file=`mktemp`
        trap "rm $file" EXIT
        echo "$file"
}

generator="$1"
solver="$2"

if [ $# -ge 3 ]
then
        file_formula="$3"
else
        file_formula=`create_temp_file`
fi

if [ $# -ge 4 ]
then
        file_model="$4"
else
        file_model=`create_temp_file`
fi

file_models=`create_temp_file`

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
