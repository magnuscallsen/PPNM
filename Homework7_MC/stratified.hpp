#ifndef STRATIFIED_HPP
#define STRATIFIED_HPP

#include "mc.hpp"
#include <functional>
#include <vector>

MCResult stratified_mc(
    const std::function<double(const std::vector<double>&)>& f,
    const std::vector<double>& a,
    const std::vector<double>& b,
    unsigned long long N,
    LCG& rng,
    unsigned long long nmin = 64
);

#endif

// above code is written by AI
