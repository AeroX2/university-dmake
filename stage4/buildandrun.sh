#!/bin/sh

PATH=$(getconf PATH)
gcc -g -Wall dmake/*.c dmake/*.h -o dmake/dmake
PATH="$PATH:`pwd`/dmake"

yes | rm -r test*
tar xf stage4.tar
cp stage4/test* -rp .

echo Stage 4
for i in {1..10}
do
	echo $i
	cd "test$i"
	valgrind -q `cat ../test$i.cmd` > ../output 2> ../output.err
	cd ../
	diff "test$i.out" output
	diff "test$i.err" output.err
	ls -A1ts test$i > output.ls
	sort output.ls > output2.ls
	sort test$i.ls | diff - output2.ls
done
