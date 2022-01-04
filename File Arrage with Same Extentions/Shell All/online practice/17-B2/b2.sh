#!/bin/bash

[ -e "1705087_output.csv" ] && rm "1705087_output.csv"

student=()
while read line
do 
    #echo $line | cut -d' ' -f1
    student+=($(echo $line | cut -d',' -f1))
done < input.csv 


#echo "${student[@]}"
ss=($(printf "%s\n" "${student[@]}" | sort -u | tr '\n' ' '))
#echo "${ss[@]}"

for id in ${ss[@]}; do
    i=$(grep -o -i $id input.csv | wc -l)
    str="$id,$i,"
    strZ=",,"
    res=($(grep -n $id input.csv|cut -d',' -f2-3))
    flag=true
    for r in ${res[@]}; do
    #echo $r
        if $flag
            then
                flag=false
                ans=$str$r
            else
                ans=$strZ$r
        fi
        #echo $ans
        echo $ans>> 1705087_output.csv
    done

done