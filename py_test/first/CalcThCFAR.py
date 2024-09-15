import numpy as np
from scipy.integrate import cumtrapz

def integral_image(img):
    """Calculate the integral image."""
    return img.cumsum(axis=0).cumsum(axis=1)

def CalcThCFAR(img):
    N, M = img.shape  # Range and Beams

    BackWinSize_rng = 50
    GuardWinSize_rng = 20
    BackWinSize_teta = 3
    GuardWinSize_teta = 2
    Pfa = 2e-10

    # Calculating integral image on Power image
    IntI = integral_image(img**2)

    ThMat = np.zeros((N, M))
    for n in range(N):
        for m in range(M):
            # Extracting pixels from background region
            r1 = max(0, n - BackWinSize_rng // 2)
            r2 = max(0, n - GuardWinSize_rng // 2)
            r3 = min(N - 1, n + GuardWinSize_rng // 2)
            r4 = min(N - 1, n + BackWinSize_rng // 2)

            c1 = max(0, m - BackWinSize_teta // 2)
            c2 = max(0, m - GuardWinSize_teta // 2)
            c3 = min(M - 1, m + GuardWinSize_teta // 2)
            c4 = min(M - 1, m + BackWinSize_teta // 2)

            B1 = IntI[r4, c4] - IntI[r4, c1] - IntI[r1, c4] + IntI[r1, c1]
            B2 = IntI[r3, c3] - IntI[r3, c2] - IntI[r2, c3] + IntI[r2, c2]

            len1 = (r4 - r1 + 1) * (c4 - c1 + 1) - (c4 - c1 - 1) - (r4 - r1)
            len2 = (r3 - r2 + 1) * (c3 - c2 + 1) - (c3 - c2 - 1) - (r3 - r2)

            refWin = B1 - B2
            len = len1 - len2
            ThMat[n, m] = np.sqrt((Pfa**(-1/len) - 1) * refWin)

    Th = np.max(ThMat)
    return Th

# Example usage:
# img = np.random.rand(100, 100)  # Replace with your actual image data
# Th = CalcThCFAR(img)
# print(f"Calculated threshold: {Th}")
