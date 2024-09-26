#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LARGE 1e4
#define PI 3.1415926535897

// Assuming these helper functions are defined elsewhere
#include "AuctionRoee.h"
#include "cov_plot_calcRoee.h"
#include "Kalman_filterRoee.h"

void ping_to_ping_associationRoee(double* TracksMat, double* TracksVecMat, double* TracksP, double* TracksX, double* TracksMissMat,
                                  double* TracksDataBinMat, double* TracksDataMat, double* MergedRng, double* MergedTeta, double* MergedYc,
                                  int NumDetect, int ping_ind, int sigmaTeta, int cov_fact, int Win_dlt, int xmax, int Tping, int* CurrentTargetInd) {

    int MaxTarget = 360; // Placeholder, replace with the proper dimension of TracksDataBinMat
    int undeleted_tracks_count = 0;

    // Find undeleted tracks
    for (int i = 0; i < MaxTarget; i++) {
        if (TracksMat[i] == 0) {
            undeleted_tracks_count++;
        }
    }

    int* undeleted_tracks_indA = (int*)malloc(undeleted_tracks_count * sizeof(int));
    int undeleted_tracks_indA_count = 0;

    for (int i = 0; i < MaxTarget; i++) {
        if (TracksMat[i] == 0) {
            undeleted_tracks_indA[undeleted_tracks_indA_count++] = i;
        }
    }

    double* Amat = (double*)calloc(undeleted_tracks_count * NumDetect, sizeof(double));
    int* tracks_id = (int*)calloc(undeleted_tracks_count, sizeof(int));

    // Create association matrix
    for (int n = 0; n < undeleted_tracks_count; n++) {
        for (int j = 0; j < MaxTarget; j++) {
            if (TracksDataBinMat[undeleted_tracks_indA[n] * MaxTarget + j] == 1) {
                tracks_id[n] = n;
                Amat[n * NumDetect + j] = TracksDataMat[undeleted_tracks_indA[n] * MaxTarget + j];
            }
        }
    }

    // Find assigned plots
    int plots_index_new_count = 0;
    for (int j = 0; j < NumDetect; j++) {
        double sum = 0;
        for (int i = 0; i < undeleted_tracks_count; i++) {
            sum += Amat[i * NumDetect + j];
        }
        if (sum != 0) {
            plots_index_new_count++;
        }
    }

    int* plots_index_new = (int*)malloc(plots_index_new_count * sizeof(int));
    int index = 0;
    for (int j = 0; j < NumDetect; j++) {
        double sum = 0;
        for (int i = 0; i < undeleted_tracks_count; i++) {
            sum += Amat[i * NumDetect + j];
        }
        if (sum != 0) {
            plots_index_new[index++] = j;
        }
    }

    int NumDetect_new = plots_index_new_count;
    int Ntr = undeleted_tracks_count;

    // Replace 0s with LARGE
    for (int i = 0; i < Ntr * NumDetect_new; i++) {
        if (Amat[i] == 0) {
            Amat[i] = LARGE;
        }
    }

    // Call Auction algorithm
    int* Assigned = (int*)malloc(Ntr * sizeof(int));
    AuctionRoee(Amat, Ntr, NumDetect_new, Assigned);

    // Handle track assignments based on the number of detections and tracks
    if (Ntr > NumDetect_new) {
        // Handling assigned tracks
        for (int i = 0; i < Ntr; i++) {
            if (Assigned[i] != 0) {
                int plot_id = plots_index_new[Assigned[i] - 1];
                int track_id = undeleted_tracks_indA[i];

                if (Amat[i * NumDetect_new + Assigned[i] - 1] == LARGE) {
                    TracksVecMat[track_id * 4 + 3] = ping_ind;
                    for (int p = 0; p < 5; p++) {
                        TracksMissMat[track_id * 5 * MaxTarget + p * MaxTarget + ping_ind] =
                            TracksMissMat[track_id * 5 * MaxTarget + p * MaxTarget + (ping_ind - 1)];
                    }
                } else {
                    double r = MergedRng[plot_id];
                    double teta = MergedTeta[plot_id] * PI / 180;
                    double yc[2] = {MergedYc[plot_id * 2], MergedYc[plot_id * 2 + 1]};

                    double Rc[2*2];
                    cov_plot_calcRoee(r, teta, sigmaTeta, Rc);

                    int last_ping_number = 0;
                    for (int k = 0; k < ping_ind; k++) {
                        if (TracksMissMat[track_id * 5 * MaxTarget + k] == 0) {
                            last_ping_number = k;
                        }
                    }

                    double T = (ping_ind - last_ping_number) * Tping;
                    double X_upd[4], P_upd[4*4], teta_rad;

                    // Kalman_filterRoee(yc, Rc, T, &TracksX[track_id * 4], &TracksP[track_id * 16], X_upd, P_upd, &teta_rad);
                    Kalman_filterRoee(X_upd, P_upd, &teta_rad, yc, Rc, T,  &TracksX[track_id * 4], &TracksP[track_id * 16]);

                    for (int j = 0; j < 4; j++) {
                        TracksX[track_id * 4 + j] = X_upd[j];
                        for (int k = 0; k < 4; k++) {
                            TracksP[track_id * 16 + j * 4 + k] = P_upd[j*4+k];
                        }
                    }

                    TracksVecMat[track_id * 4 + 1] = sqrt(X_upd[0] * X_upd[0] + X_upd[1] * X_upd[1]);
                    TracksVecMat[track_id * 4 + 2] = teta_rad * 180 / PI;
                    TracksVecMat[track_id * 4 + 3] = ping_ind;
                    TracksMissMat[track_id * 5 * MaxTarget + ping_ind] = 0;
                    for (int j = 1; j < 5; j++) {
                        TracksMissMat[track_id * 5 * MaxTarget + j * MaxTarget + ping_ind] = X_upd[j - 1];
                    }
                }
            }
        }
    }

    free(undeleted_tracks_indA);
    free(Amat);
    free(tracks_id);
    free(plots_index_new);
    free(Assigned);
}
