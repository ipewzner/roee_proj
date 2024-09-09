import Shortproc_ping_dataRoee  as spd
import csvToArray as csv2A
import IntegralImage
import numpy as np


def test_Shortproc_ping_dataRoee():
    
    
    
    PingData, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/11.csv')
    matched_filter, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/mf.csv')
    azBeams, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/azbeem.csv')
    pos_sensors, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/possens.csv')
    img, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/forTest/img.csv')
    IndVec, size = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/IndVec.csv')
    rVec, size = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/forTest/rvec.csv')

    
    r_IndVec, r_rVec, r =spd.Shortproc_ping_dataRoee(PingData, matched_filter, 15, 128e3, azBeams, pos_sensors, 3201, 999, 0, 1)
    
     # Use np.allclose() to compare arrays with tolerance for floating point errors
    assert np.array_equal(r_IndVec, IndVec), "r_IndVec and IndVec do not match"
    assert np.allclose(r_rVec, rVec), "r_rVec and rVec do not match"
    assert r == 999, "r is not equal to 999"

    print("r_IndVec", r_IndVec)
    print("IndVec", IndVec)
    print("r_rVec", r_rVec)
    print("rVec", rVec)

if __name__ == "__main__":
    test_Shortproc_ping_dataRoee()
    print("Everything passed")
