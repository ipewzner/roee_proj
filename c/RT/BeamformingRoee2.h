#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 3201  // Adjust according to image dimensions
#define M 72    // Adjust according to image dimensions

// Function to perform convolution
void convolve(const double* signal, int signal_len, const double* filter, int filter_len, double* result) {
    for (int n = 0; n < signal_len + filter_len - 1; n++) {
        result[n] = 0;
        for (int k = 0; k < filter_len; k++) {
            if (n - k >= 0 && n - k < signal_len) {
                result[n] += signal[n - k] * filter[k];
            }
        }
    }
}

// Beamforming function
void BeamformingRoee(double* PingData, double* matched_filter, double* azBeams, double* pos_sensors,
                     int fs, int pri_samples, double* Beam, int num_azBeams, int num_sensors, int data_length) {
    double snd_vel = 1500;  // [m/sec]
    double v = 0;           // No elevation measurement
    double u, w, tau;
    int shift;

    // Initialize Beam matrix to zeros
    memset(Beam, 0, pri_samples * num_azBeams * sizeof(double));

    // Beamforming (Delay and Sum)
    for (int m = 0; m < num_azBeams; m++) {
        u = sin(azBeams[m]);
        w = cos(azBeams[m]);

        double* data_beam = (double*)calloc(data_length, sizeof(double));

        for (int k = 0; k < num_sensors; k++) {
            tau = 1.0 / snd_vel * (pos_sensors[k * 3] * u + pos_sensors[k * 3 + 1] * w + pos_sensors[k * 3 + 2] * v);
            shift = round(tau * fs);

            if (shift > 0) {
                for (int j = 0; j < data_length - shift; j++) {
                    data_beam[j] += PingData[k * data_length + j + shift];
                }
            } else if (shift < 0) {
                for (int j = 0; j < data_length + shift; j++) {
                    data_beam[j - shift] += PingData[k * data_length + j];
                }
            } else {
                for (int j = 0; j < data_length; j++) {
                    data_beam[j] += PingData[k * data_length + j];
                }
            }
        }

        // Convolution with matched filter
        int filter_length = 1281;  // Assuming filter length
        double* MF = (double*)calloc(data_length + filter_length - 1, sizeof(double));
        convolve(data_beam, data_length, matched_filter, filter_length, MF);

        // Store the magnitude of the convolution result into Beam
        for (int i = 0; i < pri_samples; i++) {
            Beam[i * num_azBeams + m] = fabs(MF[i]);
        }

        free(data_beam);
        free(MF);
    }
}
