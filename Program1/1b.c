#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SEED 3655942
#define ROOT 0

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double t;
    if (rank == ROOT) {
        t = MPI_Wtime();
    }

    int n = 10000; 
    int count = 0, red_count;

    srand(SEED + rank); 
    for (int i = 0; i < n / size; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        double z = x * x + y * y;
        if (z <= 1.0) count++;
    }

    
    MPI_Reduce(&count, &red_count, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

    if (rank == ROOT) {
        t = MPI_Wtime() - t;
        printf("Time: %f seconds\n", t);

        double pi = (double)red_count / n * 4;
        printf("No. of threads: %d\n", size);
        printf("No. of trials: %d\n", n);
        printf("Estimated value of pi: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
