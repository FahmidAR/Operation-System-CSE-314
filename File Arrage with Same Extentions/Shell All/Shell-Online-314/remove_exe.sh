#!/bin/bash
#1705087

echo "1705087-Online"

find . -name "{1..9}" -exec chmod +x {} \;

DIR_WORKING_INPUT="."

if (($#==1))
then
    if [[ -d $1 ]]
        then
            DIR_WORKING_INPUT="$1"
    fi
fi



#ls -lR

[ -e "1705087_output.txt" ] && rm "1705087_output.txt"
touch "1705087_output.txt"

cur=($(ls $DIR_WORKING_INPUT))

for f in "${cur[@]}";
do 
    str=$f
    i=$($(find $DIR_WORKING_INPUT -type f | perl -ne 'print $1 if m/\.([^.\/]+)$/' | sort -u)| wc -l)
    echo "$str">>1705087_output.txt
done

files=($(find $DIR_WORKING_INPUT -type f))

for f in "${files[@]}";
do  
    [ -x $f ] && echo "$f is executable"
    [ -x $f ] || echo "$f is not executable"

    [ -x $f ] || chmod 666 $f 
    echo "$f exicutable permission have been updated"
done