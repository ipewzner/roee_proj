#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

// Function to find the max value and its index in an array
void find_max(double *array, int length, double *max_value, int *max_index) {
    *max_value = array[0];
    *max_index = 0;
    for (int i = 1; i < length; i++) {
        if (array[i] > *max_value) {
            *max_value = array[i];
            *max_index = i;
        }
    }
}
// Helper function to calculate the sum of an array
int sum(int *array, int length) {
    int total = 0;
    for (int i = 0; i < length; i++) {
        total += array[i];
    }
    return total;
}

// Helper function to find indices of unassigned elements
int* find_indices(int *array, int length, int zz) {
    int *indices = (int *)malloc(zz * sizeof(int));
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (array[i] < 1) {
            indices[count++] = i;
        }
    }
    return indices;
}


// Main auction algorithm function in C
void AuctionRoee(double *assign_mtrx, int N, int M, int *Assigned) {
    // A = -assign_mtrx
    double *A = (double *)malloc(N * M * sizeof(double));
    for (int i = 0; i < N * M; i++) {
        A[i] = -assign_mtrx[i];
    }

    double eps = 1.0 / max(N, M);
    
    // Initialize Assigned and Price
    int k = max(N, M);
    double *Price = (double *)calloc(k, sizeof(double));
    int *Assigned_indx = (int *)calloc(min(N, M), sizeof(int));

    if (N > M) {
        // Case of more tracks than plots
        while (sum(Assigned_indx, M) != M) {
            int zz = M - sum(Assigned_indx, M);
            int *indices = find_indices(Assigned_indx, M, zz); // Helper to find unassigned elements
            for (int q = 0; q < zz; q++) {
                int m = indices[q];
                
                // assinment_val = A(:, m)' - Price
                double *assignment_val = (double *)malloc(N * sizeof(double));
                for (int i = 0; i < N; i++) {
                    assignment_val[i] = A[i * M + m] - Price[i];
                }

                double max_val1;
                int i1;
                find_max(assignment_val, N, &max_val1, &i1);

                if (Assigned[i1] != 0) {
                    Assigned_indx[Assigned[i1]] = 0;
                }

                Assigned[i1] = m;
                Assigned_indx[m] = 1;

                assignment_val[i1] *= 100; // finding the next best "bid"
                double max_val2;
                int i2;
                find_max(assignment_val, N, &max_val2, &i2);

                double delta1 = A[i2 * M + m] - Price[i2];
                double delta2 = A[i1 * M + m] - Price[i1];
                double delta = delta2 - delta1;

                Price[i1] += delta + eps;

                free(assignment_val);
            }
            free(indices);
        }
    } else {
        // Case of more plots than tracks
        while (sum(Assigned_indx, N) != N) {
            int zz = N - sum(Assigned_indx, N);
            int *indices = find_indices(Assigned_indx, N, zz);
            for (int q = 0; q < zz; q++) {
                int n = indices[q];

                // assignment_val = A(n, :) - Price
                double *assignment_val = (double *)malloc(M * sizeof(double));
                for (int j = 0; j < M; j++) {
                    assignment_val[j] = A[n * M + j] - Price[j];
                }

                double max_val1;
                int i1;
                find_max(assignment_val, M, &max_val1, &i1);

                if (Assigned[i1] != 0) {
                    Assigned_indx[Assigned[i1]] = 0;
                }

                Assigned[i1] = n;
                Assigned_indx[n] = 1;

                assignment_val[i1] *= 100; // finding the next best "bid"
                double max_val2;
                int i2;
                find_max(assignment_val, M, &max_val2, &i2);

                double delta1 = A[n * M + i2] - Price[i2];
                double delta2 = A[n * M + i1] - Price[i1];
                double delta = delta2 - delta1;

                Price[i1] += delta + eps;

                free(assignment_val);
            }
            free(indices);
        }
    }

    free(A);
    free(Price);
    free(Assigned_indx);
}

