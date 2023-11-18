#!/bin/bash
gcc main.c -pass-exit-codes -g -o main.o -c
if [ $? -eq 0 ]; then
	echo "[main compiled]"
fi
gcc point.c -pass-exit-codes -g -o point.o -c
if [ $? -eq 0 ]; then
	echo "[point compiled]"
fi
gcc heart.c -pass-exit-codes -g -o heart.o -c
if [ $? -eq 0 ]; then
	echo "[heart compiled]"
fi
gcc -pass-exit-codes -o touchpad main.o point.o heart.o -lm
if [ $? -eq 0 ]; then
   echo "[linked]"
   sudo ./touchpad
fi
