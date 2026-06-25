#include <iostream>

int main(){

	double x;

	while (std::cin >> x) {
    		const double s = std::sin(x);
    		const double c = std::cos(x);

    		std::cout << x << ' ' << s << ' ' << c << '\n';
	}
	return 0;
}
