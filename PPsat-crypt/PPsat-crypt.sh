#!/bin/bash

generator="$1"
solver="$2"

create_temp_file()
{
        local file=`mktemp`
        trap "rm $file" EXIT
        echo "$file"
}

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
touch "$file_models"

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

    grep -v "sat" < "$file_model" >> "$file_models"

    echo "(assert" >> "$file_formula"

    grep -v "sat" < "$file_model"\
    | sed 's/([[:space:]]*\([a-zA-Z]\)/(not (= \1/g'\
    | sed 's/\([0-9]\)[[:space:]]*)/\1))/g'\
    | sed 's/(not/and (not/' >> "$file_formula"

    echo ")" >> "$file_formula"
done

cat - < "$file_models"
