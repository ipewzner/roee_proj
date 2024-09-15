import numpy as np
from scipy.io import wavfile
import os

def GetDataFromRam(pri_samples, n, Tgaurd):
    # Assuming we're always reading from file in this Python version
    from_file = True

    if from_file:
        # Sibenik trial: July 2024
        ringdown = int(0.001 * 128e3)
        FolderName = 13
        FileName = f'00{FolderName}_'
        SigMat = []

        for ChInd in range(1, 5):  # 1 to 4
            CurrentFileName = f'{FileName}{ChInd}.wav'
            # Check if the file exists
            if not os.path.exists(CurrentFileName):
                raise FileNotFoundError(f"The file {CurrentFileName} does not exist.")
            
            fs, data = wavfile.read(CurrentFileName)
            SigMat.append(data)
        
        SigMat = np.array(SigMat)
        print(f"Loaded data shape: {SigMat.shape}")

        Th = np.mean(SigMat[0, :]) + 10 * np.std(SigMat[0, :])
        loc = np.where(np.abs(SigMat[0, :]) > Th)[0]
        print(f"Number of samples above threshold: {len(loc)}")

        if len(loc) == 0:
            # If no samples above threshold, return a chunk of data anyway
            print("Warning: No samples above threshold. Returning a chunk of data.")
            start = max(0, (n-1) * pri_samples)
            end = min(start + pri_samples, SigMat.shape[1])
            return SigMat[:, start:end]

        CurrentLocPos = 1
        PingCount = 0
        PingLoc = None  # Initialize PingLoc to None

        while CurrentLocPos < len(loc):
            if loc[CurrentLocPos] - loc[CurrentLocPos-1] > Tgaurd * 3/4 * fs:
                PingCount += 1
                if PingCount == n:
                    PingLoc = loc[CurrentLocPos-1] + 1
                    break
            CurrentLocPos += 1

        if PingLoc is None:
            print(f"Warning: Could not find ping number {n}. Only found {PingCount} pings. Returning last chunk of data.")
            PingLoc = loc[-1]

        if PingLoc + ringdown + pri_samples > SigMat.shape[1]:
            print(f"Warning: Not enough data for ping {n}. Adjusting PingLoc.")
            PingLoc = max(0, SigMat.shape[1] - ringdown - pri_samples)

        PingData = SigMat[:, PingLoc+ringdown:PingLoc+ringdown+pri_samples]

    else:
        # This is a placeholder for actual RAM extraction
        raise NotImplementedError("RAM extraction not implemented")

    return PingData

# You might need to implement this function if you need RAM extraction
# def ExtractRam(pri_samples):
#     # Implement RAM extraction here
#     pass