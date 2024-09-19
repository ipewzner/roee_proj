import numpy as np

import cov_plot_calcRoee as cpc
import std_calcRoee as scr

def ping_to_ping_correlationRoee(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat,
                                 MergedRng, MergedTeta, MergedYc, NumDetect, ping_ind, Tping, sigmaTeta):
    """
    This function gets tracks and detections (from current ping) and calculates the gating.

    OUTPUT
    'uncorr_plots_list' - list of plots that were not correlated (and will open new tracks)
    """

    CNF_TH = 10
    rng_dist = 10
    teta_dist = 10
    Penalty = 50
    gamma_corr = 0.1

    MaxTarget = TracksDataBinMat.shape[1]
    ass_plots_flag = np.zeros(NumDetect)  # vector indicating whether a plot was correlated

    # Initialization
    pos = np.where(TracksMat[0, :] != 999)[0]
    TracksDataBinMat[pos, :] = np.zeros((len(pos), MaxTarget))
    TracksDataMat[pos, :] = np.zeros((len(pos), MaxTarget))

    undeleted_tracks_ind = np.where(TracksMat[0, :] == 0)[0]

    for n in range(len(undeleted_tracks_ind)):

        # Track Confirmation
        if (TracksVecMat[0, undeleted_tracks_ind[n]] > CNF_TH) and (TracksVecMat[0, undeleted_tracks_ind[n]] != 999) and (TracksMat[1, undeleted_tracks_ind[n]] == 0):
            TracksMat[1, undeleted_tracks_ind[n]] = 1
            TracksMat[2, undeleted_tracks_ind[n]] = 1

        corr_track_flag = False  # Flag indicates if track is correlated to at least one plot

        for k in range(NumDetect):
            errR = TracksVecMat[1, undeleted_tracks_ind[n]] - MergedRng[0][k]
            errTeta = MergedTeta[0][k] - TracksVecMat[2, undeleted_tracks_ind[n]]
            if errTeta > 180:
                errT = -errTeta + 360
            elif -errTeta > 180:
                errT = errTeta + 360
            else:
                errT = errTeta

            if abs(errR) < rng_dist and abs(errT) < teta_dist:
                # Calculate Rc
                Rc = cpc.cov_plot_calcRoee(MergedRng[0][k], MergedTeta[0][k] * np.pi / 180, sigmaTeta)
                last_ping_number = np.where(TracksMissMat[0, undeleted_tracks_ind[n], :] == 0)[0]
                std_dis_corr = scr.std_calcRoee(ping_ind, Tping, MergedYc[:, k], Rc,
                                            last_ping_number[-1], TracksX[:, undeleted_tracks_ind[n]], TracksP[undeleted_tracks_ind[n], :, :])

                if TracksMat[1, undeleted_tracks_ind[n]] == 0:
                    std_dis = std_dis_corr + Penalty
                else:
                    std_dis = std_dis_corr

                if std_dis_corr < gamma_corr:
                    TracksDataBinMat[undeleted_tracks_ind[n], k] = 1
                    TracksDataMat[undeleted_tracks_ind[n], k] = std_dis
                    ass_plots_flag[k] = 1
                    corr_track_flag = True

        if not corr_track_flag:  # track was not correlated to any plot
            TracksVecMat[3, undeleted_tracks_ind[n]] = ping_ind+1   #to_do: changed the orginel table to zeros so in not needed to add 1
            TracksVecMat[0, undeleted_tracks_ind[n]] = max(0, TracksVecMat[0, undeleted_tracks_ind[n]] - 1)
            TracksMissMat[0, undeleted_tracks_ind[n], ping_ind] = TracksMissMat[0, undeleted_tracks_ind[n], ping_ind - 1] + 1
            TracksMissMat[1:, undeleted_tracks_ind[n], ping_ind] = TracksMissMat[1:, undeleted_tracks_ind[n], ping_ind - 1]

    uncorr_plots_list = np.where(ass_plots_flag == 0)[0]
    return TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, uncorr_plots_list
