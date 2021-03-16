#!/usr/bin/env bash

cd $(dirname "$0")/..
RENDER="render.bmp"
TIMEFORMAT="Time : %Rs"
mkdir -p benchmark/renders
rm -f $RENDER
for entry in "scenes"/*.rt "scenes"/crash/*.rt
do
	printf "\033[1;94mTesting %s...\033[0m\n" $entry
	time leaks -quiet -atExit -- ./miniRT $entry --save
	if [ -f "$RENDER" ]
	then
		mv -f $RENDER benchmark/renders/$(basename ${entry%.*}).bmp
	fi
	printf "\n"
done
