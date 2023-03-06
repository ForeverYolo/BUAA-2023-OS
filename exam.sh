#!/bin/bash



mkdir mydir
chmod +x ./mydir
chmod +r ./mydir
chmod +w ./mydir
touch myfile
echo 2023 > myfile
mv moveme ./mydir
cp copyme ./mydir
mv ./mydir/copyme ./mydir/copied
cat readme
gcc bad.c 2>err.txt


mkdir gen
if [ $# -eq 1 ] 
then
	
	for i in $(seq 1 $1)
	do
		touch ./gen/$i.txt
	done
fi

if [ $# -eq 0 ]
then
	for j in {1..10}
	do
		touch ./gen/$j.txt
	done
fi


