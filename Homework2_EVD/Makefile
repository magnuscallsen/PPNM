CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror -I.

TARGET = main-hpp
SRC = main-hpp.cc

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(SRC) matrix.hpp
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)
