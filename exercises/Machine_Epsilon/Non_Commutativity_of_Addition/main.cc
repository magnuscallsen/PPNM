#include<iostream>
#include<iomanip>
#include<cmath>

int main(){
	double epsilon=std::pow(2,-52);
	double tiny=epsilon/2;
	double a=1+tiny+tiny;
	double b=tiny+tiny+1;
	std::cout << "a==b ? " << (a==b ? "true":"false") << "\n";
	std::cout << "a>1  ? " << (a>1  ? "true":"false") << "\n";
	std::cout << "b>1  ? " << (b>1  ? "true":"false") << "\n";
	
	std::cout << std::fixed << std::setprecision(17);
	std::cout << "       tiny=" << tiny << "\n";
	std::cout << "1+tiny+tiny=" << a << "\n";
	std::cout << "tiny+tiny+1=" << b << "\n";

	return 0;
}
