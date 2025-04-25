#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>

#ifndef TYPE
#define TYPE double
#endif

size_t N = 10e6;

int main() {
    std::vector<TYPE> arr(N);
    for (size_t i = 0; i < N; ++i) {
        arr[i] = std::sin(2 * M_PI * i / N);
    }

    TYPE sum = std::accumulate(arr.begin(), arr.end(), static_cast<TYPE>(0.0));
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}