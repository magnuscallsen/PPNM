CXX = c++
CXXFLAGS = -std=c++20 -Wall -Wextra

Out.txt: ME
	./ME > Out.txt

ME: main.cc
	$(CXX) $(CXXFLAGS) -o ME main.cc

.PHONY: clean
clean:
	rm -f Out.txt ME
