#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void CheckDetect(
    double* TracksX, double* TracksY, int* Tracksdlt_flag, int* TracksStat_id, 
    double* ReportTrackRange, int* DetectFlagVec, int MinTracketLen, int numTracks, int trackLength) {

    // Step 1: Find indices where Tracksdlt_flag == 0 and TracksStat_id == 1
    int loc[numTracks];
    int locCount = 0;
    for (int i = 0; i < numTracks; i++) {
        if (Tracksdlt_flag[i] == 0 && TracksStat_id[i] == 1) {
            loc[locCount++] = i;
        }
    }

    // Step 2: Initialize ReportTrackRange and DetectFlagVec
    for (int i = 0; i < locCount; i++) {
        ReportTrackRange[i] = 0;
        DetectFlagVec[i] = 0;
    }

    // Step 3: Loop through the found locations
    for (int TrackInd = 0; TrackInd < locCount; TrackInd++) {
        int trackIndex = loc[TrackInd];

        // Step 4: Find positions where TracksX[trackIndex] != 999
        int pos = -1;
        for (int j = 0; j < trackLength; j++) {
            //if (TracksX[trackIndex * trackLength + j] != 999) {
            if (TracksX[trackIndex * 360 + j] != 999) {
                pos = j;  // Keep track of the last valid position
            }
        }

        // Step 5: Check if the track length is greater than MinTracketLen
        if (pos != -1 && (pos + 1) > MinTracketLen) {
            // Calculate the track range
            double x = TracksX[trackIndex * trackLength + pos];
            double y = TracksY[trackIndex * trackLength + pos];
            ReportTrackRange[TrackInd] = sqrt(x * x + y * y);

            // Set the DetectFlagVec for this track
            DetectFlagVec[trackIndex] = 1;
        }
    }
}
