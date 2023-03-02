#!/bin/bash

sed 's/?//g' $1 > input.txt

grep legal input.txt
if [ $? == 0 ]
then
	echo 'yes' >> input.txt
else
	exit 0
fi

i=0
for s in $(cat input.txt)
do
	let i=0
	while ((${s:i:1} != 1))
	do
		let i+=1
	done
	break
done

let i+=1
echo $i >> input.txt
