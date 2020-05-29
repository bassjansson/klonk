#include "WaveTable.h"

WaveTable::WaveTable() : size(1024)
{
    buffer = new float[size];

    for (int i = 0; i < size; ++i)
        buffer[i] = 0.0f;
}

WaveTable::~WaveTable()
{
    delete[] buffer;
}

const float WaveTable::operator[](float phase) const
{
    phase = fabsf(phase) * size;

    int p1 = (int)phase % size;
    int p2 = (p1 + 1) % size;

    float f = phase - p1;

    return buffer[p1] * (1.0f - f) + buffer[p2] * f;
}

void WaveTable::generateSineWave()
{
    generateHarmonicWave(1, 1, 0.0f, 0.0f, 1.0f, 1.0f);
}

void WaveTable::generateTriangleWave()
{
    generateHarmonicWave(size / 4, 2, 0.25f, 0.75f, 2.0f, 0.75f);
}

void WaveTable::generateSquareWave()
{
    generateHarmonicWave(size / 4, 2, 0.0f, 0.0f, 1.0f, 1.0f);
}

void WaveTable::generateSawWave()
{
    generateHarmonicWave(size / 4, 1, 0.0f, 0.0f, 1.0f, 0.5f);
}

void WaveTable::generateHarmonicWave(
    int harmonics,
    int harmStep,
    float harmPhaseShift,
    float phaseShift,
    float rollOff,
    float gain)
{
    harmPhaseShift *= 2.0f * M_PI;
    phaseShift *= 2.0f * M_PI;

    for (int i = 0; i < size; ++i)
    {
        float phase = (float)i / size * 2.0f * M_PI + phaseShift;

        buffer[i] = 0.0f;

        for (int h = 1; h <= harmonics; h += harmStep)
            buffer[i] += sinf(phase * h + harmPhaseShift) / powf(h, rollOff);

        buffer[i] *= gain;
    }
}
