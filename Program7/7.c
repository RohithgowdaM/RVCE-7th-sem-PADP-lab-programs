#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 512 // Define matrix size

void initialize_matrix(float *matrix, int size) {
    for (int i = 0; i < size; i++) {
        matrix[i] = (float)(rand() % 100) / 10.0; // Random values between 0 and 10
    }
}

void print_matrix(float *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6.2f ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    // Allocate memory for matrices dynamically
    float *A = (float *)malloc(N * N * sizeof(float));
    float *B = (float *)malloc(N * N * sizeof(float));
    float *C = (float *)malloc(N * N * sizeof(float));
    
    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    
    // Initialize matrices
    initialize_matrix(A, N * N);
    initialize_matrix(B, N * N);
    
    // Matrix multiplication using OpenACC
    #pragma acc data copyin(A[0:N*N], B[0:N*N]) copyout(C[0:N*N])
    {
        #pragma acc parallel loop collapse(2)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                float sum = 0.0f;
                for (int k = 0; k < N; k++) {
                    sum += A[i * N + k] * B[k * N + j];
                }
                C[i * N + j] = sum;
            }
        }
    }
    
    // Print result (for small N)
    if (N <= 8) {
        printf("Matrix A:\n");
        print_matrix(A, N, N);
        printf("Matrix B:\n");
        print_matrix(B, N, N);
        printf("Resultant Matrix C:\n");
        print_matrix(C, N, N);
    } else {
        printf("Matrix multiplication completed successfully!\n");
    }
    
    // Free dynamically allocated memory
    free(A);
    free(B);
    free(C);
    
    return 0;
}