#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void cov_plot_calcRoee(double r, double teta, double sigmaTeta, double* rc) {
    double sigmaR = 0.3;
    double sigmaR2 = sigmaR * sigmaR;
    double sigmaTeta2 = sigmaTeta*sigmaTeta;
    double exp_term = exp(-2 * sigmaTeta2);
    double cosh_term = cosh(2 * sigmaTeta2) - cosh(sigmaTeta2);
    double sinh_term = sinh(2 * sigmaTeta2) - sinh(sigmaTeta2);
    double r2 = r * r;
    double cosT2 = cos(teta) * cos(teta);
    double sinT2 = sin(teta) * sin(teta);

    rc[0] = r2 * exp_term * (cosT2 * (cosh_term) + sinT2 * (sinh_term)) + sigmaR2 * exp_term * (cosT2 * (2 * cosh_term) + sinT2 * (2 * sinh_term));

    rc[3] = r2 * exp_term * (sinT2 * (cosh_term) + cosT2 * (sinh_term)) + sigmaR2 * exp_term * (sinT2 * (2 * cosh_term) + cosT2 * (2 * sinh_term));

    rc[1] = sin(teta) * cos(teta) * exp(-4 * sigmaTeta) * (sigmaR2 + (r2 + sigmaR2) * (1 - exp(sigmaTeta2)));
    rc[2] = rc[1];  // Fill in the off-diagonal element
}