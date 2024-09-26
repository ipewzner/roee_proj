import csvToArray as csv2A
import numpy as np
import csv
import matplotlib.pyplot as plt
import os
import re
import time

def get_latest_result_file(path):
    """Finds the result file with the highest number in the specified directory."""
    result_files = [f for f in os.listdir(path) if re.match(r'result_\d+\.csv', f)]
    if not result_files:
        return None
    # Extract numbers from the filenames and find the one with the highest number
    result_files.sort(key=lambda f: int(re.findall(r'\d+', f)[0]), reverse=True)
    return os.path.join(path, result_files[0])

def visualize_result_with_img(result_file, img_file):
    """Visualizes the result file and img file side by side."""
    # Load the result file and the img file
    result, _ = csv2A.load_csv_to_2d_array(result_file)
    img, _ = csv2A.load_csv_to_2d_array(img_file)

    # Ensure that img can be reshaped to (3201, 72)
    if img.size == 3201 * 72:
        img = img.reshape(3201, 72)
    else:
        raise ValueError(f"Cannot reshape img of size {img.size} into shape (3201, 72)")

    # Find the maximum absolute difference between img and result
    max_diff = np.max(np.abs(img - result))
    print(f"Maximum absolute difference between {os.path.basename(result_file)} and img: {max_diff}")

    # Visualize the result and the original img side by side
    plt.figure(figsize=(14, 6))

    # Plot the result
    plt.subplot(1, 2, 1)
    plt.imshow(result, aspect='auto', cmap='hot')
    plt.colorbar(label='Amplitude')
    plt.title(f'Beamforming Result: {os.path.basename(result_file)}')
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

def main():
    result_dir = 'C:/Users/ipewz/Desktop/roee_proj/c/RT/'  # Directory where result files are stored
    img_file = 'C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv'  # The img file to compare with

    # Loop to continuously check for the latest result file and visualize
    while True:
        latest_result_file = get_latest_result_file(result_dir)
        #latest_result_file = "c:/Users/ipewz/Desktop/roee_proj/c/RT/result_005.csv"
        if latest_result_file:
            print(f"Latest result file: {latest_result_file}")
            visualize_result_with_img(latest_result_file, img_file)
        else:
            print("No result files found.")

        # Wait before checking again
        time.sleep(1)

if __name__ == "__main__":
    main()
