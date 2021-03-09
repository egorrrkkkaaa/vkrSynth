#include "PolynomOscillator.h"
#include "Envelope.h"
#include "Filter.h"

class Voice 
{
public:
    friend class VoiceManager;
    Voice()
    : NoteNumber(-1),
		Velocity(0),
		OscMix(0.5),
    isActive(false) 
	{
		EnvVolume.finishedEnvelopeCycle.Connect(this, &Voice::setFree);
    };
   inline void setOscillatorMix(double mix) { OscMix = mix; }
    
    inline void setNoteNumber(int noteNumber) 
	{
		NoteNumber = noteNumber;
        double frequency = 440.0 * pow(2.0, (NoteNumber - 69.0) / 12.0);
		FirstOsc.setFrequency(frequency);
		SecondOsc.setFrequency(frequency);
    }
    double nextSample();
    void setFree();
    
    void reset();
private:
	PolynomOscillator FirstOsc;
	PolynomOscillator SecondOsc;
    Envelope EnvVolume;    
    Filter Filter;
    int NoteNumber;
    int Velocity;
    double OscMix; 
    bool isActive;
};

