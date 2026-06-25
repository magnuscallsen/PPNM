CXX      = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic
TARGET   = main
SRC      = main.cpp integration.cpp
OBJ      = $(SRC:.cpp=.o)

.PHONY: all run plots clean scipy
.NOTPARALLEL:

all: run scipy plots

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp integration.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) | tee output.txt

scipy:
	@if command -v python3 >/dev/null 2>&1; then \
		python3 scipy_compare.py | tee -a output.txt; \
	else \
		echo "python3 not found; skipping SciPy comparison." | tee -a output.txt; \
		printf '# id name cpp_calls scipy_calls\n1 "1/sqrt(x)" NaN NaN\n' > taskB_cpp_vs_scipy_calls.txt; \
	fi

plots:
	@if command -v gnuplot >/dev/null 2>&1; then \
		gnuplot erf_plot.gp; \
		gnuplot erf1_convergence.gp; \
		gnuplot taskB_clenshaw_curtis_calls.gp; \
		gnuplot taskB_infinite_calls.gp; \
		gnuplot taskB_scipy_comparison.gp; \
		gnuplot taskC_error_quality.gp; \
		echo "Created all PNG plots using gnuplot."; \
	else \
		echo "gnuplot is not installed, so PNG plots were not created."; \
		echo "Install gnuplot and run: make plots"; \
	fi

clean:
	rm -f $(TARGET) $(OBJ) *.txt *.png
