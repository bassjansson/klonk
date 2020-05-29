#include "Filter.h"

Filter::Filter() :
    frequency(AUDIO_SAMPLE_RATE / 2),
    resonance(0.0f),
    z1(0.0f),
    z2(0.0f)
{
    generateCoefficients();
}

Filter::~Filter()
{ }

float Filter::process(float input)
{
    float x = input * gain;
    float y = x + z1;

    z1 = x * a1 - y * b1 + z2;
    z2 = x * a2 - y * b2;

    return y;
}

float Filter::setFrequency(float freq)
{
    frequency = freq;

    generateCoefficients();
}

float Filter::setResonance(float res)
{
    resonance = res;

    generateCoefficients();
}

void Filter::generateCoefficients()
{
    float oPhase = M_PI;
    float oMag   = 1.0f;

    float pPhase = frequency / AUDIO_SAMPLE_RATE * 2.0f * M_PI;
    float pMag   = 1.0f - sinf(pPhase) * (1.0f - resonance);

    a1 = -2.0f * cosf(oPhase) * oMag;
    a2 = oMag * oMag;

    b1 = -2.0f * cosf(pPhase) * pMag;
    b2 = pMag * pMag;

    gain = (1.0f + b1 + b2) / (1.0f + a1 + a2);
}
