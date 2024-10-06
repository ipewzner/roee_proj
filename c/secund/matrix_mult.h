#ifndef MATRIX_MULT_H
#define MATRIX_MULT_H
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


#endif