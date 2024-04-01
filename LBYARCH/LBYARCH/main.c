#include <stdio.h>
#include <time.h>

#define MAX_SIZE 100 // TODO:  change according to specs

// Function prototype for NASM function
extern void nasm_DAXPY(int n, double A, double X[], double Y[], double Z[]);

// C function for DAXPY calculation
void c_DAXPY(int n, double A, double X[], double Y[], double Z[]) {
    for (int i = 0; i < n; i++) {
        Z[i] = A * X[i] + Y[i];
    }
}

int main() {
    int n;
    double A;
    double X[MAX_SIZE], Y[MAX_SIZE], Z[MAX_SIZE];
    clock_t start, end;
    double cpu_time_used;

    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read input from file
    fscanf(file, "%d", &n);
    fscanf(file, "%lf", &A);
    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf", &X[i]);
    }
    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf", &Y[i]);
    }

    fclose(file);

    // Measure time for C function
    start = clock();
    c_DAXPY(n, A, X, Y, Z);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken (C): %f seconds\n", cpu_time_used);

    // Measure time for NASM function
    start = clock();
    nasm_DAXPY(n, A, X, Y, Z);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken (NASM): %f seconds\n", cpu_time_used);

    return 0;
}
