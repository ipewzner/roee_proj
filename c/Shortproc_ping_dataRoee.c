#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "BeamformingRoee.h"
#include "CalcBinary.h"
#include "DetectionBlobsRoee.h"  // Include the bwconncomp function

#define SND_VEL 1500.0


// Main processing function
void Shortproc_ping_dataRoee(const float** PingData, const float* matched_filter, float Rmin, float fs, const float* azBeams, const float* pos_sensors, int pri_samples, float ImgTh, int FinalPingFlag, int UpdateThFlag, int** IndVec, float** rVec, float* NewImgTh) {
    // Variables
    int min_pixel = 1;
    int max_pixel = 10000;
    float rng_res = 1.0 / fs * SND_VEL / 2.0;
    
    // Step 1: Beamforming
    float** img = BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, fs, pri_samples);
    
    // Step 2: Calculate Binary Map or CFAR
    int** BinaryMap;
    if (UpdateThFlag) {
        BinaryMap = CalcBinary(img);
    } else {
        BinaryMap = DoCFAR(img, ImgTh);
    }
    
    // Step 3: Detect blobs
    int* CL = DetectionBlobsRoee(BinaryMap, min_pixel, max_pixel);
    
    // Step 4: Initialize output arrays
    if (CL != NULL) {
        int cl_length = sizeof(CL) / sizeof(CL[0]);
        *IndVec = (int*)malloc(cl_length * sizeof(int));
        *rVec = (float*)malloc(cl_length * sizeof(float));
        
        for (int nn = 0; nn < cl_length; nn++) {
            (*rVec)[nn] = Rmin + CL[nn] * rng_res;
            float max_val = -1;
            int ind = 0;
            for (int j = 0; j < pri_samples; j++) {
                if (img[(int)CL[nn]][j] > max_val) {
                    max_val = img[(int)CL[nn]][j];
                    ind = j;
                }
            }
            (*IndVec)[nn] = ind;
        }
    } else {
        *IndVec = NULL;
        *rVec = NULL;
    }
    
    // Step 5: Calculate new threshold if final ping
    if (FinalPingFlag) {
        *NewImgTh = CalcThCFAR(img);
    } else {
        *NewImgTh = ImgTh;
    }
    
    // Free memory if necessary
    // Free img, BinaryMap, and CL if dynamically allocated
}



// Example usage in main
int main() {
    // Example usage of Shortproc_ping_dataRoee
    float** PingData = NULL;   // Initialize with actual ping data
    float* matched_filter = NULL; // Initialize with matched filter data
    float Rmin = 0.0;
    float fs = 128000.0;
    float* azBeams = NULL;    // Initialize azimuth beams
    float* pos_sensors = NULL; // Initialize sensor positions
    int pri_samples = 3201;
    float ImgTh = 999.0;
    int FinalPingFlag = 0;
    int UpdateThFlag = 0;
    int* IndVec = NULL;
    float* rVec = NULL;
    float NewImgTh;

    // Call function
    Shortproc_ping_dataRoee((const float**)PingData, matched_filter, Rmin, fs, azBeams, pos_sensors, pri_samples, ImgTh, FinalPingFlag, UpdateThFlag, &IndVec, &rVec, &NewImgTh);
    
    // Free allocated memory
    free(IndVec);
    free(rVec);

    return 0;
}
