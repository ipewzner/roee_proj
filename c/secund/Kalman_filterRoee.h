#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897

// Function to perform matrix multiplication using pointers
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

// Function to perform matrix transpose using pointers
void transpose(double *A, double *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + j * rows + i) = *(A + i * cols + j);
        }
    }
}

// Function to perform matrix inversion (stub, needs real implementation for 2x2)
void invert_2x2(double *A, double *result) {
    double det = *(A) * *(A + 3) - *(A + 1) * *(A + 2); // Determinant of 2x2 matrix
    if (det == 0) {
        printf("Matrix is singular and cannot be inverted\n");
        return;
    }
    double inv_det = 1.0 / det;
    *(result) = *(A + 3) * inv_det;
    *(result + 1) = -*(A + 1) * inv_det;
    *(result + 2) = -*(A + 2) * inv_det;
    *(result + 3) = *(A) * inv_det;
}

// Function to perform scalar-matrix multiplication
void scalar_matrix_mult(double scalar, double *matrix, double *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + i * cols + j) = scalar * (*(matrix + i * cols + j));
        }
    }
}

// Kalman filter function in C using pointers
void Kalman_filterRoee(double *X_upd, double *P_upd, double *teta_rad, double *yc, double *Rc, double T, double *X, double *P) {
    // Sensors parameters
    double Vx_own = 0;
    double Vy_own = 0;
    double sigmaV = 1e-4;

    // Process model matrices
    double G[4][2] = {{T * T / 2, 0}, {0, T * T / 2}, {T, 0}, {0, T}};
    double Q[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    double F[4][4] = {{1, 0, T, 0}, {0, 1, 0, T}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    double H[2][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}};
    double Xp[4] = {0,0,0,0};
    double Pp[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    double S[2][2] = {{0,0},{0,0}};
    double K[4][2] = {{0,0},{0,0},{0,0},{0,0}};

    // Q = sigmaV^2 * G * G'
    double G_trans[2][4]= {{0, 0, 0, 0}, {0,0, 0, 0}};
    transpose((double *)G, (double *)G_trans, 4, 2); // G'
    scalar_matrix_mult(sigmaV * sigmaV, (double *)G, (double *)G, 4, 2); // sigmaV^2 * G
    matrix_mult((double *)G, (double *)G_trans, (double *)Q, 4, 2, 4);  // Q = G * G'

    // State prediction: Xp = F * X
    for (int i = 0; i < 4; i++) {
        Xp[i] = 0;
        for (int j = 0; j < 4; j++) {
            Xp[i] += F[i][j] * X[j];
        }
    }

    // Ownship velocity compensation
    Xp[0] -= Vx_own * T;
    Xp[1] -= Vy_own * T;

    // Covariance prediction: Pp = F * P * F' + Q
    double F_trans[4][4]= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};;
    double temp[4][4]= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};;
    transpose((double *)F, (double *)F_trans, 4, 4); // F'
    matrix_mult((double *)F, (double *)P, (double *)temp, 4, 4, 4); // F * P
    matrix_mult((double *)temp, (double *)F_trans, (double *)Pp, 4, 4, 4); // Pp = F * P * F'
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Pp[i][j] += Q[i][j]; // Pp += Q
        }
    }

    // S = H * Pp * H' + Rc
    double H_trans[4][2]= {{0,0},{0,0},{0,0},{0,0}};
    double Stemp[4][2]= {{0,0},{0,0},{0,0},{0,0}};
    transpose((double *)H, (double *)H_trans, 2, 4); // H'
    matrix_mult((double *)H, (double *)Pp, (double *)Stemp, 2, 4, 4); // H * Pp
    matrix_mult((double *)Stemp, (double *)H_trans, (double *)S, 2, 4, 2); // S = H * Pp * H'
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            S[i][j] += Rc[i * 2 + j]; // S += Rc
        }
    }

    // K = Pp * H' * inv(S)
    double S_inv[2][2]= {{0,0},{0,0}};
    double temp2[4][2]= {{0,0},{0,0},{0,0},{0,0}};
    invert_2x2((double *)S, (double *)S_inv); // inv(S)
    matrix_mult((double *)Pp, (double *)H_trans, (double *)temp2, 4, 4, 2); // Pp * H'
    matrix_mult((double *)temp2, (double *)S_inv, (double *)K, 4, 2, 2); // K = Pp * H' * inv(S)

    // Update state: X_upd = Xp + K * (yc - H * Xp)
    double HXp[2] = {0,0}; // H * Xp
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            HXp[i] += H[i][j] * Xp[j];
        }
    }
    double yc_minus_HXp[2] = {yc[0] - HXp[0], yc[1] - HXp[1]}; // yc - H * Xp
    for (int i = 0; i < 4; i++) {
        X_upd[i] = Xp[i];
        for (int j = 0; j < 2; j++) {
            X_upd[i] += K[i][j] * yc_minus_HXp[j]; // X_upd = Xp + K * (yc - H * Xp)
        }
    }

    printf("X_upd: %f, %f, %f, %f\n", X_upd[0], X_upd[1], X_upd[2], X_upd[3]);

    // Update covariance: P_upd = Pp - K * S * K'
    double KS[4][2]= {{0,0},{0,0},{0,0},{0,0}};
    double K_trans[4][2]= {{0,0},{0,0},{0,0},{0,0}};
    transpose((double *)K, (double *)K_trans, 4, 2); // H'

    matrix_mult((double *)K, (double *)S, (double *)KS, 4, 2, 2); // K * S
    matrix_mult((double *)KS, (double *)K_trans, (double *)P_upd, 4, 2, 4); // K * S * K'
   
   /*
    printf("P_upd: %f, %f, %f, %f\n", P_upd[0], P_upd[1], P_upd[2], P_upd[3]);
    printf("P_upd: %f, %f, %f, %f\n", P_upd[4], P_upd[5], P_upd[6], P_upd[7]);
    printf("P_upd: %f, %f, %f, %f\n", P_upd[8], P_upd[9], P_upd[10], P_upd[11]);
    printf("P_upd: %f, %f, %f, %f\n", P_upd[12], P_upd[13], P_upd[14], P_upd[15]);
   */  
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            P_upd[i * 4 + j] = Pp[i][j] - P_upd[i * 4 + j]; // P_upd = Pp - K * S * K'
        }
    }

    
    printf("P_upd: %f, %f, %f, %f\n", P_upd[0], P_upd[1], P_upd[2], P_upd[3]);
    printf("P_upd: %f, %f, %f, %f\n", P_upd[4], P_upd[5], P_upd[6], P_upd[7]);
    printf("P_upd: %f, %f, %f, %f\n", P_upd[8], P_upd[9], P_upd[10], P_upd[11]);
    printf("P_upd: %f, %f, %f, %f\n", P_upd[12], P_upd[13], P_upd[14], P_upd[15]);
    


    // Extract smooth azimuth angle: teta_rad = atan(...)
    if (X_upd[1] >= 0 && X_upd[0] >= 0) {
        *teta_rad = atan(fabs(X_upd[0] / X_upd[1]));
    } 
     if (X_upd[1] < 0 && X_upd[0] >= 0) {
        double rrr1=fabs(X_upd[0] / X_upd[1]);
        double rrr=atan(fabs(X_upd[0] / X_upd[1]));
        *teta_rad = PI - atan(fabs(X_upd[0] / X_upd[1]));
    }  
    if (X_upd[1] < 0 && X_upd[0] < 0) {
        *teta_rad = PI + atan(fabs(X_upd[0] / X_upd[1]));
    } 
     if (X_upd[1] >= 0 && X_upd[0] < 0) {
        *teta_rad = 2 * PI - atan(fabs(X_upd[0] / X_upd[1]));
    }
}
