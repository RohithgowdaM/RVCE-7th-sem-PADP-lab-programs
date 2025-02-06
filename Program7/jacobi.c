#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc.h>

#define N 1000        // Grid size in x-direction
#define M 1000        // Grid size in y-direction
#define MAX_ITER 1000 // Maximum number of iterations
#define TOL 1e-6      // Convergence tolerance

void initialize(double u[N][M], double f[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            u[i][j] = 0.0; // Initial guess
            f[i][j] = 1.0; // Right-hand side (can be modified as needed)
        }
    }
}

double jacobi(double u[N][M], double f[N][M]) {
    double u_new[N][M];
    int iter = 0;
    double max_diff;

    #pragma acc data copyin(f[0:N][0:M]) copy(u[0:N][0:M]) create(u_new[0:N][0:M])
    {
        do {
            max_diff = 0.0;

            #pragma acc parallel loop reduction(max:max_diff) collapse(2)
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < M - 1; j++) {
                    u_new[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j] + u[i][j - 1] + u[i][j + 1] - f[i][j]);
                    double diff = fabs(u_new[i][j] - u[i][j]);
                    if (diff > max_diff) {
                        max_diff = diff;
                    }
                }
            }

            #pragma acc parallel loop collapse(2)
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < M - 1; j++) {
                    u[i][j] = u_new[i][j];
                }
            }

            iter++;
        } while (max_diff > TOL && iter < MAX_ITER);
    }

    return max_diff;
}

int main() {
    double u[N][M], f[N][M];

    initialize(u, f);

    double t_start = acc_get_wtime();
    double final_diff = jacobi(u, f);
    double t_end = acc_get_wtime();

    printf("Final maximum difference: %e\n", final_diff);
    printf("Elapsed time: %f seconds\n", t_end - t_start);

    return 0;
}
