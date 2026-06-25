#include "stratified.hpp"

#include <cmath>
#include <algorithm>



MCResult stratified_mc(
    const std::function<double(const std::vector<double>&)>& f,
    const std::vector<double>& a,
    const std::vector<double>& b,
    unsigned long long N,
    LCG& rng,
    unsigned long long nmin
) {
    const size_t dim = a.size();

    if (N <= nmin) {
        return plainmc(f, a, b, N, rng);
    }

    std::vector<double> left_sum(dim, 0.0), left_sum2(dim, 0.0);
    std::vector<double> right_sum(dim, 0.0), right_sum2(dim, 0.0);

    for (unsigned long long i = 0; i < nmin; ++i) {
        std::vector<double> x(dim);
        for (size_t k = 0; k < dim; ++k) {
            x[k] = a[k] + rng.uniform() * (b[k] - a[k]);
        }

        double fx = f(x);

        for (size_t k = 0; k < dim; ++k) {
            double mid = 0.5 * (a[k] + b[k]);
            if (x[k] < mid) {
                left_sum[k] += fx;
                left_sum2[k] += fx * fx;
            } else {
                right_sum[k] += fx;
                right_sum2[k] += fx * fx;
            }
        }
    }

    size_t split_dim = 0;
    double largest_subvariance = -1.0;

    for (size_t k = 0; k < dim; ++k) {
        double ml = left_sum[k] / nmin;
        double mr = right_sum[k] / nmin;

        double vl = left_sum2[k] / nmin - ml * ml;
        double vr = right_sum2[k] / nmin - mr * mr;

        double subvariance = vl + vr;

        if (subvariance > largest_subvariance) {
            largest_subvariance = subvariance;
            split_dim = k;
        }
    }

    std::vector<double> a_left = a, b_left = b;
    std::vector<double> a_right = a, b_right = b;

    double mid = 0.5 * (a[split_dim] + b[split_dim]);

    b_left[split_dim] = mid;
    a_right[split_dim] = mid;

    unsigned long long remaining = N - nmin;

    unsigned long long N_left = remaining / 2;
    unsigned long long N_right = remaining - N_left;

    MCResult left = stratified_mc(f, a_left, b_left, N_left, rng, nmin);
    MCResult right = stratified_mc(f, a_right, b_right, N_right, rng, nmin);

    MCResult result;
    result.estimate = left.estimate + right.estimate;
    result.error_estimate = std::sqrt(
        left.error_estimate * left.error_estimate +
        right.error_estimate * right.error_estimate
    );

    return result;
}

// code written by AI
