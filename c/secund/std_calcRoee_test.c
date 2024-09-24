
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "std_calcRoee.h"

// Function to load a 2D array from a CSV file
void load_csv_to_2d_array(const char *filename, double *data, int rows, int cols) {
    FILE *file = fopen(filename, "r");
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
void load_csv_to_3d_array(const char *filename, double *data, int slices, int rows, int cols) {
    FILE *file = fopen(filename, "r");
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
void load_csv_to_1d_array(const char *filename, double *data, int size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        fscanf(file, "%lf,", &data[i]);
    }

    fclose(file);
}

// Test function for std_calcRoee
void test_std_calcRoee() {
    // Test parameters
    int ping_ind = 2;
    double Tping = 1.0;
    int k = 1;
    int last_ping_number = 1;
    int n = 2;

    // Expected result for comparison
    double expected_std_dis = 0.00001077282126338;

    // Load inputs from CSV files
    int rows = 2, cols = 2;
    int X_rows = 4, X_cols = 2;
    int P_slices = 4, P_rows = 4, P_cols = 4;

    double MergedYc[rows * cols];
    double Rc[rows * cols];
    double TracksX[X_rows * X_cols];
    double TracksP[P_slices * P_rows * P_cols];
    double undeleted_tracks_ind[2];

    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/MergedYc.csv", MergedYc, rows, cols);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/RC.csv", Rc, rows, cols);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/TracksX.csv", TracksX, X_rows, X_cols);
    load_csv_to_3d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/TracksP.csv", TracksP, P_slices, P_rows, P_cols);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/undeleted_tracks_ind.csv", undeleted_tracks_ind, 2);

    // Select the relevant columns and indices
    double MergedYc_k[2] = {MergedYc[(k - 1)], MergedYc[(k - 1) + 1]};
    double TracksX_n[4] = {21.161515372168633, 3.863649411852741, 0, 0};

    double TracksP_n[16] = {
        1.132149658268534e+03, -19.914702437256373, 0.0, 0.0,
        -19.914702437256373, 1.132149658264554e+03, 0.0, 0.0,
        0.0, 0.0, 5.0, 0.0,
        0.0, 0.0, 0.0, 5.0
    };

    // Call the std_calcRoee function
    double calculated_std_dis = std_calcRoee(ping_ind, Tping, MergedYc_k, Rc, last_ping_number, TracksX_n, TracksP_n, n);

    // Use assert to compare floating point values with tolerance
    assert(fabs(calculated_std_dis - expected_std_dis) < 1e-9 && "calculated_std_dis does not match expected value");

    printf("Test passed\n");
}

int main() {
    test_std_calcRoee();
    return 0;
}

/*

gcc -o std_calcRoee_test std_calcRoee_test.c -lm
./std_calcRoee_test

*/
