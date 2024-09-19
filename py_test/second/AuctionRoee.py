import numpy as np

def AuctionRoee(assign_mtrx):
    # Variables Definitions:
    # Price - Price vector for the auction procedure
    # Assigned[j] - indicates which detection/track is assigned to track/detection.
    
    # Inputs:
    # assign_mtrx - Cost matrix (in MATLAB called A, we negate it here to maximize the total distance)
    
    sizemat = assign_mtrx.shape
    M = sizemat[1]
    N = sizemat[0]
    A = -assign_mtrx  # The auction maximizes the total distance
    eps = 1 / max(N, M)

    # Initializations
    k = max(N, M)
    Assigned = np.zeros(k, dtype=int)  # All tracks are initially not assigned
    Price = np.zeros(k)

    k1 = min(N, M)
    Assigned_indx = np.zeros(k1, dtype=int)  # Indicates whether a plot has been already assigned to a track

    if N > M:  # More tracks than plots
        while np.sum(Assigned_indx) != M:
            zz = M - np.count_nonzero(Assigned_indx)  # Number of unassigned elements
            indices = np.where(Assigned_indx < 1)[0]  # Indices of unassigned elements
            for q in range(zz):
                m = indices[q]
                assinment_val = A[:, m] - Price
                i1 = np.argmax(assinment_val)
                if Assigned[i1] != 0:
                    Assigned_indx[Assigned[i1] - 1] = 0  # Unassign previous assignment
                Assigned[i1] = m + 1  # Assign plot m to track i1 (use 1-based indexing)
                Assigned_indx[m] = 1  # Mark the plot as assigned
                assinment_val[i1] *= 100  # Finding the next best "bid"
                i2 = np.argmax(assinment_val)
                delta1 = A[i2, m] - Price[i2]
                delta2 = A[i1, m] - Price[i1]
                delta = delta2 - delta1
                Price[i1] += delta + eps  # Update the price of a track
    else:  # More plots than tracks
        while np.sum(Assigned_indx) != N:
            zz = N - np.count_nonzero(Assigned_indx)  # Number of unassigned elements
            indices = np.where(Assigned_indx < 1)[0]  # Indices of unassigned elements
            for q in range(zz):
                n = indices[q]
                assinment_val = A[n, :] - Price
                i1 = np.argmax(assinment_val)
                if Assigned[i1] != 0:
                    Assigned_indx[Assigned[i1] - 1] = 0  # Unassign previous assignment
                Assigned[i1] = n + 1  # Assign track n to plot i1 (use 1-based indexing)
                Assigned_indx[n] = 1  # Mark the track as assigned
                assinment_val[i1] *= 100  # Finding the next best "bid"
                i2 = np.argmax(assinment_val)
                delta1 = A[n, i2] - Price[i2]
                delta2 = A[n, i1] - Price[i1]
                delta = delta2 - delta1
                Price[i1] += delta + eps  # Update the price of a track

    return Assigned
