import numpy as np

def std_calcRoee(ping_ind, Tping, yc, Rc, last_ping_number, X, P):
    sigmaV = 1e-4
    Vx_own = 0
    Vy_own = 0

    T = (ping_ind - last_ping_number) * Tping

    # Calculating matrices of process model
    G = np.array([[T**2 / 2, 0], 
                  [0, T**2 / 2], 
                  [T, 0], 
                  [0, T]])

    Q = sigmaV**2 * np.dot(G, G.T)

    F = np.array([[1, 0, T, 0], 
                  [0, 1, 0, T], 
                  [0, 0, 1, 0], 
                  [0, 0, 0, 1]])

    H = np.array([[1, 0, 0, 0], 
                  [0, 1, 0, 0]])

    Xp = np.dot(F, X)
    Xp[0] -= Vx_own * T
    Xp[1] -= Vy_own * T

    Pp = np.dot(F, np.dot(P, F.T)) + Q
    S = np.dot(H, np.dot(Pp, H.T)) + Rc

    yp = np.array([Xp[0], Xp[1]])

    std_dis = np.dot((yc - yp).T, np.linalg.inv(S)).dot(yc - yp)

    return std_dis
