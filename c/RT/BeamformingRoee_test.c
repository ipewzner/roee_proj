#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
//#include "BeamformingRoee.h" // Your BeamformingRoee function header
#include "BeamformingRoee.h" // Your BeamformingRoee function header


#define N 3201  // Number of rows
#define M 72    // Number of columns
/*
// Function to check if a file exists
int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        printf("File exists: %s\n", filename);
        return 1;
    }
    printf("File does not exist: %s\n", filename);
    return 0;
}*/
// Function to check if a file exists and print its full path
int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        printf("File exists: %s\n", filename);  // Print the full path of the existing file
        fclose(file);
        return 1;
    }
    return 0;
}

// Function to generate a unique filename with an incremented suffix
void get_unique_filename(char* base_name, char* result_name) {
    int count = 1;
    snprintf(result_name, 100, "%s_%03d.csv", base_name, count);
    while (file_exists(result_name)) {
        count++;
        snprintf(result_name, 100, "%s_%03d.csv", base_name, count);
    }
}

// Function to save the result array to a CSV file
void save_to_csv(double* array, int rows, int cols, const char* base_filename) {
    char result_filename[100];
    get_unique_filename((char*)base_filename, result_filename);

    FILE* file = fopen(result_filename, "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    // Write the 2D array (stored as 1D) to the file
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%f", array[i * cols + j]);
            if (j < cols - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Results saved to %s\n", result_filename);
}

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

// Function to read binary map from CSV (for integer arrays)
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
        }
    }

    fclose(file);
}

// Function to find the maximum absolute difference between two arrays (1D representation of 2D arrays)
double find_max_diff(double* array1, double* array2, int rows, int cols, int* max_diff_index_row, int* max_diff_index_col) {
    double max_diff = 0.0;
    *max_diff_index_row = 0;
    *max_diff_index_col = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double diff = fabs(array1[i * cols + j+1] - array2[i * cols + j]);
            if (diff > max_diff) {
                max_diff = diff;
                *max_diff_index_row = i;
                *max_diff_index_col = j;
            }
        }
    }

    return max_diff;
}

void test_BeamformingRoee() {
    // Allocate memory for input and output arrays
    double* PingData = (double*)calloc(4 * 3201, sizeof(double)); // 4 sensors, 1000 samples
    double* matched_filter = (double*)calloc(1281, sizeof(double)); // Assuming filter size is 128
    double* azBeams = (double*)calloc(72, sizeof(double));         // Assuming 72 azimuth beams
    double* pos_sensors = (double*)calloc(4 * 3, sizeof(double));  // 4 sensors, 3 coordinates each
    double* img = (double*)calloc(N * M, sizeof(double));          // Expected result (image) 3201 x 72
    double* result = (double*)calloc(N * M, sizeof(double));       // Result from beamforming function

    // Load the input data from CSV files
    ReadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv", img,N,M);
    ReadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/possens.csv", pos_sensors,4,3);
    ReadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/azbeem.csv", azBeams,1,72);
    ReadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/11.csv", PingData,4,3201);
    ReadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/mf.csv", matched_filter,1,1281);

    // Run the beamforming function
    BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, 128e3, N, result, M, 4, N);

    // Save the result to a CSV file
    save_to_csv(result, N, M, "result");

    // Find the maximum absolute difference between the result and the expected image
    int max_diff_index_row, max_diff_index_col;
    double max_diff = find_max_diff(img, result, N, M, &max_diff_index_row, &max_diff_index_col);

    // Print the maximum difference and its index
    printf("Maximum absolute difference: %f\n", max_diff);
    printf("Index with the maximum absolute difference: (%d, %d)\n", max_diff_index_row, max_diff_index_col);

    // Assert that the results are close enough to the expected output
    assert(max_diff < 1e-5 && "Beamforming output does not match the expected image.");

    // Free the allocated memory
    free(PingData);
    free(matched_filter);
    free(azBeams);
    free(pos_sensors);
    free(img);
    free(result);

    printf("Test passed.\n");
}

int main() {
    test_BeamformingRoee();
    return 0;
}


// gcc BeamformingRoee_test.c -o BeamformingRoee_test.exe -lm
//./BeamformingRoee_test.exe