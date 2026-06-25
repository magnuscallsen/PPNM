# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

# Executable
TARGET = main

# Source files
SRC = main.cpp ann.cpp minimization.cpp
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)
	./$(TARGET)
	gnuplot plot.gpi

# Link
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean
clean:
	rm -f $(OBJ) $(TARGET) *.dat *.png
