
#include "Filter.h"
#include <math.h>
double Filter::OutVoice(double inputValue)
{
	inputValue = HighCut(inputValue);
	inputValue = LowCut(inputValue);
	inputValue = Mod(inputValue);

	return inputValue;	
}


double Filter::HighCut(double inputValue) 
{
		if (inputValue == 0.0) return inputValue;
		b0 += HighCutFreq * (inputValue - b0);
		b1 += HighCutFreq * (b0 - b1);
		b2 += HighCutFreq * (b1 - b2);
		b3 += HighCutFreq * (b2 - b3);
		return b3;   
}

double Filter::LowCut(double inputValue)
{
		if (inputValue == 0.0) return inputValue;
		b10 += LowCutFreq * (inputValue - b10);
		b11 += LowCutFreq * (b10 - b11);
		b12 += LowCutFreq * (b11 - b12);
		b13 += LowCutFreq * (b12 - b13);
		return inputValue - b13;
}
double Filter::Mod(double inputValue)
{	
	PhaseIncrease = FreqLFO;
	inputValue = inputValue - inputValue * sin(Phase) * ModMix;
	Phase += PhaseIncrease;
	while (Phase >= twoPI) 
	{
		Phase -= twoPI;
	}
	return inputValue;
}
