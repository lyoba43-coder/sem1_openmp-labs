#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

// описание 3d точки
struct Point { double x, y, z; };

// Генерация файла точек
void generate_points(const string& filename, int N) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Cannot create " << filename << "\n";
        exit(1);
    }
    for (int i = 0; i < N; ++i) {
        fout << i << " " << (i * 2) << " " << (i * 3) << "\n";
    }
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

// Версия 1: параллелизм по циклу
void center_loop(const vector<Point>& pts, int num_threads,
    double& cx, double& cy, double& cz) {
    const int N = (int)pts.size();
    omp_set_num_threads(num_threads);

    double sx = 0.0, sy = 0.0, sz = 0.0;

    // reduction — безопасное суммирование:
    // у каждого потока своя локальная копия sx/sy/sz,
    // в конце они складываются
#pragma omp parallel for reduction(+:sx, sy, sz) schedule(static)
    for (int i = 0; i < N; ++i) {
        sx += pts[i].x;
        sy += pts[i].y;
        sz += pts[i].z;
    }

    cx = sx / N;
    cy = sy / N;
    cz = sz / N;
}

// Версия 2: функциональная декомпозиция
void center_sections(const vector<Point>& pts, int num_threads,
    double& cx, double& cy, double& cz) {
    const int N = (int)pts.size();
    omp_set_num_threads(num_threads);

    double sx = 0.0, sy = 0.0, sz = 0.0;

    // Каждая секция — отдельный поток. Секций три, значит
    // реально работает максимум 3 потока.
#pragma omp parallel sections
    {
#pragma omp section
        {
            for (int i = 0; i < N; ++i) sx += pts[i].x;
        }
#pragma omp section
        {
            for (int i = 0; i < N; ++i) sy += pts[i].y;
        }
#pragma omp section
        {
            for (int i = 0; i < N; ++i) sz += pts[i].z;
        }
    }

    cx = sx / N;
    cy = sy / N;
    cz = sz / N;
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
    const int    total = 100000;   // сколько точек сгенерировать

    generate_points(filename, total);

    // Прочитать точки из файла
    vector<Point> pts;
    if (!read_points(filename, pts)) {
        cerr << "Failed to read points.\n";
        return 1;
    }
    const int N = (int)pts.size();
    cout << "Loaded " << N << " points\n";

    double cx, cy, cz;

    // Версия 1: parallel for
    double t0 = omp_get_wtime();
    center_loop(pts, num, cx, cy, cz);
    double t1 = omp_get_wtime();
    cout << "\n[Loop version (parallel for)]\n";
    cout << "Center = (" << cx << ", " << cy << ", " << cz << ")\n";
    cout << "Time: " << (t1 - t0) << " s\n";

    // Версия 2: sections
    double t2 = omp_get_wtime();
    center_sections(pts, num, cx, cy, cz);
    double t3 = omp_get_wtime();
    cout << "\n[Sections version (functional decomposition)]\n";
    cout << "Center = (" << cx << ", " << cy << ", " << cz << ")\n";
    cout << "Time: " << (t3 - t2) << " s\n";

    return 0;
}