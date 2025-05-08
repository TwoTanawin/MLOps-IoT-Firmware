#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#include <Arduino.h>

class TdsSensor {
public:
    TdsSensor(uint8_t analogPin, float refVoltage = 5.0, float tempC = 25.0, uint16_t sampleCount = 30);

    void begin();
    void update();         // call this in loop() to sample and compute TDS
    float getTdsValue();   // returns current TDS value (ppm)

private:
    uint8_t pin;
    float vref;
    float temperature;
    uint16_t scount;

    int* analogBuffer;
    int* analogBufferTemp;
    int analogBufferIndex;
    float averageVoltage;
    float tdsValue;

    unsigned long lastSampleTime;
    unsigned long lastComputeTime;

    int getMedianNum(int bArray[], int iFilterLen);
};

#endif
