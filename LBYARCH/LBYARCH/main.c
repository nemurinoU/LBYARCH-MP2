#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 30 // TODO:  change according to specs

// Function prototype for NASM function
extern void asm_DAXPY(int n, double A, double* X, double* Y, double* Z);

// C function for DAXPY calculation
void c_DAXPY(int n, double A, double X[], double Y[], double Z[]) {
    for (int i = 0; i < n; i++) {
        Z[i] = A * X[i] + Y[i];
    }
}

// helper function for information
void displayMembers(int n, char c, double* arr) {
    printf("%c --> ", c);
    for (int i = 0; i < n - 1; ++i) printf("%.1f, ", arr[i]);
    printf("%.1f\n", arr[n - 1]);
}

int main(int argc, char* argv[]) {
    int n;
    double A;
    double *X, *Y, *Z;

    clock_t start, end;
    double cpu_time_used;

    scanf_s("%d", &n);
    scanf_s("%lf", &A);

    X = (double*)malloc(n * sizeof(double));
    Y = (double*)malloc(n * sizeof(double));    
    Z = (double*)malloc(n * sizeof(double));

    // get x and y
    for (int i = 0; i < n; ++i) scanf_s("%lf", &X[i]);
    for (int i = 0; i < n; ++i) scanf_s("%lf", &Y[i]);

    displayMembers(n, 'X', X);
    displayMembers(n, 'Y', Y);

    // Measure time for C function
    start = clock();
    c_DAXPY(n, A, X, Y, Z);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    

    // Print the results of the C function
    printf("\n=====[DAXPY USING C KERNEL]=====\n");
    printf("Time taken (C): %f seconds\n", cpu_time_used);
    
    displayMembers(n, 'Z', Z);

    // Clear the contents of Z
    for (int i = 0; i < n; ++i) Z[i] = 0;

    // Measure time for NASM function
    start = clock();
    asm_DAXPY(n, A, X, Y, Z);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
   
    
    // Print the results of the asm function
    printf("\n=====[DAXPY USING ASM KERNEL]=====\n");
    printf("Time taken (NASM): %f seconds\n", cpu_time_used);
    displayMembers(n, 'Z', Z);

    return 0;
}
