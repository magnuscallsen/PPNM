CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pthread

TARGET := harmonic
SRC := main.cc

N := 100000000

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

out.times: $(TARGET)
	> out.times
	> log.main
	for nthreads in 1 2 3 4 5 6 7 8; do \
		echo "Running $$nthreads threads"; \
		/usr/bin/time -p ./$(TARGET) -threads $$nthreads -terms $(N) >> log.main 2>> out.times; \
	done

clean:
	rm -f $(TARGET) out.times log.main
