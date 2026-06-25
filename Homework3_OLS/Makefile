CXX = g++
CXXFLAGS = -std=c++23 -O2

all: Out.txt fit.svg
	cat Out.txt

Out.txt fit.svg: main plot.gp
	./main
	gnuplot plot.gp

main: main.cc matrix.hpp lsfit.hpp
	$(CXX) $(CXXFLAGS) main.cc -o main

clean:
	rm -f main Out.txt data.txt fit.txt fit.svg