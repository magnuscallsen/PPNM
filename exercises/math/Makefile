CXX = c++
CXXFLAGS = -std=c++20 -Wall -Wextra

Out.txt: main
	./main > Out.txt

main: main.o sfuns.o
	$(CXX) -o main main.o sfuns.o

main.o: main.cc sfuns.h
	$(CXX) $(CXXFLAGS) -c main.cc

sfuns.o: sfuns.cc sfuns.h
	$(CXX) $(CXXFLAGS) -c sfuns.cc

.PHONY: clean
clean:
	rm -f main main.o sfuns.o Out.txt
