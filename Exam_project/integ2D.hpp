#ifndef INTEG2D_HPP
#define INTEG2D_HPP

#include <functional>
#include "integration.hpp"

struct Result2D {
    double value;
    double error_estimate;
    long outer_calls;
    int outer_depth;
};

// Computes int_a^b dx int_d(x)^u(x) dy f(x,y)
Result2D integ2D(std::function<double(double,double)> f,
                 double a,
                 double b,
                 std::function<double(double)> d,
                 std::function<double(double)> u,
                 double acc = 1e-6,
                 double eps = 1e-6);

#endif
