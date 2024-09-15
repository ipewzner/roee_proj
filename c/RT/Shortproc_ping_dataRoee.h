#include "BeamformingRoee.h"
#include "CalcBinary.h"
#include "DetectionBlobsRoee.h"  // Include the bwconncomp function
#include "DoCFAR.h"
#include "CalcThCFAR.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define constants or parameters as needed
#define SND_VEL 1500

void Shortproc_ping_dataRoee(double* PingData, double* matched_filter, double* azBeams, double* pos_sensors, 
                             double Rmin, int fs, int pri_samples, double* ImgTh, int FinalPingFlag, int UpdateThFlag, 
                             double* IndVec, double* rVec, double* NewImgTh, int num_azBeams, int num_sensors, int data_length) {
    
    int min_pixel = 1;
    int max_pixel = 10000;
    double rng_res = 1.0 / fs * SND_VEL / 2;

    // Beamforming result
    double* img = (double*)calloc(pri_samples * num_azBeams, sizeof(double));
    BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, fs, pri_samples, img, num_azBeams, num_sensors, data_length);

    int* BinaryMap = (int*)calloc(pri_samples * num_azBeams, sizeof(int));

    // Perform threshold update or CFAR processing
    if (UpdateThFlag) {
         CalcBinary( img,BinaryMap);
    } else {
        DoCFAR(img, ImgTh, pri_samples, num_azBeams, BinaryMap);
    }

    int* CL;
    int NumObjects = 0;
    DetectionBlobsRoee(BinaryMap, pri_samples, num_azBeams, min_pixel, max_pixel, &CL, &NumObjects);

    if (NumObjects > 0) {
        for (int nn = 0; nn < NumObjects; nn++) {
            rVec[nn] = Rmin + CL[nn] * rng_res;
            int max_index = 0;
            for (int i = 0; i < num_azBeams; i++) {
                if (img[(int)CL[nn] * num_azBeams + i] > img[(int)CL[nn] * num_azBeams + max_index]) {
                    max_index = i;
                }
            }
            IndVec[nn] = max_index;
        }
    } else {
        // If no objects are found, clear the result arrays
        for (int i = 0; i < NumObjects; i++) {
            IndVec[i] = 0;
            rVec[i] = 0;
        }
    }

    // If FinalPingFlag is set, calculate the new threshold
    if (FinalPingFlag) {
        //*NewImgTh = CalcThCFAR(img, pri_samples, num_azBeams);
        NewImgTh = CalcThCFAR(img, pri_samples, num_azBeams);
    } else {
        *NewImgTh = *ImgTh;
    }

    // Clean up
    free(img);
    free(BinaryMap);
    free(CL);
}
