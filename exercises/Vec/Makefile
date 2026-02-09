CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O2

OBJS   = vec.o main.o
TARGET = main
OUT    = Out.txt

all: run

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

vec.o: vec.cc vec.h
	$(CXX) $(CXXFLAGS) -c vec.cc

main.o: main.cc vec.h
	$(CXX) $(CXXFLAGS) -c main.cc

run: $(TARGET)
	./$(TARGET) > $(OUT)

clean:
	rm -f $(OBJS) $(TARGET) $(OUT)

.PHONY: all run clean
