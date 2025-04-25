#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;
using Clock = chrono::high_resolution_clock;

using Matrix = vector<vector<double>>;
using Vector = vector<double>;

void init_matrix(Matrix& mat, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i)
        for (size_t j = 0; j < mat[i].size(); ++j)
            mat[i][j] = 1.0;  // простая инициализация
}

void init_vector(Vector& vec, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i)
        vec[i] = 1.0;
}

void multiply(const Matrix& mat, const Vector& vec, Vector& result, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        double sum = 0;
        for (size_t j = 0; j < vec.size(); ++j)
            sum += mat[i][j] * vec[j];
        result[i] = sum;
    }
}

double parallel_run(size_t size, size_t num_threads) {
    Matrix matrix(size, Vector(size));
    Vector vec(size);
    Vector result(size);

    vector<thread> threads;

    // Инициализация
    size_t chunk = size / num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk;
        size_t end = (i == num_threads - 1) ? size : start + chunk;
        threads.emplace_back(init_matrix, ref(matrix), start, end);
    }
    for (auto& t : threads) t.join();

    threads.clear();
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk;
        size_t end = (i == num_threads - 1) ? size : start + chunk;
        threads.emplace_back(init_vector, ref(vec), start, end);
    }
    for (auto& t : threads) t.join();

    // Умножение
    threads.clear();
    auto start_time = Clock::now();
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk;
        size_t end = (i == num_threads - 1) ? size : start + chunk;
        threads.emplace_back(multiply, cref(matrix), cref(vec), ref(result), start, end);
    }
    for (auto& t : threads) t.join();
    auto end_time = Clock::now();

    chrono::duration<double> elapsed = end_time - start_time;
    return elapsed.count();
}

void write_csv(const vector<size_t>& thread_counts,
               const vector<double>& base_times,
               const vector<vector<double>>& times,
               const string& filename) {
    ofstream file(filename);

    for (int i : thread_counts) {
        file << "," << "T" << i << "," << "S" << i;
    }
    file << "\n";

    for (size_t row = 0; row < base_times.size(); ++row) {
        file << (row == 0 ? "20000x20000" : "40000x40000");
        for (size_t i = 1; i < thread_counts.size(); ++i) {
            double Ti = times[row][i];
            double T1 = times[row][0];
            double Si = T1 / Ti;
            file << "," << fixed << setprecision(6) << Ti << "," << Si;
        }
        file << "\n";
    }

    file.close();
}

int main() {
    vector<size_t> thread_counts = {1, 2, 4, 7, 8, 16, 20, 40};
    vector<size_t> sizes = {20000, 40000};
    vector<vector<double>> all_times;

    for (size_t size : sizes) {
        vector<double> times;
        for (size_t threads : thread_counts) {
            cout << "Running with size " << size << " and threads " << threads << "...\n";
            double t = parallel_run(size, threads);
            times.push_back(t);
        }
        all_times.push_back(times);
    }

    write_csv(thread_counts, {all_times[0][0], all_times[1][0]}, all_times, "csv_files/results1.csv");

    cout << "Результаты записаны в results1.csv\n";
    return 0;
}
