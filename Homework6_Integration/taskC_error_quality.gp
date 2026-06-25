set terminal pngcairo size 1100,700 enhanced font "Arial,12"
set output "taskC_error_quality.png"
set title "Task C: quality of integration error estimate"
set ylabel "error"
set logscale y
set grid ytics
set key outside top center horizontal
set style data histogram
set style histogram clustered gap 1
set style fill solid border -1
set boxwidth 0.85
set xtics rotate by -25
plot "taskC_error_quality.txt" using 3:xtic(2) title "actual error", \
     "taskC_error_quality.txt" using 4 title "estimated error"
