#include "Voice.h"

double Voice::nextSample()
{
    if (!isActive) return 0.0;
    
    double oscillatorOneOutput = FirstOsc.nextSample();
    double oscillatorTwoOutput = SecondOsc.nextSample();
    double oscillatorSum = ((1 - OscMix) * oscillatorOneOutput) + (OscMix * oscillatorTwoOutput);
    
    double volumeEnvelopeValue = EnvVolume.nextSample();   
	
    return Filter.OutVoice(oscillatorSum * volumeEnvelopeValue * Velocity / 127.0);

}

void Voice::setFree() 
{
    isActive = false;
}

void Voice::reset()
{
    NoteNumber = -1;
	Velocity = 0;
	FirstOsc.reset();
	SecondOsc.reset();
    EnvVolume.reset();    
	Filter.reset();
}