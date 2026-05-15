#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// Sequential Bubble Sort
void bubbleSortSeq(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }
    }
}

// Parallel Bubble Sort (Odd-Even)
void bubbleSortPar(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n-1; j += 2) {
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n-1; j += 2) {
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
        }
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n), arr2;

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    arr2 = arr;

    // Sequential
    double start = omp_get_wtime();
    bubbleSortSeq(arr);
    double end = omp_get_wtime();

    cout << "\nSequential Bubble Sort Time: " << (end - start) << " sec";

    // Parallel
    start = omp_get_wtime();
    bubbleSortPar(arr2);
    end = omp_get_wtime();

    cout << "\nParallel Bubble Sort Time: " << (end - start) << " sec";

    cout << "\nSorted Array:\n";
    for (int x : arr2) cout << x << " ";

    return 0;
}

/*OUTPUT
PS C:\Users\rutuj\Desktop\Practicals\HPCpractical> g++ bubble.cpp -fopenmp -o bubble
PS C:\Users\rutuj\Desktop\Practicals\HPCpractical> .\bubble.exe
Enter number of elements: 5
Enter elements:
5 3 4 1 2 

Sequential Bubble Sort Time: 0 sec
Parallel Bubble Sort Time: 0.00500011 sec
Sorted Array:
1 2 3 4 5 


*/