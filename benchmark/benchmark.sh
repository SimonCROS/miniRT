#!/usr/bin/env bash

cd $(dirname "$0")/..
RENDER="render.bmp"
TIMEFORMAT="Time : %Rs"
mkdir -p benchmark/renders
rm -f $RENDER
for entry in "scenes"/*.rt "scenes"/crash/*.rt
do
	printf "\033[1;94mTesting %s...\033[0m\n" $entry
	if command -v valgrind &> /dev/null
	then
		valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes ./miniRT $entry --save
	elif command -v leaks &> /dev/null
	then
		leaks -quiet -atExit -- ./miniRT $entry --save
	fi
	if [ -f "$RENDER" ]
	then
		mv -f $RENDER benchmark/renders/$(basename ${entry%.*}).bmp
	fi
	printf "\n"
done
