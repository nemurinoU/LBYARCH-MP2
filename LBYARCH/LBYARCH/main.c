#include <stdio.h>
#include <time.h>

#define MAX_SIZE 30 // TODO:  change according to specs

// Function prototype for NASM function
extern void asm_DAXPY(int n, double A, double X[], double Y[], double Z[]);

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

    // Vector length
    printf("Enter the length of the vector (up to %d): ", MAX_SIZE);
    scanf_s("%d", &n);

    // Input scalar variable A
    printf("Enter the scalar variable A: ");
    scanf_s("%lf", &A);

    // Vector X
    printf("Enter the elements of vector X:\n");
    for (int i = 0; i < n; i++) {
        printf("X[%d]: ", i);
        scanf_s("%lf", &X[i]);
    }

    // Vector Y
    printf("Enter the elements of vector Y:\n");
    for (int i = 0; i < n; i++) {
        printf("Y[%d]: ", i);
        scanf_s("%lf", &Y[i]);
    }

    // Measure time for C function
    start = clock();
    c_DAXPY(n, A, X, Y, Z);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken (C): %f seconds\n", cpu_time_used);

    // Print the results of the C function
    printf("\n=====[DAXPY USING C KERNEL]=====\n");
    printf("Z --> ");
    for (int i = 0; i < n - 1; ++i) printf("%.1f, ", Z[i]);
    printf("%.1f", Z[n - 1]);

    // Clear the contents of Z
    for (int i = 0; i < n; ++i) Z[i] = 0;

    // Measure time for NASM function
    start = clock();
    asm_DAXPY(n, A, X, Y, Z);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken (NASM): %f seconds\n", cpu_time_used);
    
    // Print the results of the asm function
    printf("\n=====[DAXPY USING ASM KERNEL]=====\n");
    printf("Z --> ");
    for (int i = 0; i < n - 1; ++i) printf("%.1f, ", Z[i]);
    printf("%.1f", Z[n - 1]);

    return 0;
}
