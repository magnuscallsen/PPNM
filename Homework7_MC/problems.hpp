#ifndef PROBLEMS_HPP
#define PROBLEMS_HPP

#include <vector>

struct EllipsoidParameters {
    double a;
    double b;
    double c;
};

double unit_circle_indicator(const std::vector<double>& x);
double exact_unit_circle_area();

double ellipsoid_indicator(const std::vector<double>& x, const EllipsoidParameters& p);
double exact_ellipsoid_volume(const EllipsoidParameters& p);

double smooth_test_integrand(const std::vector<double>& x);
double exact_smooth_test_integral();

double singular_integrand(const std::vector<double>& x);
double exact_singular_integral();

#endif
