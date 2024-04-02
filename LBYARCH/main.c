#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dirent.h"

#define MAX_SIZE 30 // TODO:  change according to specs

// Function prototype for NASM function
extern void asm_DAXPY(int n, double A, double* X, double* Y, double* Z);

// C function for DAXPY calculation
void c_DAXPY(int n, double A, double X[], double Y[], double Z[]) {
    for (int i = 0; i < n; i++) {
        Z[i] = A * X[i] + Y[i];
    }
}

// helper function to get files
int dir() {
    DIR* dp;
    struct dirent* ep;
    dp = opendir("../__tests__");
    if (dp != NULL) {
        printf("=====[AVAILABLE TESTS]=====\n");
        while ((ep = readdir(dp)) != NULL) {
            if (strstr((ep->d_name), ".txt")) printf("%s\n", (ep->d_name));
        }

        (void)closedir(dp);

        return 0;
    }

    perror("Couldn't open the directory");
    return -1;
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
    char* fname;
    char* suffix;

    fname = (char*)malloc(50 * sizeof(char));
    suffix = (char*)malloc(50 * sizeof(char));

    clock_t start, end;
    double cpu_time_used;
    

    // ask for file input
    dir();
    printf("\nEnter filename: ");
    gets(fname);

    snprintf(suffix, 50 * sizeof(char), "../__tests__/%s", fname);
    FILE* file = fopen(suffix, "r");
    if (file == NULL) {
        perror("Could not open file");
        return -1;
    }

    fscanf_s(file, "%d", &n);
    fscanf_s(file, "%lf", &A);

    //scanf_s("%d", &n);
    //scanf_s("%lf", &A);

    X = (double*)malloc(n * sizeof(double));
    Y = (double*)malloc(n * sizeof(double));
    Z = (double*)malloc(n * sizeof(double));

    // get x and y
    for (int i = 0; i < n; ++i) fscanf_s(file, "%lf", &X[i]); //scanf_s("%lf", &X[i]);
    for (int i = 0; i < n; ++i) fscanf_s(file, "%lf", &Y[i]); //scanf_s("%lf", &Y[i]);

    fclose(file);
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
