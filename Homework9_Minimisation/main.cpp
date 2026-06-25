// The code is mostly written by AI
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include "minimization.hpp"
#include <vector>
#include <fstream>

double breit_wigner(double E, double m, double Gamma, double A) {
    return A / ((E - m)*(E - m) + Gamma*Gamma/4.0);
}

static void print_result(const std::string& name, const minopt::NewtonResult& r) {
    std::cout << name << "\n";
    std::cout << "  converged  = " << std::boolalpha << r.converged << "\n";
    std::cout << "  steps      = " << r.steps << "\n";
    std::cout << "  x          = (";
    for(int i = 0; i < r.x.size(); ++i) {
        if(i) std::cout << ", ";
        std::cout << r.x[i];
    }
    std::cout << ")\n";
    std::cout << "  f(x)       = " << r.fx << "\n";
    std::cout << "  |grad f|   = " << r.grad_norm << "\n\n";
}

int main() {
    std::cout << "========= Part A - Newton's Method =============\n";
    std::cout << "We employ Newton's method to a find a minimum of the following two functions. And fairly quickly and surely find the minimum. \n";

    std::cerr << std::setprecision(12);
    std::cout << std::setprecision(12);

    minopt::Function rosenbrock = [](const pp::vector& v) {
        const double x = v[0];
        const double y = v[1];
        return std::pow(1.0 - x, 2) + 100.0 * std::pow(y - x * x, 2);
    };

    minopt::Function himmelblau = [](const pp::vector& v) {
        const double x = v[0];
        const double y = v[1];
        return std::pow(x * x + y - 11.0, 2) + std::pow(x + y * y - 7.0, 2);
    };

    const double acc = 1e-3;

    // Common starting points. Change them to test other basins of attraction.
    pp::vector rosen_start = {-1.2, 1.0};
    pp::vector himmel_start = {2.0, 2.0};

    auto rosen_result = minopt::newton(rosenbrock, rosen_start, acc);
    auto himmel_result = minopt::newton(himmelblau, himmel_start, acc);

    print_result("Rosenbrock from (-1.2, 1)", rosen_result);
    print_result("Himmelblau from (2, 2)", himmel_result);

    std::cout << "========= Part B - Higgs Boson =============\n";

    std::vector<double> energy, signal, error;
    double x, y, z;

    while (std::cin >> x >> y >> z) {
        energy.push_back(x);
        signal.push_back(y);
        error.push_back(z);
    }

    if (energy.size() > 0) {
        auto D = [&](const pp::vector& p) {
            double m = p[0];
            double Gamma = std::abs(p[1]);
            double A = p[2];

            double sum = 0.0;
            for (size_t i = 0; i < energy.size(); i++) {
               double F = breit_wigner(energy[i], m, Gamma, A);
               double r = (F - signal[i]) / error[i];
               sum += r*r;
            }
            return sum;
        };

        pp::vector start(3);
        start[0] = 125.0;  // m
        start[1] = 2.0;    // Gamma
        start[2] = 10.0;   // A

        //int steps = 0;
        minopt::NewtonResult result = minopt::newton(D, start, 1e-6);
        pp::vector fit = result.x;
        int steps = result.steps;
        
        double m = fit[0];
        double Gamma = std::abs(fit[1]);
        double A = fit[2];

        std::cout << "Higgs fit:\n";
        std::cout << "m      = " << m << " GeV\n";
        std::cout << "Gamma  = " << Gamma << " GeV\n";
        std::cout << "A      = " << A << "\n";
        std::cout << "steps  = " << steps << "\n";

        std::ofstream fitfile("out.txt");

        for (double E = 101; E <= 159; E += 0.2) {
            fitfile << E << " " << breit_wigner(E, m, Gamma, A) << "\n";
        }
    }
    std::cout << "Above the Higgs fit parameters are given In higgs-fit.pdf we see the fit. .\n";
    std::cout << "========= Part C - Central finite differences =============\n";

    auto rosen_central = minopt::newton_central(rosenbrock, rosen_start, acc);
    auto himmel_central = minopt::newton_central(himmelblau, himmel_start, acc);

    print_result("Rosenbrock with central differences", rosen_central);
    print_result("Himmelblau with central differences", himmel_central);

    std::cout << "Comparison:\n";
    std::cout << "Rosenbrock forward steps = " << rosen_result.steps
          << ", central steps = " << rosen_central.steps << "\n";

    std::cout << "Himmelblau forward steps = " << himmel_result.steps
          << ", central steps = " << himmel_central.steps << "\n\n";



    return 0;
}
