CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic
LDLIBS   = -lm

TARGET = main
SRC    = main.cpp integ2D.cpp integration.cpp
OBJ    = $(SRC:.cpp=.o)

.PHONY: all clean

all: clean $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

%.o: %.cpp integration.hpp integ2D.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)