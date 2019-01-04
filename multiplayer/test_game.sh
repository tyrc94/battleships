#!/bin/bash

g++ game_local_multi.cpp -o test.out
if [ "${PIPESTATUS}" != 0 ]
then
	exit
fi
./test.out
