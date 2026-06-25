#ifndef MC_HPP
#define MC_HPP

#include <functional>
#include <random>
#include <vector>

struct MCResult {
    double estimate;
    double error_estimate;
};

class LCG {
private:
    unsigned long long state;
public:
    explicit LCG(unsigned long long seed = 1);
    double uniform();
};

MCResult plainmc(const std::function<double(const std::vector<double>&)>& f,
                 const std::vector<double>& a,
                 const std::vector<double>& b,
                 unsigned long long N,
                 LCG& rng);

MCResult plainmc_std(const std::function<double(const std::vector<double>&)>& f,
                     const std::vector<double>& a,
                     const std::vector<double>& b,
                     unsigned long long N,
                     std::mt19937& rng);

#endif
