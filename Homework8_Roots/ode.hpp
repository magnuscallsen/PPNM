// This code is written by AI
#pragma once
#include "matrix.hpp"
#include <functional>
#include <tuple>
#include <vector>

namespace ode {

using pp::vector; // Earlier implemented class

std::tuple<vector, vector> rkstep12(
    std::function<vector(double, vector)> F,
    double x,
    vector y,
    double h
);

std::tuple<std::vector<double>, std::vector<vector>> driver(
    std::function<vector(double, vector)> F,
    double a,
    double b,
    vector yinit,
    double h = 0.125,
    double acc = 0.01,
    double eps = 0.01,
    double max_step = 0.125
);

}
