
#include "Envelope.h"

double Envelope::sampleRate = 44100.0;

double Envelope::nextSample() 
{
    if (StageNow != Off &&
        StageNow != Sustain) 
	{
        if (SampleNow == SampleChangeStage) 
		{
            Stages newStage = static_cast<Stages>((StageNow + 1) % StageCount);
            ChangeStage(newStage);
        }
        VolumeNow *= Mult;
        SampleNow++;
    }
    return VolumeNow;
}

void Envelope::calculateMult(double startLevel,
                                            double endLevel,
                                            unsigned long long lengthInSamples) 
{
    Mult = 1.0 + (log(endLevel) - log(startLevel)) / (lengthInSamples);
}

void Envelope::ChangeStage(Stages newStage) 
{
    if (StageNow == newStage) return;
    if (StageNow == Off) 
	{
        beganEnvelopeCycle();
    }
    if (newStage == Off) {
        finishedEnvelopeCycle();
    }
    
    StageNow = newStage;
    SampleNow = 0;
    if (StageNow == Off ||
        StageNow == Sustain) 
	{
        SampleChangeStage = 0;
    } else
	{
        SampleChangeStage = StageTime[StageNow] * sampleRate;
    }
    switch (newStage) 
	{
        case Off:
            VolumeNow = 0.0;
            Mult = 1.0;
            break;
        case Attack:
            VolumeNow = MinVolume;
            calculateMult(VolumeNow,
                                1.0,
                                SampleChangeStage);
            break;
        case Decay:
            VolumeNow = 1.0;
            calculateMult(VolumeNow,
                                fmax(StageTime[Sustain], MinVolume),
                                SampleChangeStage);
            break;
        case Sustain:
            VolumeNow = StageTime[Sustain];
            Mult = 1.0;
            break;
        case Release:          
            calculateMult(VolumeNow,
                                MinVolume,
                                SampleChangeStage);
            break;
        default:
            break;
    }
}

void Envelope::setSampleRate(double newSampleRate) 
{
    sampleRate = newSampleRate;
}

void Envelope::setStageValue(Stages stage,
                                      double value)
{
	StageTime[stage] = value;
    if (stage == StageNow) 
	{      
        if(StageNow == Attack ||
           StageNow == Decay ||
           StageNow == Release) 
		{
            double nextLevelValue;
            switch (StageNow) 
			{
                case Attack:
                    nextLevelValue = 1.0;
                    break;
                case Decay:
                    nextLevelValue = fmax(StageTime[Sustain], MinVolume);
                    break;
                case Release:
                    nextLevelValue = MinVolume;
                    break;
                default:
                    break;
            }            
            double StageNowProcess = (SampleNow + 0.0) / SampleChangeStage;            
            double remainingStageProcess = 1.0 - StageNowProcess;
            unsigned long long samplesUntilNextStage = remainingStageProcess * value * sampleRate;
            SampleChangeStage = SampleNow + samplesUntilNextStage;
            calculateMult(VolumeNow, nextLevelValue, samplesUntilNextStage);
        } else if(StageNow == Sustain) 
		{
            VolumeNow = value;
        }
    }
    if (StageNow == Decay && stage == Sustain) 
	{
        unsigned long long samplesUntilNextStage = SampleChangeStage - SampleNow;
        calculateMult(VolumeNow,
                            fmax(StageTime[Sustain], MinVolume),
                            samplesUntilNextStage);
    }
}
