#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

const int nsteps = 40000000;
const int num_iterations = 20;
const double a = 0.0;
const double b = 1.0;

string filepath = "csv_files/";

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// Параллельная версия интегрирования с использованием #pragma omp atomic
double integrate_omp(int num_threads) {
    double sum_time = 0.0;
    for (int i = 0; i < num_iterations; i++) { // усредняем
        auto start = omp_get_wtime();

        double step = (b - a) / nsteps;
        double sum = 0.0;
        
        #pragma omp parallel num_threads(num_threads)
        {
            double local_sum = 0.0;
            #pragma omp for
            for (int i = 0; i < nsteps; i++) {
                double x = a + (i + 0.5) * step;
                //local_sum += f(x);
                #pragma omp atomic
                sum += f(x);
            }
        }
        double result = sum * step;

        auto end = omp_get_wtime();
        sum_time += end - start;
    }

    return sum_time / num_iterations;
}

void save_results(vector<int> threads) {
    ofstream file(filepath, ios::app);

    double base_time = integrate_omp(1);
    cout << " Время последновательной программы для " << nsteps << ": " << base_time << " сек" << endl;

    file << nsteps;
    
    for (int num_threads : threads) {
        double avg_time = integrate_omp(num_threads);
        double speedup = base_time / avg_time;

        cout << " Потоки: " << num_threads 
        << " Время: " << avg_time << " сек"
        << " Коэффициент ускорения: " << speedup << endl;

        file << "," << avg_time << "," << speedup;
    }
    file.close();
}

int main() {
    vector<int> threads = {1, 2, 4, 7, 8, 16, 20, 40};

    string filename;
    cout << "Введите куда сохранить: ";
    cin >> filename;

    filepath += filename + ".csv";

    ofstream file(filepath, ios::trunc);
    for (int i : threads) {
        file << "," << "T" << i << "," << "S" << i;
    }
    file << "\n";
    file.close();
    
    save_results(threads);
    return 0;
}
