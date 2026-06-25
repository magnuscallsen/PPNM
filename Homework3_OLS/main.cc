#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <fstream>
#include "matrix.hpp"
#include "lsfit.hpp"

int main()
{
    // Rutherford-Soddy (1902) measurements of the activity of ThX.
    // Radioactive decay is assumed to follow
    //     y(t) = a*exp(-lambda*t)

    pp::vector t(9), y(9), dy(9);

    t[0]=1;  y[0]=117;  dy[0]=6;
    t[1]=2;  y[1]=100;  dy[1]=5;
    t[2]=3;  y[2]=88;   dy[2]=4;
    t[3]=4;  y[3]=72;   dy[3]=4;
    t[4]=6;  y[4]=53;   dy[4]=4;
    t[5]=9;  y[5]=29.5; dy[5]=3;
    t[6]=10; y[6]=25.2; dy[6]=3;
    t[7]=13; y[7]=15.2; dy[7]=2;
    t[8]=15; y[8]=11.1; dy[8]=2;

    // Linearize the model:
    //     ln(y) = ln(a) - lambda*t
    // and propagate uncertainties:
    //     dln(y) = dy/y

    pp::vector lny(9), dlny(9);

    for(int i=0; i<9; i++){
        lny[i]  = std::log(y[i]);
        dlny[i] = dy[i]/y[i];
    }

    // Basis functions:
    //     f0(t)=1
    //     f1(t)=t

    std::vector<pp::Func> fs = {
        [](double){ return 1.0; },
        [](double z){ return z; }
    };

    // Least-squares fit using QR decomposition.

    auto [c,cov] = pp::lsfit(fs,t,lny,dlny);


    // c0 = ln(a), c1 = -lambda

    double ln_a   = c[0];
    double lambda = -c[1];
    double a      = std::exp(ln_a);

    std::cout << "We consider data from the Rutherford experiment (1902) of the radioactive decay of ThX.\n";
    std::cout << "We do a linear plot of the data (after taking logarithm) and report the fitting parameters found.\n";

    std::cout << "ln(a)  = " << ln_a << "\n";
    std::cout << "a      = " << a << "\n";
    std::cout << "lambda = " << lambda << "\n";

    std::cout << "\nFit function:\n";
    std::cout << "y(t) = "
              << a
              << "*exp(-"
              << lambda
              << "*t)\n";

    double d_ln_a = std::sqrt(cov(0,0));
    double d_lambda = std::sqrt(cov(1,1));

    
    double T = std::log(2)/lambda;
    double dT = std::log(2)/(lambda*lambda)*d_lambda;

    std::cout << "d ln(a)  = " << d_ln_a << "\n";
    std::cout << "d lambda = " << d_lambda << "\n";
    std::cout << "d T_1/2  = " << dT << " days\n";
    std::cout << "The difference between the Rutherford value and the modern one is more than is more than the uncertainty. \n";
    double T_half = std::log(2.0)/lambda;
    double modern_T_half = 3.6316; // 224Ra (AI)

    std::ofstream out("Out.txt");
    std::ofstream data("data.txt");
    std::ofstream fit("fit.txt");

    out << "We consider data from the Rutherford experiment (1902) of the radioactive decay of ThX.\n";
    out << "We do a linear fit after taking logarithms.\n\n";



    out << "ln(a)  = " << ln_a << "\n";
    out << "a      = " << a << "\n";
    out << "lambda = " << lambda << "\n";
    out << "T_1/2  = " << T_half << " days\n";
    out << "Modern value for 224Ra = " << modern_T_half << " days\n";

    // Below written entirely by AI

    out << "Difference = " << T_half-modern_T_half << " days\n";

    for(int i=0; i<9; i++)
        data << t[i] << " " << y[i] << " " << dy[i] << "\n";

    for(double z=0; z<=16; z+=0.1)
        fit << z << " "
            << a*std::exp(-lambda*z)
            << "\n";

    // Below is in part written by me but mostly by AI
    pp::vector dc(2);
    dc[0] = std::sqrt(cov(0,0));
    dc[1] = std::sqrt(cov(1,1));

    std::ofstream band("fit_uncertainty.txt");

    int m = c.size();

    for(double z=0; z<=16; z+=0.1){
        double best_log = 0;
        for(int k=0; k<m; k++)
            best_log += c[k]*fs[k](z);

        band << z << " " << std::exp(best_log);

    // all 2^m combinations of c_k ± dc_k
        for(int mask=0; mask < (1<<m); mask++){
            double varied_log = 0;

            for(int k=0; k<m; k++){
                double sign = (mask & (1<<k)) ? +1.0 : -1.0;
                varied_log += (c[k] + sign*dc[k]) * fs[k](z);
            }

            band << " " << std::exp(varied_log);
        }

        band << "\n";
    }
    std::cout << "In fit_uncertainty one sees both the best fit and fits where one shifts the parameters with a standard deviation \n";
    std::cout << "These deviations in these fits seem to fit well with experimental uncertainties \n";
    

    return 0;
}