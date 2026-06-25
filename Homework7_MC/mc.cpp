#include "mc.hpp"

#include <cmath>
#include <stdexcept>

LCG::LCG(unsigned long long seed) : state(seed) {}

double LCG::uniform() {
    // Quick-and-dirty 64-bit linear congruential generator.
    state = 6364136223846793005ULL * state + 1442695040888963407ULL;
    return (state >> 11) * (1.0 / 9007199254740992.0); // 53 random bits in [0,1)
}

MCResult plainmc(const std::function<double(const std::vector<double>&)>& f,
                 const std::vector<double>& a,
                 const std::vector<double>& b,
                 unsigned long long N,
                 LCG& rng) {
    if (a.size() != b.size()) throw std::runtime_error("plainmc: dimension mismatch");
    const std::size_t dim = a.size();

    double volume = 1.0;
    for (std::size_t i = 0; i < dim; ++i) volume *= (b[i] - a[i]);

    std::vector<double> x(dim);
    double sum = 0.0;
    double sum2 = 0.0;

    for (unsigned long long k = 0; k < N; ++k) {
        for (std::size_t i = 0; i < dim; ++i) x[i] = a[i] + rng.uniform() * (b[i] - a[i]);
        const double fx = f(x);
        sum += fx;
        sum2 += fx * fx;
    }

    const double mean = sum / static_cast<double>(N);
    const double mean2 = sum2 / static_cast<double>(N);
    const double variance = std::max(0.0, mean2 - mean * mean);

    return {volume * mean, volume * std::sqrt(variance / static_cast<double>(N))};
}

MCResult plainmc_std(const std::function<double(const std::vector<double>&)>& f,
                     const std::vector<double>& a,
                     const std::vector<double>& b,
                     unsigned long long N,
                     std::mt19937& rng) {
    if (a.size() != b.size()) throw std::runtime_error("plainmc_std: dimension mismatch");
    const std::size_t dim = a.size();
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    double volume = 1.0;
    for (std::size_t i = 0; i < dim; ++i) volume *= (b[i] - a[i]);

    std::vector<double> x(dim);
    double sum = 0.0;
    double sum2 = 0.0;

    for (unsigned long long k = 0; k < N; ++k) {
        for (std::size_t i = 0; i < dim; ++i) x[i] = a[i] + uniform(rng) * (b[i] - a[i]);
        const double fx = f(x);
        sum += fx;
        sum2 += fx * fx;
    }

    const double mean = sum / static_cast<double>(N);
    const double mean2 = sum2 / static_cast<double>(N);
    const double variance = std::max(0.0, mean2 - mean * mean);

    return {volume * mean, volume * std::sqrt(variance / static_cast<double>(N))};
}
