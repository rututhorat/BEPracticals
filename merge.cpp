#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp(r - l + 1);
    int i = l, j = m + 1, k = 0;

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }

    while (i <= m) temp[k++] = arr[i++];
    while (j <= r) temp[k++] = arr[j++];

    for (int i = l; i <= r; i++)
        arr[i] = temp[i - l];
}

// Sequential Merge Sort
void mergeSortSeq(vector<int>& arr, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;
    mergeSortSeq(arr, l, m);
    mergeSortSeq(arr, m+1, r);
    merge(arr, l, m, r);
}

// Parallel Merge Sort
void mergeSortPar(vector<int>& arr, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        mergeSortPar(arr, l, m);

        #pragma omp section
        mergeSortPar(arr, m+1, r);
    }

    merge(arr, l, m, r);
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
    mergeSortSeq(arr, 0, n-1);
    double end = omp_get_wtime();

    cout << "\nSequential Merge Sort Time: " << (end - start) << " sec";

    // Parallel
    start = omp_get_wtime();
    mergeSortPar(arr2, 0, n-1);
    end = omp_get_wtime();

    cout << "\nParallel Merge Sort Time: " << (end - start) << " sec";

    cout << "\nSorted Array:\n";
    for (int x : arr2) cout << x << " ";

    return 0;
}

/*OUTPUT
PS C:\Users\rutuj\Desktop\Practicals> cd HPCpractical                  
PS C:\Users\rutuj\Desktop\Practicals\HPCpractical> g++ merge.cpp -fopenmp -o merge
PS C:\Users\rutuj\Desktop\Practicals\HPCpractical> .\merge.exe
Enter number of elements: 6
Enter elements:
3 2 9 5 1 6 

Sequential Merge Sort Time: 0 sec
Parallel Merge Sort Time: 0.00199986 sec
Sorted Array:
1 2 3 5 6 9 


*/