#!/bin/sh

gcc -g -Wall dmake/*.c dmake/*.h -o dmake/dmake
export PATH="$PATH:`pwd`/dmake"

echo Stage 2
for i in {1..10}
do
	echo $i
	cd "test$i"
	valgrind -q `cat ../test$i.cmd` > ../output 2> ../output.err
	cd ../
	diff "test$i.out" output
	diff "test$i.err" output.err
done
