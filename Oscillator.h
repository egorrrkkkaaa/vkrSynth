#include <math.h>
class Oscillator 
{
public:
    enum Type 
	{
        Sine,
        Saw,
        Square,
        Triangle,
        pCount
    };


    void SetType(Type type);
    void setFrequency(double frequency);
    void setSampleRate(double sampleRate);
    void synthesize(double* out, int nFrames);
    inline void setMuted(bool muted) { isMuted = muted; }
    virtual double nextSample();
    Oscillator() :
    mType(Sine),
    pi(2*acos(0.0)),
    twoPI(2 * pi),
    isMuted(true),
    mFrequency(440.0),
    PitchMod(0.0),
    Phase(0.0) { updateIncrement(); };
    
    void reset() { Phase = 0.0; }
    void setPitchMod(double amount);
protected:
    double naiveWaveformFormType(Type type);
    Type mType;
    const double pi;
    const double twoPI;
    bool isMuted;
    double mFrequency;
    double Phase;
    static double mSampleRate;
    double PhaseRaise;
    void updateIncrement();
    
    double PitchMod;
};

