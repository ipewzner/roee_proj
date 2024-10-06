#include "AnalyzeTracks.h"

#include <assert.h>
#include <math.h>
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

// Main test function
void test_AnalyzeTracks() {
    int Tping = 1;
    int numDetect = 10;
    int sigmaTeta = 3;
    int ping_number = 0;
    int cov_fact = 5;
    int Win_dlt = 10;
    int xmax = 10;
    int CurrentTargetInd = 0;
    int t_pri = 1;
    int MaxTarget = 360;
    int Dlt_th = 7;

    // Load input data
    // double* MergedRng = (double*)malloc(NumDetect * sizeof(double));
    // double* MergedTeta = (double*)malloc(NumDetect * sizeof(double));
    // double* MergedYc = (double*)malloc(NumDetect * 2 * sizeof(double));
    double* TracksDataBinMat = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksDataMat = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksMissMat = (double*)malloc(5 * 360 * 15 * sizeof(double));
    double* TracksP = (double*)malloc(360 * 4 * 4 * sizeof(double));
    double* TracksVecMat = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksX = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksMat = (double*)malloc(3 * 360 * sizeof(double));

    double MergedRng[10] = {21.073242187500000, 21.277587890625000, 21.117187500000000, 21.128906250000000, 21.152343750000000, 21.310546875000000, 30.996093750000000, 30.984375000000000, 31.013671875000000, 31.037109375000000};
    double MergedTeta[10] = {64.483945492957758, 79.652959577464813, -1.883329559154930e+02, -1.579949277464789e+02, -1.630512657746479e+02, 1.251600018309859e+02, 8.864227183098592, 34.145917323943650, -1.124878854929577e+02, -1.478822516901408e+02};
    double MergedYc[10 * 2] = {19.226777761081394, 21.161515372168633, 3.094036074712163, -8.003732123763321, -6.233984934952536, 17.613770956247659, 4.828767620724194, 17.582661934250993, -28.970201910073452, -16.682495066871997, 9.177316537555782, 3.863649411852741, -21.123780300274792, -19.804884093442283, -20.455890417864001, -12.406742662183950, 30.962327999794105, 25.924705813785828, -11.992674808685697, -26.575884742170935};
    // load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedRng.csv", MergedRng, NumDetect);
    // load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedTeta.csv", MergedTeta, NumDetect);
    // load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/MergedYc.csv", MergedYc, NumDetect, 2);
    // load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksDataBinMat.csv", TracksDataBinMat, 360, 360);
    // load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksDataMat.csv", TracksDataMat, 360, 360);
    // load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksX.csv", TracksX, 4, 360);
    // load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksVecMat.csv", TracksVecMat, 4, 360);
    // load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksMat.csv", TracksMat, 3, 360);
    // load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksMissMat.csv", TracksMissMat, 5, 360, 15);
    // load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/ping_to_ping_correlationRoee_befor/TracksP.csv", TracksP, 360, 4, 4);

    for (int i = 0; i < 3 * 360; i++) {
        TracksMat[i] = 999;
    }
    for (int i = 0; i < 5 * 360 * 15; i++) {
        TracksMissMat[i] = 999;
    }
    for (int i = 0; i < 4 * 4 * 360; i++) {
        TracksP[i] = 999;
    }
    for (int i = 0; i < 4 * 360; i++) {
        TracksVecMat[i] = 999;
        TracksX[i] = 999;
    }
    for (int i = 0; i < 360 * 360; i++) {
        TracksDataBinMat[i] = 0;
        TracksDataMat[i] = 999;
    }

    // Load expected output data
    double* TracksDataBinMat_after = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksDataMat_after = (double*)malloc(360 * 360 * sizeof(double));
    double* TracksMissMat_after = (double*)malloc(5 * 360 * 15 * sizeof(double));
    double* TracksP_after = (double*)malloc(360 * 4 * 4 * sizeof(double));
    double* TracksVecMat_after = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksX_after = (double*)malloc(4 * 360 * sizeof(double));
    double* TracksMat_after = (double*)malloc(3 * 360 * sizeof(double));
    double* uncorr_plots_list = (double*)malloc(numDetect * sizeof(double));


    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/AnalyzeTracks_after/TracksDataBinMat.csv", TracksDataBinMat_after, 360, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/AnalyzeTracks_after/TracksDataMat.csv", TracksDataMat_after, 360, 360);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/AnalyzeTracks_after/TracksMissMat.csv", TracksMissMat_after, 5, 360, 15);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/AnalyzeTracks_after/TracksP.csv", TracksP_after, 360, 4, 4);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/AnalyzeTracks_after/TracksVecMat.csv", TracksVecMat_after, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/AnalyzeTracks_after/TracksX.csv", TracksX_after, 4, 360);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/AnalyzeTracks_after/TracksMat.csv", TracksMat_after, 3, 360);

    AnalyzeTracks(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat,
                  MergedRng, MergedTeta, MergedYc, ping_number, Tping, sigmaTeta, Win_dlt, Dlt_th,
                  CurrentTargetInd, numDetect);

    double tolerance = 0.1;

    // Compare the outputs with the expected data
    assert_arrays_equal(TracksP, TracksP_after, 360 * 4 * 4, "TracksP", tolerance);
    printf("tests 1 passed!\n");
    assert_arrays_equal(TracksMat, TracksMat_after, 360 * 3, "TracksMat", tolerance);
    printf("tests 2 passed!\n");
    assert_arrays_equal(TracksDataMat, TracksDataMat_after, 360 * 360, "TracksDataMat", tolerance);
    printf("tests 3 passed!\n");
    assert_arrays_equal(TracksDataBinMat, TracksDataBinMat_after, 360 * 360, "TracksDataBinMat", tolerance);
    printf("tests 4 passed!\n");
    assert_arrays_equal(TracksX, TracksX_after, 360 * 4, "TracksX", tolerance);
    printf("tests 5 passed!\n");
    assert_arrays_equal(TracksMissMat, TracksMissMat_after, 5 * 360 * 15, "TracksMissMat", tolerance);
    printf("tests 6 passed!\n");
    assert_arrays_equal(TracksVecMat, TracksVecMat_after, 360 * 4, "TracksVecMat", tolerance);

    printf("All tests passed!\n");
};

int main() {
    test_AnalyzeTracks();
    return 0;
};

/*

gcc -o AnalyzeTracks_test AnalyzeTracks_test.c -lm
./AnalyzeTracks_test

*/
