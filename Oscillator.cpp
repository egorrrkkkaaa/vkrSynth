

#include "Oscillator.h"

double Oscillator::mSampleRate = 44100.0;

void Oscillator::SetType(Type mode) 
{
    mType = mode;
}

void Oscillator::setFrequency(double frequency) 
{
    mFrequency = frequency;
    updateIncrement();
}

void Oscillator::setSampleRate(double sampleRate)
{
    mSampleRate = sampleRate;
    updateIncrement();
}

void Oscillator::synthesize(double* out, int nFrames) 
{
    const double twoPI = 2 * pi;
    switch (mType) 
	{
        case Sine:
            for (int i = 0; i < nFrames; i++)
			{
				out[i] = sin(Phase);
                Phase += PhaseRaise;                
            }            
        case Saw:
            for (int i = 0; i < nFrames; i++) 
			{
				out[i] = 1 - (2 * Phase / twoPI);
                Phase += PhaseRaise;
                while (Phase >= twoPI) 
				{
                    Phase -= twoPI;
                }
            }
            break;
        case Square:
            for (int i = 0; i < nFrames; i++) 
			{
                if (Phase <= pi)
				{
					out[i] = 1;
                } 
				else 
				{
					out[i] = -1;
                }
                Phase += PhaseRaise;
                while (Phase >= twoPI) 
				{
                    Phase -= twoPI;
                }
            }
            break;
        case Triangle:
            for (int i = 0; i < nFrames; i++) 
			{                
				out[i] = 2 * (fabs(-1 + (2 * Phase / twoPI)) - 0.5);
                Phase += PhaseRaise;
                while (Phase >= twoPI) 
				{
                    Phase -= twoPI;
                }
            }
            break;
    }
}

double Oscillator::nextSample()
{
    double value = naiveWaveformFormType(mType);
    Phase += PhaseRaise;
    while (Phase >= twoPI) 
	{
        Phase -= twoPI;
    }
    return value;
}

void Oscillator::setPitchMod(double amount) 
{
	PitchMod = amount;
    updateIncrement();
}

void Oscillator::updateIncrement() 
{
    double pitchModAsFrequency = pow(2.0, fabs(PitchMod) * 14.0) - 1;
    if (PitchMod < 0)
	{
        pitchModAsFrequency = -pitchModAsFrequency;
    }
    double calculatedFrequency = fmin(fmax(mFrequency + pitchModAsFrequency, 0), mSampleRate/2.0);
    PhaseRaise = calculatedFrequency * 2 * pi / mSampleRate;
}

double Oscillator::naiveWaveformFormType(Type mode)
{
    double value;
    switch (mode) 
	{
        case Sine:
            value = sin(Phase);
            break;
        case Saw:
            value = (2.0 * Phase / twoPI) - 1.0;
            break;
        case Square:
            if (Phase < pi) {
                value = 1.0;
            } else {
                value = -1.0;
            }
            break;
        case Triangle:
            value = -1.0 + (2.0 * Phase / twoPI);
            value = 2.0 * (fabs(value) - 0.5);
            break;
        default:
            break;
    }
    return value;
}
