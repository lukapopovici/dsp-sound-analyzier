import numpy as np
from scipy.io import wavfile

#hard set filename so the script can be called from the NI program
samplerate, data = wavfile.read('46.wav')

num_samples = samplerate * 3

data = data[:num_samples]

wavFileInfo = open("wafeInfo.txt", "a")
wavFileInfo.write(str(samplerate) + '\n')
wavFileInfo.write(str(num_samples) + '\n')
wavFileInfo.close()

print(samplerate)
print(data.size)
print(data)

np.savetxt("waveData.txt", data, fmt="%2.0f")
