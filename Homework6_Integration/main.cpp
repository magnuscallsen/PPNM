// The exercise is made by AI

#include "integration.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct TestCase {
    std::string name;
    std::function<double(double)> f;
    double a, b, exact;
};

void print_result(const std::string& method, const Result& r, double exact) {
    std::cout << "  " << std::left << std::setw(20) << method
              << " value = " << std::right << std::setw(18) << std::setprecision(12) << r.value
              << "  exact = " << std::setw(18) << exact
              << "  actual error = " << std::setw(12) << std::abs(r.value-exact)
              << "  estimated error = " << std::setw(12) << r.error
              << "  calls = " << std::setw(7) << r.calls
              << "  depth = " << r.depth << "\n";
}

int main() {
    std::cout << std::setprecision(12);
    const double acc = 1e-6;
    const double eps = 1e-6;

    std::cout << "Homework: Recursive Adaptive Integration\n";
    std::cout << "============================================================\n";
    std::cout << "Accuracy goals used in most tests: acc = " << acc << ", eps = " << eps << "\n\n";

    std::vector<TestCase> tests = {
        {"integral_0^1 sqrt(x) dx", [](double x){ return std::sqrt(x); }, 0.0, 1.0, 2.0/3.0},
        {"integral_0^1 1/sqrt(x) dx", [](double x){ return 1.0/std::sqrt(x); }, 0.0, 1.0, 2.0},
        {"integral_0^1 sqrt(1-x^2) dx", [](double x){ return std::sqrt(1.0-x*x); }, 0.0, 1.0, M_PI/4.0},
        {"integral_0^1 log(x)/sqrt(x) dx", [](double x){ return std::log(x)/std::sqrt(x); }, 0.0, 1.0, -4.0}
    };

    std::cout << "1. Ordinary open 4-point adaptive integrator\n";
    std::cout << "------------------------------------------------------------\n";
    for(const auto& t : tests) {
        std::cout << t.name << "\n";
        Result r = integrate(t.f, t.a, t.b, acc, eps);
        print_result("ordinary", r, t.exact);
        std::cout << "\n";
    }

    std::cout << "2. Clenshaw-Curtis transformed integrator versus ordinary integrator\n";
    std::cout << "------------------------------------------------------------\n";
    std::ofstream cc_calls("taskB_cc_calls.txt");
    cc_calls << "# id name ordinary_calls cc_calls ordinary_actual_error cc_actual_error\n";
    int cc_id = 0;
    for(const auto& t : tests) {
        ++cc_id;
        std::cout << t.name << "\n";
        Result ordinary = integrate(t.f, t.a, t.b, acc, eps);
        Result cc = integrate_clenshaw_curtis(t.f, t.a, t.b, acc, eps);
        print_result("ordinary", ordinary, t.exact);
        print_result("Clenshaw-Curtis", cc, t.exact);
        std::cout << "  call ratio ordinary/CC = " << static_cast<double>(ordinary.calls)/cc.calls << "\n\n";
        cc_calls << cc_id << " ";
        cc_calls << "\"" << t.name << "\" ";
        cc_calls << ordinary.calls << " " << cc.calls << " ";
        cc_calls << std::abs(ordinary.value-t.exact) << " " << std::abs(cc.value-t.exact) << "\n";
    }
    cc_calls.close();

    std::cout << "3. Error function erf(z) from integral representation\n";
    std::cout << "------------------------------------------------------------\n";
    const double erf1_exact = 0.84270079294971486934;
    std::vector<double> zvalues = {-2.0, -1.0, 0.0, 0.5, 1.0, 1.5, 2.0, 3.0};
    for(double z : zvalues) {
        Result info;
        double val = my_erf(z, 1e-8, 1e-8, &info);
        std::cout << "  z = " << std::setw(6) << z
                  << "  my_erf(z) = " << std::setw(18) << val
                  << "  std::erf(z) = " << std::setw(18) << std::erf(z)
                  << "  difference = " << std::setw(12) << std::abs(val-std::erf(z))
                  << "  calls = " << info.calls << "\n";
    }
    Result erf1_info;
    double erf1 = my_erf(1.0, 1e-8, 0.0, &erf1_info);
    std::cout << "\n  erf(1) with acc=1e-8, eps=0\n";
    std::cout << "  calculated = " << erf1 << "\n";
    std::cout << "  tabulated  = " << erf1_exact << "\n";
    std::cout << "  difference = " << std::abs(erf1-erf1_exact) << "\n\n";

    std::ofstream erf_data("erf_data.txt");
    erf_data << "# z my_erf std_erf abs_difference calls estimated_error\n";
    for(int i=-60; i<=60; ++i) {
        double z = i/20.0; // -3 to 3
        Result info;
        double val = my_erf(z, 1e-7, 1e-7, &info);
        erf_data << z << " " << val << " " << std::erf(z) << " "
                 << std::abs(val-std::erf(z)) << " " << info.calls << " " << info.error << "\n";
    }
    erf_data.close();

    std::ofstream conv("erf1_convergence.txt");
    conv << "# acc abs_error calculated_erf1 estimated_error calls\n";
    std::cout << "4. erf(1) convergence, eps = 0\n";
    std::cout << "------------------------------------------------------------\n";
    for(int k=1; k<=10; ++k) {
        double a_goal = std::pow(10.0, -k);
        Result info;
        double val = my_erf(1.0, a_goal, 0.0, &info);
        double abs_error = std::abs(val-erf1_exact);
        conv << a_goal << " " << abs_error << " " << val << " " << info.error << " " << info.calls << "\n";
        std::cout << "  acc = " << std::setw(12) << a_goal
                  << "  erf(1) = " << std::setw(18) << val
                  << "  |error| = " << std::setw(12) << abs_error
                  << "  estimated error = " << std::setw(12) << info.error
                  << "  calls = " << info.calls << "\n";
    }
    conv.close();
    std::cout << "\n  Wrote erf_data.txt and erf1_convergence.txt for gnuplot.\n\n";

    std::cout << "5. Infinite-limit tests using finite transformations + Clenshaw-Curtis\n";
    std::cout << "------------------------------------------------------------\n";
    struct InfiniteTest {
        std::string name;
        std::function<double(double)> f;
        double a, b, exact;
    };
    std::vector<InfiniteTest> inf_tests = {
        {"integral_0^inf exp(-x) dx", [](double x){ return std::exp(-x); }, 0.0, std::numeric_limits<double>::infinity(), 1.0},
        {"integral_-inf^inf exp(-x^2) dx", [](double x){ return std::exp(-x*x); }, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::sqrt(M_PI)},
        {"integral_-inf^inf 1/(1+x^2) dx", [](double x){ return 1.0/(1.0+x*x); }, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), M_PI}
    };
    std::ofstream inf_data("taskB_infinite_calls.txt");
    inf_data << "# id name calls actual_error estimated_error\n";
    int inf_id = 0;
    for(const auto& t : inf_tests) {
        ++inf_id;
        std::cout << t.name << "\n";
        Result r = integrate_general(t.f, t.a, t.b, acc, eps);
        print_result("general infinite", r, t.exact);
        std::cout << "\n";
        inf_data << inf_id << " ";
        inf_data << "\"" << t.name << "\" ";
        inf_data << r.calls << " " << std::abs(r.value-t.exact) << " " << r.error << "\n";
    }
    inf_data.close();

    std::ofstream cpp_scipy("taskB_cpp_for_scipy_comparison.txt");
    cpp_scipy << "# id name cpp_calls cpp_value cpp_actual_error\n";
    {
        Result r1 = integrate_clenshaw_curtis([](double x){ return 1.0/std::sqrt(x); }, 0.0, 1.0, acc, eps);
        cpp_scipy << 1 << " \"1/sqrt(x)\" " << r1.calls << " " << r1.value << " " << std::abs(r1.value-2.0) << "\n";
        Result r2 = integrate_clenshaw_curtis([](double x){ return std::log(x)/std::sqrt(x); }, 0.0, 1.0, acc, eps);
        cpp_scipy << 2 << " \"log(x)/sqrt(x)\" " << r2.calls << " " << r2.value << " " << std::abs(r2.value+4.0) << "\n";
        Result r3 = integrate_general([](double x){ return std::exp(-x); }, 0.0, std::numeric_limits<double>::infinity(), acc, eps);
        cpp_scipy << 3 << " \"exp(-x), 0..inf\" " << r3.calls << " " << r3.value << " " << std::abs(r3.value-1.0) << "\n";
        Result r4 = integrate_general([](double x){ return std::exp(-x*x); }, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), acc, eps);
        cpp_scipy << 4 << " \"exp(-x*x), -inf..inf\" " << r4.calls << " " << r4.value << " " << std::abs(r4.value-std::sqrt(M_PI)) << "\n";
    }
    cpp_scipy.close();

    std::cout << "6. Quality of the returned error estimate\n";
    std::cout << "------------------------------------------------------------\n";
    std::ofstream errq("taskC_error_quality.txt");
    errq << "# id name actual_error estimated_error estimated_over_actual\n";
    int err_id = 0;
    for(const auto& t : tests) {
        ++err_id;
        Result r = integrate_clenshaw_curtis(t.f, t.a, t.b, acc, eps);
        double actual = std::abs(r.value-t.exact);
        double ratio = actual > 0 ? r.error/actual : std::numeric_limits<double>::infinity();
        std::cout << "  " << std::left << std::setw(34) << t.name
                  << std::right << " actual error = " << std::setw(12) << actual
                  << "  estimated error = " << std::setw(12) << r.error
                  << "  estimated/actual = ";
        if(actual > 0) std::cout << ratio;
        else std::cout << "inf";
        std::cout << "\n";
        errq << err_id << " ";
        errq << "\"" << t.name << "\" ";
        errq << actual << " " << r.error << " " << ratio << "\n";
    }
    errq.close();
    std::cout << "The code in the exercise has written by AI\n";
    std::cout << "\nData files created:\n";
    std::cout << "  erf_data.txt\n";
    std::cout << "  erf1_convergence.txt\n";
    std::cout << "  taskB_cc_calls.txt\n";
    std::cout << "  taskB_infinite_calls.txt\n";
    std::cout << "  taskB_cpp_for_scipy_comparison.txt\n";
    std::cout << "  taskC_error_quality.txt\n";
    std::cout << "Gnuplot scripts create:\n";
    std::cout << "  taskA_erf_plot.png\n";
    std::cout << "  taskA_erf1_convergence.png\n";
    std::cout << "  taskB_clenshaw_curtis_calls.png\n";
    std::cout << "  taskB_infinite_calls.png\n";
    std::cout << "  taskB_scipy_comparison.png\n";
    std::cout << "  taskC_error_quality.png\n";
    std::cout << "\nThe Makefile runs scipy_compare.py automatically if Python is available.\n";

    return 0;
}
