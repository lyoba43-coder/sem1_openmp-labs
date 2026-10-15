#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

// Точка в 3D
struct Point { double x, y, z; };

// Генерация файла точек
void generate_points(const string& filename, int N) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Cannot create " << filename << "\n";
        exit(1);
    }
    for (int i = 0; i < N; ++i)
        fout << i << " " << (i * 2) << " " << (i * 3) << "\n";
    cout << "Generated " << N << " points to " << filename << "\n";
}

// Чтение точек из файла
bool read_points(const string& filename, vector<Point>& pts) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Cannot open file: " << filename << "\n";
        return false;
    }
    Point p;
    while (fin >> p.x >> p.y >> p.z)
        pts.push_back(p);
    return true;
}

// Функциональная декомпозиция + глобальная сумма
double compute_formula(const vector<Point>& pts, int num_threads) {
    const int N = (int)pts.size();
    omp_set_num_threads(num_threads);

    double total = 0.0;   // глобальная сумма (общая для всех потоков)

#pragma omp parallel sections
    {
#pragma omp section
        {
            double sx = 0.0;
            for (int i = 0; i < N; ++i) sx += pts[i].x;

#pragma omp critical
            {
                total += sx;
            }
        }

#pragma omp section
        {
            double sy = 0.0;
            for (int i = 0; i < N; ++i) sy += pts[i].y;

#pragma omp critical
            {
                total += sy;
            }
        }

#pragma omp section
        {
            double sz = 0.0;
            for (int i = 0; i < N; ++i) sz += pts[i].z;

#pragma omp critical
            {
                total += sz;
            }
        }
    }

    // Финальная формула
    return total / (3.0 * N);
}

int main() {
    int num;
    cout << "Enter number of threads: ";
    cin >> num;
    if (num < 1) {
        cerr << "num must be >= 1\n";
        return 1;
    }

    const string filename = "points.txt";
    const int    total = 100000;

    generate_points(filename, total);

    // Прочитать точки
    vector<Point> pts;
    if (!read_points(filename, pts)) {
        cerr << "Failed to read points.\n";
        return 1;
    }
    const int N = (int)pts.size();
    cout << "Loaded " << N << " points\n";

    // Посчитать формулу
    double t0 = omp_get_wtime();
    double result = compute_formula(pts, num);
    double t1 = omp_get_wtime();

    cout << "\n(Sum x + Sum y + Sum z) / 3N = " << result << "\n";
    cout << "Time: " << (t1 - t0) << " s\n";

    return 0;
}