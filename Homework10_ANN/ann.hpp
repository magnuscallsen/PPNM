#pragma once

#include <functional>
#include "matrix.hpp"

struct ann {
    int n;                 // number of hidden neurons
    pp::vector p;          // parameters: a_i, b_i, w_i stored as p[3*i+0..2]

    ann(int n);

    static double f(double z);   // Gaussian wavelet activation: z*exp(-z^2)
    static double df(double z);  // derivative: (1-2z^2)*exp(-z^2)
    double derivative(double x);
    double second_derivative(double x);
    double anti_derivative(double x);
    double response(double x) const;
    double cost(const pp::vector& xs, const pp::vector& ys) const;
    pp::vector cost_gradient(const pp::vector& xs, const pp::vector& ys) const;
    void train(const pp::vector& xs, const pp::vector& ys,
               int max_steps = 20000, double acc = 1e-7);

    void train_de(double a, double b, double c, double yc, double dyc, double alpha, double beta);

};

