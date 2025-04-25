#include <fstream>
#include <iostream>
#include <string>

void test_results(const std::string& filename, const std::string& prefix) {
    std::ifstream file(filename);
    std::string line;
    int line_count = 0;
    while (std::getline(file, line)) {
        if (line.find(prefix) != 0) {
            std::cerr << "Error in file " << filename << ": " << line << '\n';
        }
        ++line_count;
    }
    std::cout << filename << ": " << line_count << " lines OK\n";
}

int main() {
    test_results("results_sin.txt", "sin");
    test_results("results_sqrt.txt", "sqrt");
    test_results("results_pow.txt", "pow");
    return 0;
}
