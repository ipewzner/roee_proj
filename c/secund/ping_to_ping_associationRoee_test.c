#include "ping_to_ping_associationRoee.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to load a 2D array from a CSV file
void load_csv_to_2d_array(const char* filename, double* data, int rows, int cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%lf,", &data[i * cols + j]);
        }
    }

    fclose(file);
}

// Function to load a 3D array from a CSV file
void load_csv_to_3d_array(const char* filename, double* data, int slices, int rows, int cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < slices; k++) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                fscanf(file, "%lf,", &data[k * rows * cols + i * cols + j]);
            }
        }
    }

    fclose(file);
}

// Function to load a 1D array from a CSV file
void load_csv_to_1d_array(const char* filename, double* data, int size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        fscanf(file, "%lf,", &data[i]);
    }

    fclose(file);
}

// Function to compare two arrays and assert their equality
void assert_arrays_equal(double* arr1, double* arr2, int size, const char* msg, double tol) {
    for (int i = 0; i < size; i++) {
        if (fabs(arr1[i] - arr2[i]) > tol) {
            printf("%s: Arrays differ at index %d (%.6f != %.6f)\n", msg, i, arr1[i], arr2[i]);
            exit(EXIT_FAILURE);
        }
    }
    printf("%s: Test passed\n", msg);
}

// Function to check if two arrays are nearly equal (for float comparison)
bool arrays_are_close(double* arr1, double* arr2, int size, double tol) {
    for (int i = 0; i < size; i++) {
        if (fabs(arr1[i] - arr2[i]) > tol) {
            return false;
        }
    }
    return true;
}

// Function to check if two matrices are nearly equal (for float comparison)
bool matrices_are_close(double matrix1[4][4], double matrix2[4][4], int rows, int cols, double tol) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fabs(matrix1[i][j] - matrix2[i][j]) > tol) {
                return false;
            }
        }
    }
    return true;
}

void test_ping_to_ping_associationRoee() {
    int NumDetect = 13;
    int ping_ind = 1;
    int Tping = 1;
    int sigmaTeta = 3;
    int ping_number = 2;
    int cov_fact = 5;
    int Win_dlt = 10;
    int xmax = 10;
    int CurrentTargetInd = 10;
    int CurrentTargetInd_after = 12;

    // Load input data
    double* TracksMat = (double*)malloc(3 * 360 * sizeof(double));
    double* TracksVecMat = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksP = (double*)malloc(360 * 4 * 4 * sizeof(double));
    double* TracksX = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksMissMat = (double*)malloc(5 * 360 * 15 * sizeof(double));
    double* TracksDataBinMat = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksDataMat = (double*)malloc(360 * 360 * sizeof(double));
    double* MergedRng = (double*)malloc(NumDetect * sizeof(double));
    double* MergedTeta = (double*)malloc(NumDetect * sizeof(double));
    double* MergedYc = (double*)malloc(NumDetect * 2 * sizeof(double));

    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedRng.csv", MergedRng, NumDetect);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedTeta.csv", MergedTeta, NumDetect);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedYc.csv", MergedYc, NumDetect, 2);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksDataBinMat.csv", TracksDataBinMat, 360, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksDataMat.csv", TracksDataMat, 360, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksX.csv", TracksX, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksVecMat.csv", TracksVecMat, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksMat.csv", TracksMat, 3, 360);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksMissMat.csv", TracksMissMat, 5, 360, 15);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksP.csv", TracksP, 360, 4, 4);

    // Load expected output data
    double* TracksMat_after = (double*)malloc(3 * 360 * sizeof(double));
    double* TracksVecMat_after = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksP_after = (double*)malloc(360 * 4 * 4 * sizeof(double));
    double* TracksX_after = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksMissMat_after = (double*)malloc(5 * 360 * 15 * sizeof(double));
    double* TracksDataBinMat_after = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksDataMat_after = (double*)malloc(360 * 360 * sizeof(double));

    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_associationRoee_after/TracksDataBinMat.csv", TracksDataBinMat_after, 360, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_associationRoee_after/TracksDataMat.csv", TracksDataMat_after, 360, 360);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_associationRoee_after/TracksMissMat.csv", TracksMissMat_after, 5, 360, 15);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_associationRoee_after/TracksP.csv", TracksP_after, 360, 4, 4);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_associationRoee_after/TracksVecMat.csv", TracksVecMat_after, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_associationRoee_after/TracksX.csv", TracksX_after, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_associationRoee_after/TracksMat.csv", TracksMat_after, 3, 360);

    // Call the function
    ping_to_ping_associationRoee(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, MergedRng, MergedTeta, MergedYc, NumDetect, ping_ind, sigmaTeta, cov_fact, Win_dlt, xmax, Tping, CurrentTargetInd);
