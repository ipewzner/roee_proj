import numpy as np
from scipy import ndimage

def DetectionBlobsRoee(BinaryMap, min_pixel, max_pixel):
    """
    This function detects blobs in a binary map and returns their mean row positions.
    
    Parameters:
    BinaryMap : 2D numpy array
        Binary image containing the blobs
    min_pixel : int
        Minimum number of pixels for a valid blob
    max_pixel : int
        Maximum number of pixels for a valid blob
    
    Returns:
    ReportCL : numpy array
        Mean row positions of valid blobs
    """
    # Find connected components
    labeled_array, num_features = ndimage.label(BinaryMap, structure=np.ones((3,3)))
    
    imageRowCnt = BinaryMap.shape[0]
    CL = 999 * np.ones(num_features)
    NumObjects = 0

    for cnt in range(1, num_features + 1):
        blob = np.where(labeled_array == cnt)
        numOfPix = len(blob[0])
        
        if min_pixel <= numOfPix <= max_pixel:
            CL[cnt-1] = np.mean(blob[0])
            NumObjects += 1

    ReportCL = CL[CL != 999]
    
    return ReportCL