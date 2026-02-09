#include "vec.h"
#include <iostream>
#include <cassert>

int main() {

    // construction
    vec a;
    vec b(1,2,3);
    vec c = b;            // copy
    vec d = vec(4,5,6);   // move (temporary)

    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "\n";
    std::cout << "d = " << d << "\n";

    // arithmetic
    vec e = b + d;
    vec f = d - b;
    vec g = 2 * b;
    vec h = b * 3;

    std::cout << "b + d = " << e << "\n";
    std::cout << "d - b = " << f << "\n";
    std::cout << "2*b = " << g << "\n";
    std::cout << "b*3 = " << h << "\n";

    // compound ops
    vec x(1,1,1);
    x += b;
    x *= 2;

    std::cout << "x after += and *= : " << x << "\n";

    // approx
    vec p(1.0, 2.0, 3.0);
    vec q(1.0 + 1e-7, 2.0, 3.0);

    assert(approx(p, q));

    // chaining
    vec r = b + d + g;
    std::cout << "chaining: " << r << "\n";

    std::cout << "All tests passed.\n";
}
