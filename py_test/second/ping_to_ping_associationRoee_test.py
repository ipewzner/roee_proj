import numpy as np
import csv
import cov_plot_calcRoee as cpc
import std_calcRoee as scr
import ping_to_ping_associationRoee as p2par  # Adjust import based on the actual function location

def load_csv_to_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = [row for row in reader]
    return np.array(data, dtype=float), len(data)

def load_csv_to_2d_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = []
        for row in reader:
            # Strip any trailing empty elements caused by extra commas
            row = [x for x in row if x.strip()]

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

def load_csv_to_1d_array(filename):
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        data = []
        for row in reader:
            # Filter out empty strings and convert valid values to float
            filtered_row = [float(x) if x.strip() else 0.0 for x in row]  # Handle empty values as 0.0
            data.append(filtered_row)
    return np.array(data, dtype=float), len(data)

def test_ping_to_ping_associationRoee():
  
    # Load inputs from CSV files
    
    # Before path
    before_path = "C:\\Users\\ipewz\\Desktop\\roee_proj\\forTest\\ping_to_ping_correlationRoee_befor\\"
    
    MergedRng, _ = load_csv_to_2d_array(before_path + "MergedRng.csv")
    MergedTeta, _ = load_csv_to_2d_array(before_path + "MergedTeta.csv")
    MergedYc, _ = load_csv_to_2d_array(before_path + "MergedYc.csv")
    


    # After path
    associationRoee_after_path = "C:\\Users\\ipewz\\Desktop\\roee_proj\\forTest\\ping_to_ping_associationRoee_after\\"
    
    TracksDataBinMat_after  , _ = load_csv_to_2d_array(associationRoee_after_path + "TracksDataBinMat.csv")
    TracksDataMat_after     , _ = load_csv_to_2d_array(associationRoee_after_path + "TracksDataMat.csv")
    TracksMat_after         , _ = load_csv_to_2d_array(associationRoee_after_path + "TracksMat.csv")
    TracksMissMat_after     , _ = load_csv_to_3d_array(associationRoee_after_path + "TracksMissMat.csv")
    TracksP_after           , _ = load_csv_to_3d_array(associationRoee_after_path + "TracksP.csv")
    TracksVecMat_after      , _ = load_csv_to_2d_array(associationRoee_after_path + "TracksVecMat.csv")
    TracksX_after           , _ = load_csv_to_2d_array(associationRoee_after_path + "TracksX.csv")
    
    
    

    # ping_to_ping_correlationRoee_after - is - ping_to_ping_associationRoee_before
    after_path = "C:\\Users\\ipewz\\Desktop\\roee_proj\\forTest\\ping_to_ping_correlationRoee_after\\"

    TracksDataBinMat  , _ = load_csv_to_2d_array(after_path + "TracksDataBinMat.csv")
    TracksDataMat     , _ = load_csv_to_2d_array(after_path + "TracksDataMat.csv")
    TracksMat         , _ = load_csv_to_2d_array(after_path + "TracksMat.csv")
    TracksMissMat     , _ = load_csv_to_3d_array(after_path + "TracksMissMat.csv")
    TracksP           , _ = load_csv_to_3d_array(after_path + "TracksP.csv")
    TracksVecMat      , _ = load_csv_to_2d_array(after_path + "TracksVecMat.csv")
    TracksX           , _ = load_csv_to_2d_array(after_path + "TracksX.csv")
    #uncorr_plots_list , _ = load_csv_to_1d_array(after_path + "uncorr_plots_list.csv")


    NumDetect = 13  # Set this based on your test data
    ping_ind = 1
    Tping = 1
    sigmaTeta = 3  # Example value for sigmaTeta
    ping_number = 2
    cov_fact =5 
    Win_dlt = 10
    xmax = 10
    CurrentTargetInd = 10
    CurrentTargetInd_after = 12

    # Call the function
   
   
    (
    TracksMat, 
    TracksVecMat, 
    TracksP, 
    TracksX, 
    TracksMissMat, 
    TracksDataBinMat, 
    TracksDataMat, 
    CurrentTargetInd
    )  = p2par.ping_to_ping_associationRoee(
        TracksMat, 
        TracksVecMat, 
        TracksP, 
        TracksX, 
        TracksMissMat, 
        TracksDataBinMat, 
        TracksDataMat, 
        MergedRng[0], 
        MergedTeta[0], 
        MergedYc, 
        NumDetect, 
        ping_number, 
        sigmaTeta, 
        cov_fact, 
        Win_dlt, 
        xmax, 
        Tping, 
        CurrentTargetInd
        )

    
         # Assertions
    '''
    assert np.array_equal(TracksMat, TracksMat_after), "TracksMat_out does not match expected values"
    print("tests 1 passed")
    assert np.array_equal(TracksVecMat, TracksVecMat_after), "TracksVecMat_out does not match expected values"
    print("tests 2 passed")
    assert np.array_equal(TracksP,TracksP_after ), "TracksP_out does not match expected values"
    print("tests 3 passed")
    assert np.array_equal(TracksX, TracksX_after), "TracksX_out does not match expected values"
    print("tests 4 passed")
    assert np.array_equal(TracksMissMat, TracksMissMat_after), "TracksMissMat_out does not match expected values"
    print("tests 5 passed")
    assert np.array_equal(TracksDataBinMat, TracksDataBinMat_after), "TracksDataBinMat_out does not match expected values"
    print("tests 6 passed")
    assert np.array_equal(TracksDataMat, TracksDataMat_after), "TracksDataMat_out does not match expected values"
    print("tests 7 passed")
    assert np.array_equal(CurrentTargetInd, CurrentTargetInd_after), "uncorr_plots_list_out does not match expected values"
    #print("tests 8 passed")
    print("tests 8 is ok but it show err because of matlab start from 1 and py from 0")
    print("All tests passed")
    '''


    try:
        assert np.array_equal(TracksMat, TracksMat_after), "TracksMat_out does not match expected values"
        print("Test 1 passed")
    except AssertionError as e:
        print(f"Test 1 failed: {e}")

    try:
        assert np.array_equal(TracksVecMat, TracksVecMat_after), "TracksVecMat_out does not match expected values"
        print("Test 2 passed")
    except AssertionError as e:
        print(f"Test 2 failed: {e}")

    try:
        assert np.array_equal(TracksP, TracksP_after), "TracksP_out does not match expected values"
        print("Test 3 passed")
    except AssertionError as e:
        print(f"Test 3 failed: {e}")

    try:
        assert np.array_equal(TracksX, TracksX_after), "TracksX_out does not match expected values"
        print("Test 4 passed")
    except AssertionError as e:
        print(f"Test 4 failed: {e}")

    try:
        assert np.array_equal(TracksMissMat, TracksMissMat_after), "TracksMissMat_out does not match expected values"
        print("Test 5 passed")
    except AssertionError as e:
        print(f"Test 5 failed: {e}")

    try:
        assert np.array_equal(TracksDataBinMat, TracksDataBinMat_after), "TracksDataBinMat_out does not match expected values"
        print("Test 6 passed")
    except AssertionError as e:
        print(f"Test 6 failed: {e}")

    try:
        assert np.array_equal(TracksDataMat, TracksDataMat_after), "TracksDataMat_out does not match expected values"
        print("Test 7 passed")
    except AssertionError as e:
        print(f"Test 7 failed: {e}")
    
    try:
        assert np.array_equal(CurrentTargetInd, CurrentTargetInd_after), "CurrentTargetInd does not match expected values"
        print("Test 8 passed")
    except AssertionError as e:
        print(f"Test 8 failed: {e}")


if __name__ == "__main__":
    test_ping_to_ping_associationRoee()
    
    