#ifndef DO_ESTIMATOR_H
#define DO_ESTIMATOR_H

class DoEstimator {
public:
    // Estimate DO based on temperature and optionally salinity
    static float estimate(float temperatureC, float salinityPpm = 0);
};

#endif
