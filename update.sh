#!/bin/bash

if [ $# -ne 2 ];
	then echo "update.sh input output; wrong number of params"
	exit
fi

input=$1
output=$2
modified="${output%.csv}_mod.csv"

if [ -f "$modified" ]; then
	rm "$modified"
fi


while IFS=';' read -r -a line; do
	surname="${line[0]}"
	name="${line[1]}"
	location="${line[2]}"
	department="${line[3]}"
	section="${line[4]}"
	role="${line[5]}"
	
	found=0
	while IFS=';' read -r -a line2; do
		surname2="${line2[0]}"
		name2="${line2[1]}"
		if [[ "$surname" == "$surname2" && "$name" == "$name2" ]];
		then
			component="${line2[6]}"
			room="${line2[7]}"
			found=1
			continue;
		fi 
	done < "$input"
	if [ $found -eq 0 ];
	then
		echo "$surname;$name;$location;$department;$section;$role;;" >> $modified
	else
		echo "$surname;$name;$location;$department;$section;$role;$component;$room" >> $modified
	fi
done < "$output"

