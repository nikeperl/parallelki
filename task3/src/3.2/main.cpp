#include "server.hpp"
#include "tasks.hpp"
#include "client.hpp"
#include <thread>

int main() {
    Server<double> server;
    server.start();

    const int N = 10;
    std::thread t1(client_sin, std::ref(server), N, "results_sin.txt");
    std::thread t2(client_sqrt, std::ref(server), N, "results_sqrt.txt");
    std::thread t3(client_pow, std::ref(server), N, "results_pow.txt");

    t1.join();
    t2.join();
    t3.join();

    server.stop();
}