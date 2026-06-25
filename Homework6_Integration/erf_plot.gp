set terminal pngcairo size 1000,700 enhanced font "Arial,12"
set output "taskA_erf_plot.png"
set title "Task A: error function computed by recursive adaptive integration"
set xlabel "z"
set ylabel "erf(z)"
set grid
set key bottom right
plot "erf_data.txt" using 1:2 with lines lw 2 title "my erf(z)", \
     "erf_data.txt" using 1:3 with points pt 7 ps 0.4 title "std::erf(z) reference"
