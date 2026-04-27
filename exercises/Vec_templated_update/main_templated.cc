#include "vec_templated.h"
#include <iostream>
#include <cassert>

int main() {

    // ===== double version =====

    // construction
    vec<double> a;
    vec<double> b(1.0, 2.0, 3.0);
    vec<double> c = b;                         // copy
    vec<double> d = vec<double>(4.0, 5.0, 6.0); // move (temporary)

    std::cout << "double vectors:\n";
    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "\n";
    std::cout << "d = " << d << "\n";

    // arithmetic
    vec<double> e = b + d;
    vec<double> f = d - b;
    vec<double> g = 2.0 * b;
    vec<double> h = b * 3.0;

    std::cout << "b + d = " << e << "\n";
    std::cout << "d - b = " << f << "\n";
    std::cout << "2*b = " << g << "\n";
    std::cout << "b*3 = " << h << "\n";

    // compound ops
    vec<double> x(1.0, 1.0, 1.0);
    x += b;
    x *= 2.0;

    std::cout << "x after += and *= : " << x << "\n";

    // dot product and cross product
    double dot_bd = b * d;
    vec<double> cross_bd = b.cross(d);

    std::cout << "b dot d = " << dot_bd << "\n";
    std::cout << "b cross d = " << cross_bd << "\n";

    // approx
    vec<double> p(1.0, 2.0, 3.0);
    vec<double> q(1.0 + 1e-7, 2.0, 3.0);

    assert(approx(p, q));

    // chaining
    vec<double> r = b + d + g;
    std::cout << "chaining: " << r << "\n";

    // ===== float version =====

    vec<float> bf(1.0f, 2.0f, 3.0f);
    vec<float> df(4.0f, 5.0f, 6.0f);

    vec<float> ef = bf + df;
    vec<float> gf = 2.0f * bf;
    float dot_float = bf * df;

    std::cout << "\nfloat vectors:\n";
    std::cout << "bf + df = " << ef << "\n";
    std::cout << "2*bf = " << gf << "\n";
    std::cout << "bf dot df = " << dot_float << "\n";

    assert(approx(bf, bf));

    std::cout << "\nAll tests passed.\n";

    return 0;
}
