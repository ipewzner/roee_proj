import numpy as np
import csv

def load_csv_to_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [row for row in reader]
    return np.array(data, dtype=float), len(data)

def load_csv_to_2d_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [row for row in reader]
    return np.array(data, dtype=float), len(data)


def load_csv_to_3d_array2(filename):
    data = []
    current_slice = []
    
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            if row:  # If the row is not empty
                current_slice.append([float(x) for x in row])
            else:  # Empty row indicates a new slice
                if current_slice:
                    data.append(current_slice)
                    current_slice = []
                    
        if current_slice:  # Add the last slice if the file does not end with an empty row
            data.append(current_slice)

    return np.array(data), len(data)


def load_csv_to_3d_array(filename, rows_per_slice, cols_per_slice):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [list(map(float, row)) for row in reader]
    
    # Convert the flat data into a 3D array (number of slices, rows_per_slice, cols_per_slice)
    num_slices = len(data)
    reshaped_data = np.array(data).reshape(num_slices, rows_per_slice, cols_per_slice)
    
    return reshaped_data, reshaped_data.shape

def load_csv_to_1d_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [row for row in reader]
    return np.array(data, dtype=float), len(data)

