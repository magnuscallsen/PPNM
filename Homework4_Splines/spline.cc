#include "spline.h"
#include <cassert>

int binsearch(const vec& x, double z) {
    assert(x.size() >= 2);
    assert(z >= x.front() && z <= x.back());

    int i = 0;
    int j = x.size() - 1;

    while (j - i > 1) {
        int mid = (i + j) / 2;
        if (z > x[mid]) i = mid;
        else j = mid;
    }

    return i;
}

double linterp(const vec& x, const vec& y, double z) {
    assert(x.size() == y.size());

    int i = binsearch(x, z);

    double dx = x[i + 1] - x[i];
    assert(dx > 0);

    double dy = y[i + 1] - y[i];

    return y[i] + dy / dx * (z - x[i]);
}

double quadterp(const vec& x, const vec& y, double z) {
    assert(x.size() == y.size());

    int i = binsearch(x, z);

    double dx = x[i + 1] - x[i];
    assert(dx > 0);

    double dy = y[i + 1] - y[i];

    return y[i] + dy / dx * (z - x[i]);
}

double linterpInteg(const vec& x, const vec& y, double z){
    assert(x.size() == y.size());
    assert(x.size() >= 2);
    assert(z >= x.front());
    assert(z <= x.back());

    int i = binsearch(x, z);

    double sum = 0;

    for(int k = 0; k < i; k++){
        double dx = x[k+1] - x[k];
        assert(dx > 0);

        double slope = (y[k+1] - y[k]) / dx;

        sum += y[k]*dx + slope*dx*dx/2;
    }

    double dx = z - x[i];
    double h = x[i+1] - x[i];
    assert(h > 0);

    double slope = (y[i+1] - y[i]) / h;

    sum += y[i]*dx + slope*dx*dx/2;

    return sum;
}
qspline::qspline(const vec& xs, const vec& ys)
    : n(xs.size()), x(xs), y(ys), b(n-1), c(n-1)
{
    assert(xs.size() == ys.size());
    assert(n >= 2);

    vec h(n-1), p(n-1);

    for(int i = 0; i < n-1; i++){
        h[i] = x[i+1] - x[i];
        assert(h[i] > 0);
        p[i] = (y[i+1] - y[i]) / h[i];
    }

    c[0] = 0;

    for(int i = 0; i < n-2; i++){
        c[i+1] = (p[i+1] - p[i] - c[i]*h[i]) / h[i+1];
    }

    c[n-2] /= 2;

    for(int i = n-3; i >= 0; i--){
        c[i] = (p[i+1] - p[i] - c[i+1]*h[i+1]) / h[i];
    }

    for(int i = 0; i < n-1; i++){
        b[i] = p[i] - c[i]*h[i];
    }
}
int qspline::binsearch(double z) const {
    assert(z >= x.front() && z <= x.back());

    int i = 0;
    int j = n - 1;

    while(j - i > 1){
        int mid = (i + j) / 2;
        if(z > x[mid]) i = mid;
        else j = mid;
    }

    return i;
}
double qspline::eval(double z) const {
    int i = binsearch(z);
    double dx = z - x[i];

    return y[i] + b[i]*dx + c[i]*dx*dx;
}
double qspline::deriv(double z) const {
    int i = binsearch(z);
    double dx = z - x[i];

    return b[i] + 2*c[i]*dx;
}
double qspline::integ(double z) const {
    int i = binsearch(z);

    double sum = 0;

    for(int k = 0; k < i; k++){
        double dx = x[k+1] - x[k];

        sum += y[k]*dx
             + b[k]*dx*dx/2
             + c[k]*dx*dx*dx/3;
    }

    double dx = z - x[i];

    sum += y[i]*dx
         + b[i]*dx*dx/2
         + c[i]*dx*dx*dx/3;

    return sum;
}
//Belwo constructor made solely by AI
cspline::cspline(const vec& xs, const vec& ys)
    : n(xs.size()), x(xs), y(ys), b(n-1), c(n-1), d(n-1)
{
    assert(xs.size() == ys.size());
    assert(n >= 2);

    vec h(n-1), p(n-1);

    for(int i=0;i<n-1;i++){
        h[i] = x[i+1]-x[i];
        assert(h[i] > 0);
        p[i] = (y[i+1]-y[i])/h[i];
    }

    // Natural cubic spline: second derivatives M[0]=M[n-1]=0
    vec a(n), diag(n), upper(n), rhs(n), M(n);

    diag[0] = 1;
    rhs[0] = 0;
    upper[0] = 0;

    for(int i=1;i<n-1;i++){
        a[i]     = h[i-1];
        diag[i]  = 2*(h[i-1] + h[i]);
        upper[i] = h[i];
        rhs[i]   = 6*(p[i] - p[i-1]);
    }

    a[n-1] = 0;
    diag[n-1] = 1;
    rhs[n-1] = 0;

    // Forward elimination
    for(int i=1;i<n;i++){
        double m = a[i]/diag[i-1];
        diag[i] -= m*upper[i-1];
        rhs[i]  -= m*rhs[i-1];
    }

    // Back substitution
    M[n-1] = rhs[n-1]/diag[n-1];

    for(int i=n-2;i>=0;i--){
        M[i] = (rhs[i] - upper[i]*M[i+1])/diag[i];
    }

    // Convert second derivatives to polynomial coefficients
    for(int i=0;i<n-1;i++){
        b[i] = p[i] - h[i]*(2*M[i] + M[i+1])/6;
        c[i] = M[i]/2;
        d[i] = (M[i+1] - M[i])/(6*h[i]);
    }
}
int cspline::binsearch(double z) const{
    int i=0,j=n-1;
    while(j-i>1){
        int m=(i+j)/2;
        if(z>x[m]) i=m;
        else j=m;
    }
    return i;
}
double cspline::eval(double z) const{
    int i=binsearch(z);
    double dx=z-x[i];

    return y[i]
        + b[i]*dx
        + c[i]*dx*dx
        + d[i]*dx*dx*dx;
}
double cspline::deriv(double z) const{
    int i=binsearch(z);
    double dx=z-x[i];

    return b[i]
        +2*c[i]*dx
        +3*d[i]*dx*dx;
}
double cspline::integ(double z) const{
    int k=binsearch(z);

    double sum=0;

    for(int i=0;i<k;i++){
        double dx=x[i+1]-x[i];

        sum += y[i]*dx
            + b[i]*dx*dx/2
            + c[i]*dx*dx*dx/3
            + d[i]*dx*dx*dx*dx/4;
    }

    double dx=z-x[k];

    sum += y[k]*dx
        + b[k]*dx*dx/2
        + c[k]*dx*dx*dx/3
        + d[k]*dx*dx*dx*dx/4;

    return sum;
}