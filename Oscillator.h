#pragma once

#include <juce_dsp/juce_dsp.h>
#include <cmath>
#include <vector>
#include <ctime>

class DirtyManner
{
public:
    enum class WaveformType
    {
        sine,
        square,
        sawtooth,
        triangle
    };

    DirtyManner() = default;

    void fetch_wavetable( time_t timestamp );

    void prepare(double sampleRate, int blockSize)
    {
        this->sampleRate = sampleRate;
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = blockSize;
        spec.numChannels = 1;
        this->phase_increment = 0.0f;
        
        // Initialize oscillators for each waveform type
        sineOsc.initialise([](float x) { return std::sin(x); });
        sineOsc.prepare(spec);
        
        squareOsc.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        squareOsc.prepare(spec);
        
        sawtoothOsc.initialise([](float x) { 
            return x / juce::MathConstants<float>::pi; 
        });
        sawtoothOsc.prepare(spec);
        
        triangleOsc.initialise([](float x) { 
            auto absX = std::abs(x);
            return 1.0f - 2.0f * absX / juce::MathConstants<float>::pi;
        });
        triangleOsc.prepare(spec);
    }

    void setFrequency(float frequency)
    {
        sineOsc.setFrequency(frequency);
        squareOsc.setFrequency(frequency);
        sawtoothOsc.setFrequency(frequency);
        triangleOsc.setFrequency(frequency);
        this->frequency = frequency;
    }

    void setWaveform(WaveformType type)
    {
        currentWaveform = type;
    }

    void reset()
    {
        sineOsc.reset();
        squareOsc.reset();
        sawtoothOsc.reset();
        triangleOsc.reset();
    }

    float getSample()
    {
        switch (currentWaveform)
        {
            case WaveformType::sine:
                return sineOsc.processSample(0.0f);
            case WaveformType::square:
                return squareOsc.processSample(0.0f);
            case WaveformType::sawtooth:
                return sawtoothOsc.processSample(0.0f);
            case WaveformType::triangle:
                return triangleOsc.processSample(0.0f);
            default:
                return 0.0f;
        }
    }

    float getWavetableValue()
    {
        this->phase_increment += this->frequency / this->sampleRate;
        if (this->phase_increment > 1.0f)
            this->phase_increment = 0.0f;
        return wavetable.at((int)(phase_increment * wavetable.size()));
    }

private:
    juce::dsp::Oscillator<float> sineOsc;
    juce::dsp::Oscillator<float> squareOsc;
    juce::dsp::Oscillator<float> sawtoothOsc;
    juce::dsp::Oscillator<float> triangleOsc;
    WaveformType currentWaveform = WaveformType::sine;
    double sampleRate = 48000.0;
    std::vector<float>  wavetable;
    float               phase_increment;
    float               frequency;
};
