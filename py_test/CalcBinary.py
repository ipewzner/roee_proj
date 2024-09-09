import numpy as np

def CalcBinary(img):
    N, M = img.shape

    BackWinSize_rng = 50
    GuardWinSize_rng = 20
    BackWinSize_teta = 3
    GuardWinSize_teta = 2
    Pfa = 2e-10

    #print(img**2)
    # Calculate integral image on Power image
    IntI = np.cumsum(np.cumsum(img**2, axis=0), axis=1)
    #print(IntI)

    BinaryMap = np.zeros((N, M))
    for n in range(1, N + 1):
        for m in range(1, M + 1):
            # Extract pixels from background region
            r1 = max(1, n - BackWinSize_rng // 2)
            r2 = max(1, n - GuardWinSize_rng // 2)
            r3 = min(N, n + GuardWinSize_rng // 2)
            r4 = min(N, n + BackWinSize_rng // 2)

            c1 = max(1, m - BackWinSize_teta // 2)
            c2 = max(1, m - GuardWinSize_teta // 2)
            c3 = min(M, m + GuardWinSize_teta // 2)
            c4 = min(M, m + BackWinSize_teta // 2)

            B1 = IntI[r4 - 1, c4 - 1] - IntI[r4 - 1, c1 - 1] - IntI[r1 - 1, c4 - 1] + IntI[r1 - 1, c1 - 1]
            B2 = IntI[r3 - 1, c3 - 1] - IntI[r3 - 1, c2 - 1] - IntI[r2 - 1, c3 - 1] + IntI[r2 - 1, c2 - 1]
            len1 = (r4 - r1 + 1) * (c4 - c1 + 1) - (c4 - c1 - 1) - (r4 - r1)
            len2 = (r3 - r2 + 1) * (c3 - c2 + 1) - (c3 - c2 - 1) - (r3 - r1)

            refWin = B1 - B2
            len = len1 - len2
            # Handle zero division
            if len == 0:
                thRayleigh = 0  # Set thRayleigh to 0 (or a suitable default)
            else:
                thRayleigh = np.sqrt((Pfa**(-1 / len) - 1) * np.sum(refWin))

            if img[n - 1, m - 1] > thRayleigh:
                BinaryMap[n - 1, m - 1] = 1

    return BinaryMap