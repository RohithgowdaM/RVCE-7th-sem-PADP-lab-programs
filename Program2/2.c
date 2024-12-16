#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <matrix_dimension> <num_threads>\n", argv[0]);
        return -1;
    }

    int DIM = atoi(argv[1]);        
    int thread_count = atoi(argv[2]); 

    if (DIM <= 0 || thread_count <= 0) {
        printf("Matrix dimension and number of threads must be positive integers.\n");
        return -1;
    }

    // Allocate memory for matrices
    int *A = (int *)malloc(DIM * DIM * sizeof(int));
    int *B = (int *)malloc(DIM * DIM * sizeof(int));
    int *C = (int *)malloc(DIM * DIM * sizeof(int));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    // Initialize matrices A and B, and set C to zero
    for (int i = 0; i < DIM * DIM; i++) {
        A[i] = rand() % 10;  // Random values between 0 and 9
        B[i] = rand() % 10;
        C[i] = 0;
    }

    double t = omp_get_wtime(); // Start time

    // Parallel matrix multiplication
    #pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            for (int k = 0; k < DIM; k++) {
                C[DIM * i + k] += A[DIM * i + j] * B[DIM * j + k];
            }
        }
    }

    t = omp_get_wtime() - t; // End time

    // Print execution time
    printf("Matrix Dimension: %d, Threads: %d, Time: %f seconds\n", DIM, thread_count, t);

    // Free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}
