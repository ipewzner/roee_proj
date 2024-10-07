#include "helperFunction.h"

#ifndef CONFIG_H
#define CONFIG_H

#define M_S 72
#define N_S 3201

// Set Parameters
#define NUM_OF_PINGS 15
#define RMIN 15  // [m]

// General parameters
#define DLT_TH 7
#define MEDIAN_FILTER_SIZE 101
#define PRI_SAMPLES 3201  // rf_samples - r0_samples + 1
#define IMG_TH 999
#define T_PRI 1  // [sec]
#define SIGMA_TETA 3
#define WIN_DLT 10          // 20
#define MIN_TRACKET_LEN 10  // Number of detected pings for valid target

#define PI 3.14159265358979323846

//--------------------
// Array configuration
//--------------------
#define NS 4                          // Number of hydrophones
#define RS 0.255                      // Radius of array [m], sibenik, subnero
#define TETA_ARRAY {90, 0, -90, 180}  // Array angles linspace(0,2*pi-2*pi/Ns,Ns);
// Calculate POS_SENSORS array based on the given angles and RS value
// pos_sensors=rs.*[sind(teta_array);cosd(teta_array);zeros(1,Ns)]; % 3 X Ns
/*
#define POS_SENSORS_RS {RS * sin(90 * M_PI / 180.0), RS * sin(0 * M_PI / 180.0), RS * sin(-90 * M_PI / 180.0), RS * sin(180 * M_PI / 180.0), \
                        RS * cos(90 * M_PI / 180.0), RS * cos(0 * M_PI / 180.0), RS * cos(-90 * M_PI / 180.0), RS * cos(180 * M_PI / 180.0), \
                        0, 0, 0, 0}
*/
#define POS_SENSORS_RS {RS, 0, -RS, 0, 0, RS, 0, -RS, 0, 0, 0, 0}

//----------------
// Beam parameters
//----------------
#define RATIO 5
#define NAZ (360 / RATIO)  // Number of azimuth beams

/*
void linspace(double start, double end, int num, double* result) {
    double step = (end - start) / (num - 1);
    for (int i = 0; i < num; i++) {
        result[i] = start + i * step;
    }
}

// AZ_BEAMS would be a computed array; this is a placeholder
#define AZ_BEAMS linspace(0, 2 * PI - 2 * PI / (NAZ * RATIO), NAZ);
*/

#define MAX_TARGET 360

//--------------------
// Waveform parameters
//--------------------
#define FS 128000   // [Hz]  //128e3
#define FMAX 41000  // 41e3
#define FMIN 31000  // 31e3
#define T_P 0.01    // [sec] Pulse duration

#define T_SAMP_SIZE (int)(T_P * FS) + 1
#define T_SAMP arrayMaker(0, 1.0 / FS, T_P, T_SAMP_SIZE)
#define LFM compute_lfm(T_SAMP, T_SAMP_SIZE,FMIN,FMAX,T_P)
#define MATCHED_FILTER create_matched_filter(LFM,T_SAMP_SIZE)

// Other parameters
#define TGAURD 1     // [sec] delay between each ping
//#define E 0.0067394  // e=sqrt(0.0067394);
#define E sqrt(0.0067394)
#define RE 6378135
#define T0 150939  // for 17022024

#endif  // CONFIG_H

/*
Notes:
    TETA_ARRAY: Represents the angles of the hydrophones.

    POS_SENSORS_RS: Uses trigonometric functions to
        calculate the sensor positions based on the angles and radius RS.

    AZ_BEAMS, T_SAMP, LFM, MATCHED_FILTER: These would normally be
        arrays that are calculated based on the parameters provided.
        Since arrays and their calculations are complex in C,
        you would typically compute these in your code rather than defining them in a header file.

You would include this header file in your C project to define these parameters globally
*/