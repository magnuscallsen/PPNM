#ifndef QMC_HPP
#define QMC_HPP

#include "mc.hpp"

#include <functional>
#include <vector>

class Halton {
private:
    std::vector<int> bases;
    unsigned long long index;
    static double radical_inverse(unsigned long long n, int base);
public:
    explicit Halton(const std::vector<int>& bases_, unsigned long long start_index = 1);
    std::vector<double> next();
};

MCResult quasi_mc(const std::function<double(const std::vector<double>&)>& f,
                  const std::vector<double>& a,
                  const std::vector<double>& b,
                  unsigned long long N,
                  Halton& sequence1,
                  Halton& sequence2);

#endif
