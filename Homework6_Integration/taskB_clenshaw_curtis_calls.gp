set terminal pngcairo size 1200,750 enhanced font "Arial,12"
set output "taskB_clenshaw_curtis_calls.png"
set title "Task B: function evaluations, ordinary vs Clenshaw-Curtis"
set ylabel "number of function evaluations"
set grid ytics
set key outside top center horizontal
set style data histogram
set style histogram clustered gap 1
set style fill solid border -1
set boxwidth 0.85
set xtics rotate by -25
plot "taskB_cc_calls.txt" using 3:xtic(2) title "ordinary", \
     "taskB_cc_calls.txt" using 4 title "Clenshaw-Curtis"
