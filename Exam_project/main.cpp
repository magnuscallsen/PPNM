#include "integ2D.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
bool test(const std::string& name, double got, double exact, double tol)
{
    double diff = std::abs(got - exact);
    bool ok = diff < tol;
    std::cout << std::left << std::setw(34) << name
              << " got = " << std::setprecision(12) << got
              << " exact = " << exact
              << " |diff| = " << diff
              << (ok ? "  PASS" : "  FAIL") << '\n';
    return ok;
}
}

int main()
{
    const double acc = 1e-6;
    const double eps = 1e-6;
    bool ok = true;

    // Test 1: unit square, int_0^1 dx int_0^1 dy (x+y) = 1.
    Result2D r1 = integ2D([](double x, double y) { return x + y; },
                          0.0, 1.0,
                          [](double) { return 0.0; },
                          [](double) { return 1.0; },
                          acc, eps);
    ok &= test("unit square: x+y", r1.value, 1.0, 5e-5);

    // Test 2: triangular region, int_0^1 dx int_0^x dy 1 = 1/2.
    Result2D r2 = integ2D([](double, double) { return 1.0; },
                          0.0, 1.0,
                          [](double) { return 0.0; },
                          [](double x) { return x; },
                          acc, eps);
    ok &= test("triangle: 1", r2.value, 0.5, 5e-5);

    // Test 3: curved upper boundary, int_0^1 dx int_0^sqrt(1-x^2) dy 1 = pi/4.
    Result2D r3 = integ2D([](double, double) { return 1.0; },
                          0.0, 1.0,
                          [](double) { return 0.0; },
                          [](double x) { return std::sqrt(1.0 - x*x); },
                          acc, eps);
    ok &= test("quarter disk area", r3.value, M_PI/4.0, 5e-5);

    // Test 4: non-constant function on a triangle:
    // int_0^1 dx int_0^x dy x*y = int_0^1 x*x^2/2 dx = 1/8.
    Result2D r4 = integ2D([](double x, double y) { return x*y; },
                          0.0, 1.0,
                          [](double) { return 0.0; },
                          [](double x) { return x; },
                          acc, eps);
    ok &= test("triangle: x*y", r4.value, 1.0/8.0, 5e-5);

    std::cout << "\nOuter calls in quarter disk test: " << r3.outer_calls
              << ", estimated outer error: " << r3.error_estimate << '\n';

    if(!ok) {
        std::cerr << "At least one test failed.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}
