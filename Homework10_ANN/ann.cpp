// Code is written by AI

#include "ann.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "minimization.hpp"

ann::ann(int n_) : n(n_), p(3*n_) {
    if(n <= 0) throw std::invalid_argument("ann: number of neurons must be positive");

    // Reasonable initial guess for data on [-1,1].
    for(int i = 0; i < n; ++i) {
        const double t = (n == 1) ? 0.0 : -1.0 + 2.0*i/(n - 1.0);
        p[3*i + 0] = t;       // a_i: center
        p[3*i + 1] = 0.30;    // b_i: width, must be nonzero
        p[3*i + 2] = 0.05;    // w_i: output weight
    }
}

double ann::f(double z) {
    return z*std::exp(-z*z);
}

double ann::df(double z) {
    return (1.0 - 2.0*z*z)*std::exp(-z*z);
}

double ann::response(double x) const {
    double sum = 0.0;
    for(int i = 0; i < n; ++i) {
        const double a = p[3*i + 0];
        const double b = p[3*i + 1];
        const double w = p[3*i + 2];
        const double z = (x - a)/b;
        sum += w*f(z);
    }
    return sum;
}

double ann::cost(const pp::vector& xs, const pp::vector& ys) const {
    if(xs.size() != ys.size()) throw std::invalid_argument("cost: x and y sizes differ");

    double c = 0.0;
    for(int k = 0; k < xs.size(); ++k) {
        const double r = response(xs[k]) - ys[k];
        c += r*r;
    }
    return c;
}

pp::vector ann::cost_gradient(const pp::vector& xs, const pp::vector& ys) const {
    if(xs.size() != ys.size()) throw std::invalid_argument("gradient: x and y sizes differ");

    pp::vector g(3*n);

    for(int k = 0; k < xs.size(); ++k) {
        const double x = xs[k];
        const double error = response(x) - ys[k];

        for(int i = 0; i < n; ++i) {
            const double a = p[3*i + 0];
            const double b = p[3*i + 1];
            const double w = p[3*i + 2];
            const double z = (x - a)/b;

            // F_i = w_i*f(z), z=(x-a_i)/b_i.
            // dC/dq = 2*sum_k error_k*dF/dq.
            g[3*i + 0] += 2.0*error*w*df(z)*(-1.0/b);       // d/da_i
            g[3*i + 1] += 2.0*error*w*df(z)*(-z/b);         // d/db_i
            g[3*i + 2] += 2.0*error*f(z);                   // d/dw_i
        }
    }

    return g;
}

void ann::train(const pp::vector& xs, const pp::vector& ys, int max_steps, double acc) {
    double c = cost(xs, ys);

    for(int step = 0; step < max_steps; ++step) {
        pp::vector g = cost_gradient(xs, ys);
        const double gnorm = g.norm();

        if(gnorm < acc) {
            std::cerr << "training converged after " << step
                      << " steps, cost=" << c << ", |grad|=" << gnorm << "\n";
            return;
        }

        // Steepest descent with Armijo backtracking. This is slow but robust
        // for a small homework network and uses the analytic gradient above.
        double lambda = 0.05;
        bool accepted = false;

        while(lambda > 1e-12) {
            pp::vector trial = p - lambda*g;

            // Keep widths away from zero.
            for(int i = 0; i < n; ++i) {
                if(std::abs(trial[3*i + 1]) < 1e-3)
                    trial[3*i + 1] = (trial[3*i + 1] < 0 ? -1e-3 : 1e-3);
            }

            pp::vector oldp = p;
            p = trial;
            const double c_trial = cost(xs, ys);

            if(c_trial < c) {
                c = c_trial;
                accepted = true;
                break;
            }

            p = oldp;
            lambda *= 0.5;
        }

        if(!accepted) {
            std::cerr << "training stopped: line search failed at step " << step
                      << ", cost=" << c << ", |grad|=" << gnorm << "\n";
            return;
        }

        if(step % 1000 == 0)
            std::cerr << "step " << step << " cost " << c << " |grad| " << gnorm << "\n";
    }

    std::cerr << "training reached max_steps, cost=" << c
              << ", |grad|=" << cost_gradient(xs, ys).norm() << "\n";
}
double ann::derivative(double x){
    double sum = 0;

    for(int i = 0; i < n; i++){
        double a = p[3*i];
        double b = p[3*i+1];
        double w = p[3*i+2];

        double z = (x-a)/b;

        sum += w*(1.0/b)*(1 - 2*z*z)*std::exp(-z*z);
    }

    return sum;
}

double ann::second_derivative(double x){
    double sum = 0;

    for(int i = 0; i < n; i++){
        double a = p[3*i];
        double b = p[3*i+1];
        double w = p[3*i+2];

        double z = (x-a)/b;

        sum += w*(1.0/(b*b))*(4*z*z*z - 6*z)*std::exp(-z*z);
    }

    return sum;
}

double ann::anti_derivative(double x){
    double sum = 0;

    for(int i = 0; i < n; i++){
        double a = p[3*i];
        double b = p[3*i+1];
        double w = p[3*i+2];

        double z = (x-a)/b;

        sum += -0.5*w*b*std::exp(-z*z);
    }

    return sum;
}
void ann::train_de(
    double a, double b,
    double c,
    double yc,
    double dyc,
    double alpha,
    double beta
){
    auto Phi = [this](double x){
        /*
            Example differential equation:

                y'' + y = 0

            Therefore

                Phi[y] = y''(x) + y(x)

            Exact solution depends on boundary conditions.
        */

        return this->second_derivative(x) + this->response(x);
    };

    auto cost = [&](const pp::vector& q){
        pp::vector oldp = p;
        p = q;

        int N = 30;
        double h = (b-a)/(N-1);

        double integral = 0;

        for(int i = 0; i < N; i++){
            double x = a + i*h;
            double phix = Phi(x);

            double weight = 1.0;
            if(i == 0 || i == N-1) weight = 0.5;

            integral += weight*phix*phix;
        }

        integral *= h;

        double bc1 = response(c) - yc;
        double bc2 = derivative(c) - dyc;

        double result = integral + alpha*bc1*bc1 + beta*bc2*bc2;

        p = oldp;
        return result;
    };

    minopt::NewtonResult result = minopt::newton_central(cost, p, 1e-4, 150);
    p = result.x;

    std::cout << "\nDifferential equation training:\n";
    std::cout << "Cost       = " << result.fx << "\n";
    std::cout << "Steps      = " << result.steps << "\n";
    std::cout << "Converged  = " << result.converged << "\n";
}