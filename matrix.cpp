// matrix_mul_leetgpu.cu

#include <stdio.h>

#define N 3   // matrix size (change if needed)

// CUDA Kernel
__global__ void matMul(float *A, float *B, float *C) {

    int row = threadIdx.y;
    int col = threadIdx.x;

    if (row < N && col < N) {

        float sum = 0;

        for (int k = 0; k < N; k++) {
            sum += A[row * N + k] * B[k * N + col];
        }

        C[row * N + col] = sum;
    }
}

int main() {

    float A[N*N] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    float B[N*N] = {
        9, 8, 7,
        6, 5, 4,
        3, 2, 1
    };

    float C[N*N];

    float *d_A, *d_B, *d_C;

    size_t size = N * N * sizeof(float);

    // Allocate GPU memory
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    // Copy data to GPU
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Launch kernel (1 block, NxN threads)
    dim3 threads(N, N);
    matMul<<<1, threads>>>(d_A, d_B, d_C);

    // Copy result back
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    // Print result
    printf("Result Matrix:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", C[i*N + j]);
        }
        printf("\n");
    }

    // Free memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}

/*OUTPUT


Result Matrix:
30.00 24.00 18.00
84.00 69.00 54.00
138.00 114.00 90.00
*/