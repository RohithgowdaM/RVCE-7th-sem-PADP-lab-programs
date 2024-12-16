#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int strike(int* comp, int start, int step, int stop) {
    int i;
    for (i = start; i <= stop; i += step)
        comp[i] = 1;
    return i;
}

int unfriendlySieve(int n) {
    int m = (int)sqrt(n);
    int* comp = (int*)calloc(n + 1, sizeof(int));

    double t = omp_get_wtime();

    int count = 0;
    for (int i = 2; i <= m; i++) {
        if (!comp[i]) {
            count++;
            strike(comp, 2 * i, i, n);
        }
    }
    for (int i = m + 1; i <= n; i++) {
        if (!comp[i]) {
            count++;
        }
    }

    t = omp_get_wtime() - t;
    printf("Unfriendly Time: %f seconds\n", t);

    free(comp);
    return count;
}

int friendlySieve(int n) {
    int m = (int)sqrt(n);
    int* comp = (int*)calloc(n + 1, sizeof(int));

    int* striker = (int*)malloc(m * sizeof(int));
    int* factor = (int*)malloc(m * sizeof(int));
    int fac_cnt = 0;

    double t = omp_get_wtime();

    int count = 0;
    for (int i = 2; i <= m; i++) {
        if (!comp[i]) {
            count++;
            factor[fac_cnt] = i;
            striker[fac_cnt] = strike(comp, 2 * i, i, m);
            fac_cnt++;
        }
    }
    for (int left = m + 1; left <= n; left += m) {
        int right = (left + m - 1 < n) ? left + m - 1 : n;
        for (int k = 0; k < fac_cnt; k++) {
            striker[k] = strike(comp, striker[k], factor[k], right);
        }
        for (int i = left; i <= right; i++) {
            if (!comp[i]) {
                count++;
            }
        }
    }

    t = omp_get_wtime() - t;
    printf("Friendly Time: %f seconds\n", t);

    free(striker);
    free(factor);
    free(comp);
    return count;
}

int parallelySieve(int n) {
    int m = (int)sqrt(n);
    int* comp = (int*)calloc(n + 1, sizeof(int));

    int* striker = (int*)malloc(m * sizeof(int));
    int* factor = (int*)malloc(m * sizeof(int));
    int fac_cnt = 0;

    double t = omp_get_wtime();

    int count = 0;
    for (int i = 2; i <= m; i++) {
        if (!comp[i]) {
            count++;
            factor[fac_cnt] = i;
            striker[fac_cnt] = strike(comp, 2 * i, i, m);
            fac_cnt++;
        }
    }
    for (int left = m + 1; left <= n; left += m) {
        int right = (left + m - 1 < n) ? left + m - 1 : n;

        #pragma omp parallel for
        for (int k = 0; k < fac_cnt; k++) {
            striker[k] = strike(comp, striker[k], factor[k], right);
        }

        #pragma omp parallel for reduction(+:count)
        for (int i = left; i <= right; i++) {
            if (!comp[i]) {
                count++;
            }
        }
    }

    t = omp_get_wtime() - t;
    printf("Parallely Time: %f seconds\n", t);

    free(striker);
    free(factor);
    free(comp);
    return count;
}

int main() {
    int n = 100000000;

    printf("Unfriendly Count: %d\n", unfriendlySieve(n));
    printf("Friendly Count: %d\n", friendlySieve(n));
    printf("Parallely Count: %d\n", parallelySieve(n));

    return 0;
}