/*
    printf("\nTracksX:       ");
    for (size_t i = 0; i < 360; i++) {
        if (*(TracksX + 360 + i) < 100) {
            printf("%f ", *(TracksX + 360 + i));
        }
    }

    printf("\nTracksX_after  ");
    for (size_t i = 0; i < 360; i++) {
        if (*(TracksX_after + 360 + i) < 100) {
            printf("%f ", *(TracksX_after + 360 + i));
        }
    }

    printf("\nTracksVecMat:       ");
    for (size_t j = 0; j < 4; j++) {
        printf("\n");
        for (size_t i = 0; i < 360; i++) {
            if (*(TracksVecMat + j * 360 + i) < 900) {
                printf("%f ", *(TracksVecMat + j * 360 + i));
            }
        }
    }

    printf("\nTracksVecMat_after  ");
    for (size_t j = 0; j < 4; j++) {
        printf("\n");
        for (size_t i = 0; i < 360; i++) {
            if (*(TracksVecMat_after + j * 360 + i) < 900) {
                printf("%f ", *(TracksVecMat_after + j * 360 + i));
            }
        }
    }
*/
/*
    printf("\n TracksP:       ");
    for (size_t j = 0; j < 360; j++) {
        printf("\n");
        for (size_t i = 0; i < 16; i++) {
         //   if (*(TracksDataMat + j * 360 + i) < 900) {
                printf("%f ", *(TracksP + j * 16 + i));
                printf("%f ", *(TracksP_after + j * 16 + i));
          //  }
        }
    }
*/
 /*printf("\n TracksMissMat:       ");
    for (size_t j = 0; j < 5; j++) {
       // printf("\n");
        for (size_t i = 0; i < 360; i++) {
         printf("\n");
        for (size_t k = 0; k < 15; k++) {
         //   if (*(TracksDataMat + j * 360 + i) < 900) {
                printf("%f ", *(TracksMissMat + j* 360 * 15+ i * 360+k));
                printf("%f ", *(TracksMissMat_after + j* 360 * 15+ i * 360+k));
          //  }
        }}
    }
*/
    // Tolerance for float comparison
    double tolerance = 0.1;
    printf("\n-------------------------------\n");

    if (!arrays_are_close(TracksDataMat, TracksDataMat_after, 360 * 360, tolerance)) {
        printf("\nTest failed: TracksDataMat does not match expected values\n");
    } else {
        printf("\nTest passed: TracksDataMat matches expected values\n");
    }
    if (!arrays_are_close(TracksDataBinMat, TracksDataBinMat_after, 360 * 360, tolerance)) {
        printf("\nTest failed: TracksDataBinMat does not match expected values\n");
    } else {
        printf("\nTest passed: TracksDataBinMat matches expected values\n");
    }

    if (!arrays_are_close(TracksVecMat, TracksVecMat_after, 360 * 4, tolerance)) {
        printf("\nTest failed: TracksVecMat does not match expected values\n");
    } else {
        printf("\nTest passed: TracksVecMat matches expected values\n");
    }
    if (!arrays_are_close(TracksP, TracksP_after, 360 * 4*4, tolerance)) {
        printf("\nTest failed: TracksP does not match expected values\n");
    } else {
        printf("\nTest passed: TracksP matches expected values\n");
    }
    if (!arrays_are_close(TracksMissMat, TracksMissMat_after,5*360*15, tolerance)) {
        printf("\nTest failed: TracksMissMat does not match expected values\n");
    } else {
        printf("\nTest passed: TracksMissMat matches expected values\n");
    }
    if (!arrays_are_close(TracksMat, TracksMat_after, 360 * 3, tolerance)) {
        printf("\nTest failed: TracksMat does not match expected values\n");
    } else {
        printf("\nTest passed: TracksMat matches expected values\n");
    }
    if (!arrays_are_close(TracksX, TracksX_after, 360 * 4, tolerance)) {
        printf("\nTest failed: TracksX does not match expected values\n");
    } else {
        printf("\nTest passed: TracksX matches expected values\n");
    }
    printf("\n-------------------------------\n");

    // Compare the outputs with the expected data
    assert_arrays_equal(TracksDataMat, TracksDataMat_after, 360 * 360, "\nTracksDataMat",tolerance);
    printf("\ntests 1 passed!\n");
    assert_arrays_equal(TracksDataBinMat, TracksDataBinMat_after, 360 * 360, "\nTracksDataBinMat",tolerance);
    printf("\ntests 2 passed!\n");
    assert_arrays_equal(TracksMat, TracksMat_after, 360 * 3, "\nTracksMat",tolerance);
    printf("tests 3 passed!\n");
    assert_arrays_equal(TracksMissMat, TracksMissMat_after, 5 * 360 * 15, "\nTracksMissMat",tolerance);
    printf("tests 4 passed!\n");
    assert_arrays_equal(TracksP, TracksP_after, 360 * 4 * 4, "\nTracksP",tolerance);
    printf("tests 5 passed!\n");
    assert_arrays_equal(TracksVecMat, TracksVecMat_after, 360 * 4, "\nTracksVecMat",tolerance);
    printf("tests 6 passed!\n");
    assert_arrays_equal(TracksX, TracksX_after, 360 * 4, "\nTracksX",tolerance);

    printf("\nAll tests passed!\n");
}

int main() {
    test_ping_to_ping_associationRoee();
    return 0;
}

/*

 gcc ping_to_ping_associationRoee_test.c -o ping_to_ping_associationRoee_test.exe -lm
./ping_to_ping_associationRoee_test.exe

*/
