#include "problems.hpp"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double unit_circle_indicator(const std::vector<double>& x) {
    return (x[0] * x[0] + x[1] * x[1] <= 1.0) ? 1.0 : 0.0;
}

double exact_unit_circle_area() {
    return M_PI;
}

double ellipsoid_indicator(const std::vector<double>& x, const EllipsoidParameters& p) {
    const double value = x[0] * x[0] / (p.a * p.a)
                       + x[1] * x[1] / (p.b * p.b)
                       + x[2] * x[2] / (p.c * p.c);
    return (value <= 1.0) ? 1.0 : 0.0;
}

double exact_ellipsoid_volume(const EllipsoidParameters& p) {
    return (4.0 / 3.0) * M_PI * p.a * p.b * p.c;
}

// Smooth, non-singular 3D test integral over [0,1]^3.
// Integral of x*y*z is 1/8.
double smooth_test_integrand(const std::vector<double>& x) {
    return x[0] * x[1] * x[2];
}

double exact_smooth_test_integral() {
    return 1.0 / 1.0;
}

// The homework singular integral is written as an ordinary integral over [0,pi]^3
// with measure dx/pi dy/pi dz/pi. Since plainmc integrates with dx dy dz,
// we include the factor 1/pi^3 in the integrand.
double singular_integrand(const std::vector<double>& x) {
    const double denominator = 1.0 - std::cos(x[0]) * std::cos(x[1]) * std::cos(x[2]);
    return 1.0 / (M_PI * M_PI * M_PI * denominator);
}

double exact_singular_integral() {
    return 1.3932039296856768591842462603255;
}
