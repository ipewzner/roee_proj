#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Shortproc_ping_dataRoee.h"
// #define SND_VEL 1500.0


// Function to read data from a CSV file into an array (1D representation of 2D array)
void ReadCSV(const char* filename, double* array, int rows, int cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fscanf(file, "%lf%*[,]", &array[i * cols + j]) != 1) {
                perror("Failed to read data from file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}


// CSV loader function for 1D array
void load_csv_to_array(const char* filename, double* array, int size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        fscanf(file, "%lf,", &array[i]);
    }

    fclose(file);
}

// CSV loader function for 2D array (flattened)
void load_csv_to_2d_array(const char* filename, double* array, int rows, int cols) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%lf,", &array[i * cols + j]);
        }
    }

    fclose(file);
}

// Helper function to compare arrays (floating point tolerance)
int compare_arrays(const double* arr1, const double* arr2, int size, double tol) {
    for (int i = 0; i < size; i++) {
        if (fabs(arr1[i] - arr2[i]) > tol) {
            return 0;  // Arrays are not equal
        }
    }
    return 1;  // Arrays are equal
}

// Helper function to compare arrays (floating point tolerance)
int compare_arrays_from_matlab(const double* arr1, const double* arr2, int size, double tol) {
    for (int i = 0; i < size; i++) {
        if (fabs(arr1[i] - arr2[i]+1) > tol) {
            return 0;  // Arrays are not equal
        }
    }
    return 1;  // Arrays are equal
}
void test_Shortproc_ping_dataRoee() {
    // Load data from CSV files
    int pri_samples = 3201;
    double PingData[pri_samples*4];
    load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/forTest/11.csv", PingData, pri_samples);

    double matched_filter[1281];
    load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/forTest/mf.csv", matched_filter, pri_samples);

    double azBeams[72];
    load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/forTest/azbeem.csv", azBeams, pri_samples);

    double pos_sensors[4*3];
    load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/forTest/possens.csv", pos_sensors, pri_samples);

    // Expected output data
    double IndVec_expected[24];
    load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/forTest/IndVec.csv", IndVec_expected, pri_samples);

    double rVec_expected[24];
    load_csv_to_array("C:/Users/ipewz/Desktop/roee_proj/forTest/rvec.csv", rVec_expected, pri_samples);
    
    double* img = (double*)calloc(N * M, sizeof(double));          // Expected result (image) 3201 x 72
    ReadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv", img,N,M);

    // Output variables
    double IndVec[24];
    double rVec[24];
    double NewImgTh;
    int num_azBeams = 72;
    // Call the function to test
    Shortproc_ping_dataRoee(img,IndVec, rVec, &NewImgTh, PingData, matched_filter, 15, 128e3, azBeams, num_azBeams, pos_sensors, pri_samples, 999, 0, 1, 4, N);

    // Compare results
    assert(fabs(NewImgTh - 999) < 1e-6 && "NewImgTh is not equal to 999");
    assert(compare_arrays(rVec, rVec_expected, 24, 1e-6) && "rVec does not match expected values");
    assert(compare_arrays_from_matlab(IndVec, IndVec_expected, 24, 1e-6) && "IndVec does not match expected values");

    // Output results
    printf("IndVec and rVec match expected values.\n");
    printf("NewImgTh = %f\n", NewImgTh);
}

int main() {
    test_Shortproc_ping_dataRoee();
    printf("Everything passed\n");
    return 0;
}

/*

    gcc Shortproc_ping_dataRoee_test.c -o Shortproc_ping_dataRoee_test.exe -lm
    ./Shortproc_ping_dataRoee_test.exe

*/