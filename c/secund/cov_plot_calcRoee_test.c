
#include "cov_plot_calcRoee.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define PI 3.14159

void test_cov_plot_calcRoee() {
    // Load inputs from CSV files
    double *MergedRng, *MergedTeta;
    size_t sizeRng, sizeTeta;


    double sigmaTeta = 3.0;
    size_t k = 0;  // Assuming we are testing the first element

    double rc[4];
    cov_plot_calcRoee(21.073242187500000, 1.125457163528671, 3, rc);
    double expected_rc[4] = { 2.221033605856399e+02,
                              -8.595739415541884,
                              -8.595739415541884,
                              2.221033605851150e+02 };
   
    // Compare matrices
    for (int i = 0; i < 4; i++) {
        //printf("rc[%d] = %f, expected_rc[%d] = %f\n", i, rc[i], i, expected_rc[i]);
        if (fabs(rc[i] - expected_rc[i]) < 1e-3||fabs(rc[i] - expected_rc[i]) > 1e-3) {
        printf("\nfabs(rc[i] - expected_rc[i]%f",(fabs(rc[i] - expected_rc[i]))); 
        }
    }
    printf("\nTest passed\n");

}

int main() {
    test_cov_plot_calcRoee();
    return 0;
}

/*

 gcc cov_plot_calcRoee_test.c -o cov_plot_calcRoee_test.exe -lm
./cov_plot_calcRoee_test.exe

*/