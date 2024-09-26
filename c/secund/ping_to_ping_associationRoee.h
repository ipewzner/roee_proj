#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGE 1e4
#define PI 3.1415926535897

// Assuming these helper functions are defined elsewhere
#include "AuctionRoee.h"
#include "Kalman_filterRoee.h"
#include "cov_plot_calcRoee.h"

// #define TEST
#ifndef TEST
double MergedRng[13];
double MergedTeta[13];
double MergedYc[13 * 2];
double TracksDataBinMat[360 * 360];
double TracksDataMat[360 * 360];
double TracksMissMat[5 * 360 * 15];
double TracksP[360 * 4 * 4];
double TracksVecMat[4 * 360];
double TracksX[4 * 360];
double TracksMat[3 * 360];
#endif
#ifdef TEST
void ping_to_ping_associationRoee(double* TracksMat, double* TracksVecMat, double* TracksP, double* TracksX, double* TracksMissMat,
                                  double* TracksDataBinMat, double* TracksDataMat, double* MergedRng, double* MergedTeta, double* MergedYc,
                                  int NumDetect, int ping_ind, int sigmaTeta, int cov_fact, int Win_dlt, int xmax, int Tping, int* CurrentTargetInd) {
#else
void ping_to_ping_associationRoee(
    double *TracksMatT, double *TracksVecMatT, double *TracksPT, double *TracksXT, double *TracksMissMatT,
    double *TracksDataBinMatT, double *TracksDataMatT, double *MergedRngT, double *MergedTetaT, double *MergedYcT,
    int NumDetect, int ping_ind, int sigmaTeta, int cov_fact, int Win_dlt, int xmax, int Tping, int *CurrentTargetInd) {
    // Copy data from pointers with 'T' to the corresponding arrays
    memcpy(MergedRng, MergedRngT, 13 * sizeof(double));
    memcpy(MergedTeta, MergedTetaT, 13 * sizeof(double));
    memcpy(MergedYc, MergedYcT, 13 * 2 * sizeof(double));
    memcpy(TracksDataBinMat, TracksDataBinMatT, 360 * 360 * sizeof(double));
    memcpy(TracksDataMat, TracksDataMatT, 360 * 360 * sizeof(double));
    memcpy(TracksMissMat, TracksMissMatT, 5 * 360 * 15 * sizeof(double));
    memcpy(TracksP, TracksPT, 360 * 4 * 4 * sizeof(double));
    memcpy(TracksVecMat, TracksVecMatT, 4 * 360 * sizeof(double));
    memcpy(TracksX, TracksXT, 4 * 360 * sizeof(double));
    memcpy(TracksMat, TracksMatT, 3 * 360 * sizeof(double));

#endif

    int MaxTarget = 360;  // Placeholder, replace with the proper dimension of TracksDataBinMat
    int undeleted_tracks_count = 0;

    // Find undeleted tracks
    for (int i = 0; i < MaxTarget; i++) {
        if (TracksMat[i] == 0) {
            undeleted_tracks_count++;
        }
    }

    int undeleted_tracks_indA_count = 0;
    int undeleted_tracks_indA[undeleted_tracks_count];

    for (int i = 0; i < MaxTarget; i++) {
        if (TracksMat[i] == 0) {
            undeleted_tracks_indA[undeleted_tracks_indA_count++] = i;
        }
    }

    /*
        double* Amat = (double*)calloc(undeleted_tracks_count * NumDetect, sizeof(double));
        int* tracks_id = (int*)calloc(undeleted_tracks_count, sizeof(int));
    */
    double Amat[undeleted_tracks_count * NumDetect];
    int tracks_id[undeleted_tracks_count];

    for (size_t i = 0; i < undeleted_tracks_count * NumDetect; i++) {
        Amat[i] = 0;
    }
    for (size_t i = 0; i < undeleted_tracks_count; i++) {
        tracks_id[i] = 0;
    }

    // Create association matrix
    for (int n = 0; n < undeleted_tracks_count; n++) {
        for (int j = 0; j < MaxTarget; j++) {
            if (TracksDataBinMat[undeleted_tracks_indA[n] * MaxTarget + j] == 1) {
                tracks_id[n] = n;
                Amat[n * NumDetect + j] = TracksDataMat[undeleted_tracks_indA[n] * MaxTarget + j];
            }
        }
    }
    /*
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

           //int* plots_index_new = (int*)malloc(plots_index_new_count * sizeof(int));
           int plots_index_new[plots_index_new_count];
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
       */

    // loc = find(tracks_id ~= 0);
    int loc[10];
    for (size_t i = 0; i < 10; i++) {
        loc[i] = -1;
    }

    // int* loc = (int*)malloc(10 * sizeof(int));

    for (int i = 0, count = 0; i < 10; i++) {
        if (tracks_id[i] != 0) {
            loc[count] = i;
            count++;
        }
    }

    // plots_index_new = find(sum(Amat(loc, :))~=0); %Plots correlated to track
    int plots_index_new[NumDetect];
    int num_plots_new = 0;

    for (int j = 0; j < NumDetect; j++) {
        double sum = 0.0;
        for (int i = 0; i < undeleted_tracks_count; i++) {
            if (loc[i] != -1) {
                sum += Amat[loc[i] * NumDetect + j];
            }
        }
        if (sum != 0.0) {
            plots_index_new[num_plots_new] = j;
            num_plots_new++;
        }
    }
    for (int i = num_plots_new; i < NumDetect; i++) {
        plots_index_new[i] = -1;
    }

    // A = Amat(loc, plots_index_new);
    //  Allocate memory for the submatrix
    //*A = (double*)malloc(num_loc * num_plots_new * sizeof(double));
    int num_loc = 0;
    for (size_t i = 0; i < 10; i++) {
        if (loc[i] != -1) {
            num_loc++;
        }
    }

    double A[num_loc * num_plots_new];

    // Extract the submatrix
    for (int i = 0; i < num_loc; i++) {
        for (int j = 0; j < num_plots_new; j++) {
            A[i * num_plots_new + j] = Amat[loc[i] * 13 + plots_index_new[j]];
        }
    }

    // undeleted_tracks_ind = undeleted_tracks_indA(loc);

    // Ntr = length(undeleted_tracks_ind); %Number of undeleted tracks
    // Allocate memory for the undeleted_tracks_ind array
    //  undeleted_tracks_ind = (int *)malloc(num_undeleted * sizeof(int));
    int undeleted_tracks_ind[num_loc];
    int Ntr = 0;
    // Extract the indices
    for (int i = 0; i < num_loc; i++) {
        for (int j = 0; j < undeleted_tracks_count; j++) {
            if (loc[i] == undeleted_tracks_indA[j]) {
                undeleted_tracks_ind[Ntr] = j;
                Ntr++;
                break;
            }
        }
    }

    // NumDetect_new = length(plots_index_new);
    int NumDetect_new = num_plots_new;

    // A(A==0) = LARGE;
    for (int i = 0; i < num_loc * num_plots_new; i++) {
        if (A[i] == 0.0) {
            A[i] = LARGE;
        }
    }

    // Call Auction algorithm
    // int* Assigned = (int*)malloc(Ntr * sizeof(int));
    int Assigned[Ntr];
    AuctionRoee(A, Ntr, NumDetect_new, Assigned);

    // Handle track assignments based on the number of detections and tracks
    if (Ntr > NumDetect_new) {
        // Handling assigned tracks
        for (int i = 0; i < Ntr; i++) {
            if (Assigned[i] != -1) {
                int plot_id = plots_index_new[Assigned[i]];
                int track_id = undeleted_tracks_indA[i];

                if (Amat[i * NumDetect_new + Assigned[i]] == LARGE) {
                    TracksVecMat[track_id * 4 + 3] = ping_ind;
                    for (int p = 0; p < 5; p++) {
                        TracksMissMat[track_id * 5 * MaxTarget + p * MaxTarget + ping_ind] =
                            TracksMissMat[track_id * 5 * MaxTarget + p * MaxTarget + (ping_ind - 1)];
                    }
                } else {
                    double r = MergedRng[plot_id];
                    double teta = MergedTeta[plot_id] * PI / 180;
                    double yc[2] = {MergedYc[plot_id * 2], MergedYc[plot_id * 2 + 1]};

                    double Rc[2 * 2];
                    cov_plot_calcRoee(r, teta, sigmaTeta, Rc);

                    int last_ping_number = 0;
                    for (int k = 0; k < ping_ind; k++) {
                        if (TracksMissMat[track_id * 5 * MaxTarget + k] == 0) {
                            last_ping_number = k;
                        }
                    }

                    double T = (ping_ind - last_ping_number) * Tping;
                    double X_upd[4], P_upd[4 * 4], teta_rad;

                    // Kalman_filterRoee(yc, Rc, T, &TracksX[track_id * 4], &TracksP[track_id * 16], X_upd, P_upd, &teta_rad);
                    Kalman_filterRoee(X_upd, P_upd, &teta_rad, yc, Rc, T, &TracksX[track_id * 4], &TracksP[track_id * 16]);

                    for (int j = 0; j < 4; j++) {
                        TracksX[track_id * 4 + j] = X_upd[j];
                        for (int k = 0; k < 4; k++) {
                            TracksP[track_id * 16 + j * 4 + k] = P_upd[j * 4 + k];
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

    // free(undeleted_tracks_indA);
    // free(Amat);
    // free(tracks_id);
    // free(plots_index_new);
    // free(Assigned);
}
