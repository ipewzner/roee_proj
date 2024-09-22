import numpy as np

def check_detect(TracksX, TracksY, Tracksdlt_flag, TracksStat_id, MinTracketLen):
    # Find indices where Tracksdlt_flag == 0 and TracksStat_id == 1
    loc = np.where((Tracksdlt_flag == 0) & (TracksStat_id == 1))[0]
    
    # Initialize ReportTrackRange and DetectFlagVec
    ReportTrackRange = np.zeros(len(loc))
    DetectFlagVec = np.zeros(len(loc))
    
    for TrackInd in range(len(loc)):
        # Find positions where TracksX is not equal to 999
        pos = np.where(TracksX[loc[TrackInd], :] != 999)[0]
        
        if len(pos) > MinTracketLen:
            # Calculate the range using the last valid position
            ReportTrackRange[TrackInd] = np.sqrt(TracksX[loc[TrackInd], pos[-1]]**2 + TracksY[loc[TrackInd], pos[-1]]**2)
            DetectFlagVec[loc[TrackInd]] = 1
    
    return ReportTrackRange, DetectFlagVec
