#include <stdio.h>
#include <cuda_runtime.h>

__global__ void vectorAdd(int *A, int *B, int *C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int n = 5;
    int size = n * sizeof(int);

    int A[] = {1,2,3,4,5};
    int B[] = {5,4,3,2,1};
    int C[5];

    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    vectorAdd<<<blocks, threads>>>(d_A, d_B, d_C, n);

    // 🔴 IMPORTANT: wait for GPU to finish
    cudaDeviceSynchronize();

    // 🔴 Check for kernel errors
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA Error: %s\n", cudaGetErrorString(err));
    }

    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    printf("Result:\n");
    for(int i=0;i<n;i++)
        printf("%d ", C[i]);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}