#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Function to generate normally distributed numbers using Box-Mueller transform
double box_mueller() {
    double u1, u2, w, mult;
    static double x1, x2;
    static int call = 0;

    if (call == 1) {
        call = !call;
        return x2;
    }

    do {
        u1 = -1 + ((double) rand() / RAND_MAX) * 2;
        u2 = -1 + ((double) rand() / RAND_MAX) * 2;
        w = pow(u1, 2) + pow(u2, 2);
    } while (w >= 1 || w == 0);

    mult = sqrt((-2 * log(w)) / w);
    x1 = u1 * mult;
    x2 = u2 * mult;

    call = !call;

    return x1;
}

// Function to perform matrix multiplication
void matrix_multiply(double **A, double **B, double **C, int m, int n, int p) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <rows_A> <cols_A_rows_B> <cols_B> <output_file>\n", argv[0]);
        return 1;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int p = atoi(argv[3]);
    char *output_file = argv[4];

    srand(time(NULL));  // Seed the random number generator

    // Allocate memory for matrices
    double **A = malloc(m * sizeof(double*));
    double **B = malloc(n * sizeof(double*));
    double **C = malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++) A[i] = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) B[i] = malloc(p * sizeof(double));
    for (int i = 0; i < m; i++) C[i] = malloc(p * sizeof(double));

    // Fill matrices A and B with random numbers
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = box_mueller();
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            B[i][j] = box_mueller();

    // Perform matrix multiplication 15 times and measure time
    clock_t start, end;
    double cpu_time_used;
    FILE *file = fopen(output_file, "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(file, "Matrix size: %dx%d * %dx%d = %dx%d\n", m, n, n, p, m, p);
    fprintf(file, "Run,Time (seconds)\n");

    for (int run = 0; run < 15; run++) {
        start = clock();
        matrix_multiply(A, B, C, m, n, p);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        fprintf(file, "%d,%f\n", run + 1, cpu_time_used);
    }

    fclose(file);

    // Free allocated memory
    for (int i = 0; i < m; i++) free(A[i]);
    for (int i = 0; i < n; i++) free(B[i]);
    for (int i = 0; i < m; i++) free(C[i]);
    free(A);
    free(B);
    free(C);

    return 0;
}