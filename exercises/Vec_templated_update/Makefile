CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O2

OBJS   = main_templated.o
TARGET = main
OUT    = Out.txt

all: run

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main_templated.cc vec_templated.h
	$(CXX) $(CXXFLAGS) -c main_templated.cc

run: $(TARGET)
	./$(TARGET) > $(OUT)

clean:
	rm -f $(OBJS) $(TARGET) $(OUT)

.PHONY: all run clean
