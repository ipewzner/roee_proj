import numpy as np

def cov_plot_calcRoee(r, teta, sigmaTeta):
    sigmaR = 0.3

    # Calculate rc11
    rc11 = (r**2 * np.exp(-2 * sigmaTeta**2) * 
            (np.cos(teta)**2 * (np.cosh(2 * sigmaTeta**2) - np.cosh(sigmaTeta**2)) + 
             np.sin(teta)**2 * (np.sinh(2 * sigmaTeta**2) - np.sinh(sigmaTeta**2))) + 
            sigmaR**2 * np.exp(-2 * sigmaTeta**2) * 
            (np.cos(teta)**2 * (2 * np.cosh(2 * sigmaTeta**2) - np.cosh(sigmaTeta**2)) + 
             np.sin(teta)**2 * (2 * np.sinh(2 * sigmaTeta**2) - np.sinh(sigmaTeta**2))))

    # Calculate rc22
    rc22 = (r**2 * np.exp(-2 * sigmaTeta**2) * 
            (np.sin(teta)**2 * (np.cosh(2 * sigmaTeta**2) - np.cosh(sigmaTeta**2)) + 
             np.cos(teta)**2 * (np.sinh(2 * sigmaTeta**2) - np.sinh(sigmaTeta**2))) + 
            sigmaR**2 * np.exp(-2 * sigmaTeta**2) * 
            (np.sin(teta)**2 * (2 * np.cosh(2 * sigmaTeta**2) - np.cosh(sigmaTeta**2)) + 
             np.cos(teta)**2 * (2 * np.sinh(2 * sigmaTeta**2) - np.sinh(sigmaTeta**2))))

    # Calculate rc12
    rc12 = (np.sin(teta) * np.cos(teta) * np.exp(-4 * sigmaTeta) * 
            (sigmaR**2 + (r**2 + sigmaR**2) * (1 - np.exp(sigmaTeta**2))))

    # Return the covariance matrix as a 2x2 numpy array
    rc = np.array([[rc11, rc12], [rc12, rc22]])

    return rc
