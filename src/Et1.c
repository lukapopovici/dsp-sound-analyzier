#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Et1.h"

static int panelHandle;

#define SAMPLE_RATE		0
#define NPOINTS			1

//==============================================================================
// Global variables

float *globalFloatVector = NULL; 
size_t globalFloatVectorSize = 0;

int hand;

int waveInfo[2]; 
				
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
int showDerivative = 0;
int TotalPoint=0;
int currentStartIndex = 0; 
int currentEndIndex = 1;


int wavePanel = 0;
int acqPanel = 0;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;
	if ((panelHandle = LoadPanel (0, "Et1.uir", PANEL)) < 0)
		return -1;
	
	//adaugare manuala a optiunilor de filtru
	//panel hardcodat
	InsertListItem(PANEL, PANEL_RING, -1, "Mediana", 1); 
	InsertListItem(PANEL, PANEL_RING, -1, "Elem de Ordin 1", 0); 
	SetPanelAttribute(panelHandle, ATTR_CALLBACK_FUNCTION_POINTER, OnPanelCallback);
	
	DisplayPanel (panelHandle);
	wavePanel = LoadPanel (0, "Et1.uir", WAVE_PANEL);
	
	//DisplayPanel(wavePanel);
	//SetCtrlAttribute(wavePanel, WAVE_PANEL_TIMER, ATTR_ENABLED, 1);
	
	 RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
Error:
	if (wavePanel > 0)
		DiscardPanel (wavePanel);
}


int CVICALLBACK OnSavePlotsButtonCB(int panel, int control, int event,
                                    void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
        {

            char folderPath[256] = "";  


            char fileName1[256];
            char fileName2[256];


            sprintf(fileName1, "%sgrafic_1.jpg", folderPath); 
            sprintf(fileName2, "%sgrafic_2.jpg", folderPath);


            int bitmapID1;
            int status1 = GetCtrlDisplayBitmap(panel, PANEL_GRAPH, 1, &bitmapID1);
            if (status1 != 0) {
                return 0;
            }


            int statusSave1 = SaveBitmapToJPEGFile(bitmapID1, fileName1, JPEG_PROGRESSIVE, 100);
            if (statusSave1 != 0) {

                return 0;
            }


            int bitmapID2;
            int status2 = GetCtrlDisplayBitmap(panel, PANEL_GRAPH_2, 1, &bitmapID2);
            if (status2 != 0) {

                return 0;
            }


            int statusSave2 = SaveBitmapToJPEGFile(bitmapID2, fileName2, JPEG_PROGRESSIVE, 100);
            if (statusSave2 != 0) {

                return 0;
            }




            break;
        }
    }
    return 0;
}


int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			LaunchExecutable("python main.py");
			

			Delay(4);
			

			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			waveData = (double *) calloc(npoints, sizeof(double));
			

			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			

			 ;
			
			break;
	}
	return 0;
}

int CVICALLBACK OnPanelCallback (int panel, int event, void *callbackData,
								 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:

			QuitUserInterface(0);
			break;
	}
	return 0;
}

//functie de calculat medie
void MeanGraf(){
	int startIndex = currentStartIndex * (int)sampleRate;
            int endIndex = currentEndIndex * (int)sampleRate;


            if (startIndex < 0)
                startIndex = 0;
            if (endIndex > npoints)
                endIndex = npoints;


            int pointsToAnalyze = endIndex - startIndex;


            double mean = 0.0;
            if (pointsToAnalyze > 0)
            {
                double sum = 0.0;
                for (int i = startIndex; i < endIndex; ++i)
                {
                    sum += waveData[i];
                }
                mean = sum / pointsToAnalyze;
            }


            SetCtrlVal(PANEL, PANEL_NUMERIC, mean);	
}

int CVICALLBACK OnPrevButtonCB(int panel, int control, int event,
                               void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
 
            currentStartIndex -=1;
			currentEndIndex -= 1;
			if(currentStartIndex < 0)
				currentStartIndex=0;
			SetCtrlVal(panel, PANEL_STRING_2,(double)currentEndIndex);
            SetCtrlVal(panel,PANEL_STRING, (double)currentStartIndex);
            break;
    }
    return 0;
}



int CVICALLBACK OnNextButtonCB(int panel, int control, int event,
                               void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:

            currentStartIndex +=1;
			currentEndIndex += 1;
			SetCtrlVal(panel, PANEL_STRING_2,(double)currentEndIndex);
            SetCtrlVal(panel,PANEL_STRING, (double)currentStartIndex);
            break;
    }
    return 0;
}

