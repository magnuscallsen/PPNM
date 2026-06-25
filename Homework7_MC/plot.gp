set terminal pngcairo size 900,650 enhanced font 'Arial,12'
set grid

set output 'circle_errors.png'
set logscale x
set logscale y
set xlabel 'N'
set ylabel 'error'
set title 'Unit circle: estimated and actual error'
plot 'circle_errors.dat' using 1:3 with linespoints title 'estimated error', \
     'circle_errors.dat' using 1:4 with linespoints title 'actual error', \
     1.0/sqrt(x) with lines title '1/sqrt(N) reference'

set output 'circle_scaled_error.png'
unset logscale y
set logscale x
set xlabel 'N'
set ylabel 'sqrt(N) * actual error'
set title 'Unit circle: scaled actual error'
plot 'circle_errors.dat' using 1:5 with linespoints title 'sqrt(N)*actual error'

set output 'qmc_scaling.png'
set logscale x
set logscale y
set xlabel 'N'
set ylabel 'actual error'
set title 'Smooth integral error comparison'
plot 'qmc_scaling.dat' using 1:2 with linespoints title 'LCG pseudo-random', \
     'qmc_scaling.dat' using 1:3 with linespoints title 'std::mt19937', \
     'qmc_scaling.dat' using 1:4 with linespoints title 'Halton quasi-random', \
     0.02/sqrt(x) with lines title '1/sqrt(N) reference', \
     0.3/x with lines title '1/N reference'

set output 'qmc_error_estimate.png'
set logscale x
set logscale y
set xlabel 'N'
set ylabel 'error'
set title 'QMC actual error and two-sequence error estimate'
plot 'qmc_scaling.dat' using 1:4 with linespoints title 'QMC actual error', \
     'qmc_scaling.dat' using 1:8 with linespoints title 'QMC two-sequence estimate'

set terminal pngcairo
set output "stratified_scaling.png"
set logscale x
set logscale y
set xlabel "N"
set ylabel "actual error"
set title "Plain MC vs stratified sampling"
plot "stratified_scaling.dat" using 1:2 with linespoints title "Plain MC", \
     "stratified_scaling.dat" using 1:3 with linespoints title "Stratified MC"
