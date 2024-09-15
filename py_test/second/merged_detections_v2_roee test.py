import numpy as np
import merged_detections_v2_roee as md
import csvToArray as csv2A

def test_merged_detections_v2_roee():
    # Load test data from CSV files
    detections_rng, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_rng.csv')
    detections_teta, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_teta.csv')
    detections_yc, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/detections_yc.csv')
    
    # Load expected results
    expected_merged_detections_ping_rng, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingRng.csv')
    expected_merged_detections_ping_teta, _ = csv2A.load_csv_to_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingTeta.csv')
    expected_merged_detections_ping_yc, _ = csv2A.load_csv_to_2d_array('C:/Users/ipewz/Desktop/roee_proj/fortest_second/expected_merged_pingYc.csv')

    # Run the function to get results
    merged_detections_ping_rng, merged_detections_ping_teta, merged_detections_ping_yc = md.merged_detections_v2_roee(
        detections_rng,
        detections_teta,
        detections_yc
    )

    # Assertions to validate results
    assert np.allclose(merged_detections_ping_rng, expected_merged_detections_ping_rng), "Merged detection ranges do not match"
    assert np.allclose(merged_detections_ping_teta, expected_merged_detections_ping_teta), "Merged detection angles do not match"
    assert np.allclose(merged_detections_ping_yc, expected_merged_detections_ping_yc), "Merged detection Y coordinates do not match"

    # Print outputs for verification
    print("Merged Detection Ranges:", merged_detections_ping_rng)
    print("Expected Ranges:", expected_merged_detections_ping_rng)
    print("Merged Detection Teta:", merged_detections_ping_teta)
    print("Expected Teta:", expected_merged_detections_ping_teta)
    print("Merged Detection Yc:", merged_detections_ping_yc)
    print("Expected Yc:", expected_merged_detections_ping_yc)

if __name__ == "__main__":
    test_merged_detections_v2_roee()
    print("Everything passed")
