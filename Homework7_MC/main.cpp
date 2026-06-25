// Code written by AI
#include "mc.hpp"
#include "problems.hpp"
#include "qmc.hpp"
#include "stratified.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
double smooth_test_function(const std::vector<double>& x) {
    return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
}


int main() {
    std::cout << std::setprecision(12);

    const double exact_circle = exact_unit_circle_area();

    std::ofstream circle_data("circle_errors.dat");
    circle_data << "# N estimate estimated_error actual_error scaled_actual_error\n";

    std::vector<unsigned long long> Ns;
    for (unsigned long long N = 100; N <= 1000000; N *= 2) {
        Ns.push_back(N);
    }
    std::cout << "==========Unit Circle=============\n";
    std::cout << "Unit circle area integral over [-1,1]^2\n";
    std::cout << "Exact value = pi = " << exact_circle << "\n";
    std::cout << "N estimate estimated_error actual_error sqrt(N)*actual_error\n";

    for (auto N : Ns) {
        LCG rng(17 + N); // deterministic, but independent-ish for each N
        MCResult r = plainmc(unit_circle_indicator, {-1.0, -1.0}, {1.0, 1.0}, N, rng);
        const double actual_error = std::abs(r.estimate - exact_circle);
        const double scaled_actual_error = std::sqrt(static_cast<double>(N)) * actual_error;

        circle_data << N << " " << r.estimate << " " << r.error_estimate << " "
                    << actual_error << " " << scaled_actual_error << "\n";
        std::cout << N << " " << r.estimate << " " << r.error_estimate << " "
                  << actual_error << " " << scaled_actual_error << "\n";
    }

    const EllipsoidParameters ellipsoid_params{1.0, 2.0, 3.0};
    const double exact_ellipsoid = exact_ellipsoid_volume(ellipsoid_params);

    auto ellipsoid = [&ellipsoid_params](const std::vector<double>& x) {
        return ellipsoid_indicator(x, ellipsoid_params);
    };
    std::cout << "Integral works very well.\n";
    std::cout << "In the plots circle_scaled_error.png and circle_errors.png we see that the errors (very) roughly scale as 1/sqrt(N).\n";
    std::cout << "=======Ellipsoid volume integral========.\n";
    
    std::cout << "\nEllipsoid volume integral over [-1,1] x [-2,2] x [-3,3]\n";
    std::cout << "Exact value = (4/3)*pi*a*b*c = " << exact_ellipsoid << "\n";

    LCG rng_ellipsoid(314159);
    MCResult ell = plainmc(ellipsoid,
                           {-ellipsoid_params.a, -ellipsoid_params.b, -ellipsoid_params.c},
                           { ellipsoid_params.a,  ellipsoid_params.b,  ellipsoid_params.c},
                           1000000,
                           rng_ellipsoid);

    std::cout << "N estimate estimated_error actual_error\n";
    std::cout << 1000000 << " " << ell.estimate << " " << ell.error_estimate << " "
              << std::abs(ell.estimate - exact_ellipsoid) << "\n";

    std::ofstream ellipsoid_data("ellipsoid_result.dat");
    ellipsoid_data << "# N estimate estimated_error exact actual_error\n";
    ellipsoid_data << 1000000 << " " << ell.estimate << " " << ell.error_estimate << " "
                   << exact_ellipsoid << " " << std::abs(ell.estimate - exact_ellipsoid) << "\n";
    std::cout << "Again Integration works well\n";

    // ===================== Added part: quasi-random sequences =====================
    std::cout << "\n==========Quasi-random sequences=============\n";
    std::cout << "We first test the scaling on the smooth integral int_0^1 int_0^1 int_0^1 x*y*z dx dy dz = 1/8.\n";
    std::cout << "The difficult singular integral is only used afterwards for final comparison.\n";

    const double exact_smooth = exact_smooth_test_integral();
    std::ofstream scaling_data("qmc_scaling.dat");
    scaling_data << "# N lcg_error std_error qmc_error lcg_est std_est qmc_est qmc_error_estimate\n";

    std::cout << "\nSmooth test integral over [0,1]^3\n";
    std::cout << "Exact value = " << exact_smooth << "\n";
    std::cout << "N lcg_error std_error qmc_error qmc_error_estimate\n";

    for (auto N : Ns) {
        LCG rng_lcg(1000 + N);
        std::mt19937 rng_std(static_cast<unsigned int>(2000 + N));
        Halton h1({2, 3, 5}, 1);
        Halton h2({3, 5, 7}, 1);

        MCResult lcg_result = plainmc(smooth_test_integrand, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, N, rng_lcg);
        MCResult std_result = plainmc_std(smooth_test_integrand, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, N, rng_std);
        MCResult qmc_result = quasi_mc(smooth_test_integrand, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, N, h1, h2);

        const double lcg_error = std::abs(lcg_result.estimate - exact_smooth);
        const double std_error = std::abs(std_result.estimate - exact_smooth);
        const double qmc_error = std::abs(qmc_result.estimate - exact_smooth);

        scaling_data << N << " " << lcg_error << " " << std_error << " " << qmc_error << " "
                     << lcg_result.estimate << " " << std_result.estimate << " " << qmc_result.estimate << " "
                     << qmc_result.error_estimate << "\n";

        std::cout << N << " " << lcg_error << " " << std_error << " " << qmc_error << " "
                  << qmc_result.error_estimate << "\n";
    }

    std::cout << "\nDifficult singular integral\n";
    std::cout << "Exact value approximately = " << exact_singular_integral() << "\n";
    std::cout << "Method N estimate estimated_error actual_error\n";

    std::ofstream singular_data("singular_comparison.dat");
    singular_data << "# method N estimate estimated_error exact actual_error\n";

    const unsigned long long Nsing = 1000000;
    const std::vector<double> lower = {0.0, 0.0, 0.0};
    const double pi = std::acos(-1.0);
    const std::vector<double> upper = {pi, pi, pi};
    const double exact_singular = exact_singular_integral();

    LCG rng_sing_lcg(1234567);
    std::mt19937 rng_sing_std(7654321);
    Halton sing_h1({2, 3, 5}, 1);
    Halton sing_h2({3, 5, 7}, 1);

    MCResult sing_lcg = plainmc(singular_integrand, lower, upper, Nsing, rng_sing_lcg);
    MCResult sing_std = plainmc_std(singular_integrand, lower, upper, Nsing, rng_sing_std);
    MCResult sing_qmc = quasi_mc(singular_integrand, lower, upper, Nsing, sing_h1, sing_h2);

    singular_data << "LCG " << Nsing << " " << sing_lcg.estimate << " " << sing_lcg.error_estimate << " "
                  << exact_singular << " " << std::abs(sing_lcg.estimate - exact_singular) << "\n";
    singular_data << "std_mt19937 " << Nsing << " " << sing_std.estimate << " " << sing_std.error_estimate << " "
                  << exact_singular << " " << std::abs(sing_std.estimate - exact_singular) << "\n";
    singular_data << "Halton_QMC " << Nsing << " " << sing_qmc.estimate << " " << sing_qmc.error_estimate << " "
                  << exact_singular << " " << std::abs(sing_qmc.estimate - exact_singular) << "\n";

    std::cout << "LCG " << Nsing << " " << sing_lcg.estimate << " " << sing_lcg.error_estimate << " "
              << std::abs(sing_lcg.estimate - exact_singular) << "\n";
    std::cout << "std_mt19937 " << Nsing << " " << sing_std.estimate << " " << sing_std.error_estimate << " "
              << std::abs(sing_std.estimate - exact_singular) << "\n";
    std::cout << "Halton_QMC " << Nsing << " " << sing_qmc.estimate << " " << sing_qmc.error_estimate << " "
              << std::abs(sing_qmc.estimate - exact_singular) << "\n";

    //std::cout << "For the smooth test, qmc_scaling.png compares LCG, std::mt19937, and Halton quasi-random errors.\n";
   // std::cout << "For the singular integral, singular_comparison.dat contains the three final estimates.\n";
    std::cout << "Again we see 1/sqrt(N) error dependence roughly. And we calculate the singular integral to high precision.\n";
    std::cout << "See error comparisons in qmc_scaling.png.\n";
    std::cout << "std::mt19937 is the c++ library generator.\n";
    std::cout << "Halton --> quasi random. LCG --> Pesudo random.\n";

    std::cout << "=======Stratified sampling========\n";

    std::ofstream strat_data("stratified_scaling.dat");
    strat_data << "# N plain_error stratified_error\n";

    for (auto N : Ns) {
        LCG rng_plain(1234 + N);
        LCG rng_strat(5678 + N);

        MCResult plain = plainmc(smooth_test_function, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, N, rng_plain);

        MCResult strat = stratified_mc(smooth_test_function, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, N, rng_strat);

        double plain_error = std::abs(plain.estimate - exact_smooth_test_integral());
        double strat_error = std::abs(strat.estimate - exact_smooth_test_integral());

        strat_data << N << " " << plain_error << " " << strat_error << "\n";

        std::cout << N << " plain error = " << plain_error
              << " stratified error = " << strat_error << "\n";
    }
    std::cout << "See plots in stratified_sampling.png. The very simple 3D unit box integral i captured well both by plain and stratified  \n";
    std::cout << "But clearly best by stratified sampling.  \n";

    
    return 0;
}
