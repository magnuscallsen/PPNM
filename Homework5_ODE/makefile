CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = main
OBJECTS = main.o ode.o

.PHONY: all clean

all: solution.png Rel_solution.png Eight_solution.png

# Run the program ONCE and generate all data files
data.stamp: $(TARGET)
	./$(TARGET)
	touch data.stamp

# These files are produced by running main
harmonic.data lotka.data rel_circular.data rel_ellipse.data rel_precession.data eight.data: data.stamp

# Task A plot
solution.png: harmonic.data lotka.data plot.gpi
	gnuplot plot.gpi

# Task B plot
Rel_solution.png: rel_circular.data rel_ellipse.data rel_precession.data Rel_plot.gpi
	gnuplot Rel_plot.gpi

Eight_solution.png: eight.data Eight_plot.gpi
	gnuplot Eight_plot.gpi
# Build executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile sources
main.o: main.cpp ode.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

ode.o: ode.cpp ode.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c ode.cpp

# Clean everything
clean:
	rm -f *.o $(TARGET) *.data *.png data.stamp
