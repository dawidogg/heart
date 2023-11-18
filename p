#!/bin/bash
gnuplot -persist -e "file1='./data/$1_1.txt'; file2='./data/$1_2.txt'" plot_script.gp
