# dsp-sound-analyzier

Homework for my DSP class where we used National Instruments tools to make a sound file analyzing tool.

I got scammed into taking it because the subject had "Data" in the name, but it's still a decent project nontheless.

## Features

- **WAV File Import:**  
  Imports sound data from a `.wav` file using a Python script (`main.py`). The script extracts sample rate and a fixed number of samples, saving them to text files for use in the NI app.

   It's called directly from inside the NI program so I suggest going around and changing it accordingly based on what your wav is. 

   ### Also, I think the code is hardset to 2 audio channels, so either thinker with the audio file before processing or you lose information. 

- **Graphical User Interface:**  
  Built with National Instruments LabWindows/CVI, the GUI allows users to:
  - Load and visualize sound data.
  - Navigate through the signal using "Next" and "Previous" buttons.
  - Select intervals for analysis.

- **Signal Processing:**  
    ### Note: Default sampling rate is 44,100 Hz, and it.s changed in the code. Like:
    ```
    double numSamples = 44100.0;
    double dt = 1.0 / 44100.0;
    ```
  - **Filtering:**  
    - Median filter
    - First-order filter (low-pass)
    - Filters can be selected from a dropdown menu.
  - **FFT Analysis:**  
    - Computes and displays the Fast Fourier Transform (FFT) of selected intervals.
    - Plots both the original and windowed (Blackman, Hanning) signals.
    - If you wanna know more about FFT contact [this guy](https://extremq.com/unexpected-fourier).
  - **Statistics:**  
    - Calculates and displays mean, variance, skewness, and kurtosis for the selected interval, the usual.
  - **Derivative Calculation:**  
    - Option to display the derivative of the signal.

- **Plot Export:**  
  - Save graphs as JPEG images with timestamped filenames.
  ### Important: Some overwrite previous versions. 
- **Envelope Extraction:**  
  - Option to display the envelope of the signal.
![image](https://github.com/user-attachments/assets/10dc472e-a537-4da7-8135-12558b5bbefa)

## How It Works

1. **Data Extraction:**  
   The user runs the Python script to extract data from a WAV file. The script saves the sample rate and a fixed number of samples to text files.

2. **Data Loading:**  
   The C application loads the sample rate and data from these files and displays the waveform.

3. **Analysis:**  
   Users can select intervals, apply filters, compute FFT, and view statistics or derivatives.

4. **Export:**  
   Plots can be saved as JPEG images for reporting or further analysis.

## Requirements

- National Instruments LabWindows/CVI (good luck getting the license)
- Python 3.x (for the data extraction script)
- `numpy` and `scipy` Python packages

### Note: All the english comments in the Et1.c have been made by GPT so I claim 0 responsability for everything written there. 
