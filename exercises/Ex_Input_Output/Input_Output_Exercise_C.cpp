#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

int main(int argc, char* argv[]) {
    std::string inputFile, outputFile;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--input" && i + 1 < argc) {
            inputFile = argv[++i];
        }
        else if (arg == "--output" && i + 1 < argc) {
            outputFile = argv[++i];
        }
    }

    std::ifstream in(inputFile);
    std::ofstream out(outputFile);


    double x;
    while (in >> x) {
        out << x << ' ' << std::sin(x) << ' ' << std::cos(x) << '\n';
    }

    return 0;
}
