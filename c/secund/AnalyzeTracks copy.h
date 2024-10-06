#include <math.h>
#include <stddef.h>  // for NULL

// #include "cov_plot_calcRoee.h"
#include "ping_to_ping_associationRoee.h"
#include "ping_to_ping_correlationRoee.h"
#define MAX_TARGET 360
#define NUM_OF_PINGS 15
void update_tracks(double* TracksMat, double* TracksMissMat, int rows_mat, int cols_mat, int dim1_miss, int dim2_miss, int dim3_miss, int Win_dlt, int Dlt_th) {
    for (int kk = 0; kk < cols_mat; kk++) {
        if (TracksMat[kk] == 0) {  // TracksMat(1, :) == 0 in MATLAB
            // Find the length of non-999 values in TracksMissMat(1, undeleted_tracks_ind(kk), :)
            int Len = 0;
            for (int i = 0; i < dim3_miss; i++) {
                if (TracksMissMat[kk * dim3_miss + i] != 999) {
                    Len++;
                } else {
                    break;
                }
            }

            // Proceed if length > Win_dlt
            if (Len > Win_dlt) {
                int num_miss = 0;
                // Calculate the difference and count the number of times the difference is > 0
                for (int i = Len - Win_dlt; i < Len - 1; i++) {
                    int diff = TracksMissMat[kk * dim3_miss + i + 1] - TracksMissMat[kk * dim3_miss + i];
                    if (diff > 0) {
                        num_miss++;
                    }
                }

                // If the number of misses is greater than Dlt_th, mark the track for deletion
                if (num_miss > Dlt_th) {
                    TracksMat[kk] = 1;  // TracksMat(1, undeleted_tracks_ind(kk)) = 1
                }
            }
        }
    }
}

