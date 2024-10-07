
#include <complex.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HELPER_FUNCTION_H
#define HELPER_FUNCTION_H

#define PI 3.14159265358979323846

/*
double* linspace(double start,double end, double naz){
    double azBeams[NAZ];
    double step = (end - start) / (NAZ - 1);

    for (int i = 0; i < NAZ; i++) {
        azBeams[i] = start + i * step;
    }
}

void linspace(double start, double end, int num, double* result) {
    double step = (end - start) / (num - 1);
    for (int i = 0; i < num; i++) {
        result[i] = start + i * step;
    }
}
*/

double *linspace(double start, double end, int num) {
    double *result = (double *)malloc(num * sizeof(double));

    double step = (end - start) / (num - 1);
    for (int i = 0; i < num; i++) {
        result[i] = start + i * step;
    }
    return result;
}

// Function definition
double *arrayMaker(double from, double inJumpOf, double to, int size) {
    // Step size must be positive
    if (inJumpOf <= 0) {
        return NULL;
    }

    // MOVE OUT SIDE FOR: need to know the size for raed
    //  Calculate the number of elements in the array
    // int size = (int)((to - from) / inJumpOf) + 1;

    // Allocate memory for the array
    double *array = (double *)malloc(size * sizeof(double));
    if (array == NULL) {
        return NULL;
    }

    // Fill the array
    for (int i = 0; i < size; i++) {
        array[i] = from + i * inJumpOf;
    }

    return array;
}

double *compute_lfm(double *t_samp, int num_samples, double fmin, double fmax, double t_p) {
    double *lfm = (double *)malloc(num_samples * sizeof(double));

    double freq_slope = (fmax - fmin) / (2 * t_p);  // Frequency slope

    for (int i = 0; i < num_samples; i++) {
        double t = t_samp[i];
        lfm[i] = cos(2 * PI * (freq_slope * t * t + fmin * t));
    }
    return lfm;
}
/*
void generate_lfm(double *lfm_signal, double *t_samp, int num_samples, double fmin, double fmax, double t_p) {
    for (int i = 0; i < num_samples; i++) {
        lfm_signal[i] = cos(2 * M_PI * ((fmax - fmin) / (2 * t_p) * t_samp[i] * t_samp[i] + fmin * t_samp[i]));
    }
}


void create_matched_filter(double *matched_filter, double *lfm_signal, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        matched_filter[i] = conj(lfm_signal[num_samples - 1 - i]);
    }
}
*/

double *create_matched_filter(double *lfm, int num_samples) {
    double *matched_filter = (double *)malloc(num_samples * sizeof(double));

    for (int i = 0; i < num_samples; i++) {
        matched_filter[i] = lfm[num_samples - i - 1];  // Conjugate of real cosine is the same value
    }
    return matched_filter;
}

int find(double *arr, int size, int positive, double filter, double *result) {
    int valid_pos = 0;
    for (int i = 0; i < size; i++) {
        if ((positive == 1) ? arr[i] == filter : arr[i] != filter) {
            result[valid_pos++] = i;
        }
    }
    return valid_pos;
}

#endif  // HELPER_FUNCTION_H