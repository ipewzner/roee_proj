import matplotlib.pyplot as plt
import wave
from scipy.io import wavfile


def specgramView(sampRate,dataToView):
    fig1 = plt.figure()
    plt.title("Specgram")
    plt.xlabel("Time(s)")
    plt.ylabel("Hz")
    plt.specgram(dataToView,Fs = sampRate)

def plotView(dataToView):
    fig2 = plt.figure()
    timeArr = []
    plt.title("plot")
    plt.xlabel("Time(s)")
    plt.ylabel("Amp")
    plt.plot(dataToView)

def main():
    data = wavfile.read('C:/Users/ipewz/Desktop/roee_proj/1726488592.wav') #data[0] = sampRate , data[1] = all channels of data
    i = 0
    try:
        while True:
            specgramView(data[0],data[1][:,i]) # data[1][:,x] -> x+1 = channel in use
            plotView(data[1][:,0])
            i+=1
            plt.show()
    except:
        print("no more channels")

if __name__ == "__main__":
    main()
