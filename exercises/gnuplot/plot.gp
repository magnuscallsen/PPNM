set terminal pngcairo size 900,600
set output "lngamma_plot.png"

set title "LnGamma function  approximation"
set xlabel "x"
set ylabel "Ln(gamma(x))"
set grid

plot "data.txt" using 1:2 with lines title "approx Ln(gamma)", \
     "data.txt" using 1:3 with lines title "std::Ln(gamma)"
