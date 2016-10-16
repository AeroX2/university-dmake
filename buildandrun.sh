#!/bin/sh

gcc -g -Wall -fsanitize=undefined dmake/*.c dmake/*.h -o dmake/dmake
export PATH="$PATH:`pwd`/dmake"

for i in {1..10}
do
	echo $i
	cd "test$i"
	`cat ../test$i.cmd` > ../output 2> ../output.err
	cd ../
	diff "test$i.out" output
	diff "test$i.err" output.err
done
