#include "server.hpp"
#include "tasks.hpp"
#include <fstream>

void client_sin(Server<double>& server, int n, const std::string& filename) {
    std::vector<size_t> ids;
    for (int i = 0; i < n; ++i) {
        double arg = random_double();
        ids.push_back(server.add_task([arg]() { return fun_sin(arg); }));
    }

    std::ofstream out(filename);
    for (size_t id : ids) {
        double result = server.request_result(id);
        out << "sin -> " << result << '\n';
    }
}

void client_sqrt(Server<double>& server, int n, const std::string& filename) {
    std::vector<size_t> ids;
    for (int i = 0; i < n; ++i) {
        double arg = random_double();
        ids.push_back(server.add_task([arg]() { return fun_sqrt(arg); }));
    }

    std::ofstream out(filename);
    for (size_t id : ids) {
        double result = server.request_result(id);
        out << "sqrt -> " << result << '\n';
    }
}

void client_pow(Server<double>& server, int n, const std::string& filename) {
    std::vector<size_t> ids;
    for (int i = 0; i < n; ++i) {
        double base = random_double();
        double exp = random_double(0.5, 5.0);
        ids.push_back(server.add_task([base, exp]() { return fun_pow(base, exp); }));
    }

    std::ofstream out(filename);
    for (size_t id : ids) {
        double result = server.request_result(id);
        out << "pow -> " << result << '\n';
    }
}
