#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Merged_Detections_v2Roee.h"
#define PI 3.1415926535897

void ShortImgProc_v2Roee(double *IndVec, double *rVec, int lenRvec, double sigmaTeta, double *beam_dir_rad, double heading_deg,
                         double *merged_detections_pingRng, double *merged_detections_pingTeta, double *merged_detections_pingYc) {
    int len = lenRvec;  // length(rVec);

    double *DetectionsRng = (double *)calloc(len, sizeof(double));
    double *DetectionsTeta = (double *)calloc(len, sizeof(double));
    double *DetectionsYC = (double *)calloc(2 * len, sizeof(double));

    /*
    double DetectionsRng[len] = {0};
    double DetectionsTeta[len] = {0};
    double DetectionsYC[len * 2] = {0};
    */

    for (int nn = 0; nn < len; nn++) {
        double r = rVec[nn];
        int ind = IndVec[nn];

        double teta = beam_dir_rad[ind - 1] * (180.0 / PI) - heading_deg;

        double muc[2];
        double yc[2];

        muc[0] = r * sin(teta * PI / 180) * (exp(-sigmaTeta * sigmaTeta) - exp(-sigmaTeta * sigmaTeta / 2));
        muc[1] = r * cos(teta * PI / 180) * (exp(-sigmaTeta * sigmaTeta) - exp(-sigmaTeta * sigmaTeta / 2));
        yc[0] = r * sin(teta * PI / 180) - muc[0];
        yc[1] = r * cos(teta * PI / 180) - muc[1];

        DetectionsRng[nn] = r;
        DetectionsTeta[nn] = teta;
        DetectionsYC[nn] = yc[0];
        DetectionsYC[len + nn] = yc[1];
    }

    merged_detections(DetectionsRng, DetectionsTeta, DetectionsYC, len, merged_detections_pingRng, merged_detections_pingTeta, merged_detections_pingYc);

    free(DetectionsRng);
    free(DetectionsTeta);
    free(DetectionsYC);
}