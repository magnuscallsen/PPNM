set terminal pngcairo size 1100,700 enhanced font "Arial,12"
set output "taskB_infinite_calls.png"
set title "Task B: infinite-limit integrals transformed to finite intervals"
set ylabel "number of function evaluations"
set grid ytics
set style data histograms
set style fill solid border -1
set boxwidth 0.7
set xtics rotate by -20
plot "taskB_infinite_calls.txt" using 3:xtic(2) title "C++ generalized integrator calls"
