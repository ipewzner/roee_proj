import numpy as np

def analyze_tracks(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, 
                   MergedRng, MergedTeta, MergedYc, ping_number, Tping, sigmaTeta, Win_dlt, Dlt_th, 
                   MaxTracks, CurrentTargetInd):
    
    cov_fact = 5
    xmax = 10
    no_of_pings = TracksMissMat.shape[2]
    
    NumDetect = len(MergedRng)
    MaxTarget = TracksVecMat.shape[1]

    if ping_number > 1:
        # Ping-to-ping correlation
        TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, uncorr_plots_list = \
            ping_to_ping_correlationRoee(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, 
                                         MergedRng, MergedTeta, MergedYc, NumDetect, ping_number, Tping, sigmaTeta)

        # Ping-to-ping data association
        TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, CurrentTargetInd = \
            ping_to_ping_associationRoee(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, 
                                         MergedRng, MergedTeta, MergedYc, NumDetect, ping_number, sigmaTeta, cov_fact, Win_dlt, 
                                         xmax, Tping, CurrentTargetInd)

        # Initialize tracks
        for MergeInd in uncorr_plots_list:
            Rc = cov_plot_calcRoee(MergedRng[MergeInd], np.radians(MergedTeta[MergeInd]), sigmaTeta)
            TracksP[CurrentTargetInd + MergeInd, :, :] = cov_fact * np.array([[Rc[0, 0], Rc[0, 1], 0, 0], 
                                                                              [Rc[0, 1], Rc[1, 1], 0, 0], 
                                                                              [0, 0, (xmax / (Win_dlt * Tping))**2, 0], 
                                                                              [0, 0, 0, (xmax / (Win_dlt * Tping))**2]])

        TracksX[0, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = MergedYc[0, uncorr_plots_list]
        TracksX[1, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = MergedYc[1, uncorr_plots_list]
        TracksX[2:4, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = 0

        TracksVecMat[0, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = 1
        TracksVecMat[1, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = MergedRng[uncorr_plots_list]
        TracksVecMat[2, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = MergedTeta[uncorr_plots_list]
        TracksVecMat[3, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = ping_number

        TracksMat[:3, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = 0
        TracksMissMat[:3, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), ping_number] = 0
        TracksMissMat[3, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), ping_number] = MergedYc[0, uncorr_plots_list]
        TracksMissMat[4, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), ping_number] = MergedYc[1, uncorr_plots_list]

        TracksDataBinMat[CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), :] = 0
        TracksDataMat[CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), :] = 999

        CurrentTargetInd += len(uncorr_plots_list)

        # Tracks Maintenance
        undeleted_tracks_ind = np.where(TracksMat[0, :] == 0)[0]
        for kk in undeleted_tracks_ind:
            Len = np.where(TracksMissMat[0, kk, :] != 999)[0]
            if len(Len) > Win_dlt:
                k1 = np.diff(TracksMissMat[0, kk, Len[-Win_dlt:]])
                num_miss = np.sum(k1 > 0)
                if num_miss > Dlt_th:
                    TracksMat[0, kk] = 1

        # Cleaning Tracks structure
        undeleted_tracks_ind = np.where(TracksMat[0, :] == 0)[0]

        TracksNewP = 999 * np.ones((MaxTracks, 4, 4))
        TracksNewX = 999 * np.ones((4, MaxTracks))
        TracksNewMat = 999 * np.ones((3, MaxTracks))
        TracksVecNewMat = 999 * np.ones((4, MaxTracks))
        TracksNewMissMat = 999 * np.ones((5, MaxTarget, no_of_pings))
        TracksNewDataBinMat = np.zeros((MaxTarget, MaxTarget))  # binary
        TracksNewDataMat = 999 * np.ones((MaxTarget, MaxTarget))

        TracksNewMat[:, :len(undeleted_tracks_ind)] = TracksMat[:, undeleted_tracks_ind]
        TracksNewX[:, :len(undeleted_tracks_ind)] = TracksX[:, undeleted_tracks_ind]
        TracksNewP[:len(undeleted_tracks_ind), :, :] = TracksP[undeleted_tracks_ind, :, :]
        TracksVecNewMat[:, :len(undeleted_tracks_ind)] = TracksVecMat[:, undeleted_tracks_ind]
        TracksNewMissMat[:, :len(undeleted_tracks_ind), :] = TracksMissMat[:, undeleted_tracks_ind, :]
        TracksNewDataBinMat[:len(undeleted_tracks_ind), :] = TracksDataBinMat[undeleted_tracks_ind, :]
        TracksNewDataMat[:len(undeleted_tracks_ind), :] = TracksDataMat[undeleted_tracks_ind, :]

        TracksMat = TracksNewMat
        TracksX = TracksNewX
        TracksP = TracksNewP
        TracksVecMat = TracksVecNewMat
        TracksMissMat = TracksNewMissMat
        TracksDataBinMat = TracksNewDataBinMat
        TracksDataMat = TracksNewDataMat

    else:
        # Initialize tracks
        TracksMat[0:3, :NumDetect] = 0

        TracksX[0, :NumDetect] = MergedYc[0, :]
        TracksX[1, :NumDetect] = MergedYc[1, :]
        TracksX[2:4, :NumDetect] = 0

        TracksVecMat[0, :NumDetect] = 1
        TracksVecMat[1, :NumDetect] = MergedRng
        TracksVecMat[2, :NumDetect] = MergedTeta
        TracksVecMat[3, :NumDetect] = ping_number

        for MergeInd in range(NumDetect):
            Rc = cov_plot_calcRoee(MergedRng[MergeInd], np.radians(MergedTeta[MergeInd]), sigmaTeta)
            TracksP[CurrentTargetInd + MergeInd, :, :] = cov_fact * np.array([[Rc[0, 0], Rc[0, 1], 0, 0], 
                                                                              [Rc[0, 1], Rc[1, 1], 0, 0], 
                                                                              [0, 0, (xmax / (Win_dlt * Tping))**2, 0], 
                                                                              [0, 0, 0, (xmax / (Win_dlt * Tping))**2]])

        TracksMissMat[:3, CurrentTargetInd: CurrentTargetInd + NumDetect, ping_number] = 0
        TracksMissMat[3, CurrentTargetInd: CurrentTargetInd + NumDetect, ping_number] = MergedYc[0, :]
        TracksMissMat[4, CurrentTargetInd: CurrentTargetInd + NumDetect, ping_number] = MergedYc[1, :]

        CurrentTargetInd += NumDetect

    return (TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, CurrentTargetInd)
