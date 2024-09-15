import numpy as np
from scipy import signal

def BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, fs, pri_samples):
    """
    This function performs beamforming on the input ping data and generates 2D images (range-azimuth).

    Parameters:
    - PingData: numpy array of shape (4, n_samples)
    - matched_filter: numpy array
    - azBeams: numpy array of azimuth angles
    - pos_sensors: numpy array of shape (3, 4) for sensor positions
    - fs: sampling frequency
    - pri_samples: number of samples per ping

    Returns:
    - Beam: numpy array of shape (pri_samples, len(azBeams))
    """
    snd_vel = 1500  # [m/sec]

    # Beamforming (Delay and Sum)
    v = 0  # sin(elCut); NO ELEVATION MEASUREMENT, elCut = 0
    Beam = np.zeros((pri_samples, len(azBeams)))

    for m, az in enumerate(azBeams):
        u = np.sin(az)
        w = np.cos(az)
        print("u ", u, " w ", w)
        
        data_beam = np.zeros(PingData.shape[1])

        print("data_beam ", data_beam.shape)
        for k in range(4):  # Assuming 4 sensors
            tau = 1/snd_vel * np.sum(pos_sensors[:, k-1] * np.array([u, w, v]))
            shift = int(round(tau * fs))
            if shift > 0:
                data_beam += np.pad(PingData[k, shift:], (0, shift), mode='constant')
            elif shift < 0:
                data_beam += np.pad(PingData[k, :shift], (abs(shift), 0), mode='constant')
            else:
                data_beam += PingData[k, :]

        MF = np.abs(signal.convolve(matched_filter, data_beam, mode='full'))
        Beam[:, m] = MF[:pri_samples]

    return Beam

# Example usage:
# Beam = BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, fs, pri_samples)