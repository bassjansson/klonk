#ifndef __TRACK_H__
#define __TRACK_H__

#include <iostream>
#include <cmath>

#include "Defines.h"
#include "WaveTable.h"
#include "Filter.h"

using namespace std;

class Track
{
public:
    Track(int trackIndex, int inputChannelLeft, int inputChannelRight) :
        // trackIndex(trackIndex),
        // inputChannelLeft(inputChannelLeft),
        // inputChannelRight(inputChannelRight),
        frequency(40.0f),
        phase(0.0f),
        phase2(0.0f),
        currentPhaseStep(0.0f)
    {
        wt1.generateSawWave();
        wt2.generateSawWave();

        float res = 0.5f;

        f1.setResonance(res);
        f2.setResonance(res * 0.5f);
        f3.setResonance(res * 0.25f);
    }

    ~Track()
    { }

    void setFrequency(float freq)
    {
        frequency = freq;
    }

    float filter(float input, float freq, float phase)
    {
        float p = phase > 0.75f ? 30.0f : 10.0f;

        if (phase > 0.75f)
            phase = fmodf(phase * 22.0f, 1.0f);
        else
            phase = 1.0f - fmodf(phase * 16.0f, 1.0f);

        float f = freq * 0.25f + freq * phase * p;

        f1.setFrequency(f);
        f2.setFrequency(f);
        f3.setFrequency(f);

        return f3.process(f2.process(f1.process(input)));
    }

    void process(
        const float * inputBuffer,
        float *       outputBuffer,
        frame_t       framesPerBuffer,
        int           numInputChannels,
        int           numOutputChannels,
        frame_t       currentFrame)
    {
        float targetPhaseStep = frequency / AUDIO_SAMPLE_RATE;

        for (frame_t i = 0; i < framesPerBuffer; ++i)
        {
            float p = fmodf((float)(currentFrame + i) / AUDIO_SAMPLE_RATE * 0.18f, 1.0f);

            float osc = filter(wt1[phase] * 0.3f + wt2[phase2] * 0.2f, frequency, p);

            outputBuffer[i * numOutputChannels + LEFT]  += osc;
            outputBuffer[i * numOutputChannels + RIGHT] += osc;

            float slope     = (float) i / framesPerBuffer;
            float phaseStep = (1.0f - slope) * currentPhaseStep + slope * targetPhaseStep;

            phase = fmodf(phase + phaseStep + p * 0.00011f, 1.0f);
            phase2 = fmodf(phase2 + phaseStep * 2.97f + p * 0.00037f, 1.0f);
        }

        currentPhaseStep = targetPhaseStep;
    }

private:
    // const int trackIndex;
    // const int inputChannelLeft;
    // const int inputChannelRight;

    float frequency;
    float phase, phase2;
    float currentPhaseStep;

    WaveTable wt1, wt2;
    Filter f1, f2, f3;
};

#endif // __TRACK_H__
