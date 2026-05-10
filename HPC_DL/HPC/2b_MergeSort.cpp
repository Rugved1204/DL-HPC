#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

#define SIZE 100000

// ============================
// Merge Function
// ============================
void merge(vector<int>& arr, int l, int m, int r) {

    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;

    while (i < left.size() && j < right.size()) {

        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        }
        else {
            arr[k++] = right[j++];
        }
    }

    while (i < left.size()) {
        arr[k++] = left[i++];
    }

    while (j < right.size()) {
        arr[k++] = right[j++];
    }
}

// ============================
// Sequential Merge Sort
// ============================
void mergeSortSeq(vector<int>& arr, int l, int r) {

    if (l < r) {

        int m = (l + r) / 2;

        mergeSortSeq(arr, l, m);
        mergeSortSeq(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// ============================
// Parallel Merge Sort
// ============================
void mergeSortParallel(vector<int>& arr, int l, int r, int depth) {

    if (l < r) {

        int m = (l + r) / 2;

        if (depth <= 0) {

            mergeSortSeq(arr, l, m);
            mergeSortSeq(arr, m + 1, r);

        } else {

            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSortParallel(arr, l, m, depth - 1);

                #pragma omp section
                mergeSortParallel(arr, m + 1, r, depth - 1);
            }
        }

        merge(arr, l, m, r);
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
    double seqTime, parTime;

    // ============================
    // Sequential Merge Sort
    // ============================
    temp = arr;

    start = omp_get_wtime();

    mergeSortSeq(temp, 0, SIZE - 1);

    end = omp_get_wtime();

    seqTime = end - start;

    cout << "Sequential Merge Sort Time: "
         << seqTime << " sec\n";

    // ============================
    // Parallel Merge Sort
    // ============================
    temp = arr;

    start = omp_get_wtime();

    mergeSortParallel(temp, 0, SIZE - 1, 4);

    end = omp_get_wtime();

    parTime = end - start;

    cout << "Parallel Merge Sort Time: "
         << parTime << " sec\n";

    // ============================
    // Speedup
    // ============================
    cout << "Speedup: "
         << seqTime / parTime << endl;

    return 0;
}