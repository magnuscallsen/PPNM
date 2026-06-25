set terminal pdf
set output "higgs-fit.pdf"

set title "Breit-Wigner fit to Higgs signal"
set xlabel "Energy E [GeV]"
set ylabel "Signal"

plot \
    "higgs.data.txt" using 1:2:3 with yerrorbars title "data", \
    "out.txt" using 1:2 with lines title "Breit-Wigner fit"
