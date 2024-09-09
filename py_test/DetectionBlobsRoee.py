from scipy.ndimage import label
import numpy as np

def DetectionBlobsRoee(BinaryMap, min_pixel, max_pixel):
    # Check the dimensionality of BinaryMap
    if BinaryMap.ndim == 1:
        structure = np.ones((3,), dtype=int)  # 1D structure for 1D BinaryMap
    elif BinaryMap.ndim == 2:
        structure = np.ones((3, 3), dtype=int)  # 2D structure for 2D BinaryMap
    else:
        raise ValueError("BinaryMap must be either 1D or 2D.")

    print("structure",structure)
    # Label connected components in the binary image
    labeled_array, NumofObj = label(BinaryMap, structure=structure)
    print("labeled_array",labeled_array)
    print("NumofObj",NumofObj)

    imageRowCnt = BinaryMap.shape[0]

    CL = 999 * np.ones(NumofObj)
    NumObjects = 0
    for cnt in range(NumofObj):
        ct = np.where(labeled_array == cnt + 1)
        numOfPix = len(ct[0])
        row = np.zeros(numOfPix)
        col = np.zeros(numOfPix)
        for cntpix in range(numOfPix):
            II = ct[0][cntpix] % imageRowCnt
            col[cntpix] = np.ceil(ct[0][cntpix] / imageRowCnt)
            if II == 0:
                II = imageRowCnt
            row[cntpix] = II

        XY = np.column_stack((col, row))
        if min_pixel <= XY.shape[0] <= max_pixel:
            CL[cnt] = np.mean(row+1)
            NumObjects += 1

    pos = np.where(CL != 999)[0]
    ReportCL = CL[pos]
    return ReportCL
