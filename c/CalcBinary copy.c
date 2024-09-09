#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 256   // Adjust according to the image size
#define M 256   // Adjust according to the image size

// Function to compute the integral image
void IntegralImage(const double img[N][M], double IntI[N][M]) {
    for (int n = 0; n < N; ++n) {
        for (int m = 0; m < M; ++m) {
            IntI[n][m] = img[n][m] * img[n][m];
            if (n > 0) IntI[n][m] += IntI[n-1][m];
            if (m > 0) IntI[n][m] += IntI[n][m-1];
            if (n > 0 && m > 0) IntI[n][m] -= IntI[n-1][m-1];
        }
    }
}

// Function to calculate the binary map
void CalcBinary(const double img[N][M], int BinaryMap[N][M]) {
    int BackWinSize_rng = 50;
    int GuardWinSize_rng = 20;
    int BackWinSize_teta = 3;
    int GuardWinSize_teta = 2;
    double Pfa = 2e-10;

    double IntI[N][M];
    IntegralImage(img, IntI);

    for (int n = 0; n < N; ++n) {
        for (int m = 0; m < M; ++m) {
            // Extracting pixels from the background region
            int r1 = fmax(1, n - BackWinSize_rng / 2) - 1;
            int r2 = fmax(1, n - GuardWinSize_rng / 2) - 1;
            int r3 = fmin(N, n + GuardWinSize_rng / 2) - 1;
            int r4 = fmin(N, n + BackWinSize_rng / 2) - 1;

            int c1 = fmax(1, m - BackWinSize_teta / 2) - 1;
            int c2 = fmax(1, m - GuardWinSize_teta / 2) - 1;
            int c3 = fmin(M, m + GuardWinSize_teta / 2) - 1;
            int c4 = fmin(M, m + BackWinSize_teta / 2) - 1;

            double B1 = IntI[r4][c4] - IntI[r4][c1] - IntI[r1][c4] + IntI[r1][c1];
            double B2 = IntI[r3][c3] - IntI[r3][c2] - IntI[r2][c3] + IntI[r2][c2];

            int len1 = (r4 - r1 + 1) * (c4 - c1 + 1) - (c4 - c1 - 1) - (r4 - r1);
            int len2 = (r3 - r2 + 1) * (c3 - c2 + 1) - (c3 - c2 - 1) - (r3 - r2);

            double refWin = B1 - B2;

            int len = len1 - len2;
            double thRayleigh = sqrt((pow(Pfa, -1.0 / len) - 1) * refWin);

            if (img[n][m] > thRayleigh) {
                BinaryMap[n][m] = 1;
            } else {
                BinaryMap[n][m] = 0;
            }
        }
    }
}

// Example of usage
int main() {
    double img[N][M];     // Initialize your image data here
    int BinaryMap[N][M];

    // Initialize img with your data
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            img[i][j] = /* Your image data */;
        }
    }

    CalcBinary(img, BinaryMap);

    // Output or process BinaryMap
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%d ", BinaryMap[i][j]);
        }
        printf("\n");
    }

    return 0;
}
