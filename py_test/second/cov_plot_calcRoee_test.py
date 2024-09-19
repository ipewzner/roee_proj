import numpy as np
import csv
from cov_plot_calcRoee import cov_plot_calcRoee  # Assuming you've saved your function in this file
import math


def load_csv_to_array(file_path):
    # Load the CSV file into a NumPy array
    data = np.genfromtxt(file_path, delimiter=',')
    
    # Get the size (number of elements) in the array
    array_size = data.size
    
    return data, array_size


def load_csv_to_2d_array(file_path):
    # Load the CSV file into a NumPy array
    data = np.genfromtxt(file_path, delimiter=',')
    
    # Get the shape of the array to determine if it's 1D or 2D
    if data.ndim == 1:
        # If it's 1D, reshape to 2D with one row
        data = data.reshape(1, -1)
    
    # Get the size (number of elements) in the array
    array_size = data.size
    
    return data, array_size



def test_cov_plot_calcRoee():
    # Test parameters
    # Load inputs from CSV files (assuming r, teta, and sigmaTeta are stored in CSV files)
    MergedRng, _ = load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/cov_plot_calcRoee/MergedRng.csv')
    MergedTeta, _ = load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/cov_plot_calcRoee/MergedTeta.csv')
    #rc, _ = load_csv_to_1d_array('C:\Users\ipewz\Desktop\roee_proj\forTest\cov_plot_calcRoee\Rc.csv')
    sigmaTeta=3
    k=0
    # For simplicity, assuming r, teta, sigmaTeta are scalars

    # Call the cov_plot_calcRoee function
    calculated_rc = cov_plot_calcRoee(MergedRng[k], MergedTeta[k]*math.pi/180, sigmaTeta)

    # Define expected result (2x2 matrix)
    #expected_rc = np.array([[223.9,-3.9384], [-3.9384,223.9]])  # Replace with actual expected values
    expected_rc = np.array([[223.8971507800168865,-3.9383795899505540], [
-3.9383795899505540,223.8971507792297189]])  # Replace with actual expected values

    # Use np.allclose to compare matrices
    assert np.allclose(calculated_rc, expected_rc, rtol=1e-9), \
        f"calculated_rc {calculated_rc} does not match expected {expected_rc}"

    print("calculated_rc:", calculated_rc)
    print("expected_rc:", expected_rc)

if __name__ == "__main__":
    test_cov_plot_calcRoee()
    print("Test passed")
