#ifndef AudioFFT_H
#define AudioFFT_H
#include <FastLED.h>
#include <Arduino.h>
#include <audiolight/MCP3202.h>
#include <arduinoFFT.h>

const uint16_t samples = 1024;
const double samplingFrequency = 40000;

class AudioFFT {

  public:

    AudioFFT() {
      sampling_period_us = round(1000000*(1.0/samplingFrequency));
    }
    void sampleAndFFT();
    void begin();

  private:
    unsigned int sampling_period_us;
    unsigned long microseconds;

    /*
      These are the input and output vectors
      Input vectors receive computed results from FFT
    */
    double vReal[samples];
    double vImag[samples];    

    arduinoFFT FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency); 

    // The adc we are reading from
    MCP3202 _adc = MCP3202();

    void printVector(double *vData, uint16_t bufferSize);

};

#endif // End AudioFFT_H