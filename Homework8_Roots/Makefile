CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

all: main
	./main
	gnuplot -e "set terminal pngcairo size 800,600; set output 'hydrogen_wavefunction.png'; set title 'Hydrogen ground-state wavefunction'; set xlabel 'r'; set ylabel 'f(r)'; set grid; plot 'hydrogen_wavefunction.txt' using 1:2 with lines title 'numerical', 'hydrogen_wavefunction.txt' using 1:3 with lines title 'exact r*exp(-r)'"

main: main.cpp root.h ode.cpp ode.hpp
	$(CXX) $(CXXFLAGS) main.cpp ode.cpp -o main

clean:
	rm -f main hydrogen_wavefunction.txt hydrogen_wavefunction.png