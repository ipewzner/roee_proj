import std_calcRoee as scr
import numpy as np
import csv

def load_csv_to_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [row for row in reader]
    return np.array(data, dtype=float), len(data)

'''
def load_csv_to_2d_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [row for row in reader]
    return np.array(data, dtype=float), len(data)
'''

def load_csv_to_2d_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = []
        for row in reader:
            if any(row):  # Check if the row contains any non-empty elements
                data.append([float(x) if x else 0.0 for x in row])  # Handle empty values
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

'''
def load_csv_to_3d_array(filename, rows_per_slice, cols_per_slice):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [list(map(float, row)) for row in reader]
    
    # Convert the flat data into a 3D array (number of slices, rows_per_slice, cols_per_slice)
    num_slices = len(data)
    reshaped_data = np.array(data).reshape(num_slices, rows_per_slice, cols_per_slice)
    
    return reshaped_data, reshaped_data.shape

'''
def load_csv_to_3d_array(filename):
    data = []
    current_slice = []

    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            # Strip any trailing empty elements caused by extra commas at the end of the line
            row = [x for x in row if x.strip()]

            if row:  # If the row contains any valid elements
                current_slice.append([float(x) for x in row])
            else:
                # Empty row indicates a new 4x4 matrix slice
                if current_slice:
                    data.append(current_slice)
                    current_slice = []  # Reset for the next slice

        # Append the last slice if it doesn't end with an empty row
        if current_slice:
            data.append(current_slice)

    # Convert the data into a numpy array (3D) and return the shape
    reshaped_data = np.array(data)
    return reshaped_data, reshaped_data.shape


'''
def load_csv_to_1d_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [row for row in reader]
    return np.array(data, dtype=float), len(data)
'''

def load_csv_to_1d_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = []
        for row in reader:
            # Filter out empty strings and convert valid values to float
            filtered_row = [float(x) if x.strip() else 0.0 for x in row]  # Handle empty values as 0.0
            data.append(filtered_row)
    return np.array(data, dtype=float), len(data)


def test_std_calcRoee():
    # Test parameters
    ping_ind = 2
    Tping = 1
    k = 1
    last_ping_number = 1
    n = 2
    std_dis_corr = 1.08E-05

    # Load inputs from CSV files
    MergedYc, _ = load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/MergedYc.csv')
    Rc, _ = load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/RC.csv')
    TracksX, _ = load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/TracksX.csv')
    TracksP, _ = load_csv_to_3d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/TracksP.csv')
    undeleted_tracks_ind, _ = load_csv_to_1d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/std_calcRoee/undeleted_tracks_ind.csv')

    # Select the relevant columns and indices as in the MATLAB code
    MergedYc_k = MergedYc[:, k - 1]  # MATLAB uses 1-based indexing, Python uses 0-based
    TracksX_n = TracksX[:, int(undeleted_tracks_ind[0][n - 1]) - 1]
    TracksP_n = np.squeeze(TracksP[int(undeleted_tracks_ind[0][n - 1]) - 1, :, :])

    # Call the std_calcRoee function
    calculated_std_dis = scr.std_calcRoee(ping_ind, Tping, MergedYc_k, Rc, last_ping_number, TracksX_n, TracksP_n)

    # Define expected result for comparison
    expected_std_dis = 0.00001077282126338

    # Use np.isclose to compare floating point values with tolerance
    assert np.isclose(calculated_std_dis, expected_std_dis, rtol=1e-9), \
        f"calculated_std_dis {calculated_std_dis} does not match expected {expected_std_dis}"

    print("calculated_std_dis:", calculated_std_dis)
    print("expected_std_dis:", expected_std_dis)

if __name__ == "__main__":
    test_std_calcRoee()
    print("Test passed")
