#!/bin/sh

gcc -g -Wall dmake/*.c dmake/*.h -o dmake/dmake
export PATH="$PATH:`pwd`/dmake"

rm -r test*
tar xf stage4.tar
cp stage3/test* -rp .

for i in {1..10}
do
	echo $i
	cd "test$i"
	`cat ../test$i.cmd` > ../output 2> ../output.err
	cd ../
	diff "test$i.out" output
	diff "test$i.err" output.err
done
