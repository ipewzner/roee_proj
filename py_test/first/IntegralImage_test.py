import IntegralImage as im
import csvToArray as csv2A

import numpy as np

def test_IntegralImage():
    # Test case 1: Small 3x3 matrix
    A = np.array([[1, 2, 3], 
                  [4, 5, 6], 
                  [7, 8, 9]])
    
    print("A",A)
    expected = np.array([[ 1,  3,  6],
                         [ 5, 12, 21],
                         [12, 27, 45]])
    print("expected",expected)
    
    result = im.IntegralImage(A)

    assert np.array_equal(result, expected), f"Test failed! Expected:\n{expected}\nBut got:\n{result}"

def test_IntegralImage_2():

    # Test case 2: 2x2 matrix
    A = np.array([[1, 2],
                  [3, 4]])
    expected = np.array([[1, 3],
                         [4, 10]])
    
    result = im.IntegralImage(A)
    assert np.array_equal(result, expected), f"Test failed! Expected:\n{expected}\nBut got:\n{result}"

def test_IntegralImage_3():

    # Test case 3: 1x1 matrix
    A = np.array([[5]])
    expected = np.array([[5]])
    
    result = im.IntegralImage(A)
    assert np.array_equal(result, expected), f"Test failed! Expected:\n{expected}\nBut got:\n{result}"

def test_IntegralImage_4():
     

    data_array, size = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv')
    bm, size = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/IntI.csv')

    result = im.IntegralImage(data_array)
    expected = np.array(bm)
    
    assert np.array_equal(result, expected), f"Expected {expected}, but got {result}"



if __name__ == "__main__":
    
    # Run the tests
    test_IntegralImage()
    print("1 passed")
    test_IntegralImage_2()
    print("2 passed")
    test_IntegralImage_3()
    print("3 passed")
    test_IntegralImage_4()
    print("4 passed")
    
    print("Everything passed")
