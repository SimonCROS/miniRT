#!/usr/bin/env bash

cd $(dirname "$0")/..
TIMEFORMAT="Time : %Rs"
mkdir -p benchmark/renders
for entry in "scenes"/*.rt "scenes"/crash/*.rt
do
	printf "\033[1;94mTesting %s...\033[0m\n" $entry
	time ./miniRT $entry --save
	if [ $? -eq 0 ]
	then
		mv -f render.bmp benchmark/renders/$(basename ${entry%.*}).bmp
	fi
	printf "\n"
done
