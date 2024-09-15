import CalcBinary as CB
import numpy as np
import csvToArray as csv2A
import matplotlib.pyplot as plt



def visuelize_arrays(result,expected):
      #--------------------------------------

    # Visualize the result and the original img side by side
    
    
    plt.figure(figsize=(14, 6))

    # Plot the result
    plt.subplot(1, 2, 1)
    plt.imshow(result, aspect='auto', cmap='hot')
    plt.colorbar(label='Amplitude')
    plt.title('Result Heatmap')
    plt.xlabel('Azimuth Beam')
    plt.ylabel('Time Index')

    # Plot the original img
    plt.subplot(1, 2, 2)
    plt.imshow(expected, aspect='auto', cmap='hot')
    plt.colorbar(label='Amplitude')
    plt.title('expected')
    plt.xlabel('Azimuth Beam')
    plt.ylabel('Time Index')

    # Show the plots
    plt.tight_layout()
    plt.show()
    #--------------------------------------
    
def test_calc_binary_4x4():
    print("in 1")
    # Test input (4x4 matrix)
    img = np.array([[0.1, 0.3, 0.2, 0.4],
                    [0.5, 0.8, 0.7, 0.6],
                    [0.4, 0.3, 0.9, 0.7],
                    [0.8, 0.5, 0.6, 0.2]])
    
    # Expected output (manually computed or from MATLAB)
    expected_output = np.array([[0, 0, 0, 0],
                                [0, 0, 0, 0],
                                [0, 0, 0, 0],
                                [0, 0, 0, 0]])
    
    # Run the function
    output = CB.CalcBinary(img)
    
    # Check if the output matches the expected output
    assert np.allclose(output, expected_output), "Test failed for 4x4 matrix!"
    print("Test passed for 4x4 matrix!")

def test_calc_binary_5x5():
    # Test input (5x5 matrix)
    img = np.array([[0.1, 0.3, 0.2, 0.4, 0.7],
                    [0.5, 0.8, 0.7, 0.6, 0.9],
                    [0.4, 0.3, 0.9, 0.7, 0.5],
                    [0.8, 0.5, 0.6, 0.2, 0.1],
                    [0.6, 0.7, 0.8, 0.9, 0.4]])
    
    # Expected output (manually computed or from MATLAB)
    expected_output = np.array([[0, 0, 0, 0, 0],
                                [0, 0, 0, 0, 0],
                                [0, 0, 0, 0, 0],
                                [0, 0, 0, 0, 0],
                                [0, 0, 0, 0, 0]])
    
    # Run the function
    output = CB.CalcBinary(img)
    
    # Check if the output matches the expected output
    assert np.allclose(output, expected_output), "Test failed for 5x5 matrix!"
    print("Test passed for 5x5 matrix!")


def test_calc_binary3():
    print("in 2")

    # Test input
    img = np.array([[0.1, 0.8, 0.2],
                [0.4, 1.2, 0.5],  # Increased value in (1, 1) position
                [0.7, 0.6, 0.9]])

    expected_output = np.array([[0, 0, 0],  # May change depending on calculations
                             [1, 0, 0],  # May change depending on calculations
                             [0, 0, 0]])
    print("img",img)
    # Expected output (manually computed or from MATLAB)
    
    
    print("expected_output",expected_output)
    
    # Run the function
    output = CB.CalcBinary(img)
    
    print("output",output)
    
    # Check if the output matches the expected output
    assert np.allclose(output, expected_output), "test_calc_binary3 failed!"
    print("Test passed!")



def test_calc_binary2():
    print("in 2")

    # Test input
    img = np.array([[0.1, 0.3, 0.2],
                    [0.4, 0.8, 0.5],
                    [0.7, 0.6, 0.9]])
    print("img",img)
    # Expected output (manually computed or from MATLAB)
    expected_output = np.array([[0, 0, 0],
                                [0, 0, 0],
                                [0, 0, 0]])
    
    print("expected_output",expected_output)
    
    # Run the function
    output = CB.CalcBinary(img)
    
    print("output",output)
    
    # Check if the output matches the expected output
    assert np.allclose(output, expected_output), "Test failed!"
    print("Test passed!")


def test_CalcBinary():
    result = CB.CalcBinary(np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]]))
    expected = np.array([[0, 1, 1], [1, 1, 1], [1, 1, 1]])
    assert np.array_equal(result, expected), f"Expected {expected}, but got {result}"

def test_CalcBinary4():
     

    data_array, size = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv')
    bm, size = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest//bm.csv')

    result = CB.CalcBinary(data_array)
    expected = np.array(bm)
    
    visuelize_arrays(result,expected)
    
    assert np.array_equal(result, expected), f"Expected {expected}, but got {result}"

if __name__ == "__main__":
    
    # Run the tests
    #test_calc_binary_5x5()
    print("1 passed")
    #Stest_calc_binary_4x4()
    print("2 passed")
    #test_calc_binary3()
    print("3 passed")
    #test_calc_binary2()
    #test_CalcBinary()
    print("4 passed")
    test_CalcBinary4()
    
    print("Everything passed")
