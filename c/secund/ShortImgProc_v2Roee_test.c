#include "ShortImgProc_v2Roee.h"  // Assuming the C function is in this header file

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    // Load test data from CSV files
    int IndVec_len, rVec_len, azBeams_len = 72, expected_merged_pingRng_len=24, expected_merged_pingTeta_len=24, expected_merged_pingYc_rows=2, expected_merged_pingYc_cols=24;
    double IndVec[24], rVec[24], azBeams[72], expected_merged_pingRng[24], expected_merged_pingTeta[24], expected_merged_pingYc[2*24];
    
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/IndVec.csv", IndVec, 24);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/rVec.csv", rVec, 24);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/azBeams.csv", azBeams, 72);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingRng.csv", expected_merged_pingRng, 10);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingTeta.csv", expected_merged_pingTeta, 10);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingYc.csv", expected_merged_pingYc, 2, 10);

    // Set the number of elements to test
    int test_len = 24;

/*    // Run the function to get results
    double *merged_detections_pingRng = (double *)malloc(test_len * sizeof(double));
    double *merged_detections_pingTeta = (double *)malloc(test_len * sizeof(double));
    double *merged_detections_pingYc = (double *)malloc(2 * test_len * sizeof(double));
*/
double merged_detections_pingRng[test_len];
    double merged_detections_pingTeta[test_len];
    double merged_detections_pingYc[2 * test_len];

    ShortImgProc_v2Roee(IndVec, rVec, 24, 3.0, azBeams, 2.035019700000000e+02, merged_detections_pingRng, merged_detections_pingTeta, merged_detections_pingYc);

    // Assertions to validate results
    assert(test_len == expected_merged_pingRng_len && test_len == expected_merged_pingTeta_len && 2 * test_len == expected_merged_pingYc_rows * expected_merged_pingYc_cols);
 /*

   assert(memcmp(merged_detections_pingRng, expected_merged_pingRng,10 * sizeof(double)) == 0);
    assert(memcmp(merged_detections_pingTeta, expected_merged_pingTeta, 10 * sizeof(double)) == 0);
    assert(memcmp(merged_detections_pingYc, expected_merged_pingYc, 2 * 10 * sizeof(double)) == 0);
 */


 for (int i = 0; i <  test_len; i++) {
        assert(fabs(merged_detections_pingRng[i] - expected_merged_pingRng[i]) <= 0.000001);
        assert(fabs(merged_detections_pingTeta[i] - expected_merged_pingTeta[i]) <= 0.000001);
    }

 for (int i = 0; i <  10; i++) {
        assert(fabs(merged_detections_pingYc[i] - expected_merged_pingYc[i]) <= 0.000001);
        assert(fabs(merged_detections_pingYc[i+24] - expected_merged_pingYc[i+10]) <= 0.000001);
    }
/*
    // Print outputs for verification
    printf("Merged Detection Ranges:\n");
    for (int i = 0; i < test_len; i++) {
        printf("%f ", merged_detections_pingRng[i]);
    }
    printf("\n");

    printf("Expected Ranges:\n");
    for (int i = 0; i < test_len; i++) {
        printf("%f ", expected_merged_pingRng[i]);
    }
*/
    printf("\n");

    // ... similar output for merged_detections_pingTeta and merged_detections_pingYc

   /*
   free(IndVec);
    free(rVec);
    free(azBeams);
    free(expected_merged_pingRng);
    free(expected_merged_pingTeta);
    free(expected_merged_pingYc);
    free(merged_detections_pingRng);
    free(merged_detections_pingTeta);
    free(merged_detections_pingYc);
*/
    printf("Everything passed\n");
    return 0;
}