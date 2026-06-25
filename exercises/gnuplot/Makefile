CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O2

TARGET = main
DATA   = data.txt
PLOT   = lngamma.png

all: $(PLOT)

$(TARGET): main.cc
	$(CXX) $(CXXFLAGS) main.cc -o $(TARGET)

$(DATA): $(TARGET)
	./$(TARGET) > $(DATA)

$(PLOT): $(DATA) plot.gp
	gnuplot plot.gp

clean:
	rm -f $(TARGET) $(DATA) $(PLOT)

.PHONY: all clean
