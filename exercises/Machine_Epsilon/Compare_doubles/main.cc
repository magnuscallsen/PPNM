#include<iostream>
#include<iomanip>
#include<cmath>

bool approx(double a, double b, double acc = 1e-9, double eps = 1e-9) {
    double diff = std::abs(a - b);
    if (diff <= acc) return true;  // absolute tolerance
    double max_ab = std::max(std::abs(a), std::abs(b));
    return diff <= eps * max_ab;   // relative tolerance
}

int main(){
	std::cout << "Introduction" << "\n";
	
	double d1 = 0.1+0.1+0.1+0.1+0.1+0.1+0.1+0.1;
	double d2 = 8*0.1;
	
	std::cout << "d1==d2? " << (d1==d2 ? "true":"false") << "\n";
	
	std::cout << std::fixed << std::setprecision(17);
	std::cout << "d1=" << d1 << "\n";
	std::cout << "d2=" << d2 << "\n";

	std::cout << "The Task" <<"\n";
	std::cout << "Bool_approx(d1,d2)="
              << (approx(d1, d2) ? "true" : "false")
              << "\n";
	return 0;
}
