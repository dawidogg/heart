#!/bin/bash
gnuplot -e "file1='./data_empty/$1_1.txt'; file2='./data_empty/$1_2.txt'" plot_script.gp
