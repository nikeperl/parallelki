#pragma once

#include <cmath>
#include <random>

template<typename T>
T fun_sin(T arg) {
    return std::sin(arg);
}

template<typename T>
T fun_sqrt(T arg) {
    return std::sqrt(arg);
}

template<typename T>
T fun_pow(T base, T exp) {
    return std::pow(base, exp);
}

double random_double(double min = 1.0, double max = 100.0) {
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}
