#include <omp.h>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N;
    cout << "Enter size of array: ";
    cin >> N;
    if (N < 2) {
        cerr << "N must be >= 2\n";
        return 1;
    }

    // a[i] = i
    vector<double> a(N), b(N, 0.0);
    for (int i = 0; i < N; ++i) a[i] = static_cast<double>(i);

    int num;
    cout << "Enter number of threads: ";
    cin >> num;
    if (num < 1) {
        cerr << "num must be >= 1\n";
        return 1;
    }
    omp_set_num_threads(num);

    double t0, t1;

    // ---- 5 типов распределения работ ----

    cout << "\n--- schedule(static) ---\n";
    t0 = omp_get_wtime();
#pragma omp parallel for schedule(static)
    for (int i = 1; i < N - 1; ++i)
        b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
    t1 = omp_get_wtime();
    cout << "Time: " << (t1 - t0) << " s\n";

    cout << "\n--- schedule(dynamic) ---\n";
    t0 = omp_get_wtime();
#pragma omp parallel for schedule(dynamic)
    for (int i = 1; i < N - 1; ++i)
        b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
    t1 = omp_get_wtime();
    cout << "Time: " << (t1 - t0) << " s\n";

    cout << "\n--- schedule(guided) ---\n";
    t0 = omp_get_wtime();
#pragma omp parallel for schedule(guided)
    for (int i = 1; i < N - 1; ++i)
        b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
    t1 = omp_get_wtime();
    cout << "Time: " << (t1 - t0) << " s\n";

    cout << "\n--- schedule(static, 64) ---\n";
    t0 = omp_get_wtime();
#pragma omp parallel for schedule(static, 64)
    for (int i = 1; i < N - 1; ++i)
        b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
    t1 = omp_get_wtime();
    cout << "Time: " << (t1 - t0) << " s\n";

    cout << "\n--- schedule(dynamic, 64) ---\n";
    t0 = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 64)
    for (int i = 1; i < N - 1; ++i)
        b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
    t1 = omp_get_wtime();
    cout << "Time: " << (t1 - t0) << " s\n";

    // Контрольные значения
    cout << "\nb[1]=" << b[1]
        << "  b[100]=" << b[100]
        << "  b[N-2]=" << b[N - 2] << "\n";
    return 0;
}
