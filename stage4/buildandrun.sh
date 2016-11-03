#!/bin/sh

gcc -g -Wall dmake/*.c dmake/*.h -o dmake/dmake
export PATH="$PATH:`pwd`/dmake"

yes | rm -r test*
tar xf stage4.tar
cp stage4/test* -rp .

for i in {1..10}
do
	echo $i
	cd "test$i"
	valgrind -q `cat ../test$i.cmd` > ../output 2> ../output.err
	cd ../
	diff "test$i.out" output
	diff "test$i.err" output.err
	ls -A1ts test$i | diff test$i.ls -
done
