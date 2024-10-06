#include <math.h>
#include <stdio.h>
#include "matrix_mult.h"
#include "transpose.h"

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

/*
void matrix_mult(double *A, double *B, double *result, int m, int n, int p) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            *(result + i * p + j) = 0;
            for (int k = 0; k < n; k++) {
                *(result + i * p + j) += (*(A + i * n + k)) * (*(B + k * p + j));
            }
        }
    }
}
*/

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

/*
void transpose(double *A, double *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + j * rows + i) = *(A + i * cols + j);
        }
    }
}
*/

// Function to calculate the std_dis (returns double)
double std_calcRoee(int ping_ind, double Tping, double *yc, double *Rc, int last_ping_number, double *X, double *P, int n) {
    double sigmaV = 1e-4;
    double Vx_own = 0;
    double Vy_own = 0;

    // Time calculation
    double T = (ping_ind - last_ping_number) * Tping;

    // Process model matrices
    double G[8] = {T * T / 2, 0, 0, T * T / 2, T, 0, 0, T};

    // Q = sigmaV^2 * G * G';
    double Q[16] = {0};

    double G_trans[8];
    transpose((double *)G, (double *)G_trans, 4, 2);  // H'

    // for (int i = 0; i < 16; i++) Q[i] = 0;
    mat_mult(G, G_trans, Q, 4, 2, 4);
    for (int i = 0; i < 16; i++) Q[i] *= sigmaV * sigmaV;

    double F[16] = {1, 0, T, 0, 0, 1, 0, T, 0, 0, 1, 0, 0, 0, 0, 1};
    double H[8] = {1, 0, 0, 0, 0, 1, 0, 0};

    // Prediction step
    double Xp[4];
    mat_mult(F, X, Xp, 4, 4, 1);
    Xp[0] -= Vx_own * T;
    Xp[1] -= Vy_own * T;

    /*
        // Pp = F * P * F' + Q;
        // Covariance update
        double Ft[16];
        mat_transpose(F, Ft, 4, 4);
        double Pp[16];
        mat_mult(F, P, Pp, 4, 4, 4);
        mat_mult(Pp, Ft, Pp, 4, 4, 4);
        for (int i = 0; i < 16; i++) Pp[i] += Q[i];
    */
    // Covariance prediction: Pp = F * P * F' + Q
    double F_trans[4 * 4];
    double temp[4 * 4];
    double Pp[16];

    transpose((double *)F, (double *)F_trans, 4, 4);                        // F'
    matrix_mult((double *)F, (double *)P, (double *)temp, 4, 4, 4);         // F * P
    matrix_mult((double *)temp, (double *)F_trans, (double *)Pp, 4, 4, 4);  // Pp = F * P * F'
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Pp[i * 4 + j] += Q[i * 4 + j];  // Pp += Q
        }
    }
    /*
        // Innovation covariance
        double Ht[8];
        mat_transpose(H, Ht, 2, 4);
        double S[4];
        mat_mult(H, Pp, S, 2, 4, 2);
        mat_mult(S, Ht, S, 2, 4, 2);
        for (int i = 0; i < 4; i++) S[i] += Rc[i];
    */
    // S = H * Pp * H' + Rc
    double H_trans[4][2];
    double Stemp[4][2];
    double S[4];

    transpose((double *)H, (double *)H_trans, 2, 4);                        // H'
    matrix_mult((double *)H, (double *)Pp, (double *)Stemp, 2, 4, 4);       // H * Pp
    matrix_mult((double *)Stemp, (double *)H_trans, (double *)S, 2, 4, 2);  // S = H * Pp * H'
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            S[i * 2 + j] += Rc[i * 2 + j];  // S += Rc
        }
    }

    // Innovation
    double yp[2] = {Xp[0], Xp[1]};

    // std_dis = (yc - yp)' * inv(S) * (yc - yp);

    // Calculate std_dis
    double S_inv[4];
    mat_inv_2x2(S, S_inv);

    double diff[2] = {yc[0] - yp[0], yc[1] - yp[1]};

    // double diff_trans[2];

    double temp6[2];
    mat_mult(diff, S_inv, temp6, 1, 2, 2);
    double std_dis = temp6[0] * diff[0] + temp6[1] * diff[1];

    return std_dis;
}
