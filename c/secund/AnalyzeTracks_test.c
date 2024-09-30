#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AnalyzeTracks.h"

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
void assert_arrays_equal(double* arr1, double* arr2, int size, const char* msg) {
    for (int i = 0; i < size; i++) {
        if (fabs(arr1[i] - arr2[i]) > 1e-6) {
            printf("%s: Arrays differ at index %d (%.6f != %.6f)\n", msg, i, arr1[i], arr2[i]);
            exit(EXIT_FAILURE);
        }
    }
    printf("%s: Test passed\n", msg);
}

// Main test function
void test_AnalyzeTracks() {
    int NumDetect = 13;
    int ping_ind = 1;
    int Tping = 1;
    int numDetect = 13;
    int sigmaTeta = 3;
    int ping_number = 2;
    int cov_fact = 5;
    int Win_dlt = 10;
    int xmax = 10;
    int CurrentTargetInd = 10;
    int CurrentTargetInd_after = 12;

    // Load input data
    double* MergedRng = (double*)malloc(NumDetect * sizeof(double));
    double* MergedTeta = (double*)malloc(NumDetect * sizeof(double));
    double* MergedYc = (double*)malloc(NumDetect * 2 * sizeof(double));
    double* TracksDataBinMat = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksDataMat = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksMissMat = (double*)malloc(5 * 360 * 15 * sizeof(double));
    double* TracksP = (double*)malloc(360 * 4 * 4 * sizeof(double));
    double* TracksVecMat = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksX = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksMat = (double*)malloc(3 * 360 * sizeof(double));

    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedRng.csv", MergedRng, NumDetect);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedTeta.csv", MergedTeta, NumDetect);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedYc.csv", MergedYc, NumDetect, 2);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksDataBinMat.csv", TracksDataBinMat, 360, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksDataMat.csv", TracksDataMat, 360, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksX.csv", TracksX, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksVecMat.csv", TracksVecMat, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksMat.csv", TracksMat, 3, 360);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksMissMat.csv", TracksMissMat, 5, 360, 15);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksP.csv", TracksP, 360, 4, 4);

    // Load expected output data
    double* TracksDataBinMat_after = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksDataMat_after = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksMissMat_after = (double*)malloc(5 * 360 * 15 * sizeof(double));
    double* TracksP_after = (double*)malloc(360 * 4 * 4 * sizeof(double));
    double* TracksVecMat_after = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksX_after = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksMat_after = (double*)malloc(3 * 360 * sizeof(double));
    double* uncorr_plots_list = (double*)malloc(NumDetect * sizeof(double));

    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksDataBinMat.csv", TracksDataBinMat_after, 360, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksDataMat.csv", TracksDataMat_after, 360, 360);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksMissMat.csv", TracksMissMat_after, 5, 360, 15);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksP.csv", TracksP_after, 360, 4, 4);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksVecMat.csv", TracksVecMat_after, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksX.csv", TracksX_after, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_after/TracksMat.csv", TracksMat_after, 3, 360);

    AnalyzeTracks(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat,
                  MergedRng, MergedTeta, MergedYc, ping_number, Tping, sigmaTeta, Win_dlt, Dlt_th,
                  MaxTracks, CurrentTargetInd, numDetect, MaxTarget, no_of_pings);

    // Compare the outputs with the expected data
    assert_arrays_equal(TracksMat, TracksMat_after, 360 * 3, "TracksMat");
    printf("tests 1 passed!\n");
    assert_arrays_equal(TracksP, TracksP_after, 360 * 4 * 4, "TracksP");
    printf("tests 2 passed!\n");
    assert_arrays_equal(TracksDataMat, TracksDataMat_after, 360 * 360, "TracksDataMat");
    printf("tests 3 passed!\n");
    assert_arrays_equal(TracksDataBinMat, TracksDataBinMat_after, 360 * 360, "TracksDataBinMat");
    printf("tests 4 passed!\n");
    assert_arrays_equal(TracksX, TracksX_after, 360 * 4, "TracksX");
    printf("tests 5 passed!\n");
    assert_arrays_equal(TracksMissMat, TracksMissMat_after, 5 * 360 * 15, "TracksMissMat");
    printf("tests 6 passed!\n");
    assert_arrays_equal(TracksVecMat, TracksVecMat_after, 360 * 4, "TracksVecMat");

    printf("All tests passed!\n");
};

int main() {
    test_AnalyzeTracks();
    return 0;
};

/*

gcc -o AnalyzeTracks_test AnalyzeTracks.c -lm
./AnalyzeTracks_test

*/
