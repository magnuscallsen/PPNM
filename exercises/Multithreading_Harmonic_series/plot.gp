set title "Running Time vs Number of Threads"
set xlabel "Number of Threads"
set ylabel "Running Time (seconds)"
set grid

set terminal pngcairo size 800,600
set output "plot.png"

plot "data.dat" using 1:2 with linespoints title "Harmonic Sum"
