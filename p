# set term x11 persist font "arial,105"

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 0 \
    pointtype 7 pointsize 0.2

set style line 2  \
    linecolor rgb '#ff6000' \
    linetype 1 linewidth 0 \
    pointtype 7 pointsize 0.2

set style line 3  \
    linecolor rgb '#000000' \
    linetype 1 linewidth 0 \
    pointtype 7 pointsize 1

set style line 4  \
    linecolor rgb '#007700' \
    linetype 1 linewidth 0 \
    pointtype 7 pointsize 1

set yrange [930:0] reverse
set xrange [0:1600]
set size ratio -1
plot './point_data/finger1.txt' with linespoints linestyle 1, \
     './point_data/finger2.txt' with linespoints linestyle 2, \
     './point_data/rightmost.txt' with linespoints linestyle 3, \
	 './point_data/leftmost.txt' with linespoints linestyle 4		

pause -1