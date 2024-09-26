#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 3201  // Adjust these according to your image dimensions
#define M 72    // Adjust these according to your image dimensions

// Function to perform convolution
void convolve(const double* signal, int signal_len, const double* filter, int filter_len, double* result) {
    for (int n = 0; n < signal_len + filter_len ; n++) {
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
    double snd_vel = 1500;  // [m/sec]
    double v = 0;           // No elevation measurement
    double u, w, tau;
    int shift;

 
    // Initialize Beam matrix to zeros
    for (int i = 0; i < pri_samples * num_azBeams - 1; i++) {
        *(Beam + i) = 0.0;
    }

    // Beamforming (Delay and Sum)
    for (int m = 0; m < num_azBeams; m++) {
        u = sin(azBeams[m]);
        w = cos(azBeams[m]);

        double* data_beam = (double*)calloc(data_length, sizeof(double));

        for (int j = 0; j < data_length; j++) {
            data_beam[j] = 0.0;
        }

    

        for (int k = 0; k < num_sensors; k++) {
            tau = 1.0 / snd_vel * (pos_sensors[k + num_sensors * 0] * u + pos_sensors[k + num_sensors * 1] * w + pos_sensors[k + num_sensors * 2] * v);

            shift = round(tau * fs);

            if (shift > 0) {
                for (int j = 0; j < data_length - shift; j++) {
                    data_beam[j] += PingData[k * data_length + j + shift];
                }
            }else if (shift < 0) {
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
        int filter_length = 1281;  // data_length + filter_length - 1

        double* MF = (double*)calloc(data_length + filter_length, sizeof(double));
        /*
         pad = 2 ^ nextpow2(RefL + DataLen  - 1);
       Corr= ifft(fft(Data,pad).*fft(conj(fliplr(Ref)),pad));
        */
        convolve(data_beam, data_length, matched_filter, filter_length, MF);

     
        // Storing the magnitude and taking only pri_samples part
        for (int i = 0; i < pri_samples; i++) {
            Beam[i * M + m] = fabs(MF[i]);
        }

        free(data_beam);
        free(MF);
    }
}