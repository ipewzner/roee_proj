#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DetectionBlobsRoee.h"  // Include the bwconncomp function

#define MAX_LABELS 10000
#define EPSILON 1e-6  // Tolerance for floating-point comparison

#define N 3201  // Adjust these according to your image dimensions
#define M 72    // Adjust these according to your image dimensions

/*
// Function to load CSV data into an array
void loadCSV(const char* filename, int* array, int* rows, int* cols) {
    printf("Loading %s...\n", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
         printf("Error opening file: %s\n", filename);

        exit(EXIT_FAILURE);
    }

    int r = 0, c = 0;
    while (fscanf(file, "%lf,", &array[r * (*cols) + c]) == 1) {
        c++;
        if (c == *cols) {
            c = 0;
            r++;
        }
    }

    *rows = r;
    *cols = c ? c : *cols;  // Adjust columns if it's the last line
    fclose(file);
}
*/

void loadCSV(const char* filename, int* arr, int* size) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    printf("size: %d\n", *size);
    char line[1024];
    int i = 0, j = 0;
    while (fgets(line, 1024, fp) != NULL && i < *size) {
        char* token = strtok(line, ",");
        while (token != NULL && j < *size) {
            arr[j] = atoi(token);
            // printf("\n %d) %d ", j,arr[j]);
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }

    fclose(fp);
}

// Function to read binary map from CSV
void ReadCSVInt(const char* filename, int* array) {
    printf("Loading %s...\n", filename);

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

// Function to check if two arrays are approximately equal
int arraysAreEqual(int* arr1, int* arr2, int size) {
    printf("Checking if arrays are equal...\n");
    printf("size: %d\n", size);
    for (int i = 0; i < size; i++) {
        printf("\n%d,%d) %d", i, arr1[i], arr2[i]);

        if (fabs(arr1[i] - arr2[i]) > EPSILON) {
            return 0;  // Arrays are not equal
        }
    }
    return 1;  // Arrays are equal
}

// Function to test DetectionBlobsRoee
void test_DetectionBlobsRoee() {
    // Load expected data
    int size = 24;           // this test is 24
    int rows = N, cols = M;  // Initialize rows and cols to the correct values
    int* expected = (int*)malloc(MAX_LABELS * sizeof(int));
    int* binaryMap = (int*)malloc(N * M * sizeof(int));

    ReadCSVInt("C:/Users/ipewz/Desktop/roee_proj/forTest/bm.csv", binaryMap);
    loadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/CL.csv", expected, &size);

    printf("rows: %d, cols: %d\n", rows, cols);

    // Allocate memory for result
    int* result = (int*)malloc(MAX_LABELS * sizeof(int));
    printf("Expected:\n");
    
    for (int i = 0; i < size; i++) {
        printf("%d ", expected[i]);  // Correct format specifier for int
    }
    // Call DetectionBlobsRoee
    int numResult;
    DetectionBlobsRoee(binaryMap, rows, cols, 1, 10000, result, &numResult);

    printf("Expected:\n");
    for (int i = 0; i < numResult; i++) {
        printf("%d ", expected[i]);  // Correct format specifier for int
    }
    
    printf("\nResult:\n");
    for (int i = 0; i < numResult; i++) {
        printf("%d ", result[i]);  // Correct format specifier for int
    }

    // Compare results
    if (arraysAreEqual(result, expected, numResult)) {
        printf("\nTest passed\n");

    } else {
        printf("\nTest failed\n");
        printf("Expected:\n");
        for (int i = 0; i < numResult; i++) {
            printf("%d ", expected[i]);  // Correct format specifier for int
        }
        printf("\n");
        printf("Result:\n");
        for (int i = 0; i < numResult; i++) {
            printf("%d ", result[i]);  // Correct format specifier for int
        }
        printf("\n");
    }

    // Free allocated memory
    free(expected);
    free(binaryMap);
    free(result);
}

int main() {
    test_DetectionBlobsRoee();
    return 0;
}

/*

 gcc DetectionBlobsRoee_test.c -o DetectionBlobsRoee_test.exe -lm
./DetectionBlobsRoee_test.exe

*/