#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sndfile.h>

#define FS 128000  // Sampling rate [Hz]
#define RINGDOWN (0.001 * FS)  // Ringdown calculation
#define CHANNELS 4

// Function prototype
void GetDataFromRam(double pri_samples, int n, double Tgaurd, double PingData[CHANNELS][(int)pri_samples]);

// Helper function to read WAV file data
double* readWavFile(const char* filename, sf_count_t* numFrames) {
    SF_INFO sfInfo;
    SNDFILE *file = sf_open(filename, SFM_READ, &sfInfo);
    if (!file) {
        printf("Error: unable to open file %s\n", filename);
        return NULL;
    }

    *numFrames = sfInfo.frames;
    double *data = (double*)malloc(sfInfo.channels * sfInfo.frames * sizeof(double));
    if (sf_read_double(file, data, sfInfo.channels * sfInfo.frames) < sfInfo.channels * sfInfo.frames) {
        printf("Error: unable to read file %s\n", filename);
        free(data);
        return NULL;
    }
    sf_close(file);
    return data;
}

// Function to get data from RAM
void GetDataFromRam(double pri_samples, int n, double Tgaurd, double PingData[CHANNELS][(int)pri_samples]) {
    int FolderName = 13;
    char FileName[50];
    double* SigMat[CHANNELS] = { NULL };
    sf_count_t numFrames;

    // Reading files
    for (int ChInd = 0; ChInd < CHANNELS; ChInd++) {
        sprintf(FileName, "/00%d_%d.wav", FolderName, ChInd + 1);
        SigMat[ChInd] = readWavFile(FileName, &numFrames);
        if (SigMat[ChInd] == NULL) {
            printf("Error reading file: %s\n", FileName);
            return;
        }
    }

    // Threshold calculation
    double meanSig = 0, stdSig = 0, Th;
    for (sf_count_t i = 0; i < numFrames; i++) {
        meanSig += SigMat[0][i];
    }
    meanSig /= numFrames;
    
    for (sf_count_t i = 0; i < numFrames; i++) {
        stdSig += pow(SigMat[0][i] - meanSig, 2);
    }
    stdSig = sqrt(stdSig / numFrames);
    
    Th = meanSig + 10 * stdSig;

    // Locate threshold exceedances
    sf_count_t* loc = (sf_count_t*)malloc(numFrames * sizeof(sf_count_t));
    int locCount = 0;
    for (sf_count_t i = 0; i < numFrames; i++) {
        if (fabs(SigMat[0][i]) > Th) {
            loc[locCount++] = i;
        }
    }

    // Find the nth ping
    int CurrentLocPos = 1;
    int PingCount = 0;
    sf_count_t PingLoc = 0;
    while (CurrentLocPos < locCount) {
        if (loc[CurrentLocPos] - loc[CurrentLocPos - 1] > Tgaurd * 3 / 4) {
            PingCount++;
            if (PingCount == n) {
                PingLoc = loc[CurrentLocPos - 1] + 1;
                break;
            }
        }
        CurrentLocPos++;
    }

    // Copy Ping data from SigMat to PingData
    for (int i = 0; i < CHANNELS; i++) {
        for (int j = 0; j < pri_samples; j++) {
            PingData[i][j] = SigMat[i][PingLoc + RINGDOWN + j];
        }
    }

    // Free allocated memory
    for (int i = 0; i < CHANNELS; i++) {
        free(SigMat[i]);
    }
    free(loc);
}
