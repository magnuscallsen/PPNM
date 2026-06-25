#include <iostream>
#include "spline.h"
#include <fstream>
#include <cmath>

int main() {
    vec x = {0, 1, 2, 3, 4};
    vec y = {0, 1, 0, 1, 0};

    std::cout <<" ========== Linear Spline ================== \n";


    std::cout <<" For a set of data points x = (0,1,2,3,4) and y=(0,1,0,1,0) we determine linearly interpolated values \n";

    for (double z = x.front(); z <= x.back(); z += 0.7) {
    std::cout
        << "z = " << z
        << " yields linterp(" << z << ") = "
        << linterp(x, y, z)
        << '\n';
    }
    std::cout <<" For z=2.5 we calculate the integral \n";
    double z=2.5;
    double integral = linterpInteg(x, y, z);
    std::cout
        << "For z =" << z
        << " the integral of the linearinterpolated function up to z equals "
        << integral
        << '\n';
    
    // Data for gnuplot: xi = 0, 0.5, 1, ..., 9 and yi = cos(xi)
    vec xcos, ycos;

    for(double xi = 0; xi <= 9; xi += 0.5){
        xcos.push_back(xi);
        ycos.push_back(std::cos(xi));
    }

    std::ofstream interp_data("linear_spline.data");
    std::ofstream points_data("points.data");

    for(size_t i = 0; i < xcos.size(); i++){
        points_data << xcos[i] << " " << ycos[i] << "\n";
    }

    for(double zplot = xcos.front(); zplot <= xcos.back(); zplot += 0.01){
        interp_data << zplot << " "
                << std::cos(zplot) << " "
                << linterp(xcos, ycos, zplot) << " "
                << linterpInteg(xcos, ycos, zplot) << "\n";
    }

    interp_data.close();
    points_data.close();

    std::cout << "Wrote linear_spline.data and points.data for gnuplot\n";
    std::cout <<" ========== Linear Spline ================== \n";
    std::cout <<" ========== Quadractic Spline ================== \n";

    vec xq = {1, 2, 3, 4, 5};

    vec y_const = {1, 1, 1, 1, 1};
    vec y_linear = {1, 2, 3, 4, 5};
    vec y_quad = {1, 4, 9, 16, 25};

    qspline s_const(xq, y_const);
    qspline s_linear(xq, y_linear);
    qspline s_quad(xq, y_quad);

    std::cout << "\nQuadratic spline coefficient tests\n";

    std::cout << "Constant function y=1:\n";
    for(int i = 0; i < s_const.n-1; i++){
        std::cout << "i=" << i
              << " b=" << s_const.b[i]
              << " c=" << s_const.c[i] << "\n";
    }

    std::cout << "\nLinear function y=x:\n";
    for(int i = 0; i < s_linear.n-1; i++){
    std::cout << "i=" << i
              << " b=" << s_linear.b[i]
              << " c=" << s_linear.c[i] << "\n";
    }

    std::cout << "\nQuadratic function y=x^2:\n";
    for(int i = 0; i < s_quad.n-1; i++){
        std::cout << "i=" << i
              << " b=" << s_quad.b[i]
              << " c=" << s_quad.c[i] << "\n";
    }

    vec xxcos, yycos;

    for(double xi = 0; xi <= 9; xi += 0.5){
        xxcos.push_back(xi);
        yycos.push_back(std::cos(xi));
    }

    qspline qcos(xxcos, yycos);

    std::ofstream qdata("quadratic_spline.data");

    for(double z = xxcos.front(); z <= xxcos.back(); z += 0.01){
        qdata << z << " "
          << std::cos(z) << " "
          << qcos.eval(z) << " "
          << qcos.deriv(z) << " "
          << qcos.integ(z) << "\n";
    }

    qdata.close();

    std::cout << "Note that we find b,c=0 for constant function which makes sense as s_i = y_i +0\n";
    std::cout << "Note that we find {b,c}={1, 0} for linear function which makes sense as s_i = y_i + x-x_i=x\n";
    std::cout << "Note that we find {b,c}={2x_i, 1}. letting y_i = x_i^2.\n";
    std::cout << "We see s_i = x_i^2 +2x_i(x-x_i)+(x-x_i)^2=x^2.\n";
    std::cout <<" ========== Quadractic Spline ================== \n";
    std::cout <<" ========== Cubic Spline ================== \n";

    cspline cs(xcos, ycos);

    std::ofstream cubic("cubic_spline.data");

    for(double z = xcos.front(); z <= xcos.back(); z += 0.01){
        cubic
           << z << " "
           << std::cos(z) << " "
           << cs.eval(z) << " "
           << cs.deriv(z) << " "
           << cs.integ(z)
           << "\n";
    }

    cubic.close();

    std::cout << "Wrote cubic_spline.data for gnuplot\n";
    std::cout <<" ========== Cubic Spline ================== \n";
    return 0;
}