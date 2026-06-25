#include "qmc.hpp"

#include <cmath>
#include <stdexcept>

Halton::Halton(const std::vector<int>& bases_, unsigned long long start_index)
    : bases(bases_), index(start_index) {}

double Halton::radical_inverse(unsigned long long n, int base) {
    double result = 0.0;
    double f = 1.0 / static_cast<double>(base);
    while (n > 0) {
        result += f * static_cast<double>(n % base);
        n /= base;
        f /= static_cast<double>(base);
    }
    return result;
}

std::vector<double> Halton::next() {
    std::vector<double> x(bases.size());
    for (std::size_t i = 0; i < bases.size(); ++i) {
        x[i] = radical_inverse(index, bases[i]);
    }
    ++index;
    return x;
}

static double qmc_average(const std::function<double(const std::vector<double>&)>& f,
                          const std::vector<double>& a,
                          const std::vector<double>& b,
                          unsigned long long N,
                          Halton& sequence) {
    const std::size_t dim = a.size();
    std::vector<double> x(dim);

    double sum = 0.0;
    for (unsigned long long k = 0; k < N; ++k) {
        std::vector<double> u = sequence.next();
        for (std::size_t i = 0; i < dim; ++i) x[i] = a[i] + u[i] * (b[i] - a[i]);
        sum += f(x);
    }
    return sum / static_cast<double>(N);
}

MCResult quasi_mc(const std::function<double(const std::vector<double>&)>& f,
                  const std::vector<double>& a,
                  const std::vector<double>& b,
                  unsigned long long N,
                  Halton& sequence1,
                  Halton& sequence2) {
    if (a.size() != b.size()) throw std::runtime_error("quasi_mc: dimension mismatch");

    double volume = 1.0;
    for (std::size_t i = 0; i < a.size(); ++i) volume *= (b[i] - a[i]);

    const double mean1 = qmc_average(f, a, b, N, sequence1);
    const double mean2 = qmc_average(f, a, b, N, sequence2);

    const double estimate1 = volume * mean1;
    const double estimate2 = volume * mean2;
    const double estimate = 0.5 * (estimate1 + estimate2);

    // A practical QMC error estimate: compare two low-discrepancy sequences.
    const double error_estimate = std::abs(estimate1 - estimate2);

    return {estimate, error_estimate};
}
