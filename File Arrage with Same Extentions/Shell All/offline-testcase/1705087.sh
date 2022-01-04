#!/bin/bash
DIR_FOR_OUTPUT="Fahmid_1705087_OUTPUT"
INPUT_FILE="input.txt"
DIR_WORKING_INPUT="."
#DIR_WORKING_INPUT="./working_dir/"

file_input_function () {
INPUT_FILE="$1"
while [ true ]
do
  if [[ -f $INPUT_FILE ]]
  then
      break
  else
    echo "Please give input file with extentions list which not to arrange = "
    read INPUT_FILE
  fi
  echo "Working with Valid $INPUT_FILE File , Thanks "
done
}

if (($#==1))
then
  INPUT_FILE="$1"
  file_input_function $INPUT_FILE
elif (($#==2))
then
  #alias cd $DIR_WORKING_INPUT
  if [[ -d $1 ]]
  then
      DIR_WORKING_INPUT="$1"
  else
    echo "As directory given is not correct , Root working dir have been set , Thanku"
  fi
  INPUT_FILE="$2"
  file_input_function $INPUT_FILE
else
  file_input_function $INPUT_FILE
fi

createFolder() {
  deleteFolder $1
  mkdir "$1"
}

deleteFolder() {
  if [ -d "$1" ]; then
    rm -d -r -f "$1"
  fi
}

createFolder $DIR_FOR_OUTPUT

readarray -t arrNotExt <./$INPUT_FILE
#printf '%s\n' "${arrNotExt[@]}"

IFS=$' '
wantedFile=( $(eval ${VAR}))
printf '%s\n' "${wantedFile[@]}"

allExt=( $(find $DIR_WORKING_INPUT -type f | perl -ne 'print $1 if m/\.([^.\/]+)$/' | sort -u) )
#printf '%s\n' "${allExt[@]}"

for host in ${arrNotExt[@]}; do
  allExt=( "${allExt[@]/$host}" )
done
#printf '%s\n' "${allExt[@]}"

IFS=$'\n'
touch "1705087_output.csv"
{ echo "File_Type,No_Of_FIles"; cat 1705087_output.csv; } > 1705087_output.csv

for i in ${allExt[@]}; do
  #echo ""
  #echo $i
  mkdir ./$DIR_FOR_OUTPUT/$i
  c=($(find $DIR_WORKING_INPUT -name "*".$i))
  find $DIR_WORKING_INPUT -name "*".$i -print > ./$DIR_FOR_OUTPUT/$i/desc_$i.txt
  find $DIR_WORKING_INPUT -name "*".$i -exec cp {} ./$DIR_FOR_OUTPUT/$i \; 
  echo "$i,${#c[@]}" >> 1705087_output.csv
done

#echo ""
#echo "other"
mkdir ./$DIR_FOR_OUTPUT/others
c=($(find $DIR_WORKING_INPUT -type f ! -name "*.*"))
find $DIR_WORKING_INPUT -type f ! -name "*.*" -print > ./$DIR_FOR_OUTPUT/others/desc_others.txt
find $DIR_WORKING_INPUT -type f ! -name "*.*" -exec cp {} ./$DIR_FOR_OUTPUT/others \;
echo "others,${#c[@]}" >> 1705087_output.csv

VAR="find ./working_dir \("
failed=true

for i in "${arrNotExt[@]}";
do  
  if $failed
  then 
    VAR+=" -name \"*.\"${i}"
    failed=false
  else 
    VAR+=" -o -name \"*\".${i}"
  fi
done

VAR+=" \) -type f| sort -u"
c=($(eval $VAR))
echo "ignored,${#c[@]}" >> 1705087_output.csv
