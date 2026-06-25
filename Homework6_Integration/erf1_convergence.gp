set terminal pngcairo size 1000,700 enhanced font "Arial,12"
set output "taskA_erf1_convergence.png"
set title "Task A / C: convergence of erf(1), eps = 0"
set xlabel "requested absolute accuracy acc"
set ylabel "|calculated erf(1) - tabulated erf(1)|"
set logscale xy
set grid
set key bottom right
plot "erf1_convergence.txt" using 1:2 with linespoints lw 2 pt 7 title "actual error", \
     "erf1_convergence.txt" using 1:4 with linespoints lw 2 pt 5 title "estimated error"