void clean_tracks(
    double* TracksMat, double* TracksX, double* TracksP, double* TracksVecMat,
    double* TracksMissMat, double* TracksDataBinMat, double* TracksDataMat,
    int rows_mat, int cols_mat, int rows_x, int rows_p) {
    // Initialize new arrays
    double* TracksNewP = (double*)malloc(MAX_TARGET * 16 * sizeof(double));                      // 4x4 flattened to 16
    double* TracksNewX = (double*)malloc(4 * MAX_TARGET * sizeof(double));                       // 4xMAX_TARGET
    double* TracksNewMat = (double*)malloc(3 * MAX_TARGET * sizeof(double));                     // 3xMAX_TARGET
    double* TracksVecNewMat = (double*)malloc(4 * MAX_TARGET * sizeof(double));                  // 4xMAX_TARGET
    double* TracksNewMissMat = (double*)malloc(5 * MAX_TARGET * NUM_OF_PINGS * sizeof(double));  // 5xMAX_TARGETxNUM_OF_PINGS
    double* TracksNewDataBinMat = (double*)calloc(MAX_TARGET * MAX_TARGET, sizeof(double));      // binary matrix
    double* TracksNewDataMat = (double*)malloc(MAX_TARGET * MAX_TARGET * sizeof(double));        // 999 initialized

    // Initialize new arrays with default values
    for (int i = 0; i < MAX_TARGET * 16; i++) {
        TracksNewP[i] = 999.0;
    }
    for (int i = 0; i < 4 * MAX_TARGET; i++) {
        TracksNewX[i] = 999.0;
        TracksVecNewMat[i] = 999.0;
    }
    for (int i = 0; i < 3 * MAX_TARGET; i++) {
        TracksNewMat[i] = 999.0;
    }
    for (int i = 0; i < 5 * MAX_TARGET * NUM_OF_PINGS; i++) {
        TracksNewMissMat[i] = 999.0;
    }
    for (int i = 0; i < MAX_TARGET * MAX_TARGET; i++) {
        TracksNewDataMat[i] = 999.0;
    }

    // Step 1: Find undeleted tracks (TracksMat(1, :) == 0)
    int undeleted_tracks_count = 0;
    int* undeleted_tracks_ind = (int*)malloc(cols_mat * sizeof(int));

    for (int i = 0; i < cols_mat; i++) {
        if (TracksMat[i] == 0) {  // TracksMat[0, i] is represented by i in the flattened array
            undeleted_tracks_ind[undeleted_tracks_count] = i;
            undeleted_tracks_count++;
        }
    }

    // Step 2: Copy data for undeleted tracks into the new structures
    for (int i = 0; i < undeleted_tracks_count; i++) {
        int idx = undeleted_tracks_ind[i];

        // Copy TracksMat
        for (int j = 0; j < 3; j++) {
            TracksNewMat[j * MAX_TARGET + i] = TracksMat[j * cols_mat + idx];
        }

        // Copy TracksX
        for (int j = 0; j < 4; j++) {
            TracksNewX[j * MAX_TARGET + i] = TracksX[j * cols_mat + idx];
        }

        // Copy TracksP (4x4 matrix for each target)
        for (int j = 0; j < 16; j++) {
            TracksNewP[i * 16 + j] = TracksP[idx * 16 + j];
        }

        // Copy TracksVecMat
        for (int j = 0; j < 4; j++) {
            TracksVecNewMat[j * MAX_TARGET + i] = TracksVecMat[j * cols_mat + idx];
        }

        // Copy TracksMissMat
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < NUM_OF_PINGS; k++) {
                TracksNewMissMat[j * MAX_TARGET * NUM_OF_PINGS + i * NUM_OF_PINGS + k] = TracksMissMat[j * MAX_TARGET * NUM_OF_PINGS + idx * NUM_OF_PINGS + k];
            }
        }

        // Copy TracksDataBinMat
        for (int j = 0; j < MAX_TARGET; j++) {
            TracksNewDataBinMat[i * MAX_TARGET + j] = TracksDataBinMat[idx * MAX_TARGET + j];
        }

        // Copy TracksDataMat
        for (int j = 0; j < MAX_TARGET; j++) {
            TracksNewDataMat[i * MAX_TARGET + j] = TracksDataMat[idx * MAX_TARGET + j];
        }
    }

    // Step 3: Replace old structures with new structures
    memcpy(TracksMat, TracksNewMat, 3 * MAX_TARGET * sizeof(double));
    memcpy(TracksX, TracksNewX, 4 * MAX_TARGET * sizeof(double));
    memcpy(TracksP, TracksNewP, MAX_TARGET * 16 * sizeof(double));
    memcpy(TracksVecMat, TracksVecNewMat, 4 * MAX_TARGET * sizeof(double));
    memcpy(TracksMissMat, TracksNewMissMat, 5 * MAX_TARGET * NUM_OF_PINGS * sizeof(double));
    memcpy(TracksDataBinMat, TracksNewDataBinMat, MAX_TARGET * MAX_TARGET * sizeof(double));
    memcpy(TracksDataMat, TracksNewDataMat, MAX_TARGET * MAX_TARGET * sizeof(double));

    // Clean up
    free(undeleted_tracks_ind);
    free(TracksNewP);
    free(TracksNewX);
    free(TracksNewMat);
    free(TracksVecNewMat);
    free(TracksNewMissMat);
    free(TracksNewDataBinMat);
    free(TracksNewDataMat);
}
/*
void initialize_tracks(double* TracksMat, double* TracksX, double* TracksVecMat, double* MergedYc, double* MergedRng, double* MergedTeta, int NumDetect, double ping_number) {
    // Initialize TracksMat (flattened)
    for (int i = 0; i < NumDetect; i++) {
        TracksMat[i] = 0;            // TracksMat(1, 1: NumDetect) = zeros(1, NumDetect);
        TracksMat[360 + i] = 0;      // TracksMat(2, 1: NumDetect) = zeros(1, NumDetect);
        TracksMat[2 * 360 + i] = 0;  // TracksMat(3, 1: NumDetect) = zeros(1, NumDetect);

        TracksX[i] = MergedYc[i];                    // TracksX(1, 1: NumDetect) = MergedYc(1, :);
        TracksX[360 + i] = MergedYc[NumDetect + i];  // TracksX(2, 1: NumDetect) = MergedYc(2, :);
        TracksX[2 * 360 + i] = 0;                    // TracksX(3, 1: NumDetect) = zeros(1, NumDetect);
        TracksX[3 * 360 + i] = 0;                    // TracksX(4, 1: NumDetect) = zeros(1, NumDetect);

        TracksVecMat[i] = 1;                        // TracksVecMat(1, 1: NumDetect) = ones(1, NumDetect);
        TracksVecMat[360 + i] = MergedRng[i];       // TracksVecMat(2, 1: NumDetect) = MergedRng;
        TracksVecMat[2 * 360 + i] = MergedTeta[i];  // TracksVecMat(3, 1: NumDetect) = MergedTeta;
        TracksVecMat[4 * 360 + i] = ping_number;    // TracksVecMat(4, 1: NumDetect) = ping_number * ones(1, NumDetect);
    }
    printf("");
}
*/


