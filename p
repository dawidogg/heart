#!/bin/bash
gnuplot -e "file1='$1_1.txt'; file2='$1_2.txt'" plot_script.gp
