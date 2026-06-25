#ifndef INTEGRATION_HPP
#define INTEGRATION_HPP

#include <functional>
#include <limits>

struct Result {
    double value;
    double error;
    long calls;
    int depth;
};

class FunctionCounter {
public:
    explicit FunctionCounter(std::function<double(double)> f);
    double operator()(double x);
    long calls() const;
    void reset();
private:
    std::function<double(double)> f_;
    long calls_;
};

Result integrate(std::function<double(double)> f,
                 double a,
                 double b,
                 double acc=1e-3,
                 double eps=1e-3);

Result integrate_clenshaw_curtis(std::function<double(double)> f,
                                 double a,
                                 double b,
                                 double acc=1e-3,
                                 double eps=1e-3);

Result integrate_general(std::function<double(double)> f,
                         double a,
                         double b,
                         double acc=1e-3,
                         double eps=1e-3);

double my_erf(double z, double acc=1e-6, double eps=1e-6, Result* info=nullptr);

#endif
