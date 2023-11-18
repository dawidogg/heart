set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 0 \
    pointtype 7 pointsize 1

set style line 2  \
    linecolor rgb '#ff6000' \
    linetype 1 linewidth 0 \
    pointtype 7 pointsize 1

set yrange [930:0] reverse
set xrange [0:1600]
set size ratio -1
plot file1 with linespoints linestyle 1, \
	 file2 with linespoints linestyle 2

pause 2