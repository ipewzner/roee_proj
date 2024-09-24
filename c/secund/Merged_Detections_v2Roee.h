#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #define TEST

#ifdef TEST
void merged_detections(double *DetectionsRng, double *DetectionsTeta, double *DetectionsYC, int len,
                       double *merged_detections_pingRng, double *merged_detections_pingTeta, double *merged_detections_pingYc) {
    int *merged_flag = (int *)calloc(len, sizeof(int));
#else
void merged_detections(double *DetectionsRngT, double *DetectionsTetaT, double *DetectionsYCT, int len,
                       double *merged_detections_pingRngT, double *merged_detections_pingTetaT, double *merged_detections_pingYcT) {
    // ... original function body ...

    // Create new variables with appended "T" and deep copy the contents
    double DetectionsRng[len];

    double DetectionsTeta[len];
    double merged_detections_pingRng[len];
    double merged_detections_pingTeta[len];
    double merged_detections_pingYc[2 * len];
    double DetectionsYC[2 * len];

    for (int i = 0; i < len; i++) {
        DetectionsRng[i] = DetectionsRngT[i];
        DetectionsTeta[i] = DetectionsTetaT[i];
        merged_detections_pingRng[i] = merged_detections_pingRngT[i];
        merged_detections_pingTeta[i] = merged_detections_pingTetaT[i];
    }

    for (int i = 0; i < 2 * len; i++) {
        merged_detections_pingYc[i] = merged_detections_pingYcT[i];
        DetectionsYC[i] = DetectionsYCT[i];
    }
    int merged_flag[len];
#endif

    double rng_merged_dis = 5.0;
    double teta_merged_dis = 3.0;

    // Initialize merged arrays with 999 (equivalent to infinity)
    for (int i = 0; i < len; i++) {
        merged_detections_pingRng[i] = 999;
        merged_detections_pingTeta[i] = 999;
        merged_detections_pingYc[2 * i] = 999;
        merged_detections_pingYc[2 * i + 1] = 999;
        merged_flag[i] = 0;
    }

    for (int n = 0; n < len; n++) {
        if (n == len - 1) {
            if (merged_flag[n] != 1) {
                merged_detections_pingRng[n] = DetectionsRng[n];
                merged_detections_pingTeta[n] = DetectionsTeta[n];

                /*
                 merged_detections_pingYc[2 * n] = DetectionsYC[2 * n];
                 merged_detections_pingYc[2 * n + 1] = DetectionsYC[2 * n + 1];
                */

                // merged_detections_pingYc(:, n) = DetectionsYC(:, n);
                for (int i = 0; i < 2; i++) {
                    merged_detections_pingYc[i * len + n] = DetectionsYC[i * len + n];
                }
            }
        } else {
            if (merged_flag[n] != 1) {
                int ind[len];       // Array to store indices
                int ind_count = 0;  // Counter for valid indices

                for (int i = n + 1; i < len; i++) {
                    if (fabs(DetectionsRng[n] - DetectionsRng[i]) < rng_merged_dis &&
                        fabs(DetectionsTeta[n] - DetectionsTeta[i]) < teta_merged_dis) {
                        ind[ind_count++] = i - n;  // Store valid index
                        merged_flag[i] = 1;        // Mark merged detection
                    }
                }

                // Process if ind array is not empty
                if (ind_count > 0) {
                    // Compute means using ind array
                    double sum_rng = 0;   // DetectionsRng[n];
                    double sum_teta = 0;  // DetectionsTeta[n];
                    for (int k = 0; k < ind_count; k++) {
                        sum_rng += DetectionsRng[n + ind[k]];
                        sum_teta += DetectionsTeta[n + ind[k]];
                    }
                    merged_detections_pingRng[n] = sum_rng / (ind_count);
                    merged_detections_pingTeta[n] = sum_teta / (ind_count);

                    // Handle merged_detections_pingYc for each index
                    for (int j = 0; j < 2; j++) {
                        double sum_yc = 0;  // DetectionsYC[n * 2 + j];
                        for (int k = 0; k < ind_count; k++) {
                            sum_yc += DetectionsYC[n + ind[k] + j * len];
                        }
                        merged_detections_pingYc[n + j * len] = sum_yc / (ind_count);
                    }
                    printf("M");
                } else {
                    merged_detections_pingRng[n] = DetectionsRng[n];
                    merged_detections_pingTeta[n] = DetectionsTeta[n];
                    merged_detections_pingYc[n] = DetectionsYC[n];
                    merged_detections_pingYc[n + 24] = DetectionsYC[n + 24];
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
            merged_detections_pingYc[pos] = merged_detections_pingYc[i];
            merged_detections_pingYc[pos + len] = merged_detections_pingYc[i + len];
            pos++;
        }
    }
    for (size_t i = pos; i < len; i++) {
        merged_detections_pingRng[i] = 0;
        merged_detections_pingTeta[i] = 0;
        merged_detections_pingYc[i] = 0;
        merged_detections_pingYc[i + len] = 0;
    }

    printf("\n");

#ifdef TEST

    free(merged_flag);
#endif
}
