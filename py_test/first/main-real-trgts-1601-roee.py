import numpy as np
from SetParameters import get_parameters
from GetDataFromRam import GetDataFromRam
from Shortproc_ping_dataRoee import Shortproc_ping_dataRoee
from ShortImgProc_v2Roee import ShortImgProc_v2Roee
from AnalyzeTracks import AnalyzeTracks
from CheckDetect import CheckDetect

# Placeholder functions
def GetYawFromCompass(PingNum):
    return 0  # Replace with actual implementation

def main():
    # Get all parameters
    params = get_parameters()
    
    UpdateThFlag = 1
    NewImgTh = params['ImgTh']  # Initialize NewImgTh with the default value

    while True:
        # Main Loop (on Pings)
        IndMat = 999 * np.ones((params['no_of_pings'], params['MaxTarget']), dtype=int)
        rMat = 999 * np.ones((params['no_of_pings'], params['MaxTarget']))
        YawVec = np.zeros(params['no_of_pings'])
        FinalPingFlag = 0
        
        for PingNum in range(params['no_of_pings']):
            if (PingNum == params['no_of_pings'] - 1) and (UpdateThFlag == 0):
                FinalPingFlag = 1
            
            try:
                # Extract one ping from the RAM
                PingData = GetDataFromRam(params['pri_samples'], PingNum + 1, params['Tgaurd'] * params['fs'])
                
                # Extract current yaw from compass
                YawVec[PingNum] = GetYawFromCompass(PingNum + 1)

                IndVec, rVec, NewImgTh = Shortproc_ping_dataRoee(
                    PingData, 
                    params['matched_filter'],
                    params['Rmin'], 
                    params['fs'], 
                    params['azBeams'], 
                    params['pos_sensors'], 
                    params['pri_samples'], 
                    params['ImgTh'], 
                    FinalPingFlag, 
                    UpdateThFlag
                )
                
                IndMat[PingNum, :len(IndVec)] = IndVec
                rMat[PingNum, :len(rVec)] = rVec
                
                print(f"Successfully processed ping {PingNum + 1}")
            except Exception as e:
                print(f"Error processing ping {PingNum + 1}: {str(e)}")
                continue  # Continue to the next ping instead of breaking
        
        
        exit()



        # Initialize tracking variables
        TracksP = 999 * np.ones((params['MaxTarget'], 4, 4))
        TracksX = 999 * np.ones((4, params['MaxTarget']))
        TracksMat = 999 * np.ones((3, params['MaxTarget']))
        TracksVecMat = 999 * np.ones((4, params['MaxTarget']))
        TracksMissMat = 999 * np.ones((5, params['MaxTarget'], params['no_of_pings']))
        TracksDataBinMat = np.zeros((params['MaxTarget'], params['MaxTarget']))
        TracksDataMat = 999 * np.ones((params['MaxTarget'], params['MaxTarget']))

        CurrentTargetInd = 0
        for PingNum in range(params['no_of_pings']):
            print(f"Processing Ping {PingNum + 1}")
            CurrentYaw = YawVec[PingNum]
            IndVec = IndMat[PingNum, :]
            rVec = rMat[PingNum, :]
            pos = np.where(rVec != 999)[0]
            
            if len(pos) > 0:
                MergedRng, MergedTeta, MergedYc = ShortImgProc_v2Roee(
                    IndVec[pos], rVec[pos], params['sigmaTeta'], params['azBeams'], CurrentYaw
                )
                
                TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat, CurrentTargetInd = AnalyzeTracks(
                    TracksMat, TracksVecMat, TracksP, TracksX, TracksMissMat, TracksDataBinMat, TracksDataMat,
                    MergedRng, MergedTeta, MergedYc, PingNum + 1, params['t_pri'], params['sigmaTeta'], params['Win_dlt'], params['Dlt_th'], params['MaxTarget'], CurrentTargetInd
                )
            elif PingNum > 0:
                pos = np.where(TracksMat[0, :] != 999)[0]
                if len(pos) > 0:
                    undeleted_tracks_ind = np.where(TracksMat[0, pos] == 0)[0]
                    TracksMissMat[0, pos[undeleted_tracks_ind], PingNum] = TracksMissMat[0, pos[undeleted_tracks_ind], PingNum-1] + 1

                    # Tracks Maintenance
                    for kk in undeleted_tracks_ind:
                        Len = np.where(TracksMissMat[0, pos[kk], :] != 999)[0]
                        if len(Len) > params['Win_dlt']:
                            k1 = np.diff(TracksMissMat[0, pos[kk], len(Len)-params['Win_dlt']:len(Len)])
                            num_miss = np.sum(k1 > 0)
                            if num_miss > params['Dlt_th']:
                                TracksMat[0, pos[kk]] = 1

        ReportTrackRange, DetectFlagVec = CheckDetect(
            TracksMissMat[3, :, :], TracksMissMat[4, :, :], 
            TracksMat[0, :], TracksMat[2, :], params['MinTracketLen']
        )
        
        if np.sum(DetectFlagVec) == 0:
            # Update threshold if no detection
            params['ImgTh'] = NewImgTh

        print("Processing complete.")
        break  # Remove this line to make the loop continuous

if __name__ == "__main__":
    main()