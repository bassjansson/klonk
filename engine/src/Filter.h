#ifndef __FILTER_H__
#define __FILTER_H__

#include <iostream>
#include <cmath>

#include "Defines.h"

using namespace std;

class Filter
{
public:
    Filter();
    ~Filter();

    float process(float input);

    void setFrequency(float freq);
    void setResonance(float res);

private:
    void generateCoefficients();

    float frequency;
    float resonance;

    float gain;

    float a1, a2;
    float b1, b2;

    float z1, z2;
};

#endif // __FILTER_H__
