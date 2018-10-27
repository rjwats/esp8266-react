#include <audiolight/AudioFFT.h>

void AudioFFT::begin(){
  _adc.begin();
}

void AudioFFT::sampleAndFFT() {
		microseconds = micros();
		for(uint16_t  i=0; i<samples; i++) {
				vReal[i] = _adc.sample(_adc.CH0) - 2048;
				vImag[i] = 0;
				while(micros() - microseconds < sampling_period_us){
					// spin a loop for sampling_period_us
				}
				microseconds += sampling_period_us;
		}
    FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
    FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
    FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */

		EVERY_N_SECONDS(1){
			Serial.println(FFT.MajorPeak(), 6); //Print out what frequency is the most dominant.
			printVector(vReal, (samples >> 1));
		}
}

/*
void AudioFFT::printBands(double *vData, uint16_t bufferSize) {
	int samplesPerBucket = samples/7;
  for (int i=0; i<7; i++) {
		Serial.print("Band ");
		Serial.print(i);
		Serial.print("Starts at ");
		double abscissa = ((i * 1.0 * samplingFrequency) / samples);
		
	}

  for (uint16_t i = 0; i < bufferSize; i++) {
    double abscissa = ((i * 1.0 * samplingFrequency) / samples);
    Serial.print(abscissa, 6);
    Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
}
*/

void AudioFFT::printVector(double *vData, uint16_t bufferSize){
	Serial.print("Buffer size is ");
	Serial.println(bufferSize);

    double abscissa = ((1 * 1.0 * samplingFrequency) / samples);
    Serial.print(abscissa, 6);
    Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[1], 4);

		abscissa = (((bufferSize-1) * 1.0 * samplingFrequency) / samples);
    Serial.print(abscissa, 6);
    Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[bufferSize-1], 4);
}
