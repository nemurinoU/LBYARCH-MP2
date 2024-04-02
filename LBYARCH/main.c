#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dirent.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>

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
    int n, chk;
    double A;
    double *X, *Y, *ZC, *ZASM;
    char* fname;
    char* suffix;
    char* folder;

    fname = (char*)malloc(100 * sizeof(char));
    suffix = (char*)malloc(100 * sizeof(char));

    clock_t start, end;
    double cpu_time_used;
    

    // ask for file input
    dir();
    printf("\nEnter filename: ");
    gets(fname);

    snprintf(suffix, 100 * sizeof(char), "../__tests__/%s", fname);
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
    ZC = (double*)malloc(n * sizeof(double));
    ZASM = (double*)malloc(n * sizeof(double));

    // get x and y
    for (int i = 0; i < n; ++i) fscanf_s(file, "%lf", &X[i]); //scanf_s("%lf", &X[i]);
    for (int i = 0; i < n; ++i) fscanf_s(file, "%lf", &Y[i]); //scanf_s("%lf", &Y[i]);

    fclose(file);

    if (n <= 10) displayMembers(n, 'X', X);
    else {
        displayMembers(10, 'X', X);
        printf("... and %d more members...\n", n - 10);
    }

    if (n <= 10) displayMembers(n, 'Y', Y);
    else {
        displayMembers(10, 'Y', Y);
        printf("... and %d more members...\n", n - 10);
    }

    // Measure time for C function
    start = clock();
    c_DAXPY(n, A, X, Y, ZC);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    

    // Print the results of the C function
    printf("\n=====[DAXPY USING C KERNEL]=====\n");
    printf("Time taken (C): %f seconds\n", cpu_time_used);
    
    if (n <= 10) displayMembers(n, 'Z', ZC);
    else {
        displayMembers(10, 'Z', ZC);
        printf("... and %d more members...\n", n - 10);
    }

    snprintf(suffix, 100 * sizeof(char), "../__tests__/outputs/%s", fname);
    if (_mkdir(suffix, 0777) == -1) {
        perror("Could not make new directory");
    }
    
    snprintf(suffix, 100 * sizeof(char), "../__tests__/outputs/%s/C-OUT_%s", fname, fname);
    FILE* file2 = fopen(suffix, "w");
    if (file2 == NULL) {
        perror("Could not open-write file");
        return -1;
    }

    printf("\n[INFO] Output saved to /__tests__/outputs/%s/C-OUT_%s\n\n", fname, fname);
    // save to output bin
    for (int i = 0; i < n - 1; ++i) fprintf(file2, "%.1f ", ZC[i]);
    fprintf(file2, "%.1f\n", ZC[n - 1]);

    fclose(file2);
    

    // Clear the contents of Z
    //for (int i = 0; i < n; ++i) Z[i] = 0;

    // Measure time for NASM function
    start = clock();
    asm_DAXPY(n, A, X, Y, ZASM);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
   
    
    // Print the results of the asm function
    printf("\n=====[DAXPY USING ASM KERNEL]=====\n");
    printf("Time taken (NASM): %f seconds\n", cpu_time_used);
    if (n <= 10) displayMembers(n, 'Z', ZASM);
    else {
        displayMembers(10, 'Z', ZASM);
        printf("... and %d more members...\n", n - 10);
    }
    

    snprintf(suffix, 100 * sizeof(char), "../__tests__/outputs/%s/NASM-OUT_%s", fname, fname);
    FILE* file3 = fopen(suffix, "w");
    if (file3 == NULL) {
        perror("Could not open-write file");
        return -1;
    }

    printf("\n[INFO] Output saved to /__tests__/outputs/%s/NASM-OUT_%s\n\n", fname, fname);
    // save to output bin
    fprintf(file3, "Z --> ");
    for (int i = 0; i < n - 1; ++i) fprintf(file3, "%.1f ", ZASM[i]);
    fprintf(file3, "%.1f\n", ZASM[n - 1]);

    // Correctness check
    chk = 0;
    for (int i = 0; i < n; ++i) {
        if (ZC[i] == ZASM[i]) ++chk;
    }
    printf("\n=====[!SANITY CHECK!]=====\n");
    printf("[INFO] C version is sanity check answer key.\n");
    printf("[INFO] %d out of %d match. ", chk, n);

    if (chk == n) printf("x85_64 is consistent and correct with C.\n");

    fclose(file3);
    return 0;
}
