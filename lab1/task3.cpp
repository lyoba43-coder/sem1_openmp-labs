#include <omp.h>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int num, method;
    cout << "Enter number of threads: ";
    cin >> num;
    cout << "Enter method (1-5): ";
    cin >> method;

    if (num < 1) {
        cerr << "num must be >= 1\n";
        return 1;
    }

    omp_set_num_threads(num);

    switch (method) {

        // ---- Способ 1: ordered ----
    case 1: {
        cout << "[Method 1: ordered]\n";
#pragma omp parallel
        {
#pragma omp for ordered
            for (int i = num - 1; i >= 0; --i) {
#pragma omp ordered
                cout << "Thread " << omp_get_thread_num()
                    << " (iter " << i << ")\n";
            }
        }
        break;
    }

          // ---- Способ 2: critical + обратный счётчик ----
    case 2: {
        cout << "[Method 2: critical + reverse counter]\n";
        int next = num - 1;
#pragma omp parallel
        {
            int tid = omp_get_thread_num();
            while (true) {
                bool my_turn = false;
#pragma omp critical
                { my_turn = (next == tid); }
                if (my_turn) break;
            }
#pragma omp critical
            {
                cout << "Thread " << tid << "\n";
                --next;
            }
        }
        break;
    }

          // ---- Способ 3: barrier + шаг ----
    case 3: {
        cout << "[Method 3: barrier + step]\n";
#pragma omp parallel
        {
            for (int s = 0; s < num; ++s) {
#pragma omp barrier
                if (omp_get_thread_num() == num - 1 - s)
                    cout << "Thread " << omp_get_thread_num() << "\n";
            }
        }
        break;
    }

          // ---- Способ 4: master + массив флагов ----
    case 4: {
        cout << "[Method 4: master + flags array]\n";
        vector<int> done(num, 0);
#pragma omp parallel
        {
            int tid = omp_get_thread_num();
#pragma omp critical
            { done[tid] = 1; }
#pragma omp barrier
#pragma omp master
            {
                for (int id = num - 1; id >= 0; --id) {
                    while (!done[id]) { /* spin */ }
                    cout << "Thread " << id << "\n";
                }
            }
        }
        break;
    }

          // ---- Способ 5: single + обратный цикл ----
    case 5: {
        cout << "[Method 5: single + reverse loop]\n";
#pragma omp parallel
        {
#pragma omp single
            {
                for (int id = num - 1; id >= 0; --id)
                    cout << "Thread " << id << "\n";
            }
        }
        break;
    }

    default:
        cerr << "method must be 1..5\n";
        return 1;
    }
    return 0;
}
