#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BeamformingRoee_old.h"
#include "CalcBinary.h"
#include "CalcThCFAR.h"
#include "DetectionBlobsRoee.h"  // Include the bwconncomp function
#include "DoCFAR.h"

// Define constants
#define MIN_PIXEL 1
#define MAX_PIXEL 1e4
#define SND_VEL 1500

// rng_res=1/fs*snd_vel/2;

int find_max_index(double* array, int size) {
    int max_index = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] > array[max_index]) {
            max_index = i;
        }
    }
    return max_index;
}

int find_max_index_in_row(double* img, int row_index, int num_cols) {
    int max_index = 0;
    double max_value = img[row_index * num_cols];  // Start with the first element in the row

    for (int col = 1; col < num_cols; col++) {
        double value = img[row_index * num_cols + col];  // Access the next column in the row
        if (value > max_value) {
            max_value = value;
            max_index = col;  // Update the max index
        }
    }

    return max_index;  // Return the column index of the maximum value in this row
}

void process_data(int* CL, int CL_size, double* img, int img_rows, int img_cols, double* IndVec) {
    for (int nn = 0; nn < CL_size; nn++) {
        int row_index = (int)floor(CL[nn])-1;  // Convert CL[nn] to integer (row index)

        // Ensure the row index is within bounds
        if (row_index >= 0 && row_index < img_rows) {
            // Find the column index of the maximum value in the specified row of img
            IndVec[nn] = find_max_index_in_row(img, row_index, img_cols);
            //IndVec[nn] = find_max_index((img+row_index*72), img_cols);
        } else {
            IndVec[nn] = -1;  // If out of bounds, mark as invalid
        }
    }
}

// Main function conversion
void Shortproc_ping_dataRoee(double* img, double* IndVec, double* rVec, double* NewImgTh,
                             double* PingData, double* matched_filter,
                             double Rmin, double fs, double* azBeams, int num_azBeams,
                             double* pos_sensors, int pri_samples,
                             double ImgTh, int FinalPingFlag, int UpdateThFlag, int num_sensors, int data_length) {
    double rng_res = (1 / fs) * SND_VEL / 2;

    // img will store the result from BeamformingRoee (assuming img has been flattened)
    // double* img = (double*)malloc(N*M * sizeof(double));
    // BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, fs, pri_samples, img, num_azBeams, num_sensors, data_length);

    // BinaryMap array
    // double* BinaryMap = (double*)malloc(pri_samples * sizeof(double));
    int* BinaryMap = (int*)malloc(N * M * sizeof(int));

    // Update threshold flag
    if (UpdateThFlag) {
        CalcBinary(img, BinaryMap);
    } else {
        // DoCFAR(BinaryMap, img, ImgTh);
        DoCFAR(img, ImgTh, pri_samples, num_azBeams, BinaryMap);
    }

    // Detection blobs
    // double* CL = (double*)malloc(pri_samples * sizeof(double));
    int* CL = (int*)malloc(pri_samples * sizeof(int));
    int num_blobs = 0;
    DetectionBlobsRoee(BinaryMap, pri_samples, num_azBeams, MIN_PIXEL, MAX_PIXEL, CL, &num_blobs);

    
      printf("\nResult:\n");
     for (int i = 0; i < num_blobs; i++) {
         printf("%d ", CL[i]);  // Correct format specifier for int
     }
   

    if (num_blobs > 0) {

        for (int i = 0; i < num_blobs; i++) {
            IndVec[i] = 0;
            rVec[i] = 0.0;
        }

        for (int i = 0; i < num_blobs; i++) {
            // Calculate rVec
            rVec[i] = Rmin + CL[i] * rng_res;
/*
            // Find the index of the maximum value in the row of img corresponding to floor(CL[i])
            int row_index = (int)floor(CL[i]);  // Convert CL[i] to integer for array indexing
            if (row_index >= 0 && row_index < N) {
                IndVec[i] = find_max_index(&img[row_index * M], M);  // Get max index for row
            } else {
                IndVec[i] = -1;  // Invalid index, handle accordingly
            }
            */
//IndVec(nn) = ind;
        }
    } else {
        // If CL is empty, set IndVec and rVec to NULL or clear their contents
        // In C, you cannot set arrays to empty like in MATLAB, but you can set their size to 0 or use NULL pointers
        //IndVec = NULL;
        rVec = NULL;
    }
               process_data(CL, num_blobs, img, N, M, IndVec);

    // Final ping flag to calculate threshold for the next iteration
    if (FinalPingFlag) {
        // CalcThCFAR(NewImgTh, img);
        NewImgTh = CalcThCFAR(img, pri_samples, 111);  // num_azBeams);

    } else {
        *NewImgTh = ImgTh;
    }

    // Free allocated memory
    free(img);
   // free(BinaryMap);
    //free(CL);
}