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
    int T = 30;

    int n, chk;
    double A;
    double *X, *Y, *ZC, *ZASM;
    char* fname;
    char* suffix;
    char* folder;

    fname = (char*)malloc(10 * sizeof(char));
    suffix = (char*)malloc(20 * sizeof(char));

    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    long double cpu_time_used;
    long double c_cpu_ave = 0;
    long double asm_cpu_ave = 0;
    

    // ask for file input
    dir();
    printf("\nEnter filename: ");
    gets(fname);

    snprintf(suffix, 70 * sizeof(char), "../__tests__/%s", fname);
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

    if (X == NULL || Y == NULL || ZC == NULL || ZASM == NULL) {
        printf("Failed to allocate enough memory...\n");
        return -1;
    }

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

    QueryPerformanceFrequency(&frequency);

    // Measure time for C function
    for (int t = 0; t < T; ++t) {
        QueryPerformanceCounter(&start);
        c_DAXPY(n, A, X, Y, ZC);
        QueryPerformanceCounter(&end);
        cpu_time_used = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        c_cpu_ave += cpu_time_used;
    }


    // Print the results of the C function
    printf("\n=====[DAXPY USING C KERNEL]=====\n");
    printf("Time taken (C): %f seconds\n", cpu_time_used);
    
    if (n <= 10) displayMembers(n, 'Z', ZC);
    else {
        displayMembers(10, 'Z', ZC);
        printf("... and %d more members...\n", n - 10);
    }
    

    // Measure time for NASM function

    QueryPerformanceFrequency(&frequency);
 
    for (int t = 0; t < T; ++t) {
        QueryPerformanceCounter(&start);
        asm_DAXPY(n, A, X, Y, ZASM);
        QueryPerformanceCounter(&end);
        cpu_time_used = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;


        // Print the contents of start and end
        //printf("Start: %llu\n", start.QuadPart);
        //printf("End: %llu\n", end.QuadPart);

        // Print the elapsed time
        printf("Elapsed time: %lf \n", cpu_time_used);

        asm_cpu_ave += cpu_time_used;

    }
    printf("Total Time: %lf \n", asm_cpu_ave);
    //asm_cpu_ave = asm_cpu_ave / 30;
    //printf("Ave Time: %lf \n", asm_cpu_ave);


    
    
    // Print the results of the asm function
    printf("\n=====[DAXPY USING ASM KERNEL]=====\n");
    printf("Time taken (NASM): %f seconds\n", cpu_time_used);
    if (n <= 10) displayMembers(n, 'Z', ZASM);
    else {
        displayMembers(10, 'Z', ZASM);
        printf("... and %d more members...\n", n - 10);
    }    

    // Correctness check
    chk = 0;
    for (int i = 0; i < n; ++i) {
        if (*(ZC+i) == *(ZASM+i)) ++chk;
    }
    printf("\n=====[!SANITY CHECK!]=====\n");
    printf("[INFO] C version is sanity check answer key.\n");
    printf("[INFO] %d out of %d match. ", chk, n);

    if (chk == n) printf("x86_64 is consistent and correct with C.\n\n\n\n");
    printf("Average Time (C) / 30 runs: %lf s\n", c_cpu_ave / 30);
    printf("Average Time (NASM) / 30 runs: %lf s\n", asm_cpu_ave / 30);
    
    // free memory
    free(X);
    free(Y);
    free(ZC);
    free(ZASM);
    free(fname);
    free(suffix);

    return 0;
}
