#include <limits>
#include <iostream>
#include <cstdio>

int main() {
    float f = 1.0f;
    while ((float)(1.0f + f) != 1.0f) {
        f /= 2.0f;
    }
    f *= 2.0f;

    double d = 1.0;
    while ((double)(1.0 + d) != 1.0) {
        d /= 2.0;
    }
    d *= 2.0;

    long double l = 1.0L;
    while ((long double)(1.0L + l) != 1.0L) {
        l /= 2.0L;
    }
    l *= 2.0L;

    std::printf("      float eps=%g\n", f);
    std::printf("     double eps=%g\n", d);
    std::printf("long double eps=%Lg\n", l);

    std::cout << std::numeric_limits<float>::epsilon() << "\n";
    std::cout << std::numeric_limits<double>::epsilon() << "\n";
    std::cout << std::numeric_limits<long double>::epsilon() << "\n";

    return 0;
}
