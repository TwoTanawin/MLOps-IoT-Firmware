#include "PhSensor.h"

PhSensor::PhSensor(int phPin, float slope, float intercept, int samples)
    : phPin(phPin), m(slope), C(intercept), samples(samples) {}

float PhSensor::readPhVoltage() {
    long phTotal = 0;

    for (int i = 0; i < samples; i++) {
        phTotal += analogRead(phPin);
        delay(10);
    }

    float phAvg = phTotal / float(samples);
    return phAvg * (5.0 / 1023.0); // Convert to volts
}

float PhSensor::readPh() {
    float phVoltage = readPhVoltage();
    float rawPh = (phVoltage * m) + C;

    // Clamp the value to a realistic pH range (0–14)
    if (rawPh < 0.0) rawPh = 0.0;
    if (rawPh > 14.0) rawPh = 14.0;

    return rawPh;
}
