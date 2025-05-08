#include "TdsSensor.h"

TdsSensor::TdsSensor(uint8_t analogPin, float refVoltage, float tempC, uint16_t sampleCount)
    : pin(analogPin), vref(refVoltage), temperature(tempC), scount(sampleCount)
{
    analogBuffer = new int[scount];
    analogBufferTemp = new int[scount];
    analogBufferIndex = 0;
    averageVoltage = 0;
    tdsValue = 0;
    lastSampleTime = 0;
    lastComputeTime = 0;
}

void TdsSensor::begin() {
    pinMode(pin, INPUT);
}

void TdsSensor::update() {
    unsigned long now = millis();

    // Sampling every 40 ms
    if (now - lastSampleTime > 40) {
        lastSampleTime = now;
        analogBuffer[analogBufferIndex++] = analogRead(pin);
        if (analogBufferIndex == scount) analogBufferIndex = 0;
    }

    // Compute every 800 ms
    if (now - lastComputeTime > 800) {
        lastComputeTime = now;

        for (int i = 0; i < scount; i++)
            analogBufferTemp[i] = analogBuffer[i];

        averageVoltage = getMedianNum(analogBufferTemp, scount) * (float)vref / 1024.0;
        float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
        float compensationVoltage = averageVoltage / compensationCoefficient;

        float rawTds = (133.42 * pow(compensationVoltage, 3)
                        - 255.86 * pow(compensationVoltage, 2)
                        + 857.39 * compensationVoltage) * 0.5;

        // Clamp rawTds to realistic TDS ppm range: 0–3000 ppm
        if (rawTds < 0.0) rawTds = 0.0;
        if (rawTds > 3000.0) rawTds = 3000.0;

        tdsValue = rawTds;
    }
}

float TdsSensor::getTdsValue() {
    return tdsValue;
}

int TdsSensor::getMedianNum(int bArray[], int iFilterLen) {
    int bTab[iFilterLen];
    for (int i = 0; i < iFilterLen; i++) bTab[i] = bArray[i];
    for (int j = 0; j < iFilterLen - 1; j++) {
        for (int i = 0; i < iFilterLen - j - 1; i++) {
            if (bTab[i] > bTab[i + 1]) {
                int temp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = temp;
            }
        }
    }
    if (iFilterLen % 2 == 1)
        return bTab[iFilterLen / 2];
    else
        return (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
}