//de pastrat daca mai vreau sa fac update la selectare in viitor (extrag functionalitate de la Apply)
int CVICALLBACK OnRingChangeCB(int panel, int control, int event,
                               void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:

            int selectedIndex;
            GetCtrlVal(panel, control, &selectedIndex); 
            switch (selectedIndex)
            {
                case 0:     
                    break;
                case 1: 
                    break;
                default:
                    break;
            }

            break;
    }
    return 0;
}

//filtru mediere val fixa de 16
void FilterMediere(const double *signal, double *filt, int npoints)
{
    for (int i = 0; i < npoints; i++)
    {
        double sum = 0.0;
        int count = 0;

        for (int j = i; j > i - 16 && j >= 0; j--)
        {
            sum += signal[j];
            count++;
        }

        filt[i] = sum / count;
    }
}

void FilterOrdinI(const double *signal, double *filt, int npoints, double alpha)
{
    filt[0] = signal[0];

    for (int i = 1; i < npoints; i++)
    {
        filt[i] = (1 - alpha) * filt[i - 1] + alpha * signal[i];
    }
}

void CalculateDerivative(const double *signal, double *derivative, int npoints, double sampleRate)
{
    for (int i = 1; i < npoints - 1; i++)
    {
        derivative[i] = (signal[i + 1] - signal[i - 1]) * 0.5 * sampleRate;
    }

    derivative[0] = 0.0;
    derivative[npoints - 1] = 0.0;
}



int CVICALLBACK OnBinarySwitchCB(int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:

            int switchState;
            GetCtrlVal(panel, control, &switchState); 
            showDerivative = switchState;

            OnApplyButtonCB(panel, control, EVENT_COMMIT, callbackData, eventData1, eventData2);

            break;
    }
 
	return 0;
}

void Cal(const double *signal, int npoints, const char *filename) {

    if (signal == NULL || npoints <= 0 || filename == NULL) {

        return;
    }


    int maxSize = sizeof(signal) / sizeof(signal[0]);


    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return;
    }


    double min = signal[0];
    double max = signal[0];
    int minIndex = 0;
    int maxIndex = 0;
    double sum = 0.0;
    double sumSquared = 0.0;
    int zeroCrossings = 0;


    for (int i = 0; i < maxSize; i++) {
        double value = signal[i];
        sum += value;
        sumSquared += value * value;

        if (value < min) {
            min = value;
            minIndex = i;
        }
        if (value > max) {
            max = value;
            maxIndex = i;
        }


        if (i > 0 && ((signal[i - 1] > 0 && value <= 0) || (signal[i - 1] < 0 && value >= 0))) {
            zeroCrossings++;
        }
    }

    double mean = sum / maxSize;
    double variance = (sumSquared / maxSize) - (mean * mean);

    double *sortedSignal = (double *)malloc(maxSize * sizeof(double));
    if (sortedSignal == NULL) {

        fclose(file);
        return;
    }


    memcpy(sortedSignal, signal, maxSize * sizeof(double));
    for (int i = 0; i <maxSize - 1; i++) {
        for (int j = i + 1; j <maxSize; j++) {
            if (sortedSignal[i] > sortedSignal[j]) {
                double temp = sortedSignal[i];
                sortedSignal[i] = sortedSignal[j];
                sortedSignal[j] = temp;
            }
        }
    }

    double median = (maxSize % 2 == 0)
                        ? (sortedSignal[maxSize / 2 - 1] + sortedSignal[maxSize / 2]) / 2
                        : sortedSignal[maxSize / 2];

    free(sortedSignal);

    fprintf(file, "Min: %lf at index %d\n", min, minIndex);
    fprintf(file, "Max: %lf at index %d\n", max, maxIndex);
    fprintf(file, "Mean: %lf\n", mean);
    fprintf(file, "Median: %lf\n", median);
    fprintf(file, "Variance: %lf\n", variance);
    fprintf(file, "Zero Crossings: %d\n", zeroCrossings);

    fclose(file);
}
//functie defecta
int CVICALLBACK OnSaveAnalysisCB(int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if (globalFloatVector == NULL || globalFloatVectorSize == 0) {

                return 0;
            }

            char filename[MAX_PATHNAME_LEN] = " "; 


            if (FileSelectPopup("", "*.txt", "*.txt",
                                "Save Analysis", VAL_SAVE_BUTTON,
                                0, 0, 1, 1, filename) == VAL_NO_FILE_SELECTED) {
                return 0; 
            }

            Cal((const double *)globalFloatVector, (int)globalFloatVectorSize, filename);

            break;
    }
    return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    
    switch (event)
    {
        case EVENT_COMMIT:
            if (panel == 1) 
            {
				wavePanel = LoadPanel (0, "Et1.uir", WAVE_PANEL);
				acqPanel = LoadPanel (0, "Et1.uir", PANEL);
				HidePanel(acqPanel);
                DisplayPanel(wavePanel);
                
            }
            else if (panel == wavePanel) 
            {
				wavePanel = LoadPanel (0, "Et1.uir", WAVE_PANEL);
				acqPanel = LoadPanel (0, "Et1.uir", PANEL);
                HidePanel(wavePanel);
				DisplayPanel(acqPanel);
                
            }
            break;
    }
    return 0;
}