void update_tracks2(
    double* MergedRng, double* MergedTeta, double sigmaTeta, int NumDetect, int CurrentTargetInd,
    double cov_fact, double xmax, double Win_dlt, double Tping, double* TracksP, double* TracksMissMat,
    double* MergedYc, int ping_number) {
    double Rc[4];  // To store the 2x2 covariance matrix

    for (int MergeInd = 0; MergeInd < NumDetect; MergeInd++) {
        // Calculate the covariance matrix Rc
        cov_plot_calcRoee(MergedRng[MergeInd], MergedTeta[MergeInd] * PI / 180, sigmaTeta, Rc);

        // Construct the 4x4 matrix and store it in TracksP (flattened array)
        int idx = (CurrentTargetInd + MergeInd) * 16;  // TracksP is 4x4 flattened, index for CurrentTargetInd+MergeInd
        TracksP[idx + 0] = cov_fact * Rc[0];           // Rc(1,1)
        TracksP[idx + 1] = cov_fact * Rc[1];           // Rc(1,2)
        TracksP[idx + 2] = 0;
        TracksP[idx + 3] = 0;

        TracksP[idx + 4] = cov_fact * Rc[1];  // Rc(1,2)
        TracksP[idx + 5] = cov_fact * Rc[3];  // Rc(2,2)
        TracksP[idx + 6] = 0;
        TracksP[idx + 7] = 0;

        double factor = (xmax / (Win_dlt * Tping)) * (xmax / (Win_dlt * Tping));

        TracksP[idx + 8] = 0;
        TracksP[idx + 9] = 0;
        TracksP[idx + 10] = cov_fact * factor;  // (xmax / (Win_dlt * Tping))^2
        TracksP[idx + 11] = 0;

        TracksP[idx + 12] = 0;
        TracksP[idx + 13] = 0;
        TracksP[idx + 14] = 0;
        TracksP[idx + 15] = cov_fact * factor;  // (xmax / (Win_dlt * Tping))^2
    }

    // Update TracksMissMat
    for (int i = 0; i < NumDetect; i++) {
        int target_idx = CurrentTargetInd + i;

        // TracksMissMat is flattened, update specific slices
        TracksMissMat[0 * NumDetect + target_idx] = 0;               // TracksMissMat(1, CurrentTargetInd+1: CurrentTargetInd+NumDetect, ping_number) = zeros(1, NumDetect);
        TracksMissMat[1 * NumDetect + i] = 0;                        // TracksMissMat(2, 1: NumDetect, ping_number) = zeros(1, NumDetect);
        TracksMissMat[2 * NumDetect + i] = 0;                        // TracksMissMat(3, 1: NumDetect, ping_number) = zeros(1, NumDetect);
        TracksMissMat[3 * NumDetect + i] = MergedYc[i];              // TracksMissMat(4, 1: NumDetect, ping_number) = MergedYc(1, :);
        TracksMissMat[4 * NumDetect + i] = MergedYc[i + NumDetect];  // TracksMissMat(5, 1: NumDetect, ping_number) = MergedYc(2, :);
    }
}

// #define TEST1
#ifndef TEST1

