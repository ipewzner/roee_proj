import numpy as np
import AuctionRoee as ar

A = np.array([
    [50.000010772821263,10000,10000,10000,10000,10000],
    [10000,10000,50.002659693357657,10000,10000,50.038823749619070],
    [10000,50.000007769266581,10000,10000,50.032959921056111,10000],
    [10000,10000,10000,50.016144755473334,10000,10000]
])
assigned= np.array([1,3,2,4,0,0])


def test_AuctionRoee():
  
    result=ar.AuctionRoee(A)
    assert np.allclose(assigned, result), "result does not match expected values"
    print("test passed")
    

if __name__ == "__main__":
    test_AuctionRoee()
    