int CalculatePlotIndices(int *startIndex, int *endIndex, int *pointsToPlot)
{
    *startIndex = currentStartIndex * (int)sampleRate;
    *endIndex = currentEndIndex * (int)sampleRate;

    if (*startIndex < 0)
        *startIndex = 0;
    if (*endIndex > npoints)
        *endIndex = npoints;

    *pointsToPlot = *endIndex - *startIndex;

    return (*pointsToPlot > 0);
}

int AllocateGlobalFloatVector(int pointsToPlot)
{
    if (globalFloatVector)
        free(globalFloatVector);

    globalFloatVector = (float *)calloc(pointsToPlot, sizeof(float));
    if (globalFloatVector == NULL)
        return 0;

    globalFloatVectorSize = pointsToPlot;
    return 1;
}

void CopyWaveDataToGlobalVector(int startIndex, int pointsToPlot)
{
    for (int i = 0; i < pointsToPlot; i++)
    {
        globalFloatVector[i] = (float)(waveData[startIndex + i]);
    }
}

void DeleteExistingPlots(int panel)
{
    DeleteGraphPlot(panel, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
    DeleteGraphPlot(panel, PANEL_GRAPH_2, -1, VAL_IMMEDIATE_DRAW);
}

double* AllocateDoubleArray(int size)
{
    return (double *)calloc(size, sizeof(double));
}


int GetDerivativeFlag()
{
    return showDerivative;
}

void ProcessWaveDataForPlot(int startIndex, double *dataToPlot, int pointsToPlot, int showDerivative)
{
    if (showDerivative)
    {
        CalculateDerivative(waveData + startIndex, dataToPlot, pointsToPlot, sampleRate);
    }
    else
    {
        memcpy(dataToPlot, waveData + startIndex, pointsToPlot * sizeof(double));
    }
}

void PlotData(int panel, int control, double *data, int size, int color)
{
    hand=PlotY(panel, control, data, size, VAL_DOUBLE, VAL_THIN_LINE, 
          VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, color);
}

int HandleMemoryError()
{
    free(globalFloatVector);
    globalFloatVector = NULL;
    globalFloatVectorSize = 0;
    return 0;
}

int HandleMemoryErrorWithData(double *dataToPlot, double *filteredData)
{
    free(dataToPlot);
    if (filteredData != NULL)
        free(filteredData);
    return HandleMemoryError();
}




void CalculateStatistics(int startIndex, int pointsToPlot, double *mean, double *variance, 
                         double *skewness, double *kurtosis)
{
    *mean = 0.0;
    *variance = 0.0;
    *skewness = 0.0;
    *kurtosis = 0.0;

    for (int i = 0; i < pointsToPlot; i++)
    {
        *mean += waveData[startIndex + i];
    }
    *mean /= pointsToPlot;

    for (int i = 0; i < pointsToPlot; i++)
    {
        double deviation = waveData[startIndex + i] - *mean;
        *variance += deviation * deviation;
        *skewness += deviation * deviation * deviation;
        *kurtosis += deviation * deviation * deviation * deviation;
    }
    *variance /= pointsToPlot;
    *skewness /= (pointsToPlot * pow(*variance, 1.5));
    *kurtosis = (*kurtosis / (pointsToPlot * (*variance) * (*variance))) - 3.0;
}

void UpdateStatisticsDisplay(int panel, double skewness, double kurtosis)
{
    SetCtrlVal(panel, PANEL_KURT, kurtosis);
    SetCtrlVal(panel, PANEL_SKEW, skewness);
}

int ApplyFilter(int panel, int startIndex, double *filteredData, int pointsToPlot)
{
    int selectedFilter;
    double alpha;

    GetCtrlVal(panel, PANEL_RING, &selectedFilter);
    GetCtrlVal(panel, PANEL_ALPHA, &alpha);

    if (alpha < 0.0 || alpha > 1.0)
        return 0;

    switch (selectedFilter)
    {
        case 0:
            FilterOrdinI(waveData + startIndex, filteredData, pointsToPlot, alpha);
            break;
        case 1:
            FilterMediere(waveData + startIndex, filteredData, pointsToPlot);
            break;
        default:
            return 0;
    }
    return 1;
}

int PlotEnvelope(int panel, int startIndex, double *filteredData, int pointsToPlot)
{
    double *peakLocations = NULL;
    double *peakAmplitudes = NULL;
    double *peakSecondDerivatives = NULL;
    ssize_t peakCount = 0;

    double threshold = 0.1;
    ssize_t width = 3;
    int polarity = 0;

    PeakDetector(waveData + startIndex, pointsToPlot, threshold, width, polarity, 1, 0,
                 &peakCount, &peakLocations, &peakAmplitudes, &peakSecondDerivatives);

    double *envelope = AllocateDoubleArray(pointsToPlot);
    if (envelope == NULL)
    { 
        FreeAnalysisMem(peakLocations);
        FreeAnalysisMem(peakAmplitudes);
        FreeAnalysisMem(peakSecondDerivatives);
        return 0;
    }

    for (ssize_t i = 0; i < peakCount; i++)
    {
        ssize_t location = (ssize_t)peakLocations[i];
        if (location >= 0 && location < pointsToPlot)
        {
            envelope[location] = peakAmplitudes[i];
        }
    }

    PlotData(panel, PANEL_GRAPH_2, envelope, pointsToPlot, VAL_GREEN);

    FreeAnalysisMem(peakLocations);
    FreeAnalysisMem(peakAmplitudes);
    FreeAnalysisMem(peakSecondDerivatives);
    free(envelope);

    return 1;
}


int CVICALLBACK OnApplyButtonCB(int panel, int control, int event,
                                void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
        {
            int startIndex, endIndex, pointsToPlot;

            // Calculate indices and number of points to plot based on current selection
            if (!CalculatePlotIndices(&startIndex, &endIndex, &pointsToPlot))
                return 0;

            // Allocate or reallocate the global float vector for plotting
            if (!AllocateGlobalFloatVector(pointsToPlot))
                return 0;

            // Copy the selected wave data segment to the global float vector
            CopyWaveDataToGlobalVector(startIndex, pointsToPlot);

            // Remove any existing plots from the panel
            DeleteExistingPlots(panel);

            // Allocate memory for the data to be plotted
            double *dataToPlot = AllocateDoubleArray(pointsToPlot);
            if (dataToPlot == NULL)
                return HandleMemoryError();

            // Check if the derivative should be shown and process the data accordingly
            int showDerivative = GetDerivativeFlag();
            ProcessWaveDataForPlot(startIndex, dataToPlot, pointsToPlot, showDerivative);

            // Plot the main data (red)
            PlotData(panel, PANEL_GRAPH, dataToPlot, pointsToPlot, VAL_RED);

            // Check the state of the binary switch (for envelope or filtered plot)
            int binarySwitchState;
            GetCtrlVal(panel, PANEL_BINARYSWITCH_2, &binarySwitchState);

            // Allocate memory for the filtered data
            double *filteredData = AllocateDoubleArray(pointsToPlot);
            if (filteredData == NULL)
                return HandleMemoryErrorWithData(dataToPlot, NULL);

            // Calculate statistics (mean, variance, skewness, kurtosis) for the selected data
            double mean, variance, skewness, kurtosis;
            CalculateStatistics(startIndex, pointsToPlot, &mean, &variance, &skewness, &kurtosis);
            UpdateStatisticsDisplay(panel, skewness, kurtosis);

            // Apply the selected filter to the data
            if (!ApplyFilter(panel, startIndex, filteredData, pointsToPlot))
                return HandleMemoryErrorWithData(dataToPlot, filteredData);

            // Plot either the filtered data (blue) or the envelope (green) based on the switch
            if (binarySwitchState == 0)
            {
                PlotData(panel, PANEL_GRAPH_2, filteredData, pointsToPlot, VAL_BLUE);
            }
            else
            {
                if (!PlotEnvelope(panel, startIndex, filteredData, pointsToPlot))
                    return HandleMemoryErrorWithData(dataToPlot, filteredData);
            }

            // Free allocated memory for this operation
            free(filteredData);
            free(dataToPlot);

            // Update the UI with the number of points plotted and the mean value
            SetCtrlVal(panel, PANEL_STRING_3, (double)pointsToPlot);
            MeanGraf();

            break;
        }
    }
    return 0;
}



