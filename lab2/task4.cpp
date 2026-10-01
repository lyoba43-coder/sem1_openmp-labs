#include <omp.h>
#include <iostream>
#include <vector>
using namespace std;

// Последовательная версия
void mul_serial(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C,
    int N) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            double s = 0.0;
            for (int k = 0; k < N; ++k)
                s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
}

// Параллельная версия
void mul_parallel(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C,
    int N) {
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            double s = 0.0;
            for (int k = 0; k < N; ++k)
                s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
}

int main() {
    int N;
    cout << "Enter matrix size N: ";
    cin >> N;
    if (N < 1) {
        cerr << "N must be >= 1\n";
        return 1;
    }

    // Инициализация матриц
    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
    vector<vector<double>> C(N, vector<double>(N, 0.0));

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            A[i][j] = (i + j) % 10;
            B[i][j] = (i - j + 10) % 10;
        }

    // ---- Последовательная версия ----
    double t0 = omp_get_wtime();
    mul_serial(A, B, C, N);
    double t1 = omp_get_wtime();
    cout << "\nSerial: " << (t1 - t0) << " s\n";
    cout << "C[0][0]=" << C[0][0] << "  C[N-1][N-1]=" << C[N - 1][N - 1] << "\n";

    // ---- Параллельная версия на 1, 2, 4, 8 потоках ----
    int threads_list[] = { 1, 2, 4, 8 };
    for (int t : threads_list) {
        omp_set_num_threads(t);

        // Обнуляем C
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                C[i][j] = 0.0;

        double p0 = omp_get_wtime();
        mul_parallel(A, B, C, N);
        double p1 = omp_get_wtime();

        cout << "Parallel (" << t << " threads): " << (p1 - p0) << " s\n";
    }

    return 0;
}