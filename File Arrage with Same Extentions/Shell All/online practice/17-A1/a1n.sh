#!/bin/bash

files=( $(find ./working_dir -type f -exec wc -l {} + | sort -rn ) )
i=-2

for f in ${files[@]}; do
    filename=$(basename -- "$f")
    extension="${filename##*.}"
    if [[ -f $f ]]
        then
            cp $f ./$i.$extension
    fi
    i=$((i+1))
done