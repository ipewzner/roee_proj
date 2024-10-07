
#ifndef INTEGRAL_IMAGE_H
#define INTEGRAL_IMAGE_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
// #define N 3201
// #define M 72

// Function to compute the integral image
void IntegralImage(const double* img, double* IntI) {
    for (int n = 0; n < N_S; ++n) {
        for (int m = 0; m < M_S; ++m) {
            IntI[n * M_S + m] = img[n * M_S + m] * img[n * M_S + m];

            if (n > 0) {
                IntI[n * M_S + m] += IntI[(n - 1) * M_S + m];
            }

            if (m > 0) {
                IntI[n * M_S + m] += IntI[n * M_S + m - 1];
            }

            if (n > 0 && m > 0) {
                IntI[n * M_S + m] -= IntI[(n - 1) * M_S + m - 1];
            }
        }
    }
}
#endif  // INTEGRAL_IMAGE_H