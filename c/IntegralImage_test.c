#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "IntegralImage.h"

#define N 3201  // Adjust these according to your image dimensions
#define M 72

/*

// Function to compute the integral image
void IntegralImage(const double* img, double* IntI) {
   
    for (int n = 0; n < N; ++n) {
   
        for (int m = 0; m < M; ++m) {
   
            IntI[n * M + m] = img[n * M + m] * img[n * M + m];

            if (n > 0) {
                IntI[n * M + m] += IntI[(n - 1) * M + m];
            }
   
            if (m > 0) {
                IntI[n * M + m] += IntI[n * M + m - 1];
            }
   
            if (n > 0 && m > 0) {
                IntI[n * M + m] -= IntI[(n - 1) * M + m - 1];
            }
   
        }
   
    }

}
*/
/*
void IntegralImage1(const double* img, double* IntI) {
    // Dynamically allocate memory for the intermediate sum array
    double* z = (double*)malloc(N * M * sizeof(double));
    if (z == NULL) {
        perror("Failed to allocate memory for z");
        exit(EXIT_FAILURE);
    }

    // Iterate over columns (m) and rows (n)
    for (int m = 0; m < M; ++m) {
        for (int n = 0; n < N; ++n) {

            // Calculate the intermediate sum z[n, m]
            double z1 = (m > 0) ? z[n * M + (m - 1)] : 0;
            z[n * M + m] = z1 + img[n * M + m];

            // Calculate the integral image value IntI[n, m]
            double I1 = (n > 0) ? IntI[(n - 1) * M + m] : 0;
            IntI[n * M + m] = I1 + z[n * M + m];
            printf("\nimg [%d][%d] = %f", n, m, IntI[n * M + m]);
        }
    }

    // Free the dynamically allocated memory
    free(z);
}
// Function to read data from a CSV file into a 1D array
*/

void ReadCSV(const char* filename, double* array) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (fscanf(file, "%lf%*[,]", &array[i * M + j]) != 1) {
                perror("Failed to read data from file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}

// Function to read expected data from a CSV file into a 1D array
void ReadExpectedCSV(const char* filename, double* array) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (fscanf(file, "%lf%*[,]", &array[i * M + j]) != 1) {
                perror("Failed to read data from file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}

// Function to compare two 1D arrays
int CompareArrays(const double* arr1, const double* arr2) {
    for (int i = 0; i < N * M; ++i) {
        //printf("1) %f 2) %f\n", arr1[i], arr2[i]);

        // Adjust tolerance as needed
        if (fabs(arr1[i] - arr2[i]) > 1e-6) {
            return 0;  // Arrays are not identical
        }
    }

    return 1;  // Arrays are identical
}

// Test function for IntegralImage
void TestIntegralImage() {
    double* img = (double*)malloc(N * M * sizeof(double));
    double* computedIntI = (double*)malloc(N * M * sizeof(double));
    double* expectedIntI = (double*)malloc(N * M * sizeof(double));



    if (computedIntI == NULL || expectedIntI == NULL || img == NULL) {
        perror("Failed to allocate memory");
        free(img);
        exit(EXIT_FAILURE);
    }

    ReadCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv", img);
    ReadExpectedCSV("C:/Users/ipewz/Desktop/roee_proj/forTest/IntI.csv", expectedIntI);

    // Compute the integral image
    IntegralImage(img, computedIntI);

    // Compare the computed integral image with the expected image
    if (CompareArrays(computedIntI, expectedIntI)) {
        printf("Test passed");
    } else {
        printf("Test failed");
    }

    free(img);
    free(computedIntI);
    free(expectedIntI);
}

// Main function to run the test
int main() {
    printf("0.\n");

    TestIntegralImage();
    return 0;
}

// gcc IntegralImage_test.c -o IntegralImage_test.exe -lm
//./IntegralImage_test.exe
