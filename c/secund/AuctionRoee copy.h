#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Utility function to find the index of the max value in an array
int max_index(double *array, int length) {
    int index = 0;
    for (int i = 1; i < length; i++) {
        if (array[i] > array[index]) {
            index = i;
        }
    }
    return index;
}

// Utility function to count non-zero elements
int nonzero(int *array, int length) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (array[i] != -1) {
            count++;
        }
    }
    return count;
}

// Utility function to calculate the sum of an array
int sum(int *array, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += array[i];
    }
    return sum;
}

// Utility function to find indices of zero elements
void find(int *array, int length, int *indices) {
    int index = 0;
    for (int i = 0; i < length; i++) {
        if (array[i] == -1) {
            indices[index++] = i;
        }
    }
}

void AuctionRoee(double *assign_mtrx, int N, int M, int *Assigned) {
    // Variables Definitions
    int k = (N > M) ? N : M;
    double eps = 1.0 / fmax(N, M);
    double A[N * M]; // Flattened cost matrix
    double Price[k]; // Price vector
    int Assigned_indx[(N < M) ? N : M]; // Indicates assigned plots
    int i1, i2;
    
    // Initialize arrays
    for (int i = 0; i < N * M; i++) {
        A[i] = -assign_mtrx[i]; // A = -assign_mtrx for auction maximization
    }

    for (int i = 0; i < k; i++) {
        Assigned[i] = -1; // All tracks initially unassigned
        Price[i] = 0;    // Initial price vector is zero
    }

    for (int i = 0; i < ((N < M) ? N : M); i++) {
        Assigned_indx[i] = -1; // Initially all plots unassigned
    }

    // Case of more tracks than plots
    if (N > M) {
        while (sum(Assigned_indx, M) != M) {
            int zz = M - nonzero(Assigned_indx, M);
            int indices[zz]; 
            find(Assigned_indx, M, indices); // Find unassigned plots

            for (int q = 0; q < zz; q++) {
                int m = indices[q];

                // Assignment values for current column
                double assignment_val[N];
                for (int i = 0; i < N; i++) {
                    assignment_val[i] = A[i * M + m] - Price[i];
                }

                i1 = max_index(assignment_val, N);

                if (Assigned[i1] != -1) {
                    Assigned_indx[(int)Assigned[i1] - 1] = 0; // Unassign previous
                }

                Assigned[i1] = m + 1; // Assign plot m to track i1
                Assigned_indx[m] = 1; // Mark as assigned

                assignment_val[i1] = 100 * assignment_val[i1]; // Find next best bid
                i2 = max_index(assignment_val, N);

                double delta1 = A[i2 * M + m] - Price[i2];
                double delta2 = A[i1 * M + m] - Price[i1];
                double delta = delta2 - delta1;

                Price[i1] = Price[i1] + delta + eps; // Update price of track
            }
        }
    } else { 
        // Case of more plots than tracks
        while (sum(Assigned_indx, N) != N) {
            int zz = N - nonzero(Assigned_indx, N);
            int indices[zz];
            find(Assigned_indx, N, indices); // Find unassigned tracks

            for (int q = 0; q < zz; q++) {
                int n = indices[q];

                // Assignment values for current row
                double assignment_val[M];
                for (int i = 0; i < M; i++) {
                    assignment_val[i] = A[n * M + i] - Price[i];
                }

                i1 = max_index(assignment_val, M);

                if (Assigned[i1] != -1) {
                    Assigned_indx[(int)Assigned[i1] - 1] = 0; // Unassign previous
                }

                Assigned[i1] = n + 1; // Assign track n to plot i1
                Assigned_indx[n] = 1; // Mark as assigned

                assignment_val[i1] = 100 * assignment_val[i1]; // Find next best bid
                i2 = max_index(assignment_val, M);

                double delta1 = A[n * M + i2] - Price[i2];
                double delta2 = A[n * M + i1] - Price[i1];
                double delta = delta2 - delta1;

                Price[i1] = Price[i1] + delta + eps; // Update price of track
            }
        }
    }
}
