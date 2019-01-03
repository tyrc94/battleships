#!/bin/bash

g++ game.cpp -o test.out
if [ "${PIPESTATUS}" != 0 ]
then
	exit
fi
./test.out
