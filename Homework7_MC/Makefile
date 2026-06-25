CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET = main
OBJECTS = main.o mc.o problems.o qmc.o stratified.o
.PHONY: all clean run plot

all: $(TARGET) run plot

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

main.o: main.cpp mc.hpp problems.hpp qmc.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

mc.o: mc.cpp mc.hpp
	$(CXX) $(CXXFLAGS) -c mc.cpp

problems.o: problems.cpp problems.hpp
	$(CXX) $(CXXFLAGS) -c problems.cpp

qmc.o: qmc.cpp qmc.hpp mc.hpp
	$(CXX) $(CXXFLAGS) -c qmc.cpp

run: $(TARGET)
	./$(TARGET)

plot:
	@if command -v gnuplot >/dev/null 2>&1; then \
		gnuplot plot.gp; \
	else \
		echo "Gnuplot is not installed, so plots were not generated here."; \
		echo "Install gnuplot and run make again to create the png plots."; \
	fi

clean:
	rm -f $(TARGET) *.o *.dat *.png
