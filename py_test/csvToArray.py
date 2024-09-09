import numpy as np

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
