#include <iostream>
#include <vector>
#include <omp.h>
#include <limits.h>
using namespace std;

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    int sum = 0;
    int min_val = INT_MAX;
    int max_val = INT_MIN;

    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < n; i++) {
        sum += arr[i];

        if (arr[i] < min_val)
            min_val = arr[i];

        if (arr[i] > max_val)
            max_val = arr[i];
    }

    double end = omp_get_wtime();

    double avg = (double)sum / n;

    cout << "\nMinimum: " << min_val;
    cout << "\nMaximum: " << max_val;
    cout << "\nSum: " << sum;
    cout << "\nAverage: " << avg;

    cout << "\nExecution Time: " << (end - start) << " sec\n";

    return 0;
}

/*OUTPUT
PS C:\Users\rutuj\Desktop\Practicals\HPCpractical> g++ reduction.cpp -fopenmp -o reduction
PS C:\Users\rutuj\Desktop\Practicals\HPCpractical> .\reduction.exe
Enter number of elements: 5
Enter elements:
10 20 5 40 15

Minimum: 5
Maximum: 40
Sum: 90
Average: 18
Execution Time: 0.00399995 sec



*/