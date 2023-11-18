#!/bin/bash
gcc heart_test.c -g -o heart_test.o -c
gcc point.c -g -o point.o -c
gcc -o htest point.o heart_test.o -lm
# gcc heart_test.c point.c -lm -g -o htest
./htest
