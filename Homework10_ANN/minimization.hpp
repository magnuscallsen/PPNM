// Code taken from earlier assignment

#pragma once

#include <functional>
#include "matrix.hpp"

namespace minopt {

using Function = std::function<double(const pp::vector&)>;

struct NewtonResult {
    pp::vector x;
    int steps = 0;
    double fx = 0.0;
    double grad_norm = 0.0;
    bool converged = false;
};

pp::vector gradient(const Function& phi, const pp::vector& x);
pp::matrix hessian(const Function& phi, const pp::vector& x);

NewtonResult newton(
    const Function& phi,
    pp::vector x,
    double acc = 1e-3,
    int max_steps = 1000
);

NewtonResult newton_central(
    const Function& phi,
    pp::vector x,
    double acc = 1e-3,
    int max_steps = 1000
);

} // namespace minopt