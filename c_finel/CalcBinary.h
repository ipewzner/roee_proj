
#ifndef ACLC_BINARY_H
#define ACLC_BINARY_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "IntegralImage.h"

// #define N 3201  // Adjust these according to your image dimensions
// #define M 72    // Adjust these according to your image dimensions
#define PFA 2e-10

double calculateThreshold(double refWin, int len) {
    return sqrt((pow(PFA, -1.0 / len) - 1) * refWin);
}

void CalcBinary(const double* img, int* BinaryMap) {
    // Allocate memory for the integral image (1D array)
    double* IntI = (double*)malloc(N_S * M_S * sizeof(double));

    // Calculate the integral image
    IntegralImage(img, IntI);

    // Parameters for the guard and background windows
    int BackWinSize_rng = 50;
    int GuardWinSize_rng = 20;
    int BackWinSize_teta = 3;
    int GuardWinSize_teta = 2;

    // Main loop for binary map calculation
    for (int n = 0; n < N_S; n++) {
        for (int m = 0; m < M_S; m++) {
            // Define the background and guard window ranges
            int r1 = (n - BackWinSize_rng / 2 > 0) ? n - BackWinSize_rng / 2 : 0;
            int r2 = (n - GuardWinSize_rng / 2 > 0) ? n - GuardWinSize_rng / 2 : 0;
            int r3 = (n + GuardWinSize_rng / 2 < N_S) ? n + GuardWinSize_rng / 2 : N_S - 1;
            int r4 = (n + BackWinSize_rng / 2 < N_S) ? n + BackWinSize_rng / 2 : N_S - 1;

            int c1 = (m - BackWinSize_teta / 2 > 0) ? m - BackWinSize_teta / 2 : 0;
            int c2 = (m - GuardWinSize_teta / 2 > 0) ? m - GuardWinSize_teta / 2 : 0;
            int c3 = (m + GuardWinSize_teta / 2 < M_S) ? m + GuardWinSize_teta / 2 : M_S - 1;
            int c4 = (m + BackWinSize_teta / 2 < M_S) ? m + BackWinSize_teta / 2 : M_S - 1;

            // Calculate B1 and B2 using 1D index calculations
            double B1 = IntI[r4 * M_S + c4] - IntI[r4 * M_S + c1] - IntI[r1 * M_S + c4] + IntI[r1 * M_S + c1];
            double B2 = IntI[r3 * M_S + c3] - IntI[r3 * M_S + c2] - IntI[r2 * M_S + c3] + IntI[r2 * M_S + c2];

            // Calculate lengths
            int len1 = (r4 - r1 + 1) * (c4 - c1 + 1) - (c4 - c1 - 1) - (r4 - r1);
            int len2 = (r3 - r2 + 1) * (c3 - c2 + 1) - (c3 - c2 - 1) - (r3 - r1);

            // Reference window
            double refWin = B1 - B2;
            int len = len1 - len2;

            // Calculate Rayleigh threshold
            double thRayleigh = calculateThreshold(refWin, len);

            // Set binary map value
            if (img[n * M_S + m] > thRayleigh) {
                BinaryMap[n * M_S + m] = 1;
            } else {
                BinaryMap[n * M_S + m] = 0;
            }
        }
    }

    // Free allocated memory for the integral image
    free(IntI);
}
#endif  // ACLC_BINARY_H