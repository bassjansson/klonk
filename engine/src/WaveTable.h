#ifndef __WAVE_TABLE_H__
#define __WAVE_TABLE_H__

#include <iostream>
#include <cmath>

using namespace std;

class WaveTable
{
public:
    WaveTable();
    ~WaveTable();

    const float operator[](float phase) const;

    void generateSineWave();
    void generateTriangleWave();
    void generateSquareWave();
    void generateSawWave();

private:
    void generateHarmonicWave(
        int harmonics,
        int harmStep,
        float harmPhaseShift,
        float phaseShift,
        float rollOff,
        float gain);

    const int size;
    float * buffer;
};

#endif // __WAVE_TABLE_H__
