#ifndef TRANSPOSE_H
#define TRANSPOSE_H
void transpose(double *A, double *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + j * rows + i) = *(A + i * cols + j);
        }
    }
}
#endif