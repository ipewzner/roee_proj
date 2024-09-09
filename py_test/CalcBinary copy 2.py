import math
import numpy as np
import IntegralImage as ii




#**-*---*-**--*-**--**-*--**--*-*
# len1 == len2 !!!!!!!!!!!!
#**-*---*-**--*-**--**-*--**--*-*


def integral_image(img):
    return np.pad(np.cumsum(np.cumsum(img, axis=0), axis=1), ((1, 0), (1, 0)), mode='constant')

def CalcBinary(img):
    N, M = img.shape

    BackWinSize_rng = 50
    GuardWinSize_rng = 20
    BackWinSize_teta = 3
    GuardWinSize_teta = 2
    Pfa = 2e-10

    # Calculating integral image on Power image
    ###IntI = ii.IntegralImage(img ** 2)
    IntI = np.cumsum(np.cumsum(img**2, axis=0), axis=1)

    BinaryMap = np.zeros((N, M))
    ##print("IntI",IntI)
    ##print("BinaryMap",BinaryMap)
    for n in range(N):
        for m in range(M):
            #print("n",n,"m",m)
            
            # Extracting pixels from background region
            r1 = max(0, n - math.floor(BackWinSize_rng / 2))
            r2 = max(0, n - math.floor(GuardWinSize_rng / 2))
            r3 = min(N-1, n + math.floor(GuardWinSize_rng / 2))
            r4 = min(N-1, n + math.floor(BackWinSize_rng / 2))

            c1 = max(0, m - math.floor(BackWinSize_teta / 2))
            c2 = max(0, m - math.floor(GuardWinSize_teta / 2))
            c3 = min(M-1, m + math.floor(GuardWinSize_teta / 2))
            c4 = min(M-1, m + math.floor(BackWinSize_teta / 2))


    
            #print("r4", r4,"c4", c4)
            
            B1 = IntI[r4, c4] - IntI[r4, c1] - IntI[r1, c4] + IntI[r1, c1]
            B2 = IntI[r3, c3] - IntI[r3, c2] - IntI[r2, c3] + IntI[r2, c2]
            len1 = (r4 - r1 + 1) * (c4 - c1 + 1) - (c4 - c1 - 1) - (r4 - r1)
            len2 = (r3 - r2 + 1) * (c3 - c2 + 1) - (c3 - c2 - 1) - (r3 - r2)
            #print("len1", len1)
            #print("len2", len2)
            
            refWin = B1 - B2
            len = len1 - len2
            
            if not len == 0:
                #print("(Pfa ** (-1 / len) - 1)", (Pfa ** (-1 / len) - 1))
                thRayleigh = np.sqrt((Pfa ** (-1 / len) - 1) * np.sum(refWin))
            else:
                thRayleigh = 0  # or some other default value
        
            #print("thRayleigh", thRayleigh)
            
            if img[n, m] > thRayleigh:
                BinaryMap[n, m] = 1

    #print("BinaryMap", BinaryMap)
    return BinaryMap.astype(int)
