import numpy as np

''' it work the same may be butter

def integral_image(img):
    return np.cumsum(np.cumsum(img, axis=0), axis=1)

'''
def IntegralImage(A):
    """
    This function calculates the integral image of 'A'.
    """

    N, M = A.shape
    
    z = np.zeros((N, M))
    I = np.zeros((N, M))

    for m in range(M):
        for n in range(N):
            z1 = z[n, m-1] if m > 0 else 0
            z[n, m] = z1 + A[n, m]

            I1 = I[n-1, m] if n > 0 else 0
            I[n, m] = I1 + z[n, m]

    return I
'''
# Example usage:
A = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
I = IntegralImage(A)
print(I)
'''
