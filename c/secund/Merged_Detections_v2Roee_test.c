#include "Merged_Detections_v2Roee.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define EPSILON 0.0001  // Define the tolerance for floating-point comparison

// Function declaration (assuming it was implemented in another file)
void merged_detections(double *DetectionsRng, double *DetectionsTeta, double *DetectionsYC, int len, 
                       double *merged_detections_pingRng, double *merged_detections_pingTeta, double *merged_detections_pingYc);

// Function to compare two floating-point arrays
bool compare_arrays(double *arr1, double *arr2, int len) {
    for (int i = 0; i < len; i++) {
        if (fabs(arr1[i] - arr2[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

// Function to load a 1D array from a CSV file
double *load_csv_to_array(const char *filename, int len) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    double *array = (double *)malloc(len * sizeof(double));
    for (int i = 0; i < len; i++) {
        fscanf(file, "%lf,", &array[i]);
    }

    fclose(file);
    return array;
}

// Function to load a 2D array from a CSV file (flattened as 1D)
double *load_csv_to_2d_array(const char *filename, int rows, int cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    double *array = (double *)malloc(rows * cols * sizeof(double));
    for (int i = 0; i < rows * cols; i++) {
        fscanf(file, "%lf,", &array[i]);
    }

    fclose(file);
    return array;
}

void test_merged_detections() {
    int len = 10;  // Example length; adjust based on actual data

    // Load test data from CSV files
    double *detections_rng = load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_rng.csv", len);
    double *detections_teta = load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_teta.csv", len);
    double *detections_yc = load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_yc.csv", 2, len);

    // Load expected results
    double *expected_merged_detections_ping_rng = load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingRng.csv", len);
    double *expected_merged_detections_ping_teta = load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingTeta.csv", len);
    double *expected_merged_detections_ping_yc = load_csv_to_2d_array("C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingYc.csv", 2, len);

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
    } else if (!compare_arrays(merged_detections_ping_teta, expected_merged_detections_ping_teta, len)) {
        printf("Test failed: Merged detection angles do not match\n");
    } else if (!compare_arrays(merged_detections_ping_yc, expected_merged_detections_ping_yc, 2 * len)) {
        printf("Test failed: Merged detection Y coordinates do not match\n");
    } else {
        printf("Test passed: All results match the expected output\n");
    }

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
    printf("\nMerged Detection Yc:\n");
    for (int i = 0; i < 2 * len; i++) {
        printf("%f ", merged_detections_ping_yc[i]);
    }
    printf("\nExpected Yc:\n");
    for (int i = 0; i < 2 * len; i++) {
        printf("%f ", expected_merged_detections_ping_yc[i]);
    }
    
    // Free memory
    free(detections_rng);
    free(detections_teta);
    free(detections_yc);
    free(expected_merged_detections_ping_rng);
    free(expected_merged_detections_ping_teta);
    free(expected_merged_detections_ping_yc);
}

int main() {
    test_merged_detections();
    return 0;
}


/*

gcc -o Merged_Detections_v2Roee_test Merged_Detections_v2Roee_test.c -lm
./Merged_Detections_v2Roee_test

*/