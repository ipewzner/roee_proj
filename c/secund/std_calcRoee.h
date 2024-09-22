#include <stdio.h>
#include <math.h>

// Function for matrix multiplication
void mat_mult(double *a, double *b, double *result, int m, int n, int p) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            result[i * p + j] = 0;
            for (int k = 0; k < n; k++) {
                result[i * p + j] += a[i * n + k] * b[k * p + j];
            }
        }
    }
}

// Function for matrix transpose
void mat_transpose(double *a, double *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j * rows + i] = a[i * cols + j];
        }
    }
}

// Function for matrix inversion (2x2 matrix)
void mat_inv_2x2(double *a, double *result) {
    double det = a[0] * a[3] - a[1] * a[2];
    result[0] = a[3] / det;
    result[1] = -a[1] / det;
    result[2] = -a[2] / det;
    result[3] = a[0] / det;
}

// Function to calculate the std_dis (returns double)
double std_calcRoee(int ping_ind, double Tping, double *yc, double *Rc, int last_ping_number, double *X, double *P, int n) {
    double sigmaV = 1e-4;
    double Vx_own = 0;
    double Vy_own = 0;

    // Time calculation
    double T = (ping_ind - last_ping_number) * Tping;

    // Process model matrices
    double G[8] = {T * T / 2, 0, 0, T * T / 2, T, 0, 0, T};
    double Q[16];
    for (int i = 0; i < 16; i++) Q[i] = 0;
    mat_mult(G, G, Q, 4, 2, 4);
    for (int i = 0; i < 16; i++) Q[i] *= sigmaV * sigmaV;

    double F[16] = {1, 0, T, 0, 0, 1, 0, T, 0, 0, 1, 0, 0, 0, 0, 1};
    double H[8] = {1, 0, 0, 0, 0, 1, 0, 0};

    // Prediction step
    double Xp[4];
    mat_mult(F, X, Xp, 4, 4, 1);
    Xp[0] -= Vx_own * T;
    Xp[1] -= Vy_own * T;

    // Covariance update
    double Ft[16];
    mat_transpose(F, Ft, 4, 4);
    double Pp[16];
    mat_mult(F, P, Pp, 4, 4, 4);
    mat_mult(Pp, Ft, Pp, 4, 4, 4);
    for (int i = 0; i < 16; i++) Pp[i] += Q[i];

    // Innovation covariance
    double Ht[8];
    mat_transpose(H, Ht, 2, 4);
    double S[4];
    mat_mult(H, Pp, S, 2, 4, 2);
    mat_mult(S, Ht, S, 2, 4, 2);
    for (int i = 0; i < 4; i++) S[i] += Rc[i];

    // Innovation
    double yp[2] = {Xp[0], Xp[1]};

    // Calculate std_dis
    double S_inv[4];
    mat_inv_2x2(S, S_inv);

    double diff[2] = {yc[0] - yp[0], yc[1] - yp[1]};
    double temp[2];
    mat_mult(diff, S_inv, temp, 1, 2, 2);
    double std_dis = temp[0] * diff[0] + temp[1] * diff[1];

    return std_dis;
}
