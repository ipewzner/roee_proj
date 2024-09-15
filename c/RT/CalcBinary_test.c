#include "CalcBinary.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3201  // Adjust these according to your image dimensions
#define M 72    // Adjust these according to your image dimensions
#define PFA 2e-10

// Function to read data from a CSV file into an array
void ReadCSV(const char* filename, double* array) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (fscanf(file, "%lf%*[,]", &array[i * M + j]) != 1) {
                perror("Failed to read data from file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            /*
            if (i > (N - 10) && (j < 2 || j > 69)) {
                printf("\n%d,%d) %f", i, j, array[i * M + j]);
            }
            if (i<2 && (j < 2 || j > 69)) {
                printf("\n%d,%d) %f", i, j, array[i * M + j]);
            }
            */
        }
    }

    fclose(file);
}

// Function to read binary map from CSV
void ReadCSVInt(const char* filename, int* array) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (fscanf(file, "%d,", &array[i * M + j]) != 1) {
                perror("Failed to read data from file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            /*
            if (i > (N - 10) && (j < 2 || j > 69)) {
                printf("\n%d,%d) %d", i, j, array[i * M + j]);
            }
            if (i == 2733 && (j < 2 || j > 69)) {
                printf("\n%d,%d) %d", i, j, array[i * M + j]);
            }
            */
        }
    }

    fclose(file);
}

// Function to compare two binary maps
int CompareBinaryMaps(const int* map1, const int* map2) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            /*
            if (map1[i * M + j] != 0 || map2[i * M + j] != 0) {
                printf("\n%d,%d) map1) %d, map2) %d ", i, j, map1[i * M + j], map2[i * M + j]);
            }
            */

            if (map1[i * M + j] != map2[i * M + j]) {
                return 0;  // Maps are not identical
            }
        }
    }

    return 1;  // Maps are identical
}

// Test function
void TestCalcBinary() {
    //printf("0.1.\n");

    double* img = (double*)malloc(N * M * sizeof(double));
    int* expectedBinaryMap = (int*)malloc(N * M * sizeof(int));
    int* computedBinaryMap = (int*)malloc(N * M * sizeof(int));

    if (expectedBinaryMap == NULL || computedBinaryMap == NULL || img == NULL) {
        perror("Failed to allocate memory for binary maps");
        free(img);
        exit(EXIT_FAILURE);
    }

    //printf("1.\n");

    // Read the image and expected binary map from the CSV files
    ReadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv", img);
    //printf("3.\n");
    ReadCSVInt("C:/Users/ipewz/Desktop/roee_proj/forTest/bm.csv", expectedBinaryMap);
    //printf("4.\n");

    // Calculate the binary map
    CalcBinary(img, computedBinaryMap);
    //printf("5.\n");

    // Compare the computed binary map with the expected binary map
    if (CompareBinaryMaps(computedBinaryMap, expectedBinaryMap)) {
        printf("Test passed");
    } else {
        printf("Test failed");
    }

    // Free allocated memory
    free(img);
    free(expectedBinaryMap);
    free(computedBinaryMap);
}

// Main function
int main() {
    //printf("0.\n");

    TestCalcBinary();
    return 0;
}

// gcc CalcBinary.c -o CalcBinary.exe -lm
//./CalcBinary.exe