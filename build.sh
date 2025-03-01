#!/usr/bin/sh

echo --- BUILDING PROGRAM ---
clang++ -std=c++23 -O0 -g -Wall -o game -DDEBUG -Iinclude $(find src -name "*.cpp") -Llib -lglfw3

if [ $? -ne 0 ]
then
	echo BUILD FAILED
	exit 1
fi

if [ "$1" = "run" ]
then
	echo --- RUNNING PROGRAM ---
	./game
	exit $?
fi
