import numpy as np

def CheckDetect(TracksX, TracksY, Tracksdlt_flag, TracksStat_id, MinTracketLen):
    # Find indices where Tracksdlt_flag is 0 and TracksStat_id is 1
    loc = np.where((Tracksdlt_flag == 0) & (TracksStat_id == 1))[0]
    
    ReportTrackRange = np.zeros(len(loc))
    DetectFlagVec = np.zeros(len(Tracksdlt_flag), dtype=int)
    
    for TrackInd, track_loc in enumerate(loc):
        # Find non-999 entries in TracksX for this track
        pos = np.where(TracksX[track_loc, :] != 999)[0]
        
        if len(pos) > MinTracketLen:
            # Calculate the range using the last valid position
            last_pos = pos[-1]
            ReportTrackRange[TrackInd] = np.sqrt(
                TracksX[track_loc, last_pos]**2 + 
                TracksY[track_loc, last_pos]**2
            )
            DetectFlagVec[track_loc] = 1
    
    return ReportTrackRange, DetectFlagVec
