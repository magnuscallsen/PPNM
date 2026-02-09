#include <iostream>
#include<complex>
#include<cmath>
#include "sfuns.h"


using complex=std::complex<double>;
constexpr double pi = 3.14159265358979324;
constexpr double e = 2.71828182845904523;
constexpr complex I = complex(0, 1);

int main(){
    std::cout << "log(I)=" << std::log(I)   <<"\n";
    std::cout << "   I^I=" << std::pow(I,I) <<"\n";
    std::cout << "   π^I=" << std::pow(pi,I) <<"\n";
    std::cout << "   E^I=" << std::pow(e,I) <<"\n";
    std::cout << "   2^(1/5)=" << std::pow(2.0,0.2) <<"\n";
    std::cout << " pi^e=" << std::pow(pi, e)<<"\n";

    for (int n = 1; n<=10; ++n){
	std::cout << "Gamma(" << n << ")    = "
                  << sfuns::fgamma(n) << "\n";
        std::cout << "lnGamma(" << n << ") = "
                  << sfuns::lngamma(n) << "\n\n";
    }
    return 0;
}
