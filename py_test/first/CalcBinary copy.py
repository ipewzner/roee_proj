import numpy as np
from IntegralImage import IntegralImage

def CalcBinary(img):
    """
    This function calculates a binary map from the input image using CFAR-like thresholding.
    
    Parameters:
    img : 2D numpy array
        Input image
    
    Returns:
    BinaryMap : 2D numpy array
        Binary map of detections
    """
    N, M = img.shape  # Range, Beams
    BackWinSize_rng = 50
    GuardWinSize_rng = 20
    BackWinSize_teta = 3
    GuardWinSize_teta = 2
    Pfa = 2e-10

    # Calculating integral image on Power image
    IntI = IntegralImage(img**2)
    BinaryMap = np.zeros((N, M), dtype=int)

    
    for n in range(N):
        for m in range(M):
            # Extracting pixels from background region
            r1 = max(0, n - BackWinSize_rng//2)
            r2 = max(0, n - GuardWinSize_rng//2)
            r3 = min(N-1, n + GuardWinSize_rng//2)
            r4 = min(N-1, n + BackWinSize_rng//2)
            c1 = max(0, m - BackWinSize_teta//2)
            c2 = max(0, m - GuardWinSize_teta//2)
            c3 = min(M-1, m + GuardWinSize_teta//2)
            c4 = min(M-1, m + BackWinSize_teta//2)

            B1 = IntI[r4, c4] - IntI[r4, c1] - IntI[r1, c4] + IntI[r1, c1]
            B2 = IntI[r3, c3] - IntI[r3, c2] - IntI[r2, c3] + IntI[r2, c2]

            len1 = (r4-r1+1)*(c4-c1+1) - (c4-c1-1) - (r4-r1)
            len2 = (r3-r2+1)*(c3-c2+1) - (c3-c2-1) - (r3-r2)
            refWin = B1 - B2
            len_total = len1 - len2

            thRayleigh = np.sqrt((Pfa**(-1/len_total) - 1) * np.sum(refWin))

            if img[n, m] > thRayleigh:
                BinaryMap[n, m] = 1

    
    return BinaryMap