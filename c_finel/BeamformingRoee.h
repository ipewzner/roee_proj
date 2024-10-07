#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fftw3.h>
#include <complex.h> 

#define N 3201  // Adjust these according to your image dimensions
#define M 72    // Adjust these according to your image dimensions


void fft_convolve(const double* signal, int signal_len, const double* filter, int filter_len, double* result) {
    int n = signal_len + filter_len - 1;
    
    // Allocate memory for input and output arrays
    fftw_complex *in_signal = fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex *in_filter = fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex *out_signal = fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex *out_filter = fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex *out_result = fftw_malloc(sizeof(fftw_complex) * n);
    
    // Initialize input arrays (zero padding)
    for (int i = 0; i < n; i++) {
        in_signal[i][0] = (i < signal_len) ? signal[i] : 0.0;  // Real part
        in_signal[i][1] = 0.0;  // Imaginary part
        in_filter[i][0] = (i < filter_len) ? filter[i] : 0.0;  // Real part
        in_filter[i][1] = 0.0;  // Imaginary part
    }

    // Create FFT plans
    fftw_plan p_signal = fftw_plan_dft_1d(n, in_signal, out_signal, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan p_filter = fftw_plan_dft_1d(n, in_filter, out_filter, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan p_result = fftw_plan_dft_1d(n, out_result, in_signal, FFTW_BACKWARD, FFTW_ESTIMATE);

    // Perform FFT on both the signal and the filter
    fftw_execute(p_signal);
    fftw_execute(p_filter);

    // Multiply the two FFT results (pointwise multiplication)
    for (int i = 0; i < n; i++) {
        out_result[i][0] = out_signal[i][0] * out_filter[i][0] - out_signal[i][1] * out_filter[i][1];  // Real part
        out_result[i][1] = out_signal[i][0] * out_filter[i][1] + out_signal[i][1] * out_filter[i][0];  // Imaginary part
    }

    // Perform the inverse FFT to get the convolved result
    fftw_execute(p_result);

    // Normalize and copy the real part of the result to the output array
    for (int i = 0; i < n; i++) {
        result[i] = in_signal[i][0] / n;  // Divide by n to normalize
    }

    // Cleanup
    fftw_destroy_plan(p_signal);
    fftw_destroy_plan(p_filter);
    fftw_destroy_plan(p_result);
    fftw_free(in_signal);
    fftw_free(in_filter);
    fftw_free(out_signal);
    fftw_free(out_filter);
    fftw_free(out_result);
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

        printf("\n data_length + filter_length-1: %d",data_length + filter_length-1);
      
        fft_convolve(data_beam,  data_length, matched_filter, 1281,MF);

        // Storing the magnitude and taking only pri_samples part
        for (int i = 0; i < pri_samples; i++) {
            Beam[i * M + m] = fabs(MF[i]);
        }

        free(data_beam);
        free(MF);
    }
}