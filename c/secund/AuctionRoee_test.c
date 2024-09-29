#include "AuctionRoee.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// Function declarations (add your AuctionRoee function and helper functions here)

// Helper function to compare two arrays and check if they are equal
int arrays_equal(int *arr1, int *arr2, int length) {
    for (int i = 0; i < length; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;  // Not equal
        }
    }
    return 1;  // Equal
}
/*
int arrays_equal(double *arr1, int *arr2, int length) {
    for (int i = 0; i < length; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;  // Not equal
        }
    }
    return 1;  // Equal
}

*/

// Test function for AuctionRoee
void test_AuctionRoee() {
    // The input matrix A (from the Python example)
    double A[4][6] = {
        {50.000010772821263, 10000, 10000, 10000, 10000, 10000},
        {10000, 10000, 50.002659693357657, 10000, 10000, 50.038823749619070},
        {10000, 50.000007769266581, 10000, 10000, 50.032959921056111, 10000},
        {10000, 10000, 10000, 50.016144755473334, 10000, 10000}};

    // Expected assigned array (from the Python example)
    int expected_assigned[6] = {0, 2, 1, 3, -1, -1};
    // int expected_assigned[6] = {1, 3, 2, 4, 0, 0};

    // Output array for the result of the AuctionRoee function
    // int Assigned[6] = {0};  // Initialize as unassigned
    int *Assigned = (int *)malloc(6 * sizeof(int));
    for (size_t i = 0; i < 6; i++) {
        Assigned[i] = -1;
    }

    // Call the AuctionRoee function
    AuctionRoee((double *)A, 4, 6, Assigned);

    printf("\nAssigned: ");
    for (size_t i = 0; i < 6; i++) {
        printf("%d ", Assigned[i]);
    }
    printf("\nexpected: ");

    for (size_t i = 0; i < 6; i++) {
        printf("%d ", expected_assigned[i]);
    }

    // Check if the result matches the expected assigned array
    if (arrays_equal(Assigned, expected_assigned, 6)) {
        printf("\nTest passed!\n");
    } else {
        printf("\nTest failed: The result does not match the expected values.\n");
    }
}

int main() {
    // Run the test for AuctionRoee
    test_AuctionRoee();
    return 0;
}

/*

 gcc AuctionRoee_test.c -o AuctionRoee_test.exe -lm
./AuctionRoee_test.exe

*/