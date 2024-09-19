
#
#

#   ind missing the gpt is lazey fix it!

#
#

import numpy as np
import cov_plot_calcRoee as cpc
import AuctionRoee as ar
import Kalman_filterRoee as kfr

def ping_to_ping_associationRoee(TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat,
                                 MergedRng, MergedTeta, MergedYc, NumDetect, ping_ind, sigmaTeta, cov_fact, Win_dlt, xmax, Tping, CurrentTargetInd):
    LARGE = 1e4
    MaxTarget = TracksDataBinMat.shape[1]

    # Indexing undeleted tracks
    undeleted_tracks_indA = np.where(TracksMat[0, :] == 0)[0]
    Amat = np.zeros((len(undeleted_tracks_indA), NumDetect))
    tracks_id = np.zeros(len(undeleted_tracks_indA))

    for n in range(len(undeleted_tracks_indA)):
        plots_id = np.where(TracksDataBinMat[undeleted_tracks_indA[n], :] == 1)[0]
        if len(plots_id) > 0:
            tracks_id[n] = n
            Amat[n, plots_id] = TracksDataMat[undeleted_tracks_indA[n], plots_id]

    loc = np.where(tracks_id != 0)[0]
    plots_index_new = np.where(np.sum(Amat[loc, :], axis=0) != 0)[0]
    A = Amat[loc[:, None], plots_index_new]  # Only take loc rows and plots_index_new cols
    undeleted_tracks_ind = undeleted_tracks_indA[loc]
    NumDetect_new = len(plots_index_new)
    Ntr = len(undeleted_tracks_ind)
    A[A == 0] = LARGE

    Assigned = ar.AuctionRoee(A)

    if Ntr > NumDetect_new:
        # Handling assigned tracks
        ind1 = np.where(Assigned != 0)[0]
        for nn in ind1:
            plot_id = plots_index_new[Assigned[nn]]
            track_id = undeleted_tracks_ind[nn]
           
            if A[nn, Assigned[nn]] == LARGE:
                # Unassigned track
                TracksVecMat[3, track_id] = ping_ind
                TracksMissMat[0, track_id, ping_ind] = TracksMissMat[0, track_id, ping_ind - 1] + 1
                for i in range(1, 5):
                    TracksMissMat[i, track_id, ping_ind] = TracksMissMat[i, track_id, ping_ind - 1]
            else:
                # Assigned track
                r = MergedRng[plot_id]
                teta = MergedTeta[plot_id] * np.pi / 180
                yc = MergedYc[:, plot_id]
                Rc = cpc.cov_plot_calcRoee(r, teta, sigmaTeta)

                last_ping_number = np.where(TracksMissMat[0, track_id, :] == 0)[0]
                T = (ping_ind - last_ping_number[-1]) * Tping
                X_upd, P_upd, teta_rad = kfr.Kalman_filterRoee(yc, Rc, T, TracksX[:, track_id], TracksP[track_id, :, :])

                TracksX[:, track_id] = X_upd
                TracksP[track_id, :, :] = P_upd
                TracksVecMat[1, track_id] = np.sqrt(X_upd[0]**2 + X_upd[1]**2)
                TracksVecMat[2, track_id] = teta_rad * 180 / np.pi
                TracksVecMat[3, track_id] = ping_ind
                TracksVecMat[0, track_id] += 1

                TracksMissMat[0, track_id, ping_ind] = 0
                TracksMissMat[1:5, track_id, ping_ind] = X_upd[2:4]

        # Handling unassigned tracks
        ind2 = np.where(Assigned == 0)[0]
        for kk in ind2:
            track_id = undeleted_tracks_ind[kk]
            TracksVecMat[3, track_id] = ping_ind
            for i in range(5):
                TracksMissMat[i, track_id, ping_ind] = TracksMissMat[i, track_id, ping_ind - 1] + (i == 0)

    else:
        ind1 = np.where(Assigned != 0)[0]
        for nn in ind1:
            plot_id = plots_index_new[nn]
            track_id = undeleted_tracks_ind[Assigned[nn]]
            if A[Assigned[nn], nn] == LARGE:
                TracksVecMat[3, track_id] = ping_ind
                for i in range(5):
                    TracksMissMat[i, track_id, ping_ind] = TracksMissMat[i, track_id, ping_ind - 1] + (i == 0)
            else:
                r = MergedRng[plot_id]
                teta = MergedTeta[plot_id] * np.pi / 180
                yc = MergedYc[:, plot_id]
                Rc = cpc.cov_plot_calcRoee(r, teta, sigmaTeta)

                last_ping_number = TracksVecMat[3, track_id]
                T = (ping_ind - last_ping_number) * Tping
                X_upd, P_upd, teta_rad = kfr.Kalman_filterRoee(yc, Rc, T, TracksX[:, track_id], TracksP[track_id, :, :])

                TracksX[:, track_id] = X_upd
                TracksP[track_id, :, :] = P_upd
                TracksVecMat[1, track_id] = np.sqrt(X_upd[0]**2 + X_upd[1]**2)
                TracksVecMat[2, track_id] = teta_rad * 180 / np.pi
                TracksVecMat[3, track_id] = ping_ind
                TracksVecMat[0, track_id] += 1

                TracksMissMat[0, track_id, ping_ind] = 0
                TracksMissMat[1:5, track_id, ping_ind] = X_upd[2:4]

        # Handling unassigned plots
        uncorr_plots_list = plots_index_new[np.where(Assigned == 0)[0]]

        if len(uncorr_plots_list) > 0:
            for MergeInd in range(len(uncorr_plots_list)):
                Rc = cpc.cov_plot_calcRoee(MergedRng[uncorr_plots_list[MergeInd]], MergedTeta[uncorr_plots_list[MergeInd]] * np.pi / 180, sigmaTeta)
                TracksP[CurrentTargetInd + MergeInd, :, :] = cov_fact * np.array([[Rc[0, 0], Rc[0, 1], 0, 0],
                                                                                  [Rc[0, 1], Rc[1, 1], 0, 0],
                                                                                  [0, 0, (xmax / (Win_dlt * Tping))**2, 0],
                                                                                  [0, 0, 0, (xmax / (Win_dlt * Tping))**2]])

            TracksX[0:2, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = MergedYc[:, uncorr_plots_list]
            TracksVecMat[0, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = 1
            TracksVecMat[1, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = MergedRng[uncorr_plots_list]
            TracksVecMat[2, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = MergedTeta[uncorr_plots_list]
            TracksVecMat[3, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = ping_ind

            TracksMat[0:3, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list)] = 0
            TracksMissMat[:, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), ping_ind] = 0
            TracksMissMat[3:5, CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), ping_ind] = MergedYc[:, uncorr_plots_list]

            TracksDataBinMat[CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), :] = 0
            TracksDataMat[CurrentTargetInd: CurrentTargetInd + len(uncorr_plots_list), :] = 999

    return TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, CurrentTargetInd
