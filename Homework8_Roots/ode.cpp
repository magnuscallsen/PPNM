#include "ode.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace ode {

std::tuple<vector, vector> rkstep12(
    std::function<vector(double, vector)> F,
    double x,
    vector y,
    double h
) {
    vector k0 = F(x, y);
    vector k1 = F(x + h / 2, y + k0 * (h / 2));

    vector yh = y + k1 * h;
    vector dy = (k1 - k0) * h;

    return {yh, dy};
} // Interface debugged with AI but implementation done by me. 

std::tuple<std::vector<double>, std::vector<vector>> driver(
    std::function<vector(double, vector)> F,
    double a,
    double b,
    vector yinit,
    double h,
    double acc,
    double eps,
    double max_step
) {
    double x = a;
    vector y = yinit;

    std::vector<double> xlist;
    std::vector<vector> ylist;

    xlist.push_back(x);
    ylist.push_back(y);

    int steps = 0;
    int max_steps = 1000000;

    while (x < b) {
        if (++steps > max_steps) {
            throw std::runtime_error("driver: too many steps");
        }

        if (h < 1e-12) {
            throw std::runtime_error("driver: step size became too small");
        }

        if (h > max_step) h = max_step;
        if (x + h > b) h = b - x;

        auto [yh, dy] = rkstep12(F, x, y, h);

        double tol = (acc + eps * yh.norm()) * std::sqrt(h / (b - a));
        double err = dy.norm();

        if (err <= tol || err == 0) {
            x += h;
            y = yh;

            xlist.push_back(x);
            ylist.push_back(y);
        }

        if (err > 0) {
            double factor = 0.95 * std::pow(tol / err, 0.25);

            if (factor > 2.0) factor = 2.0;
            if (factor < 0.5) factor = 0.5;

            h *= factor;
        } else {
            h *= 2.0;
        }
    }

    return {xlist, ylist};
} // Driver written by AI

}
