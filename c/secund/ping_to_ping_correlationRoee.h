#include <math.h>
#include <stdio.h>
#include <string.h>

#include "cov_plot_calcRoee.h"
#include "std_calcRoee.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CNF_TH 10
#define RNG_DIST 10
#define TETA_DIST 10
#define PENALTY 50
#define GAMMA_CORR 0.1
#define PI 3.14159
#define TEST
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
void ping_to_ping_correlationRoee(
    double *TracksMat, double *TracksVecMat, double *TracksP, double *TracksX, double *TracksMissMat,
    double *TracksDataBinMat, double *TracksDataMat, double *MergedRng, double *MergedTeta, double *MergedYc,
    int NumDetect, int ping_ind, double Tping, double sigmaTeta, int MaxTarget, int tracks_mat_cols, double *uncorr_plots_list) {
#else
void ping_to_ping_correlationRoee(
    double *TracksMatT, double *TracksVecMatT, double *TracksPT, double *TracksXT, double *TracksMissMatT,
    double *TracksDataBinMatT, double *TracksDataMatT, double *MergedRngT, double *MergedTetaT, double *MergedYcT,
    int NumDetect, int ping_ind, double Tping, double sigmaTeta, int MaxTarget, int tracks_mat_cols, double *uncorr_plots_list) {
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

    int ass_plots_flag[NumDetect];
    for (size_t i = 0; i < NumDetect; i++) {
        ass_plots_flag[i] = 0;
    }

    int undeleted_tracks_ind[MaxTarget];
    int undeleted_count = 0;

    // Find undeleted tracks
    for (int i = 0; i < MaxTarget; i++) {
        if (TracksMat[MaxTarget + i] == 0) {
            undeleted_tracks_ind[undeleted_count++] = i;
        }
    }

    // Initialization
    for (int i = 0; i < tracks_mat_cols; i++) {
        if (TracksMat[i] != 999) {
            for (int j = 0; j < MaxTarget; j++) {
                TracksDataBinMat[i * MaxTarget + j] = 0;
                TracksDataMat[i * MaxTarget + j] = 0;
            }
        }
    }

    for (int n = 0; n < undeleted_count; n++) {
        /*
        %Track Confirmation
        if (TracksVecMat(1, undeleted_tracks_ind[n]) > CNF_TH) && (TracksVecMat(1, undeleted_tracks_ind[n]) ~= 999) && (TracksMat(2, undeleted_tracks_ind[n]) == 0)
            TracksMat(2, undeleted_tracks_ind[n]) = 1;
            TracksMat(3, undeleted_tracks_ind[n]) = 1;
        end
        */
        // Track Confirmation
        if (TracksVecMat[undeleted_tracks_ind[n]] > CNF_TH && TracksVecMat[undeleted_tracks_ind[n]] != 999 && TracksMat[1 * tracks_mat_cols + undeleted_tracks_ind[n]] == 0) {
            TracksMat[1 * tracks_mat_cols + undeleted_tracks_ind[n]] = 1;  //?0
            TracksMat[2 * tracks_mat_cols + undeleted_tracks_ind[n]] = 1;
        }

        int corr_track_flag = 0;

        for (int k = 0; k < NumDetect; k++) {
            /*
             errR = TracksVecMat(2, undeleted_tracks_ind[n]) - MergedRng(k);
             errTeta = MergedTeta(k) - TracksVecMat(3, undeleted_tracks_ind[n]);
            */
            double errR = TracksVecMat[tracks_mat_cols + undeleted_tracks_ind[n]] - MergedRng[k];
            double errTeta = MergedTeta[k] - TracksVecMat[2 * tracks_mat_cols + undeleted_tracks_ind[n]];
            double errT = (errTeta > 180) ? -errTeta + 360 : ((-errTeta > 180) ? errTeta + 360 : errTeta);

            if (fabs(errR) < RNG_DIST && fabs(errT) < TETA_DIST) {
                double Rc[4];  // Adjust the size of Rc based on its usage in cov_plot_calcRoee
                cov_plot_calcRoee(MergedRng[k], MergedTeta[k] * PI / 180, sigmaTeta, Rc);

                // last_ping_number = find(squeeze(TracksMissMat(1, undeleted_tracks_ind[n], :)) == 0);
                int last_ping_number = -1;
                for (int j = 0; j < 15; j++) {
                    if (TracksMissMat[undeleted_tracks_ind[n] * 15 + j] == 0) {
                        last_ping_number = j;
                        break;  //?
                    }
                }

                double MergedYc_cut[2] = {MergedYc[k], MergedYc[NumDetect + k]};  // MergedYc(:, k)
                double TracksX_cut[4] = {
                    TracksX[undeleted_tracks_ind[n]],
                    TracksX[360 + undeleted_tracks_ind[n]],
                    TracksX[360 * 2 + undeleted_tracks_ind[n]],
                    TracksX[360 * 3 + undeleted_tracks_ind[n]]};
                /*
                   double TracksP_cut[4 * 4];
                   for (size_t i = 0; i < 16; i++) {
                       TracksP_cut[i] = TracksP[undeleted_tracks_ind[n]*360+i];
                   }
   */

                double TracksP_cut[4 * 4];  // This will hold the 2D slice (4x4)

                // Loop through the 2D slice
                for (int j = 0; j < 4; j++) {
                    for (int k = 0; k < 4; k++) {
                        // Calculate the index in the flattened array
                        int index = undeleted_tracks_ind[n] * (4 * 4) + j * 4 + k;
                        // Copy the value from the flattened array to the 2D slice
                        TracksP_cut[j * 4 + k] = TracksP[index];
                    }
                }

                double std_dis_corr = std_calcRoee(ping_ind, Tping, MergedYc_cut, Rc, last_ping_number, TracksX_cut, TracksP_cut, n);

                double std_dis = (TracksMat[360 + undeleted_tracks_ind[n]] == 0) ? std_dis_corr + PENALTY : std_dis_corr;

                if (std_dis_corr < GAMMA_CORR) {
                    TracksDataBinMat[360 * undeleted_tracks_ind[n] + k] = 1;
                    TracksDataMat[360 * undeleted_tracks_ind[n] + k] = std_dis;
                    ass_plots_flag[k] = 1;
                    corr_track_flag = 1;
                }
            }
        }

        if (corr_track_flag == 0) {
            TracksVecMat[3 * 360 + undeleted_tracks_ind[n]] = ping_ind+1;//it add for matlab py start from 1
            TracksVecMat[undeleted_tracks_ind[n]] = MAX(0,(TracksVecMat[undeleted_tracks_ind[n]] - 1));

            for (int i = 0; i < 5; i++) {
                // Access the current element in TracksMissMat and update it based on the previous element
                int index = i * (360 * 15) + undeleted_tracks_ind[n] * 15 + ping_ind;
                TracksMissMat[index] = TracksMissMat[index - 1];
                if (i == 0) {
                    TracksMissMat[index] += 1;
                }
            }
        }
    }

    // uncorr_plots_list = find(ass_plots_flag == 0);
    for (int i = 0, index_count = 0; i < NumDetect; i++) {
        if (ass_plots_flag[i] == 0) {
            uncorr_plots_list[index_count] = i;
            index_count++;
        }
    }
}