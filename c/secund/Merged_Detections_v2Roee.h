#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void merged_detections(double *DetectionsRng, double *DetectionsTeta, double *DetectionsYC, int len, 
                       double *merged_detections_pingRng, double *merged_detections_pingTeta, double *merged_detections_pingYc) {
    double rng_merged_dis = 5.0;
    double teta_merged_dis = 3.0;

    // Initialize merged arrays with 999 (equivalent to infinity)
    for (int i = 0; i < len; i++) {
        merged_detections_pingRng[i] = 999;
        merged_detections_pingTeta[i] = 999;
        merged_detections_pingYc[2 * i] = 999;
        merged_detections_pingYc[2 * i + 1] = 999;
    }

    int *merged_flag = (int *)calloc(len, sizeof(int));

    for (int n = 0; n < len; n++) {
        if (n == len - 1) {
            if (merged_flag[n] != 1) {
                merged_detections_pingRng[n] = DetectionsRng[n];
                merged_detections_pingTeta[n] = DetectionsTeta[n];
                merged_detections_pingYc[2 * n] = DetectionsYC[2 * n];
                merged_detections_pingYc[2 * n + 1] = DetectionsYC[2 * n + 1];
            }
        } else {
            if (merged_flag[n] != 1) {
                for (int i = n + 1; i < len; i++) {
                    if (fabs(DetectionsRng[n] - DetectionsRng[i]) < rng_merged_dis && 
                        fabs(DetectionsTeta[n] - DetectionsTeta[i]) < teta_merged_dis) {
                        merged_flag[i] = 1;
                        // Averaging for merging
                        merged_detections_pingRng[n] = (merged_detections_pingRng[n] == 999) ? DetectionsRng[i] : 
                                                       (merged_detections_pingRng[n] + DetectionsRng[i]) / 2;
                        merged_detections_pingTeta[n] = (merged_detections_pingTeta[n] == 999) ? DetectionsTeta[i] : 
                                                        (merged_detections_pingTeta[n] + DetectionsTeta[i]) / 2;
                        merged_detections_pingYc[2 * n] = (merged_detections_pingYc[2 * n] == 999) ? DetectionsYC[2 * i] : 
                                                          (merged_detections_pingYc[2 * n] + DetectionsYC[2 * i]) / 2;
                        merged_detections_pingYc[2 * n + 1] = (merged_detections_pingYc[2 * n + 1] == 999) ? DetectionsYC[2 * i + 1] : 
                                                             (merged_detections_pingYc[2 * n + 1] + DetectionsYC[2 * i + 1]) / 2;
                    }
                }
                if (merged_detections_pingRng[n] == 999) {
                    merged_detections_pingRng[n] = DetectionsRng[n];
                    merged_detections_pingTeta[n] = DetectionsTeta[n];
                    merged_detections_pingYc[2 * n] = DetectionsYC[2 * n];
                    merged_detections_pingYc[2 * n + 1] = DetectionsYC[2 * n + 1];
                }
            }
        }
    }

    // Remove 999s (infinity) from final arrays
    int pos = 0;
    for (int i = 0; i < len; i++) {
        if (merged_detections_pingRng[i] != 999) {
            merged_detections_pingRng[pos] = merged_detections_pingRng[i];
            merged_detections_pingTeta[pos] = merged_detections_pingTeta[i];
            merged_detections_pingYc[2 * pos] = merged_detections_pingYc[2 * i];
            merged_detections_pingYc[2 * pos + 1] = merged_detections_pingYc[2 * i + 1];
            pos++;
        }
    }

    free(merged_flag);
}
