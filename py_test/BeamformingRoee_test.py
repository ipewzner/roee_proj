import BeamformingRoee as bf
import csvToArray as csv2A
import numpy as np
import csv
import matplotlib.pyplot as plt

def save_array_to_csv(array, filename):
    """Saves a 2D NumPy array to a CSV file."""
    with open(filename, 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerows(array)
    
        
def test_BeamformingRoee():
    PingData, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/11.csv')
    matched_filter, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/mf.csv')
    azBeams, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/azbeem.csv')
    pos_sensors, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/possens.csv')
    img, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv')

    # Inspect the shape and size of img
    print(f"Loaded PingData shape: {PingData.shape}")
    print(f"Loaded PingData size: {PingData.size}")
    # Inspect the shape and size of img
    print(f"Loaded matched_filter shape: {matched_filter.shape}")
    print(f"Loaded matched_filter size: {matched_filter.size}")
    # Inspect the shape and size of img
    print(f"Loaded azBeams shape: {azBeams.shape}")
    print(f"Loaded azBeams size: {azBeams.size}")
    # Inspect the shape and size of img
    print(f"Loaded pos_sensors shape: {pos_sensors.shape}")
    print(f"Loaded pos_sensors size: {pos_sensors.size}")
    # Inspect the shape and size of img
    print(f"Loaded img shape: {img.shape}")
    print(f"Loaded img size: {img.size}")

    # Ensure that img can be reshaped to (3201, 72)
    if img.size == 3201 * 72:
        img = img.reshape(3201, 72)
    else:
        raise ValueError(f"Cannot reshape img of size {img.size} into shape (3201, 72)")

    # Run the beamforming function
    result = bf.BeamformingRoee(PingData, matched_filter, azBeams, pos_sensors, 128e3, 3201)
    #result = np.round(result,5 )
    #img = np.round(img, 5)

    save_array_to_csv(result, 'C:/Users/ipewz/Desktop/roee_proj/forTest/result.csv')

  # Find the maximum absolute difference between img and result
    max_diff = np.max(np.abs(img - result))

    print("Maximum absolute difference:", max_diff)
    
    
    # Find the index with the maximum absolute difference between img and result
    max_diff_index = np.argmax(np.abs(img - result))
    max_diff_value = np.max(np.abs(img - result))

    print("Index with the maximum absolute difference:", max_diff_index)
    print("Maximum absolute difference:", max_diff_value)
    


    #--------------------------------------

    # Visualize the result and the original img side by side
    plt.figure(figsize=(14, 6))

    # Plot the result
    plt.subplot(1, 2, 1)
    plt.imshow(result, aspect='auto', cmap='hot')
    plt.colorbar(label='Amplitude')
    plt.title('Beamforming Result Heatmap')
    plt.xlabel('Azimuth Beam')
    plt.ylabel('Time Index')

    # Plot the original img
    plt.subplot(1, 2, 2)
    plt.imshow(img, aspect='auto', cmap='hot')
    plt.colorbar(label='Amplitude')
    plt.title('Original Image Heatmap')
    plt.xlabel('Azimuth Beam')
    plt.ylabel('Time Index')

    # Show the plots
    plt.tight_layout()
    plt.show()
    #--------------------------------------
    
    # Compare the arrays
    assert np.allclose(result, img), "Beamforming output does not match the expected image."

if __name__ == "__main__":
    test_BeamformingRoee()
    print("Everything passed")
