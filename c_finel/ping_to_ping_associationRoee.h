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

#define TEST
#ifndef TEST
double MergedRng[13];
double MergedTeta[13];
double MergedYc[13 * 2];
int TracksDataBinMat[360 * 360];
double TracksDataMat[360 * 360];
double TracksMissMat[5 * 360 * 15];
double TracksP[360 * 4 * 4];
double TracksVecMat[4 * 360];
double TracksX[4 * 360];
double TracksMat[3 * 360];
#endif
#ifdef TEST
void ping_to_ping_associationRoee(double* TracksMat, double* TracksVecMat, double* TracksP, double* TracksX, double* TracksMissMat,
                                  int* TracksDataBinMat, double* TracksDataMat, double* MergedRng, double* MergedTeta, double* MergedYc,
                                  int NumDetect, int ping_ind, int sigmaTeta, int cov_fact, int Win_dlt, int xmax, int Tping, int CurrentTargetInd) {
#else
void ping_to_ping_associationRoee(
    double *TracksMatT, double *TracksVecMatT, double *TracksPT, double *TracksXT, double *TracksMissMatT,
    int *TracksDataBinMatT, double *TracksDataMatT, double *MergedRngT, double *MergedTetaT, double *MergedYcT,
    int NumDetect, int ping_ind, int sigmaTeta, int cov_fact, int Win_dlt, int xmax, int Tping, int CurrentTargetInd) {
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
    // Find undeleted tracks
    int undeleted_tracks_count = 0;
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
    // int Ntr = 0;
    //  Extract the indices
    for (int i = 0; i < num_loc; i++) {
        undeleted_tracks_ind[i] = undeleted_tracks_indA[loc[i]];
    }

    /*
       for (int i = 0; i < num_loc; i++) {
           for (int j = 0; j < undeleted_tracks_count; j++) {
               if (loc[i] == undeleted_tracks_indA[j]) {
                   undeleted_tracks_ind[Ntr] = j;
                   Ntr++;
                   break;
               }
           }
       }
   */
    int Ntr = num_loc;
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
    int assigned_len = NumDetect_new > num_loc ? NumDetect_new : num_loc;
    int Assigned[assigned_len];
    AuctionRoee(A, num_loc, NumDetect_new, Assigned);

    // Handle track assignments based on the number of detections and tracks
    if (Ntr > NumDetect_new) {
        // Handling assigned tracks

        // ind1 = find(Assigned ~= 0); %List of assigned tracks
        int ind1[Ntr];
        int ind1_count = 0;
        for (int i = 0; i < Ntr; i++) {
            if (Assigned[i] != -1) {
                ind1[ind1_count++] = i;
            }
        }

        for (int i = 0; i < Ntr; i++) {
            if (Assigned[i] != -1) {
                int plot_id = plots_index_new[Assigned[ind1[i]]];
                int track_id = undeleted_tracks_indA[ind1[i]];

                if (A[ind1[i] * NumDetect_new + Assigned[ind1[i]]] == LARGE) {
                    TracksVecMat[track_id * 4 + 3] = ping_ind + 1;

                    // Update TracksMissMat
                    TracksMissMat[0 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[0 * 360 * 15 + track_id * 15 + ping_ind - 1] + 1;
                    TracksMissMat[1 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[1 * 360 * 15 + track_id * 15 + ping_ind - 1];
                    TracksMissMat[2 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[2 * 360 * 15 + track_id * 15 + ping_ind - 1];
                    TracksMissMat[3 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[3 * 360 * 15 + track_id * 15 + ping_ind - 1];
                    TracksMissMat[4 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[4 * 360 * 15 + track_id * 15 + ping_ind - 1];
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

                    TracksVecMat[track_id * 4 + 0] += 1;
                    TracksVecMat[track_id * 4 + 1] = sqrt(X_upd[0] * X_upd[0] + X_upd[1] * X_upd[1]);
                    TracksVecMat[track_id * 4 + 2] = teta_rad * 180 / PI;
                    TracksVecMat[track_id * 4 + 3] = ping_ind + 1;

                    TracksMissMat[0 * 360 * 15 + track_id * 15 + ping_ind] = 0;
                    TracksMissMat[1 * 360 * 15 + track_id * 15 + ping_ind] = X_upd[2];
                    TracksMissMat[2 * 360 * 15 + track_id * 15 + ping_ind] = X_upd[3];
                    TracksMissMat[3 * 360 * 15 + track_id * 15 + ping_ind] = X_upd[0];
                    TracksMissMat[4 * 360 * 15 + track_id * 15 + ping_ind] = X_upd[1];
                }
            }
        }
        // Free memory
        // Handling unassigned tracks
        //    ind2 = find(Assigned == 0); %List of unassigned tracks
        int ind2[Ntr];
        int ind2_count = 0;
        for (int i = 0; i < Ntr; i++) {
            if (Assigned[i] == -1) {
                ind2[ind2_count++] = i;
            }
        }

        // Handle unassigned tracks
        for (int kk = 0; kk < ind2_count; kk++) {
            int track_id = undeleted_tracks_ind[ind2[kk]];

            // Update TracksVecMat
            TracksVecMat[4 * track_id] = ping_ind + 1;

            // Update TracksMissMat
            TracksMissMat[0 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[0 * 360 * 15 + track_id * 15 + ping_ind - 1] + 1;
            TracksMissMat[1 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[1 * 360 * 15 + track_id * 15 + ping_ind - 1];
            TracksMissMat[2 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[2 * 360 * 15 + track_id * 15 + ping_ind - 1];
            TracksMissMat[3 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[3 * 360 * 15 + track_id * 15 + ping_ind - 1];
            TracksMissMat[4 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[4 * 360 * 15 + track_id * 15 + ping_ind - 1];
        }

        // free(ind2);
    }

    else {
        // ind1 = find(Assigned ~= 0); %List of assigned tracks
        int ind1[Ntr];
        int ind1_count = 0;
        for (int i = 0; i < Ntr; i++) {
            if (Assigned[i] != -1) {
                ind1[ind1_count++] = i;
            }
        }
        for (size_t nn = 0; nn < ind1_count; nn++) {
            int plot_id = plots_index_new[ind1[nn]];
            int track_id = undeleted_tracks_ind[Assigned[ind1[nn]]];

            if (A[Assigned[ind1[nn]] * NumDetect_new + ind1[nn]] == LARGE) {
                // Update TracksVecMat
                TracksVecMat[4 * track_id] = ping_ind + 1;

                // Update TracksMissMat
                TracksMissMat[0 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[0 * 360 * 15 + track_id * 15 + ping_ind - 1] + 1;
                TracksMissMat[1 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[1 * 360 * 15 + track_id * 15 + ping_ind - 1];
                TracksMissMat[2 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[2 * 360 * 15 + track_id * 15 + ping_ind - 1];
                TracksMissMat[3 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[3 * 360 * 15 + track_id * 15 + ping_ind - 1];
                TracksMissMat[4 * 360 * 15 + track_id * 15 + ping_ind] = TracksMissMat[4 * 360 * 15 + track_id * 15 + ping_ind - 1];

            } else {
                double r = MergedRng[plot_id];
                double teta = MergedTeta[plot_id] * PI / 180;
                /*
                double yc[MergedYc_col_size] ;
                for (int i = 0; i < MergedYc_num_rows; i++) {
                    yc[i] = MergedYc[i * MergedYc_num_rows + plot_id];
                }
                */
                double yc[2] = {MergedYc[plot_id], MergedYc[13 + plot_id]};

                double Rc[2 * 2];
                cov_plot_calcRoee(r, teta, sigmaTeta, Rc);

                // Calculate time between last update and current one
                int last_ping_number = TracksVecMat[3 * 360 + track_id];
                double T = (ping_ind - last_ping_number) * Tping;

                // Call Kalman filter
                double X_upd[4], P_upd[4 * 4], teta_rad = 0;
                double cut_TracksX[4] = {TracksX[track_id], TracksX[360 + track_id], TracksX[2 * 360 + track_id], TracksX[3 * 360 + track_id]};
                double cut_TracksP[16];

                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        cut_TracksP[i * 4 + j] = TracksP[track_id * 16 + i * 4 + j];
                    }
                }

                Kalman_filterRoee(X_upd, P_upd, &teta_rad, yc, Rc, T, cut_TracksX, cut_TracksP);

                for (int j = 0; j < 4; j++) {
                    TracksX[j * 360 + track_id] = X_upd[j];
                    for (int k = 0; k < 4; k++) {
                        TracksP[track_id * 16 + j * 4 + k] = P_upd[j * 4 + k];
                    }
                }

                // Update TracksVecMat
                TracksVecMat[track_id + 0 * 360] += 1;
                TracksVecMat[track_id + 1 * 360] = sqrt(X_upd[0] * X_upd[0] + X_upd[1] * X_upd[1]);
                TracksVecMat[track_id + 2 * 360] = teta_rad * 180 / PI;
                TracksVecMat[track_id + 3 * 360] = ping_ind + 1;

                TracksMissMat[0 * 360 * 15 + track_id * 15 + ping_ind] = 0;
                TracksMissMat[1 * 360 * 15 + track_id * 15 + ping_ind] = X_upd[2];
                TracksMissMat[2 * 360 * 15 + track_id * 15 + ping_ind] = X_upd[3];
                TracksMissMat[3 * 360 * 15 + track_id * 15 + ping_ind] = X_upd[0];
                TracksMissMat[4 * 360 * 15 + track_id * 15 + ping_ind] = X_upd[1];
            }
        }
        //%Handling unassigned plots
        // uncorr_plots_list = plots_index_new(find(Assigned == 0)); %List of unassigned plots

        // int* uncorr_plots_list = (int*)malloc(size * sizeof(int));
        int uncorr_plots_list[assigned_len - ind1_count];
        int uncorr_plots_count = 0;
        for (int i = 0; i < NumDetect; i++) {
            if (Assigned[i] == -1) {
                uncorr_plots_list[uncorr_plots_count++] = plots_index_new[i];
            }
        }

        if (uncorr_plots_count > 0) {
            for (int MergeInd = 0; MergeInd < uncorr_plots_count; MergeInd++) {
                int plot_id = uncorr_plots_list[MergeInd];

                // Calculate Rc
                double Rc[2 * 2];
                cov_plot_calcRoee(MergedRng[uncorr_plots_list[MergeInd]], MergedTeta[uncorr_plots_list[MergeInd]] * PI / 180, sigmaTeta, Rc);

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
            // Update TracksX
            for (int j = CurrentTargetInd; j < (CurrentTargetInd + uncorr_plots_count); j++) {
                TracksX[0 * 360 + j] = MergedYc[0 * 13 + uncorr_plots_list[j - CurrentTargetInd]];
                TracksX[1 * 360 + j] = MergedYc[1 * 13 + uncorr_plots_list[j - CurrentTargetInd]];
                TracksX[2 * 360 + j] = 0;
                TracksX[3 * 360 + j] = 0;

                TracksVecMat[0 * 360 + j] = 1;
                TracksVecMat[1 * 360 + j] = MergedRng[uncorr_plots_list[j - CurrentTargetInd]];
                TracksVecMat[2 * 360 + j] = MergedTeta[uncorr_plots_list[j - CurrentTargetInd]];
                TracksVecMat[3 * 360 + j] = (ping_ind + 1) * 1;

                TracksMat[0 * 360 + j] = 0;
                TracksMat[1 * 360 + j] = 0;
                TracksMat[2 * 360 + j] = 0;

                TracksMissMat[0 * 360 * 15 + j * 15 + ping_ind] = 0;
                TracksMissMat[1 * 360 * 15 + j * 15 + ping_ind] = 0;
                TracksMissMat[2 * 360 * 15 + j * 15 + ping_ind] = 0;
                TracksMissMat[3 * 360 * 15 + j * 15 + ping_ind] = MergedYc[0 * 13 + uncorr_plots_list[j - CurrentTargetInd]];
                TracksMissMat[4 * 360 * 15 + j * 15 + ping_ind] = MergedYc[1 * 13 + uncorr_plots_list[j - CurrentTargetInd]];

                for (int i = 0; i < 360; i++) {
                    TracksDataBinMat[j * 360 + i] = 0;
                    TracksDataMat[j * 360 + i] = 999;
                }
            }
            //  TracksMissMat(5, CurrentTargetInd+1: CurrentTargetInd+length(uncorr_plots_list), ping_ind) = MergedYc(2, uncorr_plots_list);
            //  TracksDataBinMat(CurrentTargetInd+1: CurrentTargetInd+length(uncorr_plots_list), :) = zeros(length(uncorr_plots_list), MaxTarget);
            //  TracksDataMat(CurrentTargetInd+1: CurrentTargetInd+length(uncorr_plots_list), :) = 999*ones(length(uncorr_plots_list), MaxTarget);

            CurrentTargetInd += uncorr_plots_count;
        }
    }
    // free(undeleted_tracks_indA);
    // free(Amat);
    // free(tracks_id);
    // free(plots_index_new);
    // free(Assigned);
}
