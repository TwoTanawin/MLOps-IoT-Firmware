#include "DoEstimator.h"
#include <math.h>

// Based on solubility of oxygen in water at sea level (simplified)
float DoEstimator::estimate(float temperatureC, float salinityPpm) {
    if (temperatureC < 0.0 || temperatureC > 50.0) {
        temperatureC = 25.0; // fallback temperature
    }

    // Convert TDS ppm to salinity in PSU approximation
    float salinityPSU = salinityPpm / 1000.0;

    // Approximate saturated DO in mg/L at 1 atm pressure
    float doSat = 14.6 - (0.4 * temperatureC) + (0.008 * temperatureC * temperatureC);

    // Salinity correction (approximate)
    doSat *= (1 - 0.03 * salinityPSU);

    // Clamp DO to realistic range: 0–14 mg/L
    if (doSat < 0.0) doSat = 0.0;
    if (doSat > 14.0) doSat = 14.0;

    return doSat;
}
