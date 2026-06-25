#include "integration.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace {
const int MAX_DEPTH = 100000;
const double SQRT2 = std::sqrt(2.0);

Result adapt_recursive(const std::function<double(double)>& f,
                       double a, double b, double acc, double eps,
                       double f2, double f3, int depth)
{
    const double h = b-a;
    if(!(h > 0.0) || depth > MAX_DEPTH) {
        return {0.0, std::numeric_limits<double>::infinity(), 0, depth};
    }

    // First call for this interval: evaluate inner reusable points.
    if(std::isnan(f2)) f2 = f(a + 2.0*h/6.0);
    if(std::isnan(f3)) f3 = f(a + 4.0*h/6.0);

    // New points for this interval.
    const double f1 = f(a + h/6.0);
    const double f4 = f(a + 5.0*h/6.0);

    // Higher-order open 4-point rule and lower-order embedded rule.
    const double Q = (2.0*f1 + f2 + f3 + 2.0*f4)/6.0*h;
    const double q = (    f1 + f2 + f3 +     f4)/4.0*h;
    const double err = std::abs(Q-q);
    const double tol = acc + eps*std::abs(Q);

    if(!std::isfinite(Q) || !std::isfinite(q)) {
        return {Q, std::numeric_limits<double>::infinity(), 0, depth};
    }

    // Accept if tolerance is satisfied. The h guard prevents endless splitting
    // at machine precision for extremely hard endpoint singularities.
    if(err < tol || std::abs(h) < 16.0*std::numeric_limits<double>::epsilon()*(std::abs(a)+std::abs(b)+1.0)) {
        return {Q, err, 0, depth};
    }

    const double mid = 0.5*(a+b);
    Result left  = adapt_recursive(f, a,   mid, acc/SQRT2, eps, f1, f2, depth+1);
    Result right = adapt_recursive(f, mid, b,   acc/SQRT2, eps, f3, f4, depth+1);

    return {left.value + right.value,
            std::hypot(left.error, right.error),
            0,
            std::max(left.depth, right.depth)};
}
}

FunctionCounter::FunctionCounter(std::function<double(double)> f) : f_(std::move(f)), calls_(0) {}

double FunctionCounter::operator()(double x) {
    ++calls_;
    return f_(x);
}

long FunctionCounter::calls() const { return calls_; }
void FunctionCounter::reset() { calls_ = 0; }

Result integrate(std::function<double(double)> f, double a, double b, double acc, double eps)
{
    if(a == b) return {0.0, 0.0, 0, 0};
    double sign = 1.0;
    if(a > b) { std::swap(a,b); sign = -1.0; }

    FunctionCounter counted(f);
    Result r = adapt_recursive([&](double x){ return counted(x); }, a, b, acc, eps,
                               std::numeric_limits<double>::quiet_NaN(),
                               std::numeric_limits<double>::quiet_NaN(), 0);
    r.value *= sign;
    r.calls = counted.calls();
    return r;
}

Result integrate_clenshaw_curtis(std::function<double(double)> f,
                                 double a, double b, double acc, double eps)
{
    if(!std::isfinite(a) || !std::isfinite(b))
        throw std::runtime_error("Clenshaw-Curtis wrapper needs finite limits");
    if(a == b) return {0.0, 0.0, 0, 0};

    FunctionCounter counted(f);
    const double c2 = 0.5*(b-a);

    auto transformed = [&](double theta) {
        // Algebraically this is (a+b)/2 + (b-a)/2*cos(theta),
        // but the cos^2 form avoids catastrophic cancellation near theta=pi
        // for intervals like [0,1], which is important for endpoint singularities.
        double x = a + (b-a)*std::pow(std::cos(0.5*theta), 2);
        return counted(x)*std::sin(theta)*std::abs(c2);
    };

    Result r = adapt_recursive(transformed, 0.0, M_PI, acc, eps,
                               std::numeric_limits<double>::quiet_NaN(),
                               std::numeric_limits<double>::quiet_NaN(), 0);
    r.calls = counted.calls();
    return r;
}

Result integrate_general(std::function<double(double)> f,
                         double a, double b, double acc, double eps)
{
    const bool ainf = std::isinf(a);
    const bool binf = std::isinf(b);

    if(!ainf && !binf) return integrate_clenshaw_curtis(f, a, b, acc, eps);

    if(ainf && binf) {
        auto g = [&](double t) {
            const double den = 1.0 - t*t;
            const double x = t/den;
            const double dxdt = (1.0 + t*t)/(den*den);
            return f(x)*dxdt;
        };
        return integrate_clenshaw_curtis(g, -1.0, 1.0, acc, eps);
    }

    if(!ainf && binf) {
        auto g = [&](double t) {
            const double den = 1.0 - t;
            const double x = a + t/den;
            const double dxdt = 1.0/(den*den);
            return f(x)*dxdt;
        };
        return integrate_clenshaw_curtis(g, 0.0, 1.0, acc, eps);
    }

    if(ainf && !binf) {
        auto g = [&](double t) {
            const double x = b - (1.0-t)/t;
            const double dxdt = 1.0/(t*t);
            return f(x)*dxdt;
        };
        return integrate_clenshaw_curtis(g, 0.0, 1.0, acc, eps);
    }

    throw std::runtime_error("Unsupported limits");
}

double my_erf(double z, double acc, double eps, Result* info)
{
    if(z < 0) {
        Result tmp;
        double val = -my_erf(-z, acc, eps, &tmp);
        if(info) *info = {val, tmp.error, tmp.calls, tmp.depth};
        return val;
    }

    const double factor = 2.0/std::sqrt(M_PI);
    Result r;

    if(z <= 1.0) {
        r = integrate([&](double x){ return std::exp(-x*x); }, 0.0, z, acc, eps);
        if(info) *info = {factor*r.value, factor*r.error, r.calls, r.depth};
        return factor*r.value;
    }

    auto tail_integrand = [&](double t) {
        double u = z + (1.0-t)/t;
        return std::exp(-u*u)/(t*t);
    };
    r = integrate(tail_integrand, 0.0, 1.0, acc, eps);
    const double val = 1.0 - factor*r.value;
    if(info) *info = {val, factor*r.error, r.calls, r.depth};
    return val;
}
