#ifndef PHSENSOR_H
#define PHSENSOR_H

#include <Arduino.h>

class PhSensor {
private:
    int phPin;
    float C;
    float m;
    int samples;

public:
    PhSensor(int phPin, float slope, float intercept, int samples = 10);

    float readPh();
    float readPhVoltage();
};

#endif