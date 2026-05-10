#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

#define SIZE 10000

// ============================
// Sequential Bubble Sort
// ============================
void bubbleSortSeq(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ============================
// Parallel Bubble Sort
// ============================
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ============================
// Generate Random Numbers
// ============================
void generateRandom(vector<int>& arr) {
    for (int &x : arr) {
        x = rand() % 100000;
    }
}

// ============================
// Main Function
// ============================
int main() {

    vector<int> arr(SIZE), temp;

    srand(time(0));
    generateRandom(arr);

    double start, end;

    // Sequential Bubble Sort
    temp = arr;

    start = omp_get_wtime();
    bubbleSortSeq(temp);
    end = omp_get_wtime();

    cout << "Sequential Bubble Sort Time: "
         << (end - start) << " sec\n";

    // Parallel Bubble Sort
    temp = arr;

    start = omp_get_wtime();
    bubbleSortParallel(temp);
    end = omp_get_wtime();

    cout << "Parallel Bubble Sort Time: "
         << (end - start) << " sec\n";

    return 0;
}