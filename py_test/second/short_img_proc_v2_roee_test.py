import numpy as np
import short_img_proc_v2_roee as sip
import csvToArray as csv2A

def test_short_img_proc_v2_roee():
    # Load test data from CSV files
    
    IndVec, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/IndVec.csv')
    rVec, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/rVec.csv')
    azBeams, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/azBeams.csv')
    #heading_deg, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/heading_deg.csv')
    sigmaTeta = 3  # Example value for sigmaTeta, can be adjusted

      
    # Load expected results
    expected_merged_detections_pingRng, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingRng.csv')
    expected_merged_detections_pingTeta, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingTeta.csv')
    expected_merged_detections_pingYc, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingYc.csv')

    # Run the function to get results
    merged_detections_pingRng, merged_detections_pingTeta, merged_detections_pingYc = sip.short_img_proc_v2_roee(
        IndVec[:24],
        rVec[:24], 
        sigmaTeta,
        azBeams,
        203.50197
    )

    # Assertions to validate results
    assert np.allclose(merged_detections_pingRng, expected_merged_detections_pingRng), "Merged detection ranges do not match"
    assert np.allclose(merged_detections_pingTeta, expected_merged_detections_pingTeta), "Merged detection angles do not match"
    assert np.allclose(merged_detections_pingYc, expected_merged_detections_pingYc), "Merged detection Y coordinates do not match"

    # Print outputs for verification
    print("Merged Detection Ranges:", merged_detections_pingRng)
    print("Expected Ranges:", expected_merged_detections_pingRng)
    print("Merged Detection Teta:", merged_detections_pingTeta)
    print("Expected Teta:", expected_merged_detections_pingTeta)
    print("Merged Detection Yc:", merged_detections_pingYc)
    print("Expected Yc:", expected_merged_detections_pingYc)

if __name__ == "__main__":
    test_short_img_proc_v2_roee()
    print("Everything passed")
