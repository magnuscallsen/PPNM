#include "integ2D.hpp"
#include <cmath>

Result2D integ2D(std::function<double(double,double)> f,
                 double a,
                 double b,
                 std::function<double(double)> d,
                 std::function<double(double)> u,
                 double acc,
                 double eps)
{
    // Split the requested absolute accuracy between the inner and outer integrals.
    // This uses the supplied one-dimensional adaptive integrator twice: first in y,
    // then adaptively in x.
    auto inner_integral_as_function_of_x = [&](double x) {
        double lower = d(x);
        double upper = u(x);
        double width = std::abs(upper - lower);
        double inner_acc = acc / std::sqrt(std::max(1.0, std::abs(b-a)));

        Result inner = integrate([&](double y) { return f(x,y); },
                                 lower, upper,
                                 inner_acc,
                                 eps);
        (void)width; // kept explicit: useful if one wants width-dependent tolerances later
        return inner.value;
    };

    Result outer = integrate(inner_integral_as_function_of_x, a, b, acc, eps);
    return {outer.value, outer.error, outer.calls, outer.depth};
}
