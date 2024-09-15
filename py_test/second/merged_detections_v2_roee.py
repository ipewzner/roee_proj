import numpy as np

def merged_detections_v2_roee(detections_rng, detections_teta, detections_yc):
    """
    This function merges closely-spaced detections from the current ping.

    Parameters:
    detections_rng (np.array): Array of detection ranges
    detections_teta (np.array): Array of detection angles (theta)
    detections_yc (np.array): Array of detection Y coordinates (shape: 2 x len(detections))

    Returns:
    merged_detections_ping_rng (np.array): Merged detection ranges
    merged_detections_ping_teta (np.array): Merged detection angles (theta)
    merged_detections_ping_yc (np.array): Merged detection Y coordinates (shape: 2 x len(detections))
    """

    rng_merged_dis = 5
    teta_merged_dis = 3

    length = len(detections_rng)

    merged_detections_ping_rng = 999 * np.ones(length)
    merged_detections_ping_teta = 999 * np.ones(length)
    merged_detections_ping_yc = 999 * np.ones((2, length))
    merged_flag = np.zeros(length)

    for n in range(length):
        if n == length - 1:
            if merged_flag[n] != 1:
                merged_detections_ping_rng[n] = detections_rng[n]
                merged_detections_ping_teta[n] = detections_teta[n]
                merged_detections_ping_yc[:, n] = detections_yc[:, n]
        else:
            if merged_flag[n] != 1:
                ind = np.where((np.abs(detections_rng[n] - detections_rng[n+1:]) < rng_merged_dis) & 
                               (np.abs(detections_teta[n] - detections_teta[n+1:]) < teta_merged_dis))[0]
                
                # Marking all merged detections
                if len(ind) > 0:
                    merged_flag[n+ind+1] = 1
                    merged_detections_ping_rng[n] = np.mean(detections_rng[n+ind+1])
                    merged_detections_ping_teta[n] = np.mean(detections_teta[n+ind+1])
                    merged_detections_ping_yc[:, n] = np.mean(detections_yc[:, n+ind+1], axis=1)
                else:
                    merged_detections_ping_rng[n] = detections_rng[n]
                    merged_detections_ping_teta[n] = detections_teta[n]
                    merged_detections_ping_yc[:, n] = detections_yc[:, n]

    pos = np.where(merged_detections_ping_rng != 999)[0]
    
    return (merged_detections_ping_rng[pos], 
            merged_detections_ping_teta[pos], 
            merged_detections_ping_yc[:, pos])
