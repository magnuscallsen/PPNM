set terminal svg size 800,600
set output "fit.svg"

set title "Rutherford-Soddy decay of ThX"
set xlabel "Time t [days]"
set ylabel "Activity y [relative units]"

plot \
    "data.txt" using 1:2:3 with yerrorbars title "experimental data", \
    "fit.txt" using 1:2 with lines lw 2 title "best fit"

set output "fit_uncertainty.svg"

set title "Effect of coefficient uncertainties"
set xlabel "Time t [days]"
set ylabel "Activity y"

plot \
    "data.txt" using 1:2:3 with yerrorbars title "experimental data", \
    "fit_uncertainty.txt" using 1:2 with lines lw 2 title "best fit", \
    for [col=3:6] "fit_uncertainty.txt" using 1:col with lines dt 2 notitle
