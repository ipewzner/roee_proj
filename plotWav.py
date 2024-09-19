import matplotlib.pyplot as plt
import wave
from scipy.io import wavfile


def specgramView(sampRate,dataToView):
    fig1 = plt.figure()
    plt.title("Specgram")
    plt.xlabel("Time(s)")
    plt.ylabel("Hz")
    plt.specgram(dataToView,Fs = sampRate)
    '''
    plt.specgram(dataToView[:,0],1024 ,Fs = sampRate)
    plt.specgram(dataToView[:,1],1024 ,Fs = sampRate)
    plt.specgram(dataToView[:,2],1024 ,Fs = sampRate)
    plt.specgram(dataToView[:,3],1024 ,Fs = sampRate)
    plt.specgram(dataToView[:,4],1024 ,Fs = sampRate)
    plt.specgram(dataToView[:,5],1024 ,Fs = sampRate)
    '''
    pass

def plotView(dataToView):
    fig2 = plt.figure()
    timeArr = []
    plt.title("plot")
    plt.xlabel("Time(s)")
    plt.ylabel("Amp")
    plt.plot(dataToView)
    pass


def main():
    data = wavfile.read('C:/Users/ipewz/Desktop/roee_proj/20240916-122129354.wav') #data[0] = sampRate , data[1] = all channels of data
    #data = wavfile.read('C:/Users/ipewz/Desktop/roee_proj/1726488592.wav') #data[0] = sampRate , data[1] = all channels of data
    #print(len(data[1][:,0]))
    #specgramView(data[0],data[1])

    print((data[1][:,]))
    # for i in range(len(data[0][:,])):
        # specgramView(data[0],data[1][:,i]) # data[1][:,x] -> x+1 = channel in use 
    specgramView(data[0],data[1][:,0]) # data[1][:,x] = 2 channel used in recording.
    specgramView(data[0],data[1][:,1]) # data[1][:,x] = 2 channel used in recording.
    specgramView(data[0],data[1][:,2]) # data[1][:,x] = 3 channel used in recording.
    specgramView(data[0],data[1][:,3]) # data[1][:,x] = 4 channel used in recording.
    specgramView(data[0],data[1][:,4]) # data[1][:,x] = 5 channel used in recording.
    specgramView(data[0],data[1][:,5]) # data[1][:,x] = 6 channel used in recording.

    #plotView(data[1][:,0])
    #plotView(data[1][:,1])
    #plotView(data[1][:,2])
    #plotView(data[1][:,3])
    #plotView(data[1][:,4])
    #plotView(data[1][:,5])
    # plotView(data[1])
    
    #specgramView(data[0],data[1]) #will work for 1 channel recording only.
    plt.show()

if __name__ == "__main__":
    main()
