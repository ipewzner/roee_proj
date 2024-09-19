import numpy as np
import csv
import Kalman_filterRoee as kfr

yc=[21.042781868824051, 3.841971161392103]
Rc=[[2.238971507800169e+02,-3.938379589950554],[-3.938379589950554,2.238971507792297e+02]]
T=1
track_id=2

X=[
    21.161515372168633,
3.863649411852741,
0.0,
0.0
]

P=[
    [1.132149658268534e+03,-19.914702437256373,0.0,0.0],
    [-19.914702437256373,1.132149658264554e+03,0.0,0.0],
    [0.0,0.0,5.0,0.0],
    [0.0,0.0,0.0,5.0]
]
teta_rad=1.3902064596321329
X_upd=[21.062313709494028,3.8455360465020005,-0.000437714640049,-0.000087309337809]
P_upd=[
    [1.870652549163168e+02,-3.288122964961385,0.822517256582195,-5.314025859776139e-05],
    [-3.288122964961381,1.870652549156597e+02,-5.314025859773364e-05,0.822517256582185],
    [0.822517256582195,-5.314025859774572e-05,4.981626151835971,-3.220118096084868e-04],
    [-5.314025859777465e-05,0.822517256582185,-3.220118096084868e-04,4.981626151835907]
]
def test_Kalman_filterRoee():


    result_X_upd, result_P_upd, result_teta_rad = kfr.Kalman_filterRoee(yc,Rc, T, X, P)
    
    assert np.array_equal(teta_rad, result_teta_rad), "teta_rad does not match expected values"
    assert np.allclose(X_upd, result_X_upd), "X_upd does not match expected values"
    assert np.allclose(P_upd, result_P_upd), "P_upd does not match expected values"
    print("tests 1 passed")
    
    print("All tests passed")

if __name__ == "__main__":
    test_Kalman_filterRoee()
    