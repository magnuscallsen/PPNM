
#pragma once
#include <vector>

using vec = std::vector<double>;

int binsearch(const vec& x, double z);
double linterp(const vec& x, const vec& y, double z);
double linterpInteg(const vec& x, const vec& y, double z);

struct qspline {
    int n;
    vec x, y, b, c;

    qspline(const vec& xs, const vec& ys);

    int binsearch(double z) const;
    double eval(double z) const;
    double deriv(double z) const;
    double integ(double z) const;
};
struct cspline {
    int n;
    vec x,y,b,c,d;

    cspline(const vec& xs,const vec& ys);

    int binsearch(double z) const;
    double eval(double z) const;
    double deriv(double z) const;
    double integ(double z) const;
};