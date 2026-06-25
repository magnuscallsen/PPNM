#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

constexpr double PI = 3.141592653589793238462643383279502884;

double lngamma(double x){
	if(x<=0) return std::numeric_limits<double>::quiet_NaN();
	if(x<9) return lngamma(x+1)-log(x);
return x*std::log(x+1/(12*x-1/x/10))-x+std::log(2*PI/x)/2;
}
	


double erf_approx(double x) {
    if (x < 0) return -erf_approx(-x);

    std::vector<double> a {
        0.254829592,
        -0.284496736,
        1.421413741,
        -1.453152027,
        1.061405429
    };

    double t = 1 / (1 + 0.3275911 * x);

    double sum = t * (
        a[0] + t * (
        a[1] + t * (
        a[2] + t * (
        a[3] + t * a[4]))));

    return 1 - sum * std::exp(-x * x);
}
double sgamma(double x){
	if(x<0)return PI/std::sin(PI*x)/sgamma(1-x);
	if(x<9)return sgamma(x+1)/x;
	double lnsgamma=std::log(2*PI)/2+(x-0.5)*std::log(x)-x
    		+(1.0/12)/x-(1.0/360)/(x*x*x)+(1.0/1260)/(x*x*x*x*x);
	return std::exp(lnsgamma);
}


//double gamma_approx(double z) {
    // Lanczos approximation
    // Valid for positive and negative non-integer z

 //   static const double p[] = {
 //       676.5203681218851,
 //      -1259.1392167224028,
 //       771.32342877765313,
 //      -176.61502916214059,
 //        12.507343278686905,
 //        -0.13857109526572012,
 //         9.9843695780195716e-6,
 //         1.5056327351493116e-7
 //   };

 //   if (z < 0.5) {
 //       // reflection formula
 //       return M_PI / (std::sin(M_PI * z) * gamma_approx(1.0 - z));
 //   }

 //   z -= 1.0;

//    double x = 0.99999999999980993;
//    for (int i = 0; i < 8; ++i) {
 //       x += p[i] / (z + i + 1.0);
 //   }

  //  double t = z + 7.5;

//    return std::sqrt(2.0 * M_PI)
//         * std::pow(t, z + 0.5)
//         * std::exp(-t)
//         * x;
//}

int main() {
  //  for (double x = -3.0; x <= 3.0; x += 0.01) {
  //      std::cout << x << " " << erf_approx(x) << " " << std::erf(x) << "\n";
 //   }
	

//	for (double x = 0.1; x <= 10.0; x += 0.01) {
//		std::cout << x << " " << sgamma(x) << " " << std::tgamma(x) << "\n";
//	 }
	
	for (double x = 0.1; x <= 10.0; x += 0.01) {
		std::cout << x << " " << lngamma(x) << " " << std::lgamma(x) << "\n";
	}



    return 0;
}
