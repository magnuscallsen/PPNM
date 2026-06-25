set terminal pngcairo size 1200,750 enhanced font "Arial,12"
set output "taskB_scipy_comparison.png"
set title "Task B: C++ integrator vs Python/SciPy quad, function evaluations"
set ylabel "number of function evaluations"
set grid ytics
set key outside top center horizontal
set style data histogram
set style histogram clustered gap 1
set style fill solid border -1
set boxwidth 0.85
set xtics rotate by -25
plot "taskB_cpp_vs_scipy_calls.txt" using 3:xtic(2) title "C++ integrator", \
     "taskB_cpp_vs_scipy_calls.txt" using 4 title "SciPy quad"
