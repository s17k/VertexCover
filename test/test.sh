#!/bin/bash

echo "Compiling..."
g++ -o ./../vertex_cover ./../vertex_cover.cpp -std=c++11 -O2
echo "Compiled vertex_cover"
g++ -o vertex_cover_brutal vertex_cover_brutal.cpp -std=c++11 -O2
echo "Compiled vertex_cover_brutal"

echo "Compilation completed"

echo "Begin testing"

for i in `seq 1000`
do
	echo $i
	python vertex_cover_gen.py > test.in;
	./../vertex_cover < test.in > test.out
	./vertex_cover_brutal < test.in > test.out_brutal
	diff -w -q test.out test.out_brutal
	if [ $? -ne 0 ]
	then
		echo "Wrong answer"
		break
	fi
done

echo "Testing finished"
