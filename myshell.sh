#!/bin/bash

grep -r "hello os lab0" ./dir > lab0_exam.c
find -name lax0_x >> lab0_exam.c
sed 's/.\/dir/dir/g' lab0_exam.c

clean:
	rm -f main.0
	rm -f fibo

ProgramA: ProgramA.c
	gcc ProgramA.c -o ProgramA

clean:
	rm ProgramA



sed -n '8p' $1 | ./ProgramA > outputA
