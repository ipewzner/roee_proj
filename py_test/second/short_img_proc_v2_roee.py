import numpy as np
import merged_detections_v2_roee as md

def short_img_proc_v2_roee(ind_vec, r_vec, sigma_teta, beam_dir_rad, heading_deg):
    """Processes a ping and merges closely-spaced detections.

    Args:
        ind_vec: Array of indices.
        r_vec: Array of range values.
        sigma_teta: Standard deviation of theta.
        beam_dir_rad: Array of beam directions in radians.
        heading_deg: Heading in degrees.

    Returns:
        merged_detections_ping_rng: Array of merged range detections.
        merged_detections_ping_teta: Array of merged theta detections.
        merged_detections_ping_yc: Array of merged yc detections.
    """

    detections_rng = np.zeros(len(r_vec))
    detections_teta = np.zeros(len(r_vec))
    detections_yc = np.zeros((2, len(r_vec)))

    for nn in range(len(r_vec)):
        r = r_vec[nn]
        ind = ind_vec[nn]
        teta = beam_dir_rad[int(ind)-1] * (180 / np.pi) - heading_deg
        muc = np.array([r * np.sin(np.deg2rad(teta)) * (np.exp(-sigma_teta**2) - np.exp(-sigma_teta**2 / 2)),
                       r * np.cos(np.deg2rad(teta)) * (np.exp(-sigma_teta**2) - np.exp(-sigma_teta**2 / 2))])
        yc = np.array([r * np.sin(np.deg2rad(teta)),
                       r * np.cos(np.deg2rad(teta))]) - muc

        detections_rng[nn] = r
        detections_teta[nn] = teta
        detections_yc[:, nn] = yc

    # Call the previously defined Merged_Detecgtions_v2Roee function
    merged_detections_ping_rng, merged_detections_ping_teta, merged_detections_ping_yc = md.merged_detections_v2_roee(detections_rng, detections_teta, detections_yc)

    return merged_detections_ping_rng, merged_detections_ping_teta, merged_detections_ping_yc