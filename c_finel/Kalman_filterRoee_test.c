
#include "Kalman_filterRoee.h"

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

//#define PI 3.14159

void Kalman_filterRoee(double *X_upd, double *P_upd, double *teta_rad, double *yc, double *Rc, double T, double *X, double *P);

// Function to check if two arrays are nearly equal (for float comparison)
bool arrays_are_close(double *arr1, double *arr2, int size, double tol) {
    for (int i = 0; i < size; i++) {
        if (fabs(arr1[i] - arr2[i]) > tol) {
            return false;
        }
    }
    return true;
}

// Function to check if two matrices are nearly equal (for float comparison)
bool matrices_are_close(double matrix1[4][4], double matrix2[4][4], int rows, int cols, double tol) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fabs(matrix1[i][j] - matrix2[i][j]) > tol) {
                return false;
            }
        }
    }
    return true;
}

void test_Kalman_filterRoee2() {
    // Given data from the Python test case
    double yc[] = {17.531440984772210, -12.348751288728593};
    double Rc[2][2] = {{2.250141679869500e+02,10.546310235130264},
                       {10.546310235130264,2.250141679866652e+02}};
    double T = 1.0;
    double X[] = {17.613770956247659, -12.406742662183950, 0.0, 0.0};
    double P[4][4] = {{1.135658379373986e+03, 53.227883536695956, 0.0, 0.0},
                      {53.227883536695956, 1.135658379372548e+03, 0.0, 0.0},
                      {0.0, 0.0, 5.0, 0.0},
                      {0.0, 0.0, 0.0, 5.0}};
    
    // Expected outputs
    double expected_X_upd[] = {17.545004278727038, -12.358303791918097, -0.000312021727852, 0.000226888797244};
    double expected_P_upd[4][4] = {
        {1.879398507316907e+02, 8.802294982144964, 0.823815205045379, 1.416157262703610e-04},
        {8.802294982144957, 1.879398507314530e+02, 1.416157262703888e-04, 0.823815205045375},
        {0.823815205045379, 1.416157262703862e-04, 4.981654003957392, 8.567220206341639e-04},
        {1.416157262703515e-04, 0.823815205045375, 8.567220206341639e-04, 4.981654003957369}};
    double expected_teta_rad = 2.184454078994398;

    // Variables to store the result
    double X_upd[4], P_upd[4][4], teta_rad;

    // Call the function
    Kalman_filterRoee(X_upd, (double *)P_upd, &teta_rad, yc, (double *)Rc, T, X, (double *)P);

    // Tolerance for float comparison
    double tolerance = 1e-8;

    // Check if the results are close enough to expected values
    if (fabs(teta_rad - expected_teta_rad) > tolerance) {
        printf("Test failed: teta_rad does not match expected value\n");
    } else if (!arrays_are_close(X_upd, expected_X_upd, 4, tolerance)) {
        printf("Test failed: X_upd does not match expected values\n");
    } else if (!matrices_are_close(P_upd, expected_P_upd, 4, 4, tolerance)) {
        printf("Test failed: P_upd does not match expected values\n");
    } else {
        printf("All tests passed!\n");
    }
}

void test_Kalman_filterRoee() {
    // Given data from the Python test case
    double yc[] = {21.042781868824051, 3.841971161392103};
    double Rc[2][2] = {{2.238971507800169e+02,-3.938379589950554},
                       {-3.938379589950554,2.238971507792297e+02}};
    double T = 1.0;
    double X[] = {21.161515372168633, 3.863649411852741, 0.0, 0.0};
    double P[4][4] = {{1.132149658268534e+03, -19.914702437256373, 0.0, 0.0},
                      {-19.914702437256373, 1.132149658264554e+03, 0.0, 0.0},
                      {0.0, 0.0, 5.0, 0.0},
                      {0.0, 0.0, 0.0, 5.0}};
    
    // Expected outputs
    double expected_X_upd[] = {21.062313709494028, 3.8455360465020005, -0.000437714640049, -0.000087309337809};
    double expected_P_upd[4][4] = {
        {1.870652549163168e+02, -3.288122964961385, 0.822517256582195, -5.314025859776139e-05},
        {-3.288122964961381, 1.870652549156597e+02, -5.314025859773364e-05, 0.822517256582185},
        {0.822517256582195, -5.314025859774572e-05, 4.981626151835971, -3.220118096084868e-04},
        {-5.314025859777465e-05, 0.822517256582185, -3.220118096084868e-04, 4.981626151835907}};
    double expected_teta_rad = 1.3902064596321329;

    // Variables to store the result
    double X_upd[4], P_upd[4][4], teta_rad;

    // Call the function
    Kalman_filterRoee(X_upd, (double *)P_upd, &teta_rad, yc, (double *)Rc, T, X, (double *)P);

    // Tolerance for float comparison
    double tolerance = 1e-8;

    // Check if the results are close enough to expected values
    if (fabs(teta_rad - expected_teta_rad) > tolerance) {
        printf("Test failed: teta_rad does not match expected value\n");
    } else if (!arrays_are_close(X_upd, expected_X_upd, 4, tolerance)) {
        printf("Test failed: X_upd does not match expected values\n");
    } else if (!matrices_are_close(P_upd, expected_P_upd, 4, 4, tolerance)) {
        printf("Test failed: P_upd does not match expected values\n");
    } else {
        printf("All tests passed!\n");
    }
}

int main() {
   // test_Kalman_filterRoee();
    test_Kalman_filterRoee2();
    return 0;
}

// (Insert the definition of Kalman_filterRoee here)

/*

 gcc Kalman_filterRoee_test.c -o Kalman_filterRoee_test.exe -lm
./Kalman_filterRoee_test.exe

*/