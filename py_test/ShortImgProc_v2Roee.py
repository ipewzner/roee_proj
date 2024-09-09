import numpy as np

def ShortImgProc_v2Roee(IndVec, rVec, sigmaTeta, beam_dir_rad, heading_deg):
    # Initialize output arrays
    DetectionsRng = np.zeros(len(rVec))
    DetectionsTeta = np.zeros(len(rVec))
    DetectionsYC = np.zeros((2, len(rVec)))

    for nn in range(len(rVec)):
        # Updating Detections structure from current ping
        r = rVec[nn]
        ind = IndVec[nn]
        teta = np.rad2deg(beam_dir_rad[ind]) - heading_deg
        
        muc = np.array([
            r * np.sin(np.deg2rad(teta)) * (np.exp(-sigmaTeta**2) - np.exp(-sigmaTeta**2/2)),
            r * np.cos(np.deg2rad(teta)) * (np.exp(-sigmaTeta**2) - np.exp(-sigmaTeta**2/2))
        ])
        
        yc = np.array([
            r * np.sin(np.deg2rad(teta)),
            r * np.cos(np.deg2rad(teta))
        ]) - muc

        DetectionsRng[nn] = r  # range in [m]
        DetectionsTeta[nn] = teta  # teta in [degs]
        DetectionsYC[:, nn] = yc  # converted measurements

    # Merging closely-spaced detections
    merged_detections_pingRng, merged_detections_pingTeta, merged_detections_pingYc = Merged_Detecgtions_v2Roee(
        DetectionsRng, DetectionsTeta, DetectionsYC)

    return merged_detections_pingRng, merged_detections_pingTeta, merged_detections_pingYc

def Merged_Detecgtions_v2Roee(DetectionsRng, DetectionsTeta, DetectionsYC):
    # This function needs to be implemented
    # It should merge closely-spaced detections
    # For now, we'll return the inputs as is
    return DetectionsRng, DetectionsTeta, DetectionsYC

# You might need to implement this function if it's not provided elsewhere
# def Merged_Detecgtions_v2Roee(DetectionsRng, DetectionsTeta, DetectionsYC):
#     # Implement the merging logic here
#     pass
