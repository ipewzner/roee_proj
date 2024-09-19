import matplotlib.pyplot as plt
from scipy.io import wavfile
import warnings
warnings.filterwarnings('ignore')

def specgramView(fig, sampRate, dataToView, num, channel_cnt):
    ax = fig.add_subplot(channel_cnt, 2, num * 2 - 1)  # Specgram on the left
    if num ==1 :
        ax.set_title( "Specgram", pad = 8)
    ax.set_xlabel("Time(s)")
    ax.set_ylabel("ch" + str(num))
    ax.specgram(dataToView, Fs=sampRate)

def plotView(fig, dataToView, num, channel_cnt):
    ax = fig.add_subplot(channel_cnt, 2, num * 2)  # Plot on the right
    if num ==1 :
        ax.set_title("Plot", pad = 8)
    ax.set_xlabel("Time(s)")
    ax.set_ylabel("ch" + str(num))
    ax.plot(dataToView)

def main():
    data = wavfile.read('C:/Users/ipewz/Desktop/roee_proj/1726488592.wav')  # data[0] = sampRate , data[1] = all channels of data
    fig = plt.figure(figsize=(12, 8))
    plt.subplots_adjust(wspace=0.4, hspace=0.4)
    try:
        print(len(data[1][0]), "channels on this .wav file")
        for i in range(len(data[1][0])):
            specgramView(fig, data[0], data[1][:, i],  i + 1, len(data[1][0]))
            plotView(fig, data[1][:, i], i + 1, len(data[1][0]))
    except:
        print("all channels viewed.")
    finally:
        plt.show()
        pass

if __name__ == "__main__":
    main()
