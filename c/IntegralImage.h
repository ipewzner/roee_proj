#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3201  // Adjust these according to your image dimensions
#define M 72

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
