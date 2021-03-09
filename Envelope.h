#include <cmath>
#include "SignalFramework.h"
using Gallant::Signal0;

class Envelope
{
public:
	enum Stages
	{
		Off,
		Attack,
		Decay,
		Sustain,
		Release,
		StageCount
	};

    void ChangeStage(Stages newStage);
    double nextSample();
    static void setSampleRate(double newSampleRate);
    inline Stages getStageNow() const { return StageNow; };
    const double MinVolume;
    
    Envelope() :
    MinVolume(0.0001),
    StageNow(Off),
    VolumeNow(MinVolume),
    Mult(1.0),
    SampleNow(0),
    SampleChangeStage(0) 
	{
		StageTime[Off] = 0.0;
		StageTime[Attack] = 0.01;
		StageTime[Decay] = 0.5;
		StageTime[Sustain] = 0.1;
		StageTime[Release] = 1.0;
    };
    
    void setStageValue(Stages stage, double value);
    
    Signal0<> beganEnvelopeCycle;
    Signal0<> finishedEnvelopeCycle;
    
    void reset() 
	{
        StageNow = Off;
        VolumeNow = MinVolume;
        Mult = 1.0;
        SampleNow = 0;
        SampleChangeStage = 0;
    }
private:
    Stages StageNow;
    double VolumeNow;
    double Mult;
    static double sampleRate;
    double StageTime[StageCount];
    void calculateMult(double startLevel, double endLevel, unsigned long long lengthInSamples);
    unsigned long long SampleNow;
    unsigned long long SampleChangeStage;
};

