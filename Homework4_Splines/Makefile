CXX = g++
CXXFLAGS = -O2 -Wall -std=c++17

all: cubic_compare.png

main: main.o spline.o
	$(CXX) $(CXXFLAGS) main.o spline.o -o main

main.o: main.cc spline.h
	$(CXX) $(CXXFLAGS) -c main.cc

spline.o: spline.cc spline.h
	$(CXX) $(CXXFLAGS) -c spline.cc

cubic_spline.data points.data: main
	./main

cubic_compare.png: cubic_spline.data points.data cubic_compare.gpi
	gnuplot cubic_compare.gpi

clean:
	rm -f *.o main cubic_spline.data points.data cubic_compare.png
