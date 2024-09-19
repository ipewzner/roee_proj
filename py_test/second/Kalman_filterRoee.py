import numpy as np

def Kalman_filterRoee(yc, Rc, T, X, P):
    """
    Implements a Kalman filter for tracking.

    Args:
        yc: Measurement vector.
        Rc: Measurement covariance matrix.
        T: Sampling time.
        X: State vector.
        P: State covariance matrix.

    Returns:
        X_upd: Updated state vector.
        P_upd: Updated state covariance matrix.
        teta_rad: Estimated azimuth angle in radians.
    """

    # Sensors parameters
    Vx_own = 0
    Vy_own = 0
    sigmaV = 1e-4

    # Calculating matrices of process model
    G = np.array([[T**2 / 2, 0],
                  [0, T**2 / 2],
                  [T, 0],
                  [0, T]])
    Q = sigmaV**2 * G @ G.T
    F = np.array([[1, 0, T, 0],
                  [0, 1, 0, T],
                  [0, 0, 1, 0],
                  [0, 0, 0, 1]])
    H = np.array([[1, 0, 0, 0],
                  [0, 1, 0, 0]])

    # Prediction step
    Xp = F @ X
    Xp[0] -= Vx_own * T
    Xp[1] -= Vy_own * T
    Pp = F @ P @ F.T + Q

    # Update step
    S = H @ Pp @ H.T + Rc
    K = Pp @ H.T @ np.linalg.inv(S)
    X_upd = Xp + K @ (yc - H @ Xp)
    P_upd = Pp - K @ S @ K.T

    # Extraction smooth azimuth angle
    teta_rad = np.arctan2(X_upd[0], X_upd[1])

    return X_upd, P_upd, teta_rad