// Code written by AI
#include "ann.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

double g(double x){
    return std::cos(5*x - 1)*std::exp(-x*x);
}

double dg(double x){
    return std::exp(-x*x)*
           (-5*std::sin(5*x - 1) - 2*x*std::cos(5*x - 1));
}

double ddg(double x){
    return std::exp(-x*x)*
           ((4*x*x - 27)*std::cos(5*x - 1)
            + 20*x*std::sin(5*x - 1));
}

int main() {
    std::cout << "Artificial Neural Network interpolation using a 3-layer ANN with Gaussian wavelet activation.\n";
    std::cout << "The network parameters are trained by minimizing the least-squares cost function using Newton's method.\n";
    std::cout << "Approximating g(x) = cos(5x-1)exp(-x^2) on the interval [-1,1]. See plot in ann_fit.png\n\n";
    const int npoints = 25;
    const int nneurons = 8;

    pp::vector xs(npoints), ys(npoints);
    for(int k = 0; k < npoints; ++k) {
        xs[k] = -1.0 + 2.0*k/(npoints - 1.0);
        ys[k] = g(xs[k]);
    }

    ann network(nneurons);
    std::cout << "initial cost = " << network.cost(xs, ys) << "\n";
    network.train(xs, ys);
    std::cout << "final cost   = " << network.cost(xs, ys) << "\n";

    std::ofstream table("data.txt");
    table << std::setprecision(16);
    for(int k = 0; k < npoints; ++k)
        table << xs[k] << " " << ys[k] << "\n";

    std::ofstream curve("fit.txt");
    curve << std::setprecision(16);
    for(int i = 0; i <= 400; ++i) {
        const double x = -1.0 + 2.0*i/400.0;
        curve << x << " " << g(x) << " " << network.response(x) << "\n";
    }
    double x0 = 0.0;

    std::cout << "\nComparison at x = " << x0 << "\n";
    std::cout << "Quantity        ANN              Exact function\n";
    std::cout << "f(x)        " << network.response(x0)
          << "        " << g(x0) << "\n";

    std::cout << "f'(x)       " << network.derivative(x0)
          << "        " << dg(x0) << "\n";

    std::cout << "f''(x)      " << network.second_derivative(x0)
          << "        " << ddg(x0) << "\n";

    std::cout << "Integral    " << network.anti_derivative(x0)
          << "        "
          << "no simple elementary form\n";

    double a_de = 0.0;
    double b_de = M_PI/2.0;
    ann de_network(8);
    for(int i = 0; i < de_network.n; i++){
        double t = a_de + (b_de-a_de)*i/(de_network.n-1.0);

        de_network.p[3*i+0] = t;
        de_network.p[3*i+1] = 0.5;
        de_network.p[3*i+2] = 0.1*((i % 2 == 0) ? 1.0 : -1.0);
    }
    double alpha = 10000;
    double beta  = 10000;

    

    double c = 0.0;
    double yc = 0.0;
    double dyc = 1.0;


    de_network.train_de(a_de, b_de, c, yc, dyc, alpha, beta);

    std::cout << "\nDifferential equation example: y'' + y = 0\n";
    std::cout << "Boundary conditions: y(0)=0, y'(0)=1\n";
    std::cout << "Exact solution: y=sin(x)\n\n";

    double x_de = M_PI/4.0;

    std::cout << "Comparison at x = " << x_de << "\n";
    std::cout << "ANN solution  = " << de_network.response(x_de) << "\n";
    std::cout << "Exact sin(x)  = " << std::sin(x_de) << "\n";
    std::cout << "ANN y'(x)     = " << de_network.derivative(x_de) << "\n";
    std::cout << "Exact cos(x)  = " << std::cos(x_de) << "\n";
    std::cout << "ANN y''+y     = "
          << de_network.second_derivative(x_de) + de_network.response(x_de)
          << "\n";
    std::ofstream de_plot("de_solution.dat");

    for(int i = 0; i < 200; i++){
            double x = a_de + (b_de - a_de)*i/199.0;

        de_plot << x << " "
            << de_network.response(x) << " "
            << std::sin(x) << "\n";
    }

    de_plot.close();

    std::cout << "Converges slow, cut short here.\n\n";

    return 0;
}
