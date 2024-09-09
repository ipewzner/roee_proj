import numpy as np
from BeamformingRoee import BeamformingRoee
from CalcBinary import CalcBinary
from DetectionBlobsRoee import DetectionBlobsRoee
import csvToArray as csv2A

def Shortproc_ping_dataRoee(PingData, matched_filter, Rmin, fs, azBeams, pos_sensors, pri_samples, ImgTh, FinalPingFlag, UpdateThFlag):
    # Constants
    min_pixel = 1
    max_pixel = 1e4
    snd_vel = 1500
    rng_res = 1 / fs * snd_vel / 2

    # Beamforming
    #img = BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, fs, pri_samples)
    img, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv')

    print("UpdateThFlag", UpdateThFlag)
    if UpdateThFlag:
        BinaryMap = CalcBinary(img)
    else:
        BinaryMap = DoCFAR(img, ImgTh)

    CL = DetectionBlobsRoee(BinaryMap, min_pixel, max_pixel)

    if len(CL) > 0:
        IndVec = np.zeros(len(CL), dtype=int)
        rVec = np.zeros(len(CL))
        for nn in range(len(CL)):
            rVec[nn] = Rmin + CL[nn] * rng_res
            aa=CL[nn]
            bb=img[int(CL[nn]-1), :]
            ind = np.argmax(img[int(np.floor(CL[nn]-1)), :])+1  # Find the index of the maximum value

            IndVec[nn] = ind
    else:
        IndVec = np.array([], dtype=int)
        rVec = np.array([])

    if FinalPingFlag:
        # Calculate threshold for next iteration
        NewImgTh = CalcThCFAR(img)
    else:
        NewImgTh = ImgTh

    return IndVec, rVec, NewImgTh

# The following functions still need to be implemented:

def DoCFAR(img, ImgTh):
    # Implement CFAR detection
    # Return the binary map after CFAR
    pass

def CalcThCFAR(img):
    # Implement CFAR threshold calculation
    # Return the new threshold
    pass

