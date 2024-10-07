#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AnalyzeTracks.h"
#include "CheckDetect.h"
#include "ShortImgProc_v2Roee.h"
#include "Shortproc_ping_dataRoee.h"
#include "config.h"

void fillMatrix(double* matrix, int size, double filling) {
    for (int i = 0; i < size; i++) {
        matrix[i] = filling;
    }
}

int main() {
    int UpdateThFlag = 1;

    // Main Loop (on Pings)
    while (1) {
        double FinalPingFlag = 0;

        // Initialize matrices and vectors
        double* IndMat = (double*)malloc(NUM_OF_PINGS * sizeof(double*));
        double* rMat = (double*)malloc(NUM_OF_PINGS * sizeof(double*));
        double* YawVec = (double*)calloc(NUM_OF_PINGS, sizeof(double*));
        fillMatrix(IndMat, NUM_OF_PINGS * MAX_TARGET, 999);
        fillMatrix(rMat, NUM_OF_PINGS * MAX_TARGET, 999);

        double* tSamp = arrayMaker(0, 1.0 / FS, T_P, T_SAMP_SIZE);
        double* lfm = compute_lfm(tSamp, T_SAMP_SIZE, FMIN, FMAX, T_P);
        double* matched_filter = create_matched_filter(lfm, T_SAMP_SIZE);
        double* azBeams;  // = linspace(0, 2 * PI - 2 * PI / (NAZ * RATIO), NAZ);
        azBeams = linspace(0, 2 * PI - 2 * PI / (NAZ * RATIO), NAZ);

        double pos_sensors[12] = POS_SENSORS_RS;  //{RS, 0, -RS, 0, 0, RS, 0, -RS, 0, 0, 0, 0};  // Example initialization
        double ImgTh;                             // Example initialization
        double NewImgTh;                          // Example initialization
        double* IndVec;
        double* rVec;

        int CurrentTargetInd = 0;

        // Track Detection
        double ReportTrackRange[MAX_TARGET];
        int DetectFlagVec[MAX_TARGET];

        // Loop over pings
        for (int PingNum = 1; PingNum <= NUM_OF_PINGS; PingNum++) {
            if (PingNum == NUM_OF_PINGS - 1 && UpdateThFlag == 0) {
                FinalPingFlag = 1;
            }

            // Extract one ping from RAM
            double* PingData = GetDataFromRam(PRI_SAMPLES, PingNum, TGAURD * FS);

            // Extract current yaw from compass
            YawVec[PingNum - 1] = GetYawFromCompass(PingNum);

            // Call Shortproc_ping_dataRoee function
            IndVec = (double*)calloc(MAX_TARGET, sizeof(double));
            rVec = (double*)calloc(MAX_TARGET, sizeof(double));

            double* img = (double*)calloc(N_S * M_S, sizeof(double));  // Expected result (image) 3201 x 72
            Shortproc_ping_dataRoee(img, IndVec, rVec, &NewImgTh, PingData, matched_filter, 15, 128e3, azBeams, NAZ, pos_sensors, PRI_SAMPLES, 999, 0, 1, 4, N_S);

            // memmove
            // memcpy((IndMat+(PingNum-1)*MAX_TARGET), IndVec, MAX_TARGET * sizeof(double));
            memcpy((IndMat + (PingNum - 1) * MAX_TARGET), IndVec, MAX_TARGET);
            memcpy((rMat + (PingNum - 1) * MAX_TARGET), rVec, MAX_TARGET);
            // memcpy(IndMat[PingNum - 1], IndVec, MAX_TARGET * sizeof(double));
            // memcpy(rMat[PingNum - 1], rVec, MAX_TARGET * sizeof(double));

            free(IndVec);
            free(rVec);
            free(PingData);
        }

        double* TracksP = (double*)malloc(MAX_TARGET * 4 * 4 * sizeof(double*));
        double* TracksX = (double*)malloc(MAX_TARGET * 4 * sizeof(double*));
        double* TracksMat = (double*)malloc(MAX_TARGET * 3 * sizeof(double*));
        double* TracksVecMat = (double*)malloc(MAX_TARGET * 4 * sizeof(double*));
        double* TracksMissMat = (double*)malloc(5 * MAX_TARGET * NUM_OF_PINGS * sizeof(double*));
        // NEED TO BE BINARY
        int* TracksDataBinMat = (int*)calloc(MAX_TARGET * MAX_TARGET, sizeof(int*));
        double* TracksDataMat = (double*)malloc(MAX_TARGET * MAX_TARGET * sizeof(double*));

        fillMatrix(TracksP, MAX_TARGET * 4 * 4, 999);
        fillMatrix(TracksX, MAX_TARGET * 4, 999);
        fillMatrix(TracksMat, MAX_TARGET * 3, 999);
        fillMatrix(TracksVecMat, MAX_TARGET * 4, 999);
        fillMatrix(TracksMissMat, 5 * MAX_TARGET * NUM_OF_PINGS, 999);
        fillMatrix(TracksDataMat, MAX_TARGET * MAX_TARGET, 999);

        // Initialize with default values
        /*        for (int i = 0; i < MAX_TARGET; i++) {
                    for (int j = 0; j < MAX_TARGET; j++) {
                        TracksDataBinMat[i][j] = 0;
                        TracksDataMat[i][j] = 999;
                    }
                    for (int j = 0; j < 4; j++) {
                        TracksX[j][i] = 999;
                        TracksVecMat[j][i] = 999;
                        for (int k = 0; k < 4; k++) {
                            TracksP[i][j][k] = 999;
                        }
                    }
                }
        */

        // Process each ping
        for (int PingNum = 1; PingNum <= NUM_OF_PINGS; PingNum++) {
            double CurrentYaw = YawVec[PingNum - 1];
            memcpy((IndVec + (PingNum - 1) * MAX_TARGET), IndMat, MAX_TARGET);
            memcpy((rVec + (PingNum - 1) * MAX_TARGET), rMat, MAX_TARGET);

            // Find valid positions
            double pos[MAX_TARGET];
            int valid_pos = find(rVec, MAX_TARGET, 0, 999, pos);

            if (valid_pos > 0) {
                // Process Merged Tracks
                double *MergedRng, *MergedTeta, *MergedYc;
                ShortImgProc_v2Roee(IndVec, rVec, MAX_TARGET, SIGMA_TETA, azBeams, CurrentYaw, MergedRng, MergedTeta, MergedYc);

                AnalyzeTracks(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat,
                              TracksDataBinMat, TracksDataMat, MergedRng, MergedTeta, MergedYc,
                              PingNum, T_PRI, SIGMA_TETA, WIN_DLT, DLT_TH, MAX_TARGET, &CurrentTargetInd);
            } else if (PingNum > 1) {
                valid_pos = find(TracksMat, MAX_TARGET, 0, 999, pos);
                if (valid_pos > 0) {
                    // Handle undeleted tracks
                    for (int i = 0; i < MAX_TARGET; i++) {
                        if (TracksMat[i] != 999) {
                            if (TracksMat[i] == 0) {
                                TracksMissMat[i * 15 + PingNum] = TracksMissMat[i * 15 + PingNum - 1] + 1;

                                // Tracks maintenance logic

                                int len = find(TracksMissMat + i * 15, 15, 0, 999, NULL);
                                if (WIN_DLT < len) {
                                    double sum = 0;

                                    for (int j = 0; j < len - WIN_DLT; j++) {
                                        double d = TracksMissMat[i * 15 + j + WIN_DLT] -
                                                   TracksMissMat[i * 15 + j];
                                        if (d > 0) {
                                            sum += d;
                                        }
                                    }

                                    if (sum > DLT_TH) {
                                        TracksMat[i] = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        double* TracksMissMat3 = (double*)malloc(MAX_TARGET * NUM_OF_PINGS * sizeof(double*));
        double* TracksMissMat4 = (double*)malloc(MAX_TARGET * NUM_OF_PINGS * sizeof(double*));
        double* TracksMat0 = (double*)malloc(MAX_TARGET * sizeof(double*));
        double* TracksMat2 = (double*)malloc(MAX_TARGET * sizeof(double*));
        memcpy(TracksMissMat3, (TracksMissMat + 3 * 360 * 15), MAX_TARGET * NUM_OF_PINGS);
        memcpy(TracksMissMat4, (TracksMissMat + 4 * 360 * 15), MAX_TARGET * NUM_OF_PINGS);
        memcpy(TracksMat0, TracksMat, MAX_TARGET);
        memcpy(TracksMat2, TracksMat + 360 * 2, MAX_TARGET);

        CheckDetect(TracksMissMat3, TracksMissMat4, TracksMat0, TracksMat2, MIN_TRACKET_LEN, MAX_TARGET, NUM_OF_PINGS, ReportTrackRange, DetectFlagVec);

        free(TracksMissMat3);
        free(TracksMissMat4);
        free(TracksMat0);
        free(TracksMat2);

        // Update threshold if no detection
        double sum_DetectFlagVec = 0;
        for (size_t i = 0; i < MAX_TARGET; i++) {
            sum_DetectFlagVec += DetectFlagVec[i];
        }

        if (sum_DetectFlagVec == 0) {
            ImgTh = NewImgTh;
        }

        free(IndMat);
        free(rMat);
        free(TracksDataMat);
        free(TracksDataBinMat);
    }
    return 0;
};
