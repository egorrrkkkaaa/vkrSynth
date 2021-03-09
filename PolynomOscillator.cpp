
#include "PolynomOscillator.h"

double PolynomOscillator::poly_blep(double t)
{
    double dt = PhaseRaise / twoPI; 
    if (t < dt) 
	{
        t /= dt;
        return t+t - t*t - 1.0;
    }  
    else if (t > 1.0 - dt)
	{
        t = (t - 1.0) / dt;
        return t*t + t+t + 1.0;
    }
    else return 0.0;
}

double PolynomOscillator::nextSample()
{
    double value = 0.0;
    double t = Phase / twoPI;
    
    if (mType == Sine)
	{
        value = naiveWaveformFormType(Sine);
    } else if (mType == Saw) 
	{
        value = naiveWaveformFormType(Saw);
        value -= poly_blep(t);
    } else {
        value = naiveWaveformFormType(Square);
        value += poly_blep(t);
        value -= poly_blep(fmod(t + 0.5, 1.0));
        if (mType == Triangle)
		{            
            value = PhaseRaise * value + (1 - PhaseRaise) * lastOutput;
            lastOutput = value;
        }
    }
    
    Phase += PhaseRaise;
    while (Phase >= twoPI)
	{
        Phase -= twoPI;
    }
    return value;
}