#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <functional>

struct data {
    int a;
    int b;
    double sum;
};

void harm(data& p) {
    p.sum = 0.0;

    for (int i = p.a; i < p.b; ++i) {
        p.sum += 1.0 / i;
    }
}

int main(int argc, char* argv[]) {
    int nthreads = 1;
    int nterms = (int)1e8;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-threads" && i + 1 < argc) {
            nthreads = std::stoi(argv[++i]);
        }

        if (arg == "-terms" && i + 1 < argc) {
            nterms = std::stoi(argv[++i]);
        }
    }

    std::vector<data> params(nthreads);

    for (int i = 0; i < nthreads; i++) {
        params[i].a = 1 + nterms / nthreads * i;
        params[i].b = 1 + nterms / nthreads * (i + 1);
        params[i].sum = 0.0;
    }

    params[params.size() - 1].b = nterms + 1;

    std::vector<std::thread> threads;

    for (int i = 0; i < nthreads; i++) {
        threads.emplace_back(harm, std::ref(params[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    double total = 0.0;

    for (auto& p : params) {
        total += p.sum;
    }

    std::cout << "Harmonic sum = " << total << std::endl;

    return 0;
}
