#!/bin/bash

make
touch hello_os
mkdir hello_os_dir
cp hello_os.o ./hello_os_dir
mv ./hello_os_dir/hello_os.o ./hello_os_dir/hello.os
cp hello_os.o ./hello_os_dir
rm hello_os.o

grep -i os_hello hello_os.c | awk -F 'os_hello' '{print $1}' > hello_os.txt