double MergedRng[10];
double MergedTeta[10];
double MergedYc[10 * 2];
double TracksDataBinMat[360 * 360];
double TracksDataMat[360 * 360];
double TracksMissMat[5 * 360 * 15];
double TracksP[360 * 4 * 4];
double TracksVecMat[4 * 360];
double TracksX[4 * 360];
double TracksMat[3 * 360];
#endif
#ifdef TEST1
void AnalyzeTracks(
    double* TracksMat,
    double* TracksVecMat,
    double* TracksP,
    double* TracksX,
    double* TracksMissMat,
    double* TracksDataBinMat,
    double* TracksDataMat,
    double* MergedRng,
    double* MergedTeta,
    double* MergedYc,
    int ping_number,
    double Tping,
    double sigmaTeta,
    double Win_dlt,
    double Dlt_th,
    int CurrentTargetInd,
    int numDetect) {
#else
void AnalyzeTracks(
    double* TracksMatT,
    double* TracksVecMatT,
    double* TracksPT,
    double* TracksXT,
    double* TracksMissMatT,
    double* TracksDataBinMatT,
    double* TracksDataMatT,
    double* MergedRngT,
    double* MergedTetaT,
    double* MergedYcT,
    int ping_number,
    double Tping,
    double sigmaTeta,
    double Win_dlt,
    double Dlt_th,
    int CurrentTargetInd,
    int numDetect) {
    // Copy data from pointers with 'T' to the corresponding arrays
    memcpy(MergedRng, MergedRngT, numDetect * sizeof(double));
    memcpy(MergedTeta, MergedTetaT, numDetect * sizeof(double));
    memcpy(MergedYc, MergedYcT, numDetect * 2 * sizeof(double));
    memcpy(TracksDataBinMat, TracksDataBinMatT, 360 * 360 * sizeof(double));
    memcpy(TracksDataMat, TracksDataMatT, 360 * 360 * sizeof(double));
    memcpy(TracksMissMat, TracksMissMatT, 5 * 360 * 15 * sizeof(double));
    memcpy(TracksP, TracksPT, 360 * 4 * 4 * sizeof(double));
    //memcpy(TracksVecMat, TracksVecMatT, 4 * 360 * sizeof(double));
    //memcpy(TracksX, TracksXT, 4 * 360 * sizeof(double));
    //memcpy(TracksMat, TracksMatT, 3 * 360 * sizeof(double));

#endif
    const double cov_fact = 5;
    const double xmax = 10;
    int size_uncorr_plots_list = numDetect;

    if (ping_number > 0) {
        // Call ping_to_ping_correlationRoee

        double* uncorr_plots_list = (double*)malloc(size_uncorr_plots_list * sizeof(double));
        ping_to_ping_correlationRoee(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat,
                                     MergedRng, MergedTeta, MergedYc, numDetect, ping_number, Tping, sigmaTeta, 360, 360, uncorr_plots_list);

        // Call ping_to_ping_associationRoee
        ping_to_ping_associationRoee(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, MergedRng, MergedTeta, MergedYc, numDetect, ping_number, sigmaTeta, cov_fact, Win_dlt, xmax, Tping, CurrentTargetInd);

        // Initialize tracks
        int MergeInd;
        double Rc[4 * 4];

        // for (int MergeInd = 0; MergeInd < uncorr_plots_count; MergeInd++) {
        for (int MergeInd = 0; MergeInd < size_uncorr_plots_list; MergeInd++) {
            int plot_id = uncorr_plots_list[MergeInd];

            // Calculate Rc
            double Rc[2 * 2];
            int upl = uncorr_plots_list[MergeInd];
            int mr = MergedRng[upl];
            cov_plot_calcRoee(MergedRng[upl], MergedTeta[upl] * PI / 180, sigmaTeta, Rc);

            TracksP[(CurrentTargetInd + MergeInd) * 16] = cov_fact * Rc[0];
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 1] = cov_fact * Rc[1];
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 2] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 3] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 4] = cov_fact * Rc[1];
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 5] = cov_fact * Rc[3];
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 6] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 7] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 8] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 9] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 10] = cov_fact * pow(xmax / (Win_dlt * Tping), 2);
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 11] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 12] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 13] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 14] = 0;
            TracksP[(CurrentTargetInd + MergeInd) * 16 + 15] = cov_fact * pow(xmax / (Win_dlt * Tping), 2);
        }

        for (int j = CurrentTargetInd; j < (CurrentTargetInd + size_uncorr_plots_list); j++) {
            int upl = uncorr_plots_list[j - CurrentTargetInd];

            TracksX[0 * 360 + j] = MergedYc[0 * 13 + upl];
            TracksX[1 * 360 + j] = MergedYc[1 * 13 + upl];
            TracksX[2 * 360 + j] = 0;
            TracksX[3 * 360 + j] = 0;

            TracksVecMat[0 * 360 + j] = 1;
            TracksVecMat[1 * 360 + j] = MergedRng[upl];
            TracksVecMat[2 * 360 + j] = MergedTeta[upl];
            TracksVecMat[3 * 360 + j] = (ping_number + 1) * 1;

            TracksMat[0 * 360 + j] = 0;
            TracksMat[1 * 360 + j] = 0;
            TracksMat[2 * 360 + j] = 0;

            TracksMissMat[0 * 360 * 15 + j * 15 + ping_number] = 0;
            TracksMissMat[1 * 360 * 15 + j * 15 + ping_number] = 0;
            TracksMissMat[2 * 360 * 15 + j * 15 + ping_number] = 0;
            TracksMissMat[3 * 360 * 15 + j * 15 + ping_number] = MergedYc[0 * 13 + upl];
            TracksMissMat[4 * 360 * 15 + j * 15 + ping_number] = MergedYc[1 * 13 + upl];

            for (int i = 0; i < 360; i++) {
                TracksDataBinMat[j * 360 + i] = 0;
                TracksDataMat[j * 360 + i] = 999;
            }
        }
        // %Tracks Maintenance
        //%###################
        update_tracks(TracksMat, TracksMissMat, 3, 360, 5, 360, 15, Win_dlt, Dlt_th);

        clean_tracks(TracksMat, TracksX, TracksP, TracksVecMat, TracksMissMat, TracksDataBinMat, TracksDataMat,
                     3, 360, 4, 4);

    } else {
        // initialize_tracks(TracksMat, TracksX, TracksVecMat, MergedYc, MergedRng, MergedTeta, numDetect, ping_number);
        //  Initialize TracksMat (flattened)
        for (int i = 0; i < numDetect; i++) {
            TracksMat[i] = 0;            // TracksMat(1, 1: NumDetect) = zeros(1, NumDetect);
            TracksMat[360 + i] = 0;      // TracksMat(2, 1: NumDetect) = zeros(1, NumDetect);
            TracksMat[2 * 360 + i] = 0;  // TracksMat(3, 1: NumDetect) = zeros(1, NumDetect);

            TracksX[i] = MergedYc[i];                    // TracksX(1, 1: NumDetect) = MergedYc(1, :);
            TracksX[360 + i] = MergedYc[numDetect + i];  // TracksX(2, 1: NumDetect) = MergedYc(2, :);
            TracksX[2 * 360 + i] = 0;                    // TracksX(3, 1: NumDetect) = zeros(1, NumDetect);
            TracksX[3 * 360 + i] = 0;                    // TracksX(4, 1: NumDetect) = zeros(1, NumDetect);

            TracksVecMat[i] = 1;                        // TracksVecMat(1, 1: NumDetect) = ones(1, NumDetect);
            TracksVecMat[360 + i] = MergedRng[i];       // TracksVecMat(2, 1: NumDetect) = MergedRng;
            TracksVecMat[2 * 360 + i] = MergedTeta[i];  // TracksVecMat(3, 1: NumDetect) = MergedTeta;
            TracksVecMat[4 * 360 + i] = ping_number;    // TracksVecMat(4, 1: NumDetect) = ping_number * ones(1, NumDetect);
        }
        update_tracks2(MergedRng, MergedTeta, sigmaTeta, numDetect, CurrentTargetInd, cov_fact, xmax, Win_dlt, Tping, TracksP, TracksMissMat, MergedYc, ping_number);
    }

    // Other sections of the code (e.g., cleaning tracks) should also be converted similarly, updating each array as a flattened array.
}
