#!/bin/bash

readarray -t arrCrs <./course.txt
touch "1705087_output.csv"

#echo ${arrCrs[0]}
i=0
student=()

while read line
do 
    #echo $line | cut -d' ' -f1
    student+=($(echo $line | cut -d' ' -f1))
done < ${arrCrs[0]}.txt

#echo "${student[@]}"

[ -e "1705087_output.csv" ] && rm "1705087_output.csv"
touch "1705087_output.csv"

varStr="Student no,"
for text in ${arrCrs[@]}; do
    #echo $text" "
    varStr+=$text","
done
varStr+="Total Mark,Avg Mark,Grade"
echo $varStr>> 1705087_output.csv

totalCrs=${#arrCrs[@]}

for std in ${student[@]}; do
    varStr="$std,"
    totalNumber=0
    avgmark=0
    for text in ${arrCrs[@]}; do
        i=$(grep -n "$std" "$text.txt"|cut -d' ' -f2)
        varStr+=$i","
        totalNumber=$((totalNumber+i))
    done
    avgmark=$((totalNumber/totalCrs))
    varStr+=$avgmark
    varStr+=","
    varStr+=$totalNumber
    varStr+=","

    if((avgmark>=80))
        then
            varStr+="A"
    elif((avgmark>=60))
        then
            varStr+="B"
    elif((avgmark>=40))
        then
            varStr+="C"
    else
        varStr+="D"
    fi

    echo $varStr>> 1705087_output.csv
done