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

        data_beam = np.zeros(PingData.shape[1])
        for k in range(4):  # Assuming 4 sensors
            #print(pos_sensors[:, k])
            tau = 1/snd_vel * np.sum(pos_sensors[:, k] * np.array([u, w, v]))
            shift = int(round(tau * fs))

            if shift > 0:
                print(PingData[k, shift:])
                data_beam += np.concatenate((PingData[k, shift:], np.zeros(shift)))
            elif shift < 0:
                print(PingData[k, :shift])
                data_beam += np.concatenate((np.zeros(abs(shift)), PingData[k, :shift]))
            else:
                print(PingData[k, :])
                data_beam += PingData[k, :]


        MF = np.abs(signal.convolve(matched_filter, data_beam, mode='full'))

        Beam[:, m] = MF[:pri_samples]

    return Beam
