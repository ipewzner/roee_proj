#include <stdio.h>
#include <math.h>
#include <string.h>
#include "AuctionRoee.h"
#include "Kalman_filterRoee.h"
#include "cov_plot_calcRoee.h"

#define MAX_TRACKS 100
#define MAX_PLOTS 100

// Function prototypes for imported functions from MATLAB
void AuctionRoee(float *cost_matrix, int n_tracks, int n_plots, int *assignment);
void Kalman_filterRoee(float *x, float *P, float *z, int track_idx);
void cov_plot_calcRoee(float *cov_matrix, float *plot);

void ping_to_ping_associationRoee(
    int nPlots,
    int nTracks,
    float *costMatrix, 
    float *covarianceMatrix,
    float *tracksX, 
    float *tracksP,
    float *plots,
    int *trackAssignment,
    int *track2delete,
    int *tracksMissMat,
    float *trackLogLikelihood,
    float likelihood_threshold)
{
    // Step 1: Auction Algorithm for assigning tracks to plots
    int assignment[MAX_TRACKS];
    AuctionRoee(costMatrix, nTracks, nPlots, assignment);

    // Step 2: Updating Kalman Filter and covariance for each track
    for (int trackIdx = 0; trackIdx < nTracks; ++trackIdx) {
        int plotIdx = assignment[trackIdx];  // Get assigned plot index for this track

        if (plotIdx >= 0) {
            // If track is assigned to a plot, update it using Kalman filter
            Kalman_filterRoee(tracksX, tracksP, plots, trackIdx);
            cov_plot_calcRoee(covarianceMatrix, &plots[plotIdx * 2]);

            // Reset miss matrix for this track
            tracksMissMat[trackIdx] = 0;

            // Calculate log likelihood for the assigned track-plot pair
            float likelihood = 0.0;  // Example calculation, adjust as per your logic
            trackLogLikelihood[trackIdx] = likelihood;
        } else {
            // If not assigned, increase miss matrix count
            tracksMissMat[trackIdx]++;
        }

        // Step 3: Mark for deletion if miss count exceeds threshold
        if (tracksMissMat[trackIdx] > likelihood_threshold) {
            track2delete[trackIdx] = 1;
        }
    }

    // Step 4: Handle unassigned plots (e.g., create new tracks)
    for (int plotIdx = 0; plotIdx < nPlots; ++plotIdx) {
        int assigned = 0;
        for (int i = 0; i < nTracks; ++i) {
            if (assignment[i] == plotIdx) {
                assigned = 1;
                break;
            }
        }
        if (!assigned) {
            // Handle unassigned plot (e.g., initialize new track)
            // Add logic for creating a new track
            printf("Plot %d not assigned, consider creating a new track.\n", plotIdx);
        }
    }

    // Step 5: Final track deletion and cleanup
    for (int trackIdx = 0; trackIdx < nTracks; ++trackIdx) {
        if (track2delete[trackIdx] == 1) {
            // Mark track for deletion
            printf("Track %d marked for deletion.\n", trackIdx);
        }
    }
}

