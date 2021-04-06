#!/usr/bin/env bash

cd $(dirname "$0")/..
TIMEFORMAT="Time : %Rs"
for entry in  scenes/lite/*.rt scenes/crash/*.rt scenes/crash/*.notrt
do
	printf "\033[1;94mTesting %s...\033[0m\n" $entry
	if command -v valgrind &> /dev/null
	then
		valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes ./miniRT $entry
		printf "\n"
	elif command -v leaks &> /dev/null
	then
		leaks -quiet -atExit -- ./miniRT $entry
	fi
done
