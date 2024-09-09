import DetectionBlobsRoee as DB
import numpy as np
import csvToArray as csv2A


def test_DetectionBlobsRoee():

    cl, size = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/CL.csv')
    BinaryMap, size = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest//bm.csv')

    result = DB.DetectionBlobsRoee(BinaryMap, 1, 10000)
    expected = np.array(cl)
    

    print("result",result)
    print("expected",expected)
    assert np.array_equal(result, expected), f"Expected {expected}, but got {result}"

if __name__ == "__main__":
    test_DetectionBlobsRoee()
    print("Everything passed")
    