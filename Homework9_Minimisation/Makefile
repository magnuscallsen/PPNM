CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic

TARGET = main
OBJS = main.o minimization.o

all: higgs-fit.pdf

higgs-fit.pdf: out.txt plot.gp higgs.data.txt
	gnuplot plot.gp

out.txt: $(TARGET) higgs.data.txt
	./$(TARGET) < higgs.data.txt

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp minimization.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

minimization.o: minimization.cpp minimization.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c minimization.cpp -o minimization.o

clean:
	rm -f $(OBJS) $(TARGET) out.txt log higgs-fit.pdf

.PHONY: all clean
