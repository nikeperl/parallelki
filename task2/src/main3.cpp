#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <omp.h>

using namespace std;

const int N = 1000;  // Размер системы
const int MAX_ITER = 10000;
const double EPS = 1e-6;
const int num_iterations = 20;
const vector<int> threads = {1, 2, 4, 8, 16};

string filepath = "csv_files/";

void initialize_system(vector<vector<double>> &A, vector<double> &b) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (i == j) ? 2.0 : 1.0;
        }
        b[i] = N + 1;
    }
    string filename;
    cout << "Введите куда сохранить: ";
    cin >> filename;

    filepath += filename + ".csv";

    ofstream file(filepath, ios::trunc);
    for (int i : threads) {
        file << "," << "T" << i << "," << "S" << i;
    }
    file.close();
}

// Вариант 1: #pragma omp parallel for на каждом цикле
double jacobi_parallel1(const vector<vector<double>> &A, const vector<double> &b, int num_threads) {
    auto start = omp_get_wtime();

    vector<double> x_old(N, 0.0), x_new(N, 0.0);
    
    for (int iter = 0; iter < MAX_ITER; iter++) {
        #pragma omp parallel for schedule(static) num_threads(num_threads)
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                if (j != i) sum += A[i][j] * x_old[j];
            }
            x_new[i] = (b[i] - sum) / A[i][i];
        }
        
        double error = 0.0;
        #pragma omp parallel for reduction(+:error) schedule(static) num_threads(num_threads)
        for (int i = 0; i < N; i++) {
            error += fabs(x_new[i] - x_old[i]);
            x_old[i] = x_new[i];
        }
        
        #pragma omp barrier
        if (error < EPS) break;
    }

    auto end = omp_get_wtime();

    return end - start;
}

// Вариант 2: Одна параллельная секция #pragma omp parallel
double jacobi_parallel2(const vector<vector<double>> &A, const vector<double> &b, int num_threads) {
    auto start = omp_get_wtime();

    vector<double> x_old(N, 0.0), x_new(N, 0.0);
    double error;
    
    #pragma omp parallel num_threads(num_threads)
    {
        for (int iter = 0; iter < MAX_ITER; iter++) {
            #pragma omp for schedule(static)
            for (int i = 0; i < N; i++) {
                double sum = 0.0;
                for (int j = 0; j < N; j++) {
                    if (j != i) sum += A[i][j] * x_old[j];
                }
                x_new[i] = (b[i] - sum) / A[i][i];
            }
            
            error = 0.0;
            #pragma omp for reduction(+:error) schedule(static)
            for (int i = 0; i < N; i++) {
                error += fabs(x_new[i] - x_old[i]);
                x_old[i] = x_new[i];
            }
            
            if (error < EPS) break;
        }
    }

    auto end = omp_get_wtime();

    return end - start;
}

void save_results(const vector<vector<double>> &A, const vector<double> &b) {
    ofstream file(filepath, ios::app);

    double base_time = jacobi_parallel1(A, b, 1);
    cout << " Время последновательной программы для " << "#pragma omp parallel for" << ": " << base_time << " сек" << endl;

    file << "\n#pragma omp parallel for";
    
    for (int num_threads : threads) {
        double avg_time = jacobi_parallel1(A, b, num_threads);
        double speedup = base_time / avg_time;

        cout << " Потоки: " << num_threads 
        << " Время: " << avg_time << " сек"
        << " Коэффициент ускорения: " << speedup << endl;

        file << "," << avg_time << "," << speedup;
    }

    base_time = jacobi_parallel2(A, b, 1);
    cout << " Время последновательной программы для " << "#pragma omp parallel" << ": " << base_time << " сек" << endl;

    file << "\n#pragma omp parallel";
    
    for (int num_threads : threads) {
        double avg_time = jacobi_parallel2(A, b, num_threads);
        double speedup = base_time / avg_time;

        cout << " Потоки: " << num_threads 
        << " Время: " << avg_time << " сек"
        << " Коэффициент ускорения: " << speedup << endl;

        file << "," << avg_time << "," << speedup;
    }

    file.close();
}

int main() {
    vector<vector<double>> A(N, vector<double>(N));
    vector<double> b(N);
    initialize_system(A, b);
    
    save_results(A, b);

    return 0;
}
