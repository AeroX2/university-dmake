#!/bin/sh

gcc -g -Wall dmake/*.c dmake/*.h -o dmake/dmake
export PATH="$PATH:`pwd`/dmake"

yes | rm -r test*
tar xf stage3.tar
cp stage3/test* -rp .

echo Stage 3
for i in {1..10}
do
	echo $i
	cd "test$i"
	valgrind -q `cat ../test$i.cmd` > ../output 2> ../output.err
	cd ../
	diff "test$i.out" output
	diff "test$i.err" output.err
done
