#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N 3201  // Adjust these according to your image dimensions
#define M 72    // Adjust these according to your image dimensions

// Function to perform convolution
void convolve(const double* signal, int signal_len, const double* filter, int filter_len, double* result) {
    int n;
    for (n = 0; n < signal_len + filter_len - 1; n++) {
        result[n] = 0;
        for (int k = 0; k < filter_len; k++) {
            if (n - k >= 0 && n - k < signal_len) {
                result[n] += signal[n - k] * filter[k];
            }
        }
    }
}

void BeamformingRoee(double* PingData, double* matched_filter, double* azBeams, double* pos_sensors, 
                     int fs, int pri_samples, double* Beam, int num_azBeams, int num_sensors, int data_length) {
    
    double snd_vel = 1500; // [m/sec]
    double v = 0; // No elevation measurement
    double u, w, tau;
    int shift;

    // Initialize Beam matrix to zeros
    for (int i = 0; i < pri_samples; i++) {
        for (int j = 0; j < num_azBeams; j++) {
            Beam[i*num_azBeams+j] = 0;
        }
    }

    // Beamforming (Delay and Sum)
    for (int m = 0; m < num_azBeams; m++) {
        u = sin(azBeams[m]);
        w = cos(azBeams[m]);

        double* data_beam = (double*)calloc(data_length, sizeof(double));

        for (int k = 0; k < num_sensors; k++) {
            tau = 1.0 / snd_vel * (pos_sensors[k*num_sensors + 0] * u + pos_sensors[k*num_sensors +1] * w + pos_sensors[k*num_sensors +2] * v);
            shift = round(tau * fs);

            if (shift > 0) {
                for (int j = 0; j < data_length - shift; j++) {
                    data_beam[j] += PingData[k*M+j + shift];
                }
            } else if (shift < 0) {
                for (int j = -shift; j < data_length; j++) {
                    data_beam[j] += PingData[k*M+j + shift];
                }
            } else {
                for (int j = 0; j < data_length; j++) {
                    data_beam[j] += PingData[k*M+j];
                }
            }
        }

        // Convolution with matched filter
        int filter_length = sizeof(matched_filter) / sizeof(matched_filter[0]);
        double* MF = (double*)calloc(data_length + filter_length - 1, sizeof(double));
        convolve(data_beam, data_length, matched_filter, filter_length, MF);

        // Storing the magnitude and taking only pri_samples part
        for (int i = 0; i < pri_samples; i++) {
            Beam[i*num_azBeams+m] = fabs(MF[i]);
        }

        free(data_beam);
        free(MF);
    }
}
/*
int main() {
    // Example usage with dummy data

    // Define your parameters like PingData, matched_filter, azBeams, pos_sensors here
    // For example:
    int fs = 20000; // Sampling frequency
    int pri_samples = 512; // Number of range samples
    int num_azBeams = 180; // Number of azimuth beams
    int num_sensors = 4; // Number of sensors
    int data_length = 1000; // Length of each sensor data

    // Initialize your data (for example purposes, here everything is set to zero)
    double** PingData = (double**)calloc(num_sensors, sizeof(double*));
    for (int i = 0; i < num_sensors; i++) {
        PingData[i] = (double*)calloc(data_length, sizeof(double));
    }

    double matched_filter[] = {1, 0.5, 0.25, 0.125}; // Example filter
    double azBeams[180]; // Example azimuth angles in radians
    for (int i = 0; i < num_azBeams; i++) {
        azBeams[i] = i * (M_PI / 180.0); // Convert to radians
    }

    double** pos_sensors = (double**)calloc(num_sensors, sizeof(double*));
    for (int i = 0; i < num_sensors; i++) {
        pos_sensors[i] = (double*)calloc(3, sizeof(double)); // Assuming 3D position (x, y, z)
    }

    double** Beam = (double**)calloc(pri_samples, sizeof(double*));
    for (int i = 0; i < pri_samples; i++) {
        Beam[i] = (double*)calloc(num_azBeams, sizeof(double));
    }

    // Run the beamforming algorithm
    BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, fs, pri_samples, Beam, num_azBeams, num_sensors, data_length);

    // Free allocated memory
    for (int i = 0; i < num_sensors; i++) {
        free(PingData[i]);
        free(pos_sensors[i]);
    }
    free(PingData);
    free(pos_sensors);

    for (int i = 0; i < pri_samples; i++) {
        free(Beam[i]);
    }
    free(Beam);

    return 0;
}
*/