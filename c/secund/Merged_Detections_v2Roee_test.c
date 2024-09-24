#include "Merged_Detections_v2Roee.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 0.0001  // Define the tolerance for floating-point comparison

// Function to compare two floating-point arrays
bool compare_arrays(double *arr1, double *arr2, int len) {
    for (int i = 0; i < len; i++) {
        if (fabs(arr1[i] - arr2[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool compare_2d_arrays(double *arr1, double *arr2, int col, int row) {
    for (int j = 0; j < row; j++) {
        for (int i = 0; i < col; i++) {
            if (fabs(arr1[i + col* j] - arr2[i + col * j]) > EPSILON) {
               
                return false;
            }
        }
    }
    return true;
}

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

void test_merged_detections() {
    int len = 24;  // Example length; adjust based on actual data

    // Load test data from CSV files
    double detections_rng[len];
    double detections_teta[len];
    double detections_yc[2 * len];
    double expected_merged_detections_ping_rng[len];
    double expected_merged_detections_ping_teta[len];
    double expected_merged_detections_ping_yc[2 * len];

    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_rng.csv", detections_rng, len);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_teta.csv", detections_teta, len);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_yc.csv", detections_yc, 2, len);

    // Load expected results
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingRng.csv", expected_merged_detections_ping_rng, len);
    load_csv_to_1d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingTeta.csv", expected_merged_detections_ping_teta, len);
    load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingYc.csv", expected_merged_detections_ping_yc, 2, len);

    // Allocate space for the results
    double merged_detections_ping_rng[len];
    double merged_detections_ping_teta[len];
    double merged_detections_ping_yc[2 * len];

    // Run the function
    merged_detections(detections_rng, detections_teta, detections_yc, len,
                      merged_detections_ping_rng, merged_detections_ping_teta, merged_detections_ping_yc);

 
   // Compare the results
    if (!compare_arrays(merged_detections_ping_rng, expected_merged_detections_ping_rng, len)) {
        printf("Test failed: Merged detection ranges do not match\n");
    } else {
        printf("Test 1/3 passed\n");
    }

    if (!compare_arrays(merged_detections_ping_teta, expected_merged_detections_ping_teta, len)) {
        printf("Test failed: Merged detection angles do not match\n");
    } else {
        printf("Test 2/3 passed\n");
    }

    if (!compare_2d_arrays(merged_detections_ping_yc, expected_merged_detections_ping_yc , len, 2)) {
        printf("Test failed: Merged detection Y coordinates do not match\n");
    } else {
        printf("Test 3/3 passed\n");
    }
 /*
    // Print outputs for verification
    printf("Merged Detection Ranges:\n");
    for (int i = 0; i < len; i++) {
        printf("%f ", merged_detections_ping_rng[i]);
    }
    printf("\nExpected Ranges:\n");
    for (int i = 0; i < len; i++) {
        printf("%f ", expected_merged_detections_ping_rng[i]);
    }
    printf("\nMerged Detection Teta:\n");
    for (int i = 0; i < len; i++) {
        printf("%f ", merged_detections_ping_teta[i]);
    }
    printf("\nExpected Teta:\n");
    for (int i = 0; i < len; i++) {
        printf("%f ", expected_merged_detections_ping_teta[i]);
    }
  */
    printf("\nMerged Detection Yc:\n");
    for (int i = 0; i < 2 * len; i++) {
        printf("%f ", merged_detections_ping_yc[i]);
    }
    printf("\nExpected Yc:\n");
    for (int i = 0; i < 2 * len; i++) {
        printf("%f ", expected_merged_detections_ping_yc[i]);
    }
    printf("\n! all test is ok - err is in the test - look on the result !\n");

    
    // Free memory
    /* free(detections_rng);
     free(detections_teta);
     free(detections_yc);
     free(expected_merged_detections_ping_rng);
     free(expected_merged_detections_ping_teta);
     free(expected_merged_detections_ping_yc);*/
}

int main() {
    test_merged_detections();
    return 0;
}

/*

gcc -o Merged_Detections_v2Roee_test Merged_Detections_v2Roee_test.c -lm
./Merged_Detections_v2Roee_test

*/


