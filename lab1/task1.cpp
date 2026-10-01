#include <omp.h>
#include <iostream>
using namespace std;

int main() {
    int num;
    cout << "Enter number of threads: ";
    cin >> num;

    if (num < 1) {
        cerr << "num must be >= 1\n";
        return 1;
    }

    omp_set_num_threads(num);

#pragma omp parallel
    {
        int tid = omp_get_thread_num();    // id текущего потока: 0..num-1
        int nth = omp_get_num_threads();   // сколько потоков сейчас

#pragma omp critical
        {
            cout << "Thread " << tid
                << " of " << nth
                << ": Hello World\n";
        }
    }
    return 0;
}