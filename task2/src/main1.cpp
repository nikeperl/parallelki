#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

const int N1 = 20000; // Размер матрицы
const int N2 = 40000; // Другой размер матрицы
const int num_iterations = 20;

string filepath = "csv_files/";

double matrix_vector_mult(int size, int num_threads) {
    double sum_time = 0.0;
    for (int i = 0; i < num_iterations; i++) {
        auto start = omp_get_wtime();
        vector<vector<double>> matrix(size, vector<double>(size, 1.0));
        vector<double> vec(size, 1.0);
        vector<double> result(size, 0.0);

        #pragma omp parallel for num_threads(num_threads)
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result[i] += matrix[i][j] * vec[j];
            }
        }

        auto end = omp_get_wtime();
        sum_time += end - start;

    return sum_time / num_iterations;
}

void save_results(int size, vector<int> threads) {
    ofstream file(filepath, ios::app);

    double base_time = matrix_vector_mult(size, 1);
    cout << " Время последновательной программы для " << size << "x" << size << ": " << base_time << " сек" << endl;

    file << size << "x" << size;

    for (int num_threads : threads) {
        double avg_time = matrix_vector_mult(size, num_threads);
        double speedup = base_time / avg_time;

        cout << " Потоки: " << num_threads 
        << " Время: " << avg_time << " сек"
        << " Коэффициент ускорения: " << speedup << endl;

        file << "," << avg_time << "," << speedup;
    }
    file << "\n";
    file.close();
    cout << "Строка записана" << endl;
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

    save_results(N1, threads);
    save_results(N2, threads);
    
    return 0;
}