int CVICALLBACK OnPanelCallback2 (int panel, int event, void *callbackData,
								 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:

			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK NSamplesNum (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{ 
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK GraphFreq (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}
void ProcessFFT(int numSamples, double dt, double *inputBuffer) {
    int filterSwitch;
    GetCtrlVal(wavePanel, WAVE_PANEL_FSW, &filterSwitch);
    if (wavePanel == 0) {
        wavePanel = LoadPanel(0, "Et1.uir", WAVE_PANEL);
    }

    double *autoSpectrum = malloc((numSamples / 2) * sizeof(double));
    double *convertedSpectrum = malloc((numSamples / 2) * sizeof(double));
    double *phaseSpectrum = malloc((numSamples / 2) * sizeof(double)); 
    double *blackmanBuffer = malloc(numSamples * sizeof(double));
    double *filteredSpectrum = malloc((numSamples / 2) * sizeof(double)); 
    double *realPart = malloc(numSamples * sizeof(double)); 
    double *imagPart = malloc(numSamples * sizeof(double)); 

    if (!autoSpectrum || !convertedSpectrum || !phaseSpectrum || !blackmanBuffer || !filteredSpectrum || !realPart || !imagPart) {
        free(autoSpectrum);
        free(convertedSpectrum);
        free(phaseSpectrum);
        free(blackmanBuffer);
        free(filteredSpectrum);
        free(realPart);
        free(imagPart);
        return;  
    }

    double df;
    double frequencyPeak, powerPeak;
    WindowConst winConst;

    ScaledWindowEx(inputBuffer, numSamples, HANNING_, 0, &winConst);

    for (int i = 0; i < numSamples; ++i) {
        realPart[i] = inputBuffer[i];
        imagPart[i] = 0;            
    }
    FFT(realPart, imagPart, numSamples);

   
    for (int i = 0; i < numSamples / 2; ++i) {
        phaseSpectrum[i] = atan2(imagPart[i], realPart[i]);
    }

  
    AutoPowerSpectrum(inputBuffer, numSamples, dt, autoSpectrum, &df);

  
    PowerFrequencyEstimate(autoSpectrum, numSamples / 2, 0.0, winConst, df, 10, &frequencyPeak, &powerPeak);

   
    if (filterSwitch == 1) {
        double coef[55];
        int filterLength = 55;
        double beta = 4.5;

        Ksr_HPF(1.0 / dt, 1100.0, filterLength, coef, beta);


        int convolveSize = (numSamples / 2) + filterLength - 1;
        double *tempFilteredSpectrum = malloc(convolveSize * sizeof(double));
        if (!tempFilteredSpectrum) {
            free(autoSpectrum);
            free(convertedSpectrum);
            free(phaseSpectrum);
            free(blackmanBuffer);
            free(filteredSpectrum);
            free(realPart);
            free(imagPart);
            return;  
        }

        
        Convolve(coef, filterLength, autoSpectrum, numSamples / 2, tempFilteredSpectrum);

       
        for (int i = 0; i < numSamples / 2; ++i) {
            filteredSpectrum[i] = tempFilteredSpectrum[i];
        }

       
        free(tempFilteredSpectrum);
    } else {
       
        double cutoffFrequency = 1100.0;
        int order = 5;
        Bw_HPF(autoSpectrum, numSamples / 2, 1.0 / dt, cutoffFrequency, order, filteredSpectrum);
    }

 
    char unitString[32] = "V";
    SpectrumUnitConversion(filteredSpectrum, numSamples / 2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unitString);

   
    for (int i = 0; i < numSamples; ++i) {
        double blackmanCoeff = 0.42 - 0.5 * cos(2 * 3.14 * i / (numSamples - 1)) + 0.08 * cos(4 * 3.14 * i / (numSamples - 1));
        blackmanBuffer[i] = inputBuffer[i] * blackmanCoeff;
    }

   
    DeleteGraphPlot(wavePanel, WAVE_PANEL_GRAPHBLACKMAN, -1, VAL_DELAYED_DRAW);
    PlotY(wavePanel, WAVE_PANEL_GRAPHBLACKMAN, blackmanBuffer, numSamples, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);

    DeleteGraphPlot(wavePanel, WAVE_PANEL_GRAPHHANNING, -1, VAL_DELAYED_DRAW);
    PlotY(wavePanel, WAVE_PANEL_GRAPHHANNING, inputBuffer, numSamples, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);

	DeleteGraphPlot(wavePanel, WAVE_PANEL_PHASE_GRAPH, -1, VAL_DELAYED_DRAW);
    PlotY(wavePanel, WAVE_PANEL_PHASE_GRAPH, phaseSpectrum, numSamples / 2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);

    SetCtrlVal(wavePanel, WAVE_PANEL_FREQUENCY_PEAK, frequencyPeak);
    SetCtrlVal(wavePanel, WAVE_PANEL_POWER_PEAK, 10 * log10(powerPeak));


    free(autoSpectrum);
    free(convertedSpectrum);
    free(phaseSpectrum);
    free(blackmanBuffer);
    free(filteredSpectrum);
    free(realPart);
    free(imagPart);
}

int begInterval=0;
int endInterval=1;

int CVICALLBACK FreqTmr(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
    if (acqPanel == 0) {
        acqPanel = LoadPanel(0, "Et1.uir", 1);
    }

    if (event == EVENT_TIMER_TICK) {
        if (hand > 0) {
            if (wavePanel == 0) {
                wavePanel = LoadPanel(0, "Et1.uir", WAVE_PANEL); 
            }

            double numSamples = 44100.0;
            double dt = 1.0 / 44100.0; 
            double nrsampF;

            // Get the number of samples to process from the UI control
            GetCtrlVal(wavePanel, WAVE_PANEL_NUMERIC, &nrsampF); 

            // Allocate buffers for the input data
            double *inputBufferTemp = malloc((int)(numSamples) * sizeof(double));
            double *inputBuffer = malloc((int)(nrsampF) * sizeof(double));

            if (!inputBufferTemp || !inputBuffer) {
                free(inputBufferTemp);
                free(inputBuffer);
                return 0;  
            }

            // Extract Y data from the plot into inputBufferTemp
            GetPlotAttribute(1, 6, hand, ATTR_PLOT_YDATA, inputBufferTemp);

            // Copy the relevant interval of samples into inputBuffer
            int bufferIndex = 0; 
            int startIdx = begInterval * (int)nrsampF;
            int endIdx = (int)nrsampF * endInterval - 1;
            for (int i = startIdx; i < endIdx; i++) {
                if (bufferIndex < (int)nrsampF) {
                    inputBuffer[bufferIndex] = inputBufferTemp[i];
                    bufferIndex++;
                } else {
                    break;  
                }
            }

            // Process the FFT on the selected buffer
            ProcessFFT((int)nrsampF, dt, inputBuffer);

            // Free allocated memory
            free(inputBufferTemp);
            free(inputBuffer);

            // Update interval indices for next timer tick
            if ((int)nrsampF * (endInterval + 1) < 44099) {
                begInterval++;
                endInterval++;
            } else {
                begInterval = 0;
                endInterval = 1;
            }
        }
    } 

    return 0;
}


int CVICALLBACK SAVE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_COMMIT) {
        char fileName[256];
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        int imghandle;

        // Helper macro to save a graph as a JPEG with a timestamped filename
        #define SAVE_GRAPH_TO_JPEG(ctrl_id, prefix) \
            GetCtrlDisplayBitmap(panel, ctrl_id, 1, &imghandle); \
            strftime(fileName, sizeof(fileName), prefix "_%Y-%m-%d_%H-%M-%S.jpeg", t); \
            SaveBitmapToJPEGFile(imghandle, fileName, JPEG_PROGRESSIVE, 100); \
            DiscardBitmap(imghandle);

        // Save Blackman window graph
        SAVE_GRAPH_TO_JPEG(WAVE_PANEL_GRAPHBLACKMAN, "blackman");

        // Save Hanning window graph
        SAVE_GRAPH_TO_JPEG(WAVE_PANEL_GRAPHHANNING, "hanning");

        // Save Phase graph
        SAVE_GRAPH_TO_JPEG(WAVE_PANEL_PHASE_GRAPH, "phase");

        #undef SAVE_GRAPH_TO_JPEG
    }
    return 0;
}