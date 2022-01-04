#!/bin/bash
place="Dhaka"

IFS=$'\n'
readarray -t arrFrndt <./friends.txt
readarray -t arrPlc <./places.txt

[ -e "1705087_output.csv" ] && rm "1705087_output.csv"
[ -e "1705087_output.txt" ] && rm "1705087_output.txt"
touch "1705087_output.csv"
touch "1705087_output.txt"

c=9999

for host in ${arrPlc[@]}; do
  i=$(grep -o -i $host visited.csv | wc -l)
  echo $host " " $i>> 1705087_output.csv
  echo $host " " $i>> 1705087_output.txt
  if(($i<$c))
    then
        c=$i
        place=$host
  fi
done

echo "The least visited place $place ">> 1705087_output.txt
echo "The least visited place $place ">> 1705087_output.csv